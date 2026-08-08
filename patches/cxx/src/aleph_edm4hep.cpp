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

  class event_edm4hep  {
  public:
    edm4hep::EventHeader            event_header   {  };
    edm4hep::MCParticleCollection   particles_mc   {  };
    edm4hep::TrackCollection        particles_reco {  };
    edm4hep::ClusterCollection      clusters_ecal  {  };
    edm4hep::ClusterCollection      clusters_hcal  {  };
    std::map<int, podio::ObjectID>  alpha2edm4hep_particles { };
    std::map<int, podio::ObjectID>  alpha2edm4hep_tracks    { };
    std::map<int, podio::ObjectID>  alpha2edm4hep_ecal      { };
    std::map<int, podio::ObjectID>  alpha2edm4hep_hcal      { };
    const object_table<class qvec>* alpha_tracks  { nullptr };
    const object_table<class peco>* alpha_ecal    { nullptr };
    const object_table<class phco>* alpha_hcal    { nullptr };
    
    template<typename T,typename C>
    T get_item(C& cont, podio::ObjectID id)  {
      for( auto i=cont.begin(); i != cont.end(); ++i )  {
        if( id == i->id() )  {
          return *i;
        }
      }
      throw std::runtime_error("Non existing object");
    }
    template<typename T>
    T* get_row(const object_table<T>* table, int32_t idx)  {
      T* row = table->at(idx);
      if( !row )  {
        throw std::runtime_error("Error: non existing cluster in table!");
      }
      return row;
    }

    /// Default constructor
    event_edm4hep();
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
  double enecnv(double val) { return val * ENECONV;  }

  constexpr static const double LENCONV = (TGeoUnit::GeV/G4::GeV);
  double lencnv(double val) { return val * LENCONV;  }
}

/// Default constructor
alpha::event_edm4hep::event_edm4hep()  {
}

/// Default destructor
alpha::event_edm4hep::~event_edm4hep()  {
}

edm4hep::MutableMCParticle alpha::event_edm4hep::get_particle_mc(int itk)  {
  auto it = this->alpha2edm4hep_particles.find(itk);
  if( it != this->alpha2edm4hep_particles.end() )  {
    return this->get_item<edm4hep::MutableMCParticle>(this->particles_mc, it->second);
  }
  auto*        track = this->get_row(this->alpha_tracks, itk);
  auto         mcp   = this->particles_mc.create();
  const auto* vstart = track->origin_vtx();
  const auto* vend   = track->end_vtx();
  double  part_time  = 0e0;
  int32_t sim_status = 0;

  this->alpha2edm4hep_particles[itk] = mcp.id();
  mcp.setMass(track->qm());
  mcp.setTime(part_time);
  mcp.setCharge(track->qch());
  mcp.setPDG(track->ktpcod());
  mcp.setSimulatorStatus(sim_status);

  mcp.setMomentum({ enecnv(track->qx()), enecnv(track->qy()), enecnv(track->qz()) });
  mcp.setMomentumAtEndpoint({ enecnv(track->qx()), enecnv(track->qy()), enecnv(track->qz()) });

  mcp.setVertex(   { vstart ? lencnv(vstart->x) : 0e0,
      vstart ? lencnv(vstart->y) : 0e0,
      vstart ? lencnv(vstart->z) : 0e0 } );
  mcp.setEndpoint( { vend ? lencnv(vend->x) : 0e0,
      vend ? lencnv(vend->y) : 0e0,
      vend ? lencnv(vend->z) : 0e0 } );

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
  auto* track = this->get_row(this->alpha_tracks, itk);
  auto* frft = track->frft();
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
  trk.addToSubdetectorHitNumbers(frtl ? frtl->narcV()  : 0);
  trk.addToSubdetectorHitNumbers(frtl ? frtl->narcI()  : 0);
  trk.addToSubdetectorHitNumbers(frtl ? frtl->nrEsti() : 0);
  trk.addToSubdetectorHitNumbers(frtl ? frtl->narcT()  : 0);
  trk.addToSubdetectorHitNumbers(frtl ? frtl->nrestt() : 0);
  trk.addToSubdetectorHitNumbers(0);
  auto* frid = track->frid();
  trk.addToSubdetectorHitNumbers(frid ? frid->bitPat()    : 0);
  trk.addToSubdetectorHitNumbers(frid ? frid->deadZone()  : 0);
  trk.addToSubdetectorHitNumbers(frid ? frid->bitpatC()   : 0);
  trk.addToSubdetectorHitNumbers(frid ? frid->deadzoneC() : 0);
  trk.addToSubdetectorHitNumbers(frid ? int(frid->probElec()*1000)   : 0);
  trk.addToSubdetectorHitNumbers(frid ? int(frid->probMuon()*1000)   : 0);
  trk.addToSubdetectorHitNumbers(frid ? int(frid->probpIon()*1000)   : 0);
  trk.addToSubdetectorHitNumbers(frid ? int(frid->probKaon()*1000)   : 0);
  trk.addToSubdetectorHitNumbers(frid ? int(frid->probProton()*1000) : 0);
  trk.addToSubdetectorHitNumbers(frid ? int(frid->noKinkprob()*1000) : 0);
  trk.addToSubdetectorHitNumbers(frid ? frid->qualityFlag()  : 0);
  return trk;
}

/// Create ECAL cluster from PECO row
edm4hep::MutableCluster alpha::event_edm4hep::get_ecal_cluster(int itk)  {
  auto it = this->alpha2edm4hep_ecal.find(itk);
  if( it != this->alpha2edm4hep_ecal.end() )  {
    return this->get_item<edm4hep::MutableCluster>(this->clusters_ecal, it->second);
  }
  auto* aclu = this->get_row(this->alpha_ecal, itk);
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
  auto* aclu = get_row(this->alpha_hcal, itk);
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
}

/// Default destructor
alpha::output_edm4hep::~output_edm4hep()   {
}

/// Start event saving
void alpha::output_edm4hep::begin_event()  {
  this->event  = std::make_unique<event_edm4hep>();
  this->event->alpha_tracks = params.qvec_table;
  this->event->alpha_ecal   = params.peco_table;
  this->event->alpha_hcal   = params.phco_table;

  /// First only create the particles and fill the properties
  for( int itk = qcde.KFMCT; itk <= qcde.KLMCT; ++itk )
    this->event->get_particle_mc(itk);
  /// Only create the charged reconstructed particles and fill the properties
  for( int itk = qcde.KFCHT; itk <= qcde.KLCHT; ++itk )
    this->event->get_particle_reco(itk);
  /// Create all ECAL clusters from PECO
  for( uint32_t i=1; i <= this->event->alpha_ecal->size(); ++i )
    this->event->get_ecal_cluster(i);
  /// Create all HCAL clusters from PHCO
  for( uint32_t i=1; i <= this->event->alpha_ecal->size(); ++i )
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
    auto* track = this->event->alpha_tracks->at(itk);
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
  const auto* clusters = params.peco_table;
  for(uint32_t i=1; i <= clusters->size(); ++i )   {
    const auto* cluster = clusters->at(i);
    
  }
}

/// After creation add all links to the HCAL clusters from PHCO
void alpha::output_edm4hep::fill_clusters_hcal()   {
}
