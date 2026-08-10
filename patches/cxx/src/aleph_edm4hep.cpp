//==========================================================================
//  ALEPH software suite
//--------------------------------------------------------------------------
//  Copyright (C) Organisation europeenne pour la Recherche nucleaire (CERN)
//  All rights reserved.
//
//  For the licensing terms see OnlineSys/LICENSE.
//
//--------------------------------------------------------------------------
//
//  Author     : Markus Frank
//==========================================================================
#ifndef ALPHA_EDM4HEP_OUTPUT_H
#define ALPHA_EDM4HEP_OUTPUT_H 1

/// Framework include files
#include <alpha/defs.h>
#include <alpha/qcde.h>
#include <bos/bosbank.h>

/// C/C++ include files
#include <cstdio>
#include <memory>


/// edm4hep include files
#include <edm4hep/TrackCollection.h>
#include <edm4hep/ClusterCollection.h>
#include <edm4hep/MCParticleCollection.h>
#include <edm4hep/SimTrackerHitCollection.h>
#include <edm4hep/CaloHitContributionCollection.h>
#include <edm4hep/SimCalorimeterHitCollection.h>
#include <edm4hep/EventHeader.h>
#include <edm4hep/EDM4hepVersion.h>
#include <edm4hep/Constants.h>

/// podio include files
#include <podio/CollectionBase.h>
#include <podio/podioVersion.h>
#include <podio/Frame.h>
#include <podio/FrameCategories.h>
#if PODIO_BUILD_VERSION >= PODIO_VERSION(1, 0, 0)
#include <podio/Writer.h>
#else
#if PODIO_BUILD_VERSION >= PODIO_VERSION(0, 99, 0)
#include <podio/ROOTWriter.h>
#else
#include <podio/ROOTFrameWriter.h>
namespace podio {
  using ROOTWriter = podio::ROOTFrameWriter;
}
#endif
#endif

/// ALPHA namespace declaration
namespace alpha  {

  class vdxy;

  struct bank_access_t  {
    int32_t  nami  { 0 };
    int32_t  kloc  { 0 };
    int32_t* data  { nullptr };

    void event_config()  {
      this->kloc = bos77::bcs.iw[this->nami];
      this->data = this->_pointer();
    }

    int32_t* _pointer()  const  {
      int32_t off = this->offset();
      if( off )  {
	int32_t* ptr = bos77::bcs.iw + off - bos77::bankheader_words;
	return ptr;
      }
      return nullptr;
    }

    template<typename T> T* row(int32_t idx)  {
      T* row = this->table<T>()->at(idx);
      if( row )  {
	return row;
      }
      throw std::runtime_error("Error: non existing cluster in table!");
    }

    template<typename T=int32_t> object_table<T>* table()  const  {
      return (object_table<T>*)this->data;
    }

    int32_t offset()  const  {
      return this->kloc ? *(bos77::bcs.iw + this->kloc) : 0;
    }
  };

  struct data_access_t  {
    bank_access_t qvec;  // QVEC table bank
    bank_access_t peco;  // PECO table bank
    bank_access_t phco;  // PHCO table bank
    bank_access_t vdco;  // VDET coordinated NR=0 (POT) --> FRFT
    bank_access_t vdxy;  // MVD hits in r-phi wafer.       NR=ILAYER*10000+IZED*1000+IPH-I*10+(IVIEW=1)
    bank_access_t vdzt;  // MVD hits in z wafer. (POT)     NR=ILAYER*10000+IZED*1000+IPH-I*10+(IVIEW=1)
    bank_access_t vfhl;  // VDET final hit list bank,      NR=VHLS number (POT)
    bank_access_t vfph;  // VDET final pulse height bank,  NR=VHLS number (POT)
    bank_access_t vflg;  // VDET strip flag bank,          NR=VHLS number (POT)
    bank_access_t vufk;  // Vdxy/vdzt to FKIN truth
    bank_access_t vdfk;  // Vdco to FKIN truth relation
    bank_access_t vdht;  // VDet HiT list NR=0 (GAL)
    bank_access_t vdgc;  // VDET global clusters
  };

  class event_edm4hep  {
  public:
    edm4hep::EventHeader               event_header            {  };
    edm4hep::MCParticleCollection      particles_mc            {  };
    edm4hep::TrackCollection           particles_reco          {  };
    edm4hep::ClusterCollection         clusters_ecal           {  };
    edm4hep::ClusterCollection         clusters_hcal           {  };
    edm4hep::TrackerHitPlaneCollection mvd_hits                {  };
    std::map<int, podio::ObjectID>     alpha2edm4hep_particles {  };
    std::map<int, podio::ObjectID>     fkin2edm4hep_particles  {  };
    std::map<int, podio::ObjectID>     alpha2edm4hep_tracks    {  };
    std::map<int, podio::ObjectID>     alpha2edm4hep_ecal      {  };
    std::map<int, podio::ObjectID>     alpha2edm4hep_hcal      {  };

    data_access_t& data;

    template<typename T,typename C>
    T get_item(C& cont, podio::ObjectID id)  {
      for( auto i=cont.begin(); i != cont.end(); ++i )  {
	if( id == i->id() )  {
	  return *i;
	}
      }
      throw std::runtime_error("Non existing object");
    }

    /// Default constructor
    event_edm4hep(data_access_t& data);
    /// Default destructor
    virtual ~event_edm4hep();
    /// Create MC particle
    edm4hep::MutableMCParticle get_particle_mc(int itk);
    /// Create charged reconstructed track from FRFT
    edm4hep::MutableTrack      get_particle_reco(int itk);
    /// Create ECAL cluster from PECO row
    edm4hep::MutableCluster    get_ecal_cluster(int peco_obj);
    /// Create HCAL cluster from PHCO row
    edm4hep::MutableCluster    get_hcal_cluster(int phco_obj);
  };
  
  class output_edm4hep  {
  public:
    std::unique_ptr<event_edm4hep> event;
#if PODIO_BUILD_VERSION >= PODIO_VERSION(1, 0, 0)
    using writer_t = podio::Writer;
#else
    using writer_t = podio::ROOTWriter;
#endif
    std::unique_ptr<writer_t>     podio_file  { };
    podio::Frame                  podio_frame { };

    data_access_t data  { };

    int32_t*  iw { nullptr };
    
  public:
    /// Default constructor
    output_edm4hep();
    /// Default destructor
    virtual ~output_edm4hep();
    /// Start event saving
    void begin_event();
    /// End event saving
    void end_event();
    /// After creation add all links to the MC particles
    void fill_particles_mc();
    /// After creation add all links to the reconstructed tracks
    void fill_tracks();
    /// After creation add all links to the ECAL clusters from PECO
    void fill_clusters_ecal();
    /// After creation add all links to the HCAL clusters from PHCO
    void fill_clusters_hcal();
  };
}      // End namespace alpha
#endif // ALPHA_EDM4HEP_OUTPUT_H



#include <edm4hep/GeneratorEventParametersCollection.h>

/// Alpha include files
#include <alpha/qvec.h>
#include <alpha/qvrt.h>
#include <alpha/frft.h>
#include <alpha/frtl.h>
#include <alpha/frid.h>
#include <alpha/eidt.h>
#include <alpha/hmad.h>
#include <alpha/mcad.h>
#include <alpha/muid.h>
#include <alpha/peco.h>
#include <alpha/phco.h>

#include <alpha/vdxy.h>

/// podio include files
#include <podio/CollectionBase.h>
#include <podio/podioVersion.h>
#include <podio/Frame.h>
#include <podio/FrameCategories.h>
#include <podio/Writer.h>

#include <atomic>

//#include <TGeoUnits.h>
#include <TGeoSystemOfUnits.h>
#include <TGeant4SystemOfUnits.h>

namespace G4 = TGeant4Unit;



namespace {
  edm4hep::Vector3f null_position(0e0, 0e0, 0e0);
  
  constexpr static const double ENECONV = (TGeoUnit::GeV/G4::GeV);
  double _ENE(double val) { return val * ENECONV;   }

  constexpr static const double LENCONV = (TGeoUnit::GeV/G4::GeV);
  double _LEN(double val) { return val * LENCONV;   }

  constexpr static const double TIMECONV = (TGeoUnit::ns/G4::ns);
  double _TIM(double val) { return val * TIMECONV;  }
  
  void data_access_namind( alpha::data_access_t& acc )  {
    acc.qvec.nami = bos77::namind("QVEC");
    acc.peco.nami = bos77::namind("PECO");
    acc.phco.nami = bos77::namind("PHCO");
    acc.vdco.nami = bos77::namind("VDCO");
    acc.vdxy.nami = bos77::namind("VDXY");
    acc.vdzt.nami = bos77::namind("VDZT");
    acc.vfhl.nami = bos77::namind("VFHL");
    acc.vfph.nami = bos77::namind("VFPH");
    acc.vflg.nami = bos77::namind("VFLG");
    acc.vufk.nami = bos77::namind("VUFK");
    acc.vdfk.nami = bos77::namind("VDFK");
    acc.vdht.nami = bos77::namind("VDHT");
    acc.vdgc.nami = bos77::namind("VDGC");
  }

  void data_access_event_config( alpha::data_access_t& acc )  {
    acc.qvec.event_config();
    acc.peco.event_config();
    acc.phco.event_config();
    acc.vdco.event_config();
    acc.vdxy.event_config();
    acc.vdzt.event_config();
    acc.vfhl.event_config();
    acc.vfph.event_config();
    acc.vflg.event_config();
    acc.vufk.event_config();
    acc.vdfk.event_config();
    acc.vdht.event_config();
    acc.vdgc.event_config();
  }
}

/// Default constructor
alpha::event_edm4hep::event_edm4hep(data_access_t& da)
  : data(da)
{
}

/// Default destructor
alpha::event_edm4hep::~event_edm4hep()  {
}

edm4hep::MutableMCParticle alpha::event_edm4hep::get_particle_mc(int itk)  {
  auto it = this->alpha2edm4hep_particles.find(itk);
  if( it != this->alpha2edm4hep_particles.end() )  {
    return this->get_item<edm4hep::MutableMCParticle>(this->particles_mc, it->second);
  }
  auto*        track  = this->data.qvec.row<class qvec>(itk);
  auto         mcp    = this->particles_mc.create();
  const auto*  vstart = track->origin_vtx();
  const auto*  vend   = track->end_vtx();
  double       ptime  = 0e0;
  int32_t sim_status  = 0;

  this->alpha2edm4hep_particles[itk] = mcp.id();
  this->fkin2edm4hep_particles[track->ktn()] = mcp.id();
  mcp.setMass(_ENE(track->qm()));
  mcp.setTime(_TIM(ptime));
  mcp.setCharge(track->qch());
  mcp.setPDG(track->ktpcod());
  mcp.setSimulatorStatus(sim_status);

  mcp.setMomentum( {           _ENE(track->qx()), _ENE(track->qy()), _ENE(track->qz()) } );
  mcp.setMomentumAtEndpoint( { _ENE(track->qx()), _ENE(track->qy()), _ENE(track->qz()) } );

  mcp.setVertex(   {
      vstart ? _LEN(vstart->x) : 0e0,
      vstart ? _LEN(vstart->y) : 0e0,
      vstart ? _LEN(vstart->z) : 0e0 } );
  mcp.setEndpoint( {
      vend ? _LEN(vend->x) : 0e0,
      vend ? _LEN(vend->y) : 0e0,
      vend ? _LEN(vend->z) : 0e0 } );

  // Set generator status
  mcp.setGeneratorStatus(track->klunds());
  // Set stability code
  mcp.setSimulatorStatus(track->kstabc());

#if EDM4HEP_MCPARTICLE_HAS_HELICITY
  //mcp.setHelicity(p->spin[2]);
#else
  //mcp.setSpin(p->spin);
#endif
  return mcp;
}

edm4hep::MutableTrack alpha::event_edm4hep::get_particle_reco(int itk)  {
  auto it = this->alpha2edm4hep_tracks.find(itk);
  if( it != this->alpha2edm4hep_tracks.end() )  {
    return this->get_item<edm4hep::MutableTrack>(this->particles_reco, it->second);
  }
  auto* track = this->data.qvec.row<class qvec>(itk);
  auto* frft  = track->frft();
  if( !frft )  {
    throw std::runtime_error("Error: No FRFT bank present");
  }

  auto trk  = this->particles_reco.create();
  this->alpha2edm4hep_tracks[itk] = trk.id();
  trk.setType(track->kclass());
  trk.setNholes(0);                // ???
  trk.setChi2(frft->chis2());      // Should go to track state ??
  trk.setNdf(frft->numDegFree());  // Should go to track state ??
  
  edm4hep::TrackState state;       // misses: momentum/inverse radius
  state.location = edm4hep::TrackState::AtIP;
  state.time = 0e0;
  state.D0 = frft->d0();
  state.Z0 = frft->z0();
  state.phi = frft->phi0();
  state.omega = frft->inverseRadi();
  state.tanLambda = frft->tanLambda();
  state.referencePoint = { 0e0, 0e0, 0e0 };
  for( uint32_t i=0; i<21; ++i )
    state.covMatrix.values[i] = frft->ecovarM()[i];
  trk.addToTrackStates(state);
  
  auto* frtl = track->frtl();
  trk.addToSubdetectorHitNumbers(frtl ? frtl->narcV()                : 0);
  trk.addToSubdetectorHitNumbers(frtl ? frtl->narcI()                : 0);
  trk.addToSubdetectorHitNumbers(frtl ? frtl->nrEsti()               : 0);
  trk.addToSubdetectorHitNumbers(frtl ? frtl->narcT()                : 0);
  trk.addToSubdetectorHitNumbers(frtl ? frtl->nrestt()               : 0);
  trk.addToSubdetectorHitNumbers(0);

  auto* frid = track->frid();
  trk.addToSubdetectorHitNumbers(frid ? frid->bitPat()               : 0);
  trk.addToSubdetectorHitNumbers(frid ? frid->deadZone()             : 0);
  trk.addToSubdetectorHitNumbers(frid ? frid->bitpatC()              : 0);
  trk.addToSubdetectorHitNumbers(frid ? frid->deadzoneC()            : 0);
  trk.addToSubdetectorHitNumbers(frid ? int(frid->probElec()*1000)   : 0);
  trk.addToSubdetectorHitNumbers(frid ? int(frid->probMuon()*1000)   : 0);
  trk.addToSubdetectorHitNumbers(frid ? int(frid->probpIon()*1000)   : 0);
  trk.addToSubdetectorHitNumbers(frid ? int(frid->probKaon()*1000)   : 0);
  trk.addToSubdetectorHitNumbers(frid ? int(frid->probProton()*1000) : 0);
  trk.addToSubdetectorHitNumbers(frid ? int(frid->noKinkprob()*1000) : 0);
  trk.addToSubdetectorHitNumbers(frid ? frid->qualityFlag()          : 0);
  return trk;
}

/// Create ECAL cluster from PECO row
edm4hep::MutableCluster alpha::event_edm4hep::get_ecal_cluster(int itk)  {
  auto it = this->alpha2edm4hep_ecal.find(itk);
  if( it != this->alpha2edm4hep_ecal.end() )  {
    return this->get_item<edm4hep::MutableCluster>(this->clusters_ecal, it->second);
  }
  auto* aclu = this->data.peco.row<class peco>(itk);
  auto  clu  = this->clusters_ecal.create();
  this->alpha2edm4hep_ecal[itk] = clu.id();
  clu.setIPhi(aclu->phi());
  clu.setITheta(aclu->theta());
  clu.setEnergy(aclu->ecorr());
  clu.setEnergyError( std::abs(aclu->ecorr() - aclu->eraw()) );
  clu.setType(aclu->rbits() +
	      (aclu->ccode()<<8) +
	      (aclu->kdrg()<<16));
  clu.setPosition( { } );
  clu.setPositionError( { } );
  clu.setDirectionError( { aclu->eraw(), aclu->esta1(), aclu->esta2() } );
  return clu;
}

/// Create HCAL cluster from PHCO row
edm4hep::MutableCluster alpha::event_edm4hep::get_hcal_cluster(int itk)  {
  auto it = this->alpha2edm4hep_hcal.find(itk);
  if( it != this->alpha2edm4hep_hcal.end() )  {
    return this->get_item<edm4hep::MutableCluster>(this->clusters_hcal, it->second);
  }
  auto* aclu = this->data.phco.row<class phco>(itk);
  auto  clu  = this->clusters_hcal.create();
  this->alpha2edm4hep_hcal[itk] = clu.id();
  clu.setIPhi(aclu->phi());
  clu.setITheta(aclu->theta());
  clu.setEnergy(aclu->ecorr());
  clu.setEnergyError( std::abs(aclu->ecorr() - aclu->eraw()) );
  clu.setType(aclu->rbits() +
	      (aclu->ccode()<<8) +
	      (aclu->kdrg()<<16) +
	      (aclu->noiseFlag()<<24));
  clu.setPosition( { } );
  clu.setPositionError( { } );
  clu.setDirectionError( { aclu->eraw(), 0e0, 0e0 } );
  return clu;
}

/// Default constructor
alpha::output_edm4hep::output_edm4hep()   {
  this->iw = bos77::bcs.iw;
  data_access_namind(this->data);
}

/// Default destructor
alpha::output_edm4hep::~output_edm4hep()   {
}

/// Start event saving
void alpha::output_edm4hep::begin_event()  {
  data_access_event_config(this->data);
  this->event = std::make_unique<event_edm4hep>(this->data);

  /// First only create the particles and fill the properties
  for( int itk = qcde.KFMCT; itk <= qcde.KLMCT; ++itk )
    this->event->get_particle_mc(itk);
  /// Only create the charged reconstructed particles and fill the properties
  for( int itk = qcde.KFCHT; itk <= qcde.KLCHT; ++itk )
    this->event->get_particle_reco(itk);
  /// Create all ECAL clusters from PECO
  for( uint32_t i=1; i <= this->data.peco.table<class peco>()->size(); ++i )
    this->event->get_ecal_cluster(i);
  /// Create all HCAL clusters from PHCO
  for( uint32_t i=1; i <= this->data.phco.table<class peco>()->size(); ++i )
    this->event->get_hcal_cluster(i);

  

  /// Now we handle the relations between the tracks, the hits, etc.
  /// MC particles
  this->fill_particles_mc();
  /// Reconstructed tracks
  
}

/// End event saving
void alpha::output_edm4hep::end_event()  {
  this->podio_frame.put( std::move(this->event->particles_mc)," MCParticles" );
  this->event.reset();
}

/// After creation add all links to the MC particles
void alpha::output_edm4hep::fill_particles_mc()   {
  for(int itk = qcde.KFMCT; itk <= qcde.KLMCT; ++itk )  {
    auto* track = this->data.qvec.row<class qvec>(itk);
    auto  part  = this->event->get_particle_mc(itk);
    for( uint32_t im=0; im<track->knmoth(); ++im )  {
      const auto& mot = event->get_particle_mc(track->kmoth(im));
      part.addToParents(mot);
    }
    for( uint32_t id=0; id<track->knmoth(); ++id )  {
      const auto& dau = event->get_particle_mc(track->kdau(id));
      part.addToDaughters(dau);
    }
  }
}

/// After creation add all links to the reconstructed tracks
void alpha::output_edm4hep::fill_tracks()   {
}

/// After creation add all links to the ECAL clusters from PECO
void alpha::output_edm4hep::fill_clusters_ecal()   {
  const auto* clusters = this->data.peco.table<class peco>();
  for(uint32_t i=1; i <= clusters->size(); ++i )   {
    const auto* cluster = clusters->at(i);
    
  }
}

/// After creation add all links to the HCAL clusters from PHCO
void alpha::output_edm4hep::fill_clusters_hcal()   {
}
