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

/// Framework include files
#include <alpha/output_edm4hep.h>
#include <bos/bosbank.h>
#include <alpha/qcde.h>

/// C/C++ include files
#include <cstdio>
#include <atomic>

/// edm4hep include files
#include <edm4hep/TrackCollection.h>
#include <edm4hep/ClusterCollection.h>
#include <edm4hep/MCParticleCollection.h>
#include <edm4hep/SimTrackerHitCollection.h>
#include <edm4hep/CalorimeterHitCollection.h>
#include <edm4hep/SimCalorimeterHitCollection.h>
#include <edm4hep/CaloHitContributionCollection.h>
#include <edm4hep/GeneratorEventParametersCollection.h>
#include <edm4hep/EventHeader.h>
#include <edm4hep/EDM4hepVersion.h>
#include <edm4hep/Constants.h>

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
#include <alpha/vdco.h>
#include <alpha/vdht.h>
#include <alpha/vdxy.h>
#include <alpha/vdzt.h>
#include <alpha/vcpl.h>
#include <alpha/vufk.h>


/// podio include files
#include <podio/CollectionBase.h>
#include <podio/podioVersion.h>
#include <podio/Frame.h>
#include <podio/FrameCategories.h>
#include <podio/Writer.h>

#include <TGeoSystemOfUnits.h>
#include <TGeant4SystemOfUnits.h>
#include <Math/Vector3D.h>
#include <Math/Transform3D.h>
#include <Math/Translation3D.h>
#include <Math/RotationX.h>
#include <Math/RotationY.h>
#include <Math/RotationZ.h>
#include <Math/Rotation3D.h>
#include <Math/RotationZYX.h>
#include <Math/EulerAngles.h>
#include <Math/VectorUtil.h>
#include <Math/Cylindrical3D.h>


/// podio include files
#include <podio/CollectionBase.h>
#include <podio/Frame.h>
#include <podio/FrameCategories.h>
#include <podio/podioVersion.h>
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

namespace G4 = TGeant4Unit;

namespace alpha  {

  class edm4hep_processor;
  
  /// Helper class to manager podio output of event data
  /**
   *    \author  M.Frank
   *    \date    01/08/2026
   */
  class output_edm4hep::io_t  {
  public:
#if PODIO_BUILD_VERSION >= PODIO_VERSION(1, 0, 0)
    using writer_t = podio::Writer;
#else
    using writer_t = podio::ROOTWriter;
#endif
    std::unique_ptr<writer_t>     podio_file  { };
    podio::Frame                  podio_frame { };

  public:
    /// Default constructor
    io_t();
    /// Default destructor
    ~io_t();
  };

  /// Helper class to access BOS data structures from ALEPH
  /**
   *    \author  M.Frank
   *    \date    01/08/2026
   */
  class output_edm4hep::data_access_t  {

  public:
    void event_config(bank_access_t& bnk)  {
      bnk.data = bos77::get_bank_pointer_from_namind( bnk.nami );
    }

  public:
    bank_access_t qvec;  // QVEC table bank
    bank_access_t peco;  // PECO table bank
    bank_access_t phco;  // PHCO table bank
    bank_access_t vdco;  // VDET coordinated NR=0 (POT) --> FRFT
    bank_access_t vdxy;  // MVD hits in r-phi wafer.       NR=ILAYER*10000+IZED*1000+IPH-I*10+(IVIEW=1)
    bank_access_t vdzt;  // MVD hits in z wafer. (POT)     NR=ILAYER*10000+IZED*1000+IPH-I*10+(IVIEW=1)
    bank_access_t vcpl;  // association of a track NR=NTRACK (POT)

    bank_access_t vfhl;  // VDET final hit list bank,      NR=VHLS number (POT)
    bank_access_t vfph;  // VDET final pulse height bank,  NR=VHLS number (POT)
    bank_access_t vflg;  // VDET strip flag bank,          NR=VHLS number (POT)
    bank_access_t vufk;  // Vdxy/vdzt to FKIN truth relation (monte Carlo)
    bank_access_t vdfk;  // VDFK to FKIN truth relation
    bank_access_t vdht;  // VDet HiT list NR=0 (GAL)
    bank_access_t vdgc;  // VDET global clusters

  public:
    /// Default constructor
    data_access_t();
    /// Default destructor
    ~data_access_t();
    /// Configure for new event
    void event_config();
  };

  /// Helper class to convert event data from ALEPH to EDM4HEP
  /**
   *    \author  M.Frank
   *    \date    01/08/2026
   */
  class output_edm4hep::output_edm4hep::event_t  {
  public:
    edm4hep::EventHeader               event_header            {  };
    edm4hep::MCParticleCollection      particles_mc            {  };
    edm4hep::TrackCollection           particles_reco          {  };
    edm4hep::CalorimeterHitCollection  hits_ecal               {  };
    edm4hep::CalorimeterHitCollection  hits_hcal               {  };
    edm4hep::TrackerHit3DCollection    hits_vdco               {  };
    edm4hep::TrackerHit3DCollection    hits_vdxy               {  };
    edm4hep::TrackerHit3DCollection    hits_vdzt               {  };
    edm4hep::TrackerHitPlaneCollection mvd_hits                {  };
    edm4hep::SimTrackerHitCollection   simhits_vdht            {  };
    std::map<int, podio::ObjectID>     alpha2edm4hep_particles {  };
    std::map<int, podio::ObjectID>     fkin2edm4hep_particles  {  };
    std::map<int, podio::ObjectID>     alpha2edm4hep_tracks    {  };
    std::map<int, podio::ObjectID>     alpha2edm4hep_peco      {  };
    std::map<int, podio::ObjectID>     alpha2edm4hep_phco      {  };
    std::map<int, podio::ObjectID>     alpha2edm4hep_vdco      {  };
    std::map<int, podio::ObjectID>     alpha2edm4hep_vdxy      {  };
    std::map<int, podio::ObjectID>     alpha2edm4hep_vdzt      {  };
    std::map<int, podio::ObjectID>     alpha2edm4hep_vdht      {  };

    data_access_t& data;

    template<typename T,typename C>
    T get_item(C& cont, podio::ObjectID id, const std::string& err="")  {
      for( auto i=cont.begin(); i != cont.end(); ++i )  {
        if( id == i->id() )  {
          return *i;
        }
      }
      throw std::runtime_error(err.empty() ? "Non existing object" : err.c_str());
    }

    /// Default constructor
    event_t(data_access_t& data);
    /// Default destructor
    virtual ~event_t();
    
    using calo_deposit_t = edm4hep::MutableCalorimeterHit;
    using vdet_hit_t     = edm4hep::MutableTrackerHit3D;
    using sim_trackerhit_t = edm4hep::MutableSimTrackerHit;

    edm4hep::MutableMCParticle particle_mc(int32_t itk);
    
    /// Create MC particle
    void             process_mc_tracks();
    /// Convert charged tracks. They enter the catalog with their FRFT row number
    void             process_charged_tracks();
    /// Create ECAL cluster from PECO row
    calo_deposit_t   get_peco_hit(int peco_obj);
    /// Create HCAL cluster from PHCO row
    calo_deposit_t   get_phco_hit(int phco_obj);
    /// Create VDET 3D hit from VDCO row
    vdet_hit_t       get_vdco_hit(int vdco_obj);
    /// Create VDET simulation hit from VDHT row (produced by GALEPH)
    sim_trackerhit_t get_vdht_simhit(int itk);

    /// Create VDET 3D hit from VDXY row
    void             process_vdxy(const bos77::bank* bank);
    /// Create VDET 3D hit from VDZT row
    void             process_vdzt(const bos77::bank* bank);

    /// Process information from VCPL bank
    void             process_vcpl(const bos77::bank* bank);
    /// Process information from VFHL bank
    void             process_vfhl(const bos77::bank* bank);
    /// Process VDXY/VDZT MC truth relations from VUFK row
    void             process_vufk(const bos77::bank* bank);
  };

}

namespace {
  edm4hep::Vector3f null_position(0e0, 0e0, 0e0);
  
  constexpr static const double ENECONV = (TGeoUnit::GeV/G4::GeV);
  double _ENE(double val) { return val * ENECONV;   }

  constexpr static const double LENCONV = (TGeoUnit::GeV/G4::GeV);
  double _LEN(double val) { return val * LENCONV;   }

  constexpr static const double TIMECONV = (TGeoUnit::ns/G4::ns);
  double _TIM(double val) { return val * TIMECONV;  }
  
  typedef ROOT::Math::RhoZPhiVectorF PositionRhoZPhi;
  typedef ROOT::Math::Polar3DVectorF PositionPolar;
  typedef ROOT::Math::XYZVectorF     Position;
  typedef ROOT::Math::XYZVectorF     Direction;
  typedef ROOT::Math::XYZVectorF     XYZAngles;
}


/// Default constructor
alpha::output_edm4hep::io_t::io_t()  {
}

/// Default destructor
alpha::output_edm4hep::io_t::~io_t()  {
}

/// Default constructor
alpha::output_edm4hep::data_access_t::data_access_t()  {
  this->qvec.nami = bos77::namind("QVEC");
  this->peco.nami = bos77::namind("PECO");
  this->phco.nami = bos77::namind("PHCO");
  this->vdco.nami = bos77::namind("VDCO");
  this->vdxy.nami = bos77::namind("VDXY");
  this->vdzt.nami = bos77::namind("VDZT");
  this->vcpl.nami = bos77::namind("VCPL");

  this->vfhl.nami = bos77::namind("VFHL");
  this->vfph.nami = bos77::namind("VFPH");
  this->vflg.nami = bos77::namind("VFLG");
  this->vufk.nami = bos77::namind("VUFK");
  this->vdfk.nami = bos77::namind("VDFK");
  this->vdht.nami = bos77::namind("VDHT");
  this->vdgc.nami = bos77::namind("VDGC");
}

/// Default destructor
alpha::output_edm4hep::data_access_t::~data_access_t()  {
}

void alpha::output_edm4hep::data_access_t::event_config()  {
  this->event_config( this->qvec );
  this->event_config( this->peco );
  this->event_config( this->phco );
  this->event_config( this->vdco );
  this->event_config( this->vdxy ); // MVD hits in r-phi wafer 
  this->event_config( this->vdzt ); // MVD hits in z wafer.
  this->event_config( this->vcpl ); // association of a track NR=NTRACK

  this->event_config( this->vfhl ); // VDET final hit list bank
  
  this->event_config( this->vdht ); // VDET simulation hit 
  this->event_config( this->vufk ); // Vdxy/vdzt to FKIN truth 
  this->event_config( this->vfph );
  this->event_config( this->vflg );
  this->event_config( this->vdfk );
  this->event_config( this->vdgc );
}

/// Default constructor
alpha::output_edm4hep::event_t::event_t(data_access_t& da)
  : data(da)
{
}

/// Default destructor
alpha::output_edm4hep::event_t::~event_t()  {
}

edm4hep::MutableMCParticle
alpha::output_edm4hep::event_t::particle_mc(int32_t itk)  {
  if( itk >= qcde.KFMCT && itk <= qcde.KLMCT )  {
    auto* track = this->data.qvec.row<class qvec>(itk);
    itk = track->ktn();
  }
  auto itr = this->fkin2edm4hep_particles.find(itk);
  if( itr != this->fkin2edm4hep_particles.end() )  {
    return this->get_item<edm4hep::MutableMCParticle>(this->particles_mc, itr->second);
  }
  std::string err = "Non existing object: MutableMCParticle FKIN id: "+std::to_string(itk);
  throw std::runtime_error(err);
}

void alpha::output_edm4hep::event_t::process_mc_tracks()  {
  for( int itk = qcde.KFMCT; itk <= qcde.KLMCT; ++itk )  {
    auto*        track  = this->data.qvec.row<class qvec>(itk);
    auto         mcp    = this->particles_mc.create();
    const auto*  vstart = track->origin_vtx();
    const auto*  vend   = track->end_vtx();
    double       ptime  = 0e0;
    int32_t sim_status  = 0;
    int32_t fkin_track  = track->ktn();

    std::cout << track->to_string() << std::endl;
    this->alpha2edm4hep_particles[itk] = mcp.id();
    this->fkin2edm4hep_particles[fkin_track] = mcp.id();
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
  }

  /// Now that all MC particles are registered,
  /// we establish the mother-daughter relationships
  for(int itk = qcde.KFMCT; itk <= qcde.KLMCT; ++itk )  {
    auto* table = this->data.qvec.table<class qvec>();
    auto* track = table->row(itk);
    auto  part  = this->particle_mc(track->ktn());
    /// Assign the mothers
    for( uint32_t im=0; im < track->knmoth(); ++im )  {
      const auto& mot = this->particle_mc(track->kmoth(im));
      part.addToParents(mot);
    }
    /// Assign the daughters
    for( uint32_t id=0; id < track->kndau(); ++id )  {
      const auto& dau = this->particle_mc(track->kdau(id));
      part.addToDaughters(dau);
    }
  }
}

/// Convert charged tracks. They enter the catalog with their FRFT row number
void alpha::output_edm4hep::event_t::process_charged_tracks()  {
  for( int itk = qcde.KFCHT; itk <= qcde.KLCHT; ++itk )  {
    auto* track = this->data.qvec.row<class qvec>(itk);
    auto* frft  = track->frft();
    if( !frft )  {
      throw std::runtime_error("Error: No FRFT bank present");
    }
    int32_t frft_track = track->ktn();
    auto    trk = this->particles_reco.create();
    this->alpha2edm4hep_tracks[frft_track] = trk.id();
    trk.setType(track->kclass());
    trk.setNholes(0);                // ???
    trk.setChi2(frft->chis2());      // Should go to track state ??
    trk.setNdf(frft->numDegFree());  // Should go to track state ??

    edm4hep::TrackState state;
    state.location       = edm4hep::TrackState::AtIP;
    state.time           = 0e0;
    state.D0             = frft->d0();
    state.Z0             = frft->z0();
    state.phi            = frft->phi0();
    state.omega          = frft->inverseRadi();
    state.tanLambda      = frft->tanLambda();
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
  }
}

/// Create ECAL hit from PECO row
alpha::output_edm4hep::event_t::calo_deposit_t
alpha::output_edm4hep::event_t::get_peco_hit(int itk)  {
  /*
                                        Subschema: EobjPOTBanks                 
 +------+                                                                       
 | PECO |  Electromagnetic Calorimeter                                          
 +------+  Object (PEOB and PEOT are
           replaced by PECO and PEPT )
 ..............................................................
     1          I    Number of words/calobject (=10)
     2          I    Number of calobjects
 ..............................................................
      1    ER  F    ERaw             [0.0,300.000]
                       Raw Energy (Gev)
      2    E1  F    Esta1            [0.0,1.00]
                       Proportion of raw energy in stack 1
                       "Stack 1" is defined properly for the
                       overlap
                       region according to the tower number
      3    E2  F    Esta2            [0.0,1.00]
                       Proportion of raw energy in stack 2
                       "Stack 2" is defined properly for the
                       overlap
                       region according to the tower number
      4    TH  F    THeta            [0.0,3.1416]
                       Theta. The theta, phi of stacks 1 - 3
                       are weighted by the stack energy
      5    PH  F    PHi              [0.0,6.2833]
                       Phi. The theta, phi of stacks 1 - 3 are
                       weighted by the stack energy
      6    EC  F    ECorr            [0.0,300.000]
                       Energy (Gev) corrected for geometrical
                       effects (cracks, detector
                       overlap, dead storeys)
                       Set to ERaw in JULIA 224
      7    KD  I    KDrg             [0,255]
                       Region code ; see note ALEPH # 88-134
                       for a detailed description of this code
      8    CC  I    CCode            [0,255]
                       Correction code for the cluster
                       analysis method;
                       CCode =
                       0 : Standard
                       1 : Crack between modules
                       2 : Overlap between detectors
                       3 : Dead storey
      9    RB  I    RBits            [0,255]
                       Relation bits
                       RBITS =
                       0 : no relations
                       1 : charged track relation
                       2 : HCAL relation
                       3 : Both ch tk and HC relation
      10   PC  I    PCOB
                       CalObject number

  */
  auto it = this->alpha2edm4hep_peco.find(itk);
  if( it != this->alpha2edm4hep_peco.end() )  {
    return this->get_item<edm4hep::MutableCalorimeterHit>(this->hits_ecal, it->second);
  }
  auto* ahit = this->data.peco.row<class peco>(itk);
  auto  hit  = this->hits_ecal.create();
  PositionPolar pos(1e0, ahit->theta(), ahit->phi()); // Radius is unknown from bank
  uint64_t cell = ahit->pcOB();

  this->alpha2edm4hep_peco[itk] = hit.id();
  hit.setCellID( cell );
  hit.setTime( 0e0 );
  hit.setPosition( { pos.x(), pos.y(), pos.z() } );
  hit.setEnergy( ahit->ecorr() );
  hit.setEnergyError( std::abs(ahit->ecorr() - ahit->eraw()) );
  hit.setPosition( { pos.x(), pos.y(), pos.z() } );
  hit.setType(((  0x7 & ahit->rbits())) +
              ((  0x7 & ahit->ccode()) <<  3) +
              ((  0x7 & ahit->kdrg())  <<  6) +  // Bit 9 unused.
              ((0x3FF & int(ahit->esta1()*1000e0)) << 10) +
              ((0x3FF & int(ahit->esta2()*1000e0)) << 20));
  return hit;
}

/// Create HCAL cluster from PHCO row
alpha::output_edm4hep::event_t::calo_deposit_t
alpha::output_edm4hep::event_t::get_phco_hit(int itk)  {
  /*
                                        Subschema: HobjPOTBanks                 
 +------+                                                                       
 | PHCO |  Hadron Calorimeter Object                                            
 +------+  (PHOB and PHTO are replaced
           by PHCO )
 ..............................................................
     1          I    Number of words/calobject (=9)
     2          I    Number of calobjects
 ..............................................................
      1    ER  F    ERaw             [0.0,300.00]
                       Raw Energy (Gev)
      2    TH  F    THeta            [0.0,3.142]
                       Theta
      3    PH  F    PHi              [0.0,6.284]
                       Phi
      4    EC  F    ECorr            [0.0,300.00]
                       Energy (Gev) corrected for geometrical
                       losses
                       ( cracks , spacers and coil )
      5    KD  I    KDrg             [0,255]
                       Region code =1 in overlap region
                       = 0 otherwise
      6    CC  I    CCode            [0,255]
                       Correction code
                       (not filled in JULIA 224)
      7    RB  I    RBits            [0,255]
                       Relation bits
                       RBITS =
                       0 : no relations
                       1 : charged track relation
                       2 : ECAL relation
                       3 : Both ch tk and EC relation
      8    NF  I    NoiseFlag        [0,64]
                       Noise Flag from Topological Analysis
                       and Digital Pattern matching
                       bits 0-4 Top. An. (0=OK,1=Noise)
                       bit 5 Dig. Matching (0=OK,1=Noise)
      9    PC  I    PCOB
                       CalObject number
  */
  auto it = this->alpha2edm4hep_phco.find(itk);
  if( it != this->alpha2edm4hep_phco.end() )  {
    return this->get_item<edm4hep::MutableCalorimeterHit>(this->hits_hcal, it->second);
  }
  auto* ahit = this->data.phco.row<class phco>(itk);
  auto  hit  = this->hits_hcal.create();
  PositionPolar pos(1e0, ahit->theta(), ahit->phi()); // Radius unknown from bank
  uint64_t cell = ahit->pcOB();
  this->alpha2edm4hep_phco[itk] = hit.id();

  hit.setCellID( cell );
  hit.setTime( 0e0 );
  hit.setPosition( { pos.x(), pos.y(), pos.z() } );
  hit.setEnergy(ahit->ecorr());
  hit.setEnergyError( std::abs(ahit->ecorr() - ahit->eraw()) );
  hit.setType(((  0x7 & ahit->rbits())) +
              ((  0x7 & ahit->ccode()) <<  3) +
              ((  0x7 & ahit->noiseFlag())  <<  6));
  return hit;
}

/// Create VDET 3D hit from VDCO row
alpha::output_edm4hep::event_t::vdet_hit_t
alpha::output_edm4hep::event_t::get_vdco_hit(int itk)  {
  /**
     Subschema: VdetPOTBanks                 
     +------+                                                                       
     | VDCO |  VDET coordinated NR=0 (POT)                                          
     +------+
     ..............................................................
     1          I    Number of words/coord (=8)
     2          I    Number of coords
     ..............................................................
     1    WI  I    WaferIdent       [10000,29999]
     wafer identifier
     2    R0  F    R                [0.0,20.00000]
     R coordinate
     3    PH  F    PHi              [0.0,6.3000002]
     phi coordinate
     4    Z0  F    Z                [-20.0000,20.0000]
     Z coordinate
     5    SR  F    SigRphi2         [0.0,*]
     sigma of R-phi squared
     6    SZ  F    SigZ2            [0.0,*]
     sigma of z squared
     7    QF  I    QualityFlag      [0,100000]
     Qualtity flag
     8    TN  I    TrackNumber      [0,100000]
     associated track in FRFT
  */
  auto it = this->alpha2edm4hep_vdco.find(itk);
  if( it != this->alpha2edm4hep_vdco.end() )  {
    return this->get_item<edm4hep::MutableTrackerHit3D>(this->hits_vdco, it->second);
  }
  auto* ahit = this->data.vdco.row<class vdco>(itk);
  auto  hit  = this->hits_vdco.create();
  this->alpha2edm4hep_vdco[itk] = hit.id();
  double sigrphi2 = ahit->sigRphi2();   // 
  double sigz2    = ahit->sigZ2();      // 
  PositionRhoZPhi pos(_LEN(ahit->r()), _LEN(ahit->z()), ahit->phi());
  PositionRhoZPhi err(std::sqrt(sigrphi2), std::sqrt(sigz2), std::sqrt(sigrphi2));
  uint64_t cell = ahit->waferIdent();

  hit.setCellID( cell );
  hit.setTime( _TIM(0e0) );
  hit.setEDep( _ENE(0e0) );
  hit.setEDepError( _ENE(0e0) );
  hit.setPosition( { float(pos.x()), float(pos.y()), float(pos.z()) } );
  hit.setCovMatrix( {
      err.x()*err.x(),
      err.y()*err.x(), err.y()*err.y(),
      err.z()*err.x(), err.z()*err.y(), err.z()*err.z() } );
  hit.setQuality(ahit->qualityFlag());
  hit.setType((ahit->waferIdent()) +
              (ahit->trackNumber() << 15));
  return hit;
}

/*
                                        Subschema: VdetGALBanks                 
 +------+                                                                       
 | VDFK |  Vdco to FKIN truth relation                                          
 +------+  (monte Carlo) (hits
           associated to reconstructed
           tracks)
 ..............................................................
     1          I    Number of words/relation (=6)
     2          I    Number of relations
 ..............................................................
    1-2    PC  F    PercentCharge    [0.0,2.00]
                       Fraction of the cluster charge
                       due to this track for the 2 views
    3-4    SC  I    StripsCount      [1,*]
                       Number of strips in this cluster
                       fired by this track for the 2 views
      5    FK  I    FKIN
                       Index of FKIN track
      6    VD  I    VDCO
                       Index of Vdet cluster in VDCO bank
 
==============================================================
                                        Subschema: VdetPOTBanks                 
 +------+                                                                       
 | VCOM |  VDET common mode monitor                                             
 +------+  bank, NR=VHLS number (POT)
           This bank is parallel to the
           VFHL bank storing common mode
           mean and sigma per hit in 250
           e units ComMode = CM mean +
           ISHFT(CM sigma,16)
 ..............................................................
     1          I    number of words/hit (=1)
     2          I    number of hit
 ..............................................................
      1    CM  I    ComMode          [0,*]
                       Common mode of cluster

==============================================================
                                        Subschema: VdetPOTBanks                 
 +------+                                                                       
 | VDGC |  VDET global clusters : not                                           
 +------+  multiplexed NR=0
 ..............................................................
     1          I    Number of words/cluster (=4)
     2          I    Number of clusters
 ..............................................................
      1    MD  I    MoDule           [-24,24]
                       Signed module number; sign=Z
      2    VI  I    VIew             [1,2]
                       1=Z; 2=rphi
      3    PH  F    PulseHeight      [0.0,*]
                       Total charge
      4    NA  I    NAss             [0,*]
                       Number of associated tracks

==============================================================
                                       Subschema: VdetPOTBanks                 
 +------+                                                                       
 | VFPH |  VDET final pulse height bank,                                        
 +------+  NR=VHLS number (POT) This
           bank links up the VFHL bank
           Pulseheight is stored in 250
           e units
 ..............................................................
     1          I    number of words/strip (=1)
     2          I    number of strips
 ..............................................................
      1    PH  I    PuHeight         [0,*]
                       Pulseheight
==============================================================
                                        Subschema: VdetPOTBanks                 
 +------+                                                                       
 | VDMR |  Hit multiplexing relation                                            
 +------+  (bank is parallel to
           VDXY/VDZT) NR = same as
           VDXY/VDZT + view OK
 ..............................................................
     1          I    Number of words/hit (=1)
     2          I    Number of hits in VDXY/VDZT
 ..............................................................
      1    VD  I    VD               [0,*]
                       Index of global cluster in VDGC

==============================================================

*/

/// Create VDET 3D hit from VDXY row
void alpha::output_edm4hep::event_t::process_vdxy(const bos77::bank* bank)  {
  /*
                                        Subschema: VdetPOTBanks                 
 +------+                                                                       
 | VDXY |  MVD hits in r-phi wafer.                                             
 +------+  (POT)
           NR=ILAYER*10000+IZED*1000+IPH-
           I*10+(IVIEW=1) Bank number is
           packed/unpaked by
           VAENWA,VADEWA computed with
           values starting from 0
 ..............................................................
     1          I    Number of words/hit (=12)
     2          I    Number of hits per wafer
 ..............................................................
      1    XC  F    XCor             [-99.0000,99.0000]
                       x coordinate of hit
      2    YC  F    YCor             [-99.0000,99.0000]
                       y coordinate of hit
      3    UC  F    UCor             [-5.00000,5.00000]
                       local wafer coordinate
      4    SX  F    SigmaX           [0.0,5.00000]
                       sigma on x coord
      5    SY  F    SigmaY           [0.0,5.00000]
                       sigma on y coord
      6    SU  F    SigmaU           [0.0,5.00000]
                       sigma on u coord
      7    PH  F    PulseHeight      [0.0,*]
                       pulseheight
      8    QF  I    QualityFlag      [0,9999999]
                       quality flag
      9    NA  I    NAss             [0,1000000]
                       number of ass. tracks
      10   IP  I    IPoint           [0,1000000]
                       pointer to first associated track in
                       VLST
      11   IW  I    IWaf             [0,1000000]
                       VFHL bank number
      12   IH  I    IHit             [0,1000000]
                       Hit number in VFHL bank
  */
  auto*    table = (object_table<class vdxy>*)bank;
  uint32_t NR    = table->bank_header::row();
  uint64_t iview = (NR%10);
  uint64_t iphi  = ((NR/10)%100);
  uint64_t iz    = ((NR/1000)%10);
  uint64_t layer = ((NR/10000));
  std::stringstream log;

  if( this->data.vdxy.debug )  {
    log << "VDXY: Table size: " << std::setw(4) << table->size() << " Wafers: ";
  }
  for( uint32_t i=0; i < table->size(); ++i )  {
    auto* ahit = table->at(i);
    auto  hit  = this->hits_vdxy.create();
    // this->alpha2edm4hep_vdxy[itk] = hit.id();
    Position pos(_LEN(ahit->xcor()), _LEN(ahit->ycor()), ahit->ucor());
    Position err(_LEN(ahit->sigmaX()), _LEN(ahit->sigmaY()), _LEN(ahit->sigmaU()));
    uint64_t waf  = uint64_t(ahit->iwaf());
    uint64_t cell = iview  + (waf << 4) + (layer << 16) + (iphi << 32) + (iz << 48);

    if( this->data.vdxy.debug )  {
      log << std::setw(2)  << std::right << waf << "/"
          << std::setw(10) << std::left  << cell << " ";
    }
    hit.setCellID( cell );
    hit.setTime( _TIM(0e0) );
    hit.setEDep( ahit->pulseHeight() );
    hit.setEDepError( _ENE(0e0) );
    hit.setPosition( { float(pos.x()), float(pos.y()), float(pos.z()) } );
    hit.setCovMatrix( {
        err.x()*err.x(),
        err.y()*err.x(), err.y()*err.y(),
        err.z()*err.x(), err.z()*err.y(), err.z()*err.z() } );
    hit.setQuality(ahit->qualityFlag());
    hit.setType( (0xFFFFF & ahit->ipoint()) +
                ((0xFFFFF & ahit->ihit()) << 20));
  }
  if( this->data.vdxy.debug )  {
    ::printf("%s\n", log.str().c_str());
  }
}

/// Create VDET 3D hit from VDCO row
void alpha::output_edm4hep::event_t::process_vdzt(const bos77::bank* bank)  {
  /*
                                        Subschema: VdetPOTBanks
 +------+                                                                       
 | VDZT |  MVD hits in z wafer. (POT)                                           
 +------+  NR=ILAYER*10000+IZED*1000+IPH-
           I*10+(IVIEW=1) Bank number is
           packed/unpaked by
           VAENWA,VADEWA computed with
           values starting from 0
 ..............................................................
     1          I    Number of words/hit (=10)
     2          I    Number of hits per wafer
 ..............................................................
      1    ZC  F    ZCor             [-99.0000,99.0000]
                       z coordinate of hit
      2    WC  F    WCor             [-5.0000,5.0000]
                       local wafer coordinate
      3    SZ  F    SigmaZ           [0.0,5.00000]
                       sigma on x coord
      4    SW  F    SigmaW           [0.0,5.00000]
                       sigma on w coord
      5    PH  F    PulseHeight      [0.0,9999.0000]
                       pulseheight
      6    QF  I    QualityFlag      [0,9999999]
                       quality flag
      7    NA  I    NAss             [0,1000000]
                       number of ass. tracks
      8    IP  I    IPoint           [0,1000000]
                       pointer to first associated track in
                       VLST
      9    IW  I    IWaf             [0,1000000]
                       VFHL bank number
      10   IH  I    IHit             [0,1000000]
                       Hit number in VFHL bank
   */
  auto* table    = (object_table<class vdzt>*)bank;
  uint32_t NR    = table->bank_header::row();
  uint64_t iview = (NR%10);
  uint64_t iphi  = ((NR/10)%100);
  uint64_t iz    = ((NR/1000)%10);
  uint64_t layer = ((NR/10000));
  std::stringstream log;

  if( this->data.vdzt.debug )  {
    log << "VDZT: Table size: " << std::setw(4) << table->size() << " Wafers: ";
  }
  for( uint32_t i=0; i<table->size(); ++i )  {
    auto* ahit = table->at(i);
    auto  hit  = this->hits_vdzt.create();
    //this->alpha2edm4hep_vdzt[itk] = hit.id();
    Position pos(_LEN(ahit->zcor()), _LEN(ahit->wcor()), 0e0);
    Position err(_LEN(ahit->sigmaZ()), _LEN(ahit->sigmaW()), 0e0);
    uint64_t waf  = uint64_t(ahit->iwaf());
    uint64_t cell = iview + (iphi << 4) + (iz << 10) + (layer << 14) + (waf << 18);
    if( this->data.vdzt.debug )  {
      log << std::setw(2)  << std::right << waf << "/"
          << std::setw(10) << std::left  << cell << " ";
    }
    hit.setCellID( cell );
    hit.setTime( _TIM(0e0) );
    hit.setEDep( ahit->pulseHeight() );
    hit.setEDepError( _ENE(0e0) );
    hit.setPosition( { float(pos.x()), float(pos.y()), float(pos.z()) } );
    hit.setCovMatrix( {
        err.x()*err.x(),
        err.y()*err.x(), err.y()*err.y(),
        err.z()*err.x(), err.z()*err.y(), err.z()*err.z() } );
    hit.setQuality(ahit->qualityFlag());
    hit.setType( (0xFFFFF & ahit->ipoint()) +
                ((0xFFFFF & ahit->ihit()) << 20));
  }
  if( this->data.vdzt.debug )  {
    ::printf("%s\n", log.str().c_str());
  }
}

/// Process information from VCPL bank
void alpha::output_edm4hep::event_t::process_vcpl(const bos77::bank* bank)  {
  /*
                                        Subschema: VdetPOTBanks                 
 +------+                                                                       
 | VCPL |  association of a track,                                              
 +------+  NR=NTRACK (POT)
 ..............................................................
     1          I    Number of words/hit (=5)
     2          I    Number of hits
 ..............................................................
      1    XB  I    nrXyBank         [10000,29999]
                       Nr of VDXY bank
      2    NX  I    NXy              [0,1000]
                       XY hit number
      3    ZB  I    nrZBank          [10000,30000]
                       Nr of VDZT bank
      4    NZ  I    NZ               [0,1000]
                       Z hit number
      5    C2  F    Chi2             [0.0,*]
                       chi**2 of association
  */
  auto* table    = (object_table<class vcpl>*)bank;
  std::stringstream log;

  this->data.vcpl.debug = true;
  if( this->data.vcpl.debug )  {
    log << "VCPL: Table size: " << std::setw(4) << table->size() << std::endl;
  }
  for( uint32_t i=0; i<table->size(); ++i )  {
    auto* vcpl = table->at(i);
    int32_t frft_num = table->bank_header::row();
    auto obj_id = this->alpha2edm4hep_tracks[frft_num];
    if( this->data.vcpl.debug )  {
      log << std::setw(4) << std::left << i << ": "
          << " FRFT:"  << std::setw(3) << std::left  << frft_num
          << " #xy:"   << std::setw(8) << std::right << vcpl->nrXyBank()
          << " hit:"   << std::setw(4) << std::right << vcpl->nxy()
          << " #z:"    << std::setw(8) << std::right << vcpl->nrZBank()
          << " hit:"   << std::setw(4) << std::right << vcpl->nz()
          << " Chi2:"  << double(vcpl->chi2())
          << " Obj:"   << obj_id << std::endl;
    }
    /// Associate VDXY/VDZT hits to reconstructed track
  }
  if( this->data.vcpl.debug )  {
    ::printf("%s", log.str().c_str());
  }
}

/// Process information from VFHL bank
void alpha::output_edm4hep::event_t::process_vfhl(const bos77::bank* bank)  {
  /*
                                        Subschema: VdetPOTBanks                 
 +------+                                                                       
 | VFHL |  VDET final hit list bank,                                            
 +------+  NR=VHLS number (POT) Hit
           address is computed as in
           VHLS packed/unpacked by
           VADDPK/VADDUN This bank is
           needed to unpack the
           VFPH,VFLG banks
 ..............................................................
     1          I    number of words/hit (=1)
     2          I    number of final hits
 ..............................................................
      1    HA  I    HitAdd           [0,*]
                       Hit address =Nstrip*2**18 +
                       Ilayer*2**17 + Iwafer*2**15 +
                       Iview*2**10 + Istrip

  */
}

#if 0

VDET BANKS (POT) :
  VDXY   VdetPOTBanks: MVD hits in r-phi wafer.  (POT)
  VDZT   VdetPOTBanks: MVD hits in z wafer. (POT) 
  VCPL   VdetPOTBanks: association of a track, NR=NTRACK (POT)
  VDCO   VdetPOTBanks: VDET coordinated NR=0 (POT)      
  
  
  +++ AUBLIS +++    C list contains the following banks: 
  RUNR RUNH KRUN KRAN KJOB PART KCAR RLEP KLUN KPAR KLIN MSTU PARU MSTJ PARJ PMA1 PMA2 PMA3 PMA4 MDM1
  GADM GRPL GMOB KORL TSIM LALI LCAL EZTH VOLU ACUT AFID AJOB ARUN APRO ASIM ILIV ECHE EHGF SMPD RHAH
  ZPFR JCON PTUN

  +++ AUBLIS +++    E list contains the following banks: 
  EVEH ASEV DHEA EJET FZFR HROA KEVH KMAR KWGT LUPA MUEX MUID PCOB PEPT PFXT PGID PIDI PYER REVH RTLS
  SILH TRIK VCPL VDMR VDXY VDZT X1IP X1RG X2DF XTCN YLV0 YNLI YNVH YSMO YV0V ECRQ EFOL EGPR EIDT ETDI
  ETKC EWHE FKIN FPOI FRID FVER HLTU HMAD HPDI HTUB PASL PCHY PCOI PCPA PCRL PECO PEHY PEMH PEST PEWI
  PFRF PFRT PGAC PHCO PHHY PHST PITM PPDS PPOB PPRL PRPW PT2X PTBC PTEX PTNC PTPX PTST PYFR TWZZ VCOM
  VDCO VDFK VDGC VDHT VFHL VFLG VFPH VUFK
  ;
#endif
  
/// Create VDET simulation hit from VDHT row (produced by GALEPH)
edm4hep::MutableSimTrackerHit alpha::output_edm4hep::event_t::get_vdht_simhit(int itk)  {

  /**
     Subschema: VdetGALBanks                 
 +------+                                                                       
 | VDHT |  VDet HiT list NR=0 (GAL)                                             
 +------+
 ..............................................................
     1          I    Number of words/hit (=10)
     2          I    Number of hits
 ..............................................................
      1    TN  I    TrackNumber      [1,999]
                       Galeph track number
      2    LN  I    LayerNumber
                       Layer number
      3    PN  I    PhiNumber
                       phi coordinate
      4    XE  F    XEntry
                       X-entry point
      5    YE  F    YEntry
                       Y-entry point
      6    ZE  F    ZEntry
                       Z-entry point
      7    XL  F    XLast
                       X-exit point
      8    YL  F    YLast
                       Y-exit point
      9    ZL  F    ZLast
                       Z-exit point
      10   ER  F    EnergyReleased
                       Energy released
  */
  auto it = this->alpha2edm4hep_vdht.find(itk);
  if( it != this->alpha2edm4hep_vdht.end() )  {
    return this->get_item<edm4hep::MutableSimTrackerHit>(this->simhits_vdht, it->second);
  }
  auto* ahit = this->data.vdht.row<class vdht>(itk);
  auto  hit  = this->simhits_vdht.create();
  this->alpha2edm4hep_vdht[itk] = hit.id();

  Position  pos(_LEN(ahit->xentry()), _LEN(ahit->yentry()), _LEN(ahit->zentry()));
  Position  end(_LEN(ahit->xlast()),  _LEN(ahit->ylast()),  _LEN(ahit->zlast()));
  uint64_t  cell = ahit->layerNumber() + 1000*ahit->phiNumber(); // probably wrong....
  Direction dir  = (end-pos);

  hit.setEDep(_ENE(ahit->energyReleased()));
  hit.setTime(_TIM(0e0));
  hit.setPosition( { pos.x(), pos.y(), pos.z() } );
  hit.setPathLength(_LEN(dir.r()));
  dir = dir.Unit() * hit.getEDep();
  hit.setMomentum( { dir.x(), dir.y(), dir.z() } );
  hit.setCellID( cell );
  hit.setQuality( ahit->trackNumber() );
  return hit;
}

/// Process VDXY/VDZT MC truth relations from VUFK row
void alpha::output_edm4hep::event_t::process_vufk(const bos77::bank* bank)  {
  /*
                                        Subschema: VdetGALBanks                 
 +------+                                                                       
 | VUFK |  Vdxy/vdzt to FKIN truth                                              
 +------+  relation (monte Carlo) (hits
           unused by pattern
           reconstruction)
 ..............................................................
     1          I    Number of words/relation (=6)
     2          I    Number of relations
 ..............................................................
      1    BN  I    BankNumber       [0,29999]
                       VDXY or VDZT bank number
      2    HN  I    HitNumber        [0,*]
                       VDXY or VDZT hit number
      3    VI  I    View             [1,2]
                       View
      4    PC  F    PercentCharge    [0.0,1.00]
                       Fraction of the cluster charge
                       due to this track
      5    SC  I    StripsCount      [1,*]
                       Number of strips in this cluster
                       fired by this track
      6    FK  I    FKIN
                       Index of FKIN track
  */
#if 0
  auto* table    = (object_table<class vufk>*)bank;
  std::stringstream log;

  if( this->data.vufk.debug )  {
    log << "VUFK: Table size: " << std::setw(4) << table->size() << " Wafers: ";
  }
  for( uint32_t i=0; i<table->size(); ++i )  {
    auto* ahit = table->at(i);
    auto  hit  = this->hits_vufk.create();
    //this->alpha2edm4hep_vufk[itk] = hit.id();
    Position pos(_LEN(ahit->zcor()), _LEN(ahit->wcor()), 0e0);
    Position err(_LEN(ahit->sigmaZ()), _LEN(ahit->sigmaW()), 0e0);
    uint64_t waf  = uint64_t(ahit->iwaf());
    uint64_t cell = iview + (iphi << 4) + (iz << 10) + (layer << 14) + (waf << 18);
    if( this->data.vufk.debug )  {
      log << std::setw(2)  << std::right << waf << "/"
          << std::setw(10) << std::left  << cell << " ";
    }
    hit.setCellID( cell );
    hit.setTime( _TIM(0e0) );
    hit.setEDep( ahit->pulseHeight() );
    hit.setEDepError( _ENE(0e0) );
    hit.setPosition( { float(pos.x()), float(pos.y()), float(pos.z()) } );
    hit.setCovMatrix( {
        err.x()*err.x(),
        err.y()*err.x(), err.y()*err.y(),
        err.z()*err.x(), err.z()*err.y(), err.z()*err.z() } );
    hit.setQuality(ahit->qualityFlag());
    hit.setType( (0xFFFFF & ahit->ipoint()) +
                ((0xFFFFF & ahit->ihit()) << 20));
  }
  if( this->data.vufk.debug )  {
    ::printf("%s\n", log.str().c_str());
  }
#endif
}

/// Default constructor
alpha::output_edm4hep::output_edm4hep()   {
  this->data = std::make_unique<data_access_t>();
}

/// Default destructor
alpha::output_edm4hep::~output_edm4hep()   {
  this->data.reset();
}

/// Start event saving
void alpha::output_edm4hep::begin_event()  {
  this->data->event_config();
  this->event = std::make_unique<output_edm4hep::event_t>(*this->data);
}

/// End event saving
void alpha::output_edm4hep::end_event()  {
  
  //this->podio_frame.put( std::move(this->event->particles_mc)," MCParticles" );
  this->event.reset();
}

/// Convert event data
void alpha::output_edm4hep::convert_event()  {
  auto& dat = *this->data;
  
  /// First create the Monte-Carlo particles and fill the properties
  this->event->process_mc_tracks();
  /// Create the charged reconstructed particles and fill the properties
  this->event->process_charged_tracks();
  /// Create all ECAL clusters from PECO
  for( uint32_t i=1; i <= dat.peco.table<class peco>()->size(); ++i )
    this->event->get_peco_hit(i);
  /// Create all HCAL clusters from PHCO
  for( uint32_t i=1; i <= dat.phco.table<class peco>()->size(); ++i )
    this->event->get_phco_hit(i);
  /// Create all VDET clusters from VDCO
  for( uint32_t i=1; i <= dat.vdco.table<class vdco>()->size(); ++i )
    this->event->get_vdco_hit(i);
  /// Create all VDET simulation hits from VDHT
  for( uint32_t i=1; i <= dat.vdht.table<class vdht>()->size(); ++i )
    this->event->get_vdht_simhit(i);
  /// Process the list od VDXY bank:
  for( const auto* b=bos77::get_bank("VDXY", 0); b; b=b->knext() )
    this->event->process_vdxy(b);
  /// Process the list od VDZT bank:
  for( const auto* b=bos77::get_bank("VDZT", 0); b; b=b->knext() )
    this->event->process_vdzt(b);

  bos77::print_banks_of_type("VDCO");
  bos77::print_banks_of_type("VFHL");
  bos77::print_banks_of_type("VCPL");
  bos77::print_banks_of_type("VUFK");
  /// Process the list od VDZT bank:
  for( int itk = qcde.KFCHT; itk <= qcde.KLCHT; ++itk )  {
    auto* track = this->data->qvec.row<class qvec>(itk);
    int32_t julia_track = track->ktn();
    const auto* bank = bos77::get_bank("VCPL",julia_track);
    if( bank )  {
      this->event->process_vcpl(bank);
    }
  }
  
  /// Now we handle the relations between the tracks, the hits, etc.
  /// MC particles
  this->fill_particles_mc();
  /// Reconstructed tracks
}

/// After creation add all links to the MC particles
void alpha::output_edm4hep::fill_particles_mc()   {
}

/// After creation add all links to the reconstructed tracks
void alpha::output_edm4hep::fill_tracks()   {
}

/// After creation add all links to the ECAL clusters from PECO
void alpha::output_edm4hep::fill_hits_ecal()   {
  const auto* clusters = this->data->peco.table<class peco>();
  for(uint32_t i=1; i <= clusters->size(); ++i )   {
    //const auto* cluster = clusters->at(i);
    
  }
}

/// After creation add all links to the HCAL clusters from PHCO
void alpha::output_edm4hep::fill_hits_hcal()   {
}


#include <alpha/processor.h>
namespace {
  std::unique_ptr<alpha::output_edm4hep> conv;
}

/// Framework event callback
template <> void alpha::processor<alpha::output_edm4hep>::handle_event(constants_t& /* par */)  {
  if( debug ) ::printf("output_edm4hep:  +++++++  Calling %s   KNEVT:%d\n", __FUNCTION__, qcde.KNEVT);
  ::printf("+++++++  Calling %s\n", __FUNCTION__);
  conv->begin_event();
  conv->convert_event();
  conv->end_event();
}

/// Framework termination callback
template <> void alpha::processor<alpha::output_edm4hep>::terminate()  {
  ::printf("+++++++  Calling %s\n", __FUNCTION__);
  conv.reset();
}

/// Framework termination callback
template <> void alpha::processor<alpha::output_edm4hep>::initialize()  {
  ::printf("+++++++  Calling %s\n", __FUNCTION__);
  conv = std::make_unique<alpha::output_edm4hep>();
}
