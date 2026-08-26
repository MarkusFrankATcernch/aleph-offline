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


#if 0

POT banks:

**  MC Banks:
  KEVH   EventKINGALBanks Event generator status(NR=0)
  KMAR   EventKINGALBanks Random generator status(NR=0)
  KGWT   EventKINGALBanks Kingal event partial WeiGhTs(NR=0)
  FKIN   FXXX             Fxxx monte carlo track bank.
  FPOI   FXXX             Fxxx POInters bank relating KINE track number to FKIN number
  FVER   FXXX             Fxxx monte carlo VERtex bank.

  PASL   ItcPOTBanks      JULIA track to FKIN track association list (accessed via PITM bank).
  PITM   ItcPOTBanks      JULIA track to FKIN track association (using ITC + TPC coordinate information).

  VDFK   VdetGALBanks     Vdco to FKIN truth relation (monte Carlo)
  VDHT   VdetGALBanks     VDet HiT list NR=0 (GAL)
  VUFK   VdetGALBanks     Vdxy/vdzt to FKIN truth relation (monte Carlo) (hits unused by pattern reconstruction)
  PEMH   CobjPOTBanks     Relation between ECAL and LCAL objects and FKIN tracks

**  LUPA   LUMINOSITY       Luminosity parameter bank
  
**  VDET banks:
  VDCO   VdetPOTBanks     VDET coordinated NR=0 (POT)
  VDXY   VdetPOTBanks     MVD hits in r-phi wafer.  (POT)        NR=ILAYER*10000+IZED*1000+IPH-I*10+(IVIEW=1)
  VDZT   VdetPOTBanks     MVD hits in z wafer. (POT)             NR=ILAYER*10000+IZED*1000+IPH-I*10+(IVIEW=1)
  VDGC   VdetPOTBanks     VDET global clusters : not multiplexed NR=0
  VCPL   VdetPOTBanks     association of a track,                NR=NTRACK (POT)
  VFHL   VdetPOTBanks     VDET final hit list bank,              NR=VHLS number (POT)
  VFLG   VdetPOTBanks     VDET strip flag bank,                  NR=VHLS number (POT)
  VFPH   VdetPOTBanks     VDET final pulse height bank,          NR=VHLS number (POT)

**  ITC:
  PIDI   ItcPOTBanks      Packed Itc DIgitisings (NR=0)

**  TPC:
  TWZZ   TpcJULBanks      Tpc Wire Z coordinates
  TRIK   TpcJULBanks      TRack Extrapolation from Kalman filter
  PCOI   TpcPOTBanks      Supplemental TPC coordinate information (NR=0).
  PT2X   TpcPOTBanks      Production output Tpc track dE/dX for overlapping tracks (NR=0)
  PTBC   TpcPOTBanks      Production output Tpc Bad Coordinates (NR=0)
  PTEX   TpcPOTBanks      Production output Tpc track dE/dX (NR=0)
  PTPX   TpcPOTBanks      Production output Tpc track pad dE/dX (NR=0)
  PTNC   TpcPOTBanks      Production output Tpc pad Coordinates (NR=0). (UNCORRECTED)
  
**  ECAL:
  ETKC   EcalRUNConsts    Ecal Tower Killed by Cleaning   
  ETDI   EcalRAWBanks     Ecal Tower DIgits NR=0. (RAW)
  EWHI   EcalRAWBanks     Ecal Wire HEader bank NR=0. (GAL)

  PEWI   EcalPOTBanks     Ecal Wire plane Digits Raw bank: EWDI
  PEPT   EobjPOTBanks     Electromagnetic calorimeter object Phi and Theta values
  PECO   EobjPOTBanks     Electromagnetic Calorimeter Object (Mini: DECO)
  EGPR   EcalJULBanks     Electromagnetic Gamma(gampek)  Pest Row component
  PEST   EobjPOTBanks     Pot Electromagnetic calorimeter SToreys
  PEHY   EobjPOTBanks     Electromagnetic calobject HYpothesis
  PRPW   EcalPOTBanks     Pot bank for Relation Pad/Wire
  
  PGID   EcalJULBanks     Pot electomagnetic Gamma IDentification (Mini: DGID)
  PGAC   EcalJULBanks     Photons from GAMPEX (oct 94) (Mini: DGAC)
  EIDT   ElidJULPOTBanks  Electron IDenTification. NR=0     

**  HCAL:
  HLTU   HcalJULBanks     True MC longit. coordinate of Hcal hit
  HMAD   HcalJULBanks     Hcal Muon tracks Association
  HPDI   HcalRAWBanks     Hcal Plane DIgits (RawData)
  HTUB   HcalJULBanks     Hcal TUBes data (from Julia prepare data)
  PHCO   HobjPOTBanks     Hadron Calorimeter Object (Mini: DHCO)
  PHHY   HobjPOTBanks     Hadronic calobject HYpothesis
  PPDS   HobjPOTBanks     Pot Pattern DiScriminant analysis
  PPOB   HobjPOTBanks     hadronic digital Pattern OBject

**  MUON:
  MUEX   MuonRAWGALJULPOT MU chambers EXtrapol data for current track
  MUID   MuonRAWGALJULPOT MUon IDdentification data for current track

** CAL objects:
  PCOB   CobjPOTBanks     Calorimeter OBject
  PCHY   CobjPOTBanks     Calobject HYpothesis  
  PCPA   CobjPOTBanks     Calobject neutral PArticle
  PCRL   CobjPOTBanks     Calobject ReLations (to FRFT, EC, HC)
  PPRL   CobjPOTBanks     hypothesis Particle ReLations

  PYER   YvertexPOTBanks  Vertex position. NR=0. (POT)      
  PYFR   YvertexPOTBanks  eset to hold n:m relation between vertices and FRFT as well as YNFT tracks.

  FRID   JULPOTFitTrack   Particle identification bank for charged tracks
  PFRF   JULPOTFitTrack   Global Geometrical track FiT NR=0. (POT)
  
  PFXT   JULPOTFitTrack   POT Fit eXtra Track information NR=0.(POT)
  SILH   ScalLUMI         Sical Luminosity parameter                                           
  
** Headers:  
  DHEA   MiniDSTBanks     Header containing General Event Information from Reconstruction.
  EJET   EflowJULPOTBanks Eflow JET bank
  EFOL   EflowJULPOTBanks Energy FlOw eLements
  REVH   StatusJULBanks   Reconstructio EVent Header
  
  
  
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
#include <edm4hep/RawCalorimeterHit.h>
#include <edm4hep/CalorimeterHitCollection.h>
#include <edm4hep/SimTrackerHitCollection.h>
#include <edm4hep/RawCalorimeterHitCollection.h>
#include <edm4hep/SimCalorimeterHitCollection.h>
#include <edm4hep/CaloHitContributionCollection.h>
#include <edm4hep/TrackMCParticleLinkCollection.h>
#include <edm4hep/GeneratorEventParametersCollection.h>
#include <edm4hep/TrackerHitSimTrackerHitLinkCollection.h>
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
#include <alpha/vdco.h>
#include <alpha/vdht.h>
#include <alpha/vdxy.h>
#include <alpha/vdzt.h>
#include <alpha/vcpl.h>
#include <alpha/vufk.h>
#include <alpha/vdfk.h>
#include <alpha/pasl.h>
#include <alpha/pitm.h>
#include <alpha/peco.h>
#include <alpha/phco.h>
#include <alpha/pgac.h>
#include <alpha/pewi.h>


/// podio include files
#include <podio/Frame.h>
#include <podio/Writer.h>
#include <podio/podioVersion.h>
#include <podio/CollectionBase.h>
#include <podio/FrameCategories.h>

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


namespace  {
  template<typename T> std::string to_string(const T& val, const char* fmt)  {
    char text[256];
    ::snprintf(text, sizeof(text), fmt, val);
    return { text };
  }
}

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
      bnk.data = (int32_t*)bos77::get_bank_pointer_from_namind( bnk.nami );
    }

  public:
    bank_access_t qvec;  // QVEC table bank
    bank_access_t frft;  // FRFT table bank
    bank_access_t pitm;  // PITM table bank
    bank_access_t pasl;  // PASL table bank

    bank_access_t peco;  // PECO table bank
    bank_access_t phco;  // PHCO table bank
    bank_access_t pgac;  // PGAC table bank
    bank_access_t pewi;  // PEWI table bank
    
    bank_access_t vdco;  // VDET coordinated NR=0 (POT) --> FRFT
    bank_access_t vdxy;  // MVD hits in r-phi wafer.       NR=ILAYER*10000+IZED*1000+IPH-I*10+(IVIEW=1)
    bank_access_t vdzt;  // MVD hits in z wafer. (POT)     NR=ILAYER*10000+IZED*1000+IPH-I*10+(IVIEW=1)
    bank_access_t vcpl;  // association of a track NR=NTRACK (POT)

    bank_access_t vfhl;  // VDET final hit list bank,      NR=VHLS number (POT)
    bank_access_t vfph;  // VDET final pulse height bank,  NR=VHLS number (POT)
    bank_access_t vflg;  // VDET strip flag bank,          NR=VHLS number (POT)
    bank_access_t vufk;  // Vdxy/vdzt to FKIN truth relation (monte Carlo)
    bank_access_t vdfk;  // VDFK: VDCO      to FKIN truth relation
    bank_access_t vdht;  // VDet HiT list NR=0 (GAL)
    bank_access_t vdgc;  // VDET global clusters

    bool debug_mc_particles = false;
    bool debug_charged_tracks = false;
    
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

    enum hit_types_t  {
      VDET_COORDINATE = 0,
      VDET_HIT_RPHI = 1,
      VDET_HIT_Z = 2,
      VDET_HIT_XY = 4,
      VDET_HIT_ZT = 8,

      VDET_WAFER_RPHI = VDET_HIT_RPHI,
      VDET_WAFER_Z = VDET_HIT_Z,
      
      NONE
    };

    using mcparticle_t = edm4hep::MutableMCParticle;
    using trackerhits_t = edm4hep::TrackerHit3DCollection;
    using simtrackerhits_t = edm4hep::SimTrackerHitCollection;
    using track2mctrack_t = edm4hep::TrackMCParticleLinkCollection;
    using track_hit2mchit_t = edm4hep::TrackerHitSimTrackerHitLinkCollection;
    
    edm4hep::EventHeader                 event_header            {  };
    edm4hep::MCParticleCollection        particles_mc            {  };
    edm4hep::TrackCollection             particles_reco          {  };
    track2mctrack_t                      rel_part_reco_mc        {  };

    edm4hep::TrackCollection             calorimeter_objects     {  };

    trackerhits_t                        hits_vdxy               {  };
    trackerhits_t                        hits_vdzt               {  };
    edm4hep::TrackerHitPlaneCollection   mvd_hits                {  };

    trackerhits_t                        hits_vdco               {  };
    simtrackerhits_t                     simhits_vdht            {  };

    simtrackerhits_t                     simhits_vufk_xy         {  };
    simtrackerhits_t                     simhits_vufk_z          {  };
    track_hit2mchit_t                    rel_vdxy_vufk           {  };
    track_hit2mchit_t                    rel_vdzt_vufk           {  };

    simtrackerhits_t                     simhits_vdfk_rphi       {  };
    simtrackerhits_t                     simhits_vdfk_z          {  };
    track_hit2mchit_t                    rel_vdco_vdfk_z         {  };
    track_hit2mchit_t                    rel_vdco_vdfk_rphi      {  };

    
    edm4hep::CalorimeterHitCollection    hits_ecal               {  };
    edm4hep::RawCalorimeterHitCollection hits_ecal_wire          {  };
    
    edm4hep::CalorimeterHitCollection    hits_hcal               {  };

    std::map<int, std::size_t>           alpha2edm4hep_particles {  };
    std::map<int, std::size_t>           fkin2edm4hep_particles  {  };
    std::map<int, std::size_t>           alpha2edm4hep_charged   {  };
    std::map<int, std::size_t>           alpha2edm4hep_peco      {  };
    std::map<int, std::size_t>           alpha2edm4hep_phco      {  };
    std::map<int, std::size_t>           alpha2edm4hep_vdco      {  };
    std::map<int, std::size_t>           alpha2edm4hep_vdxy      {  };
    std::map<int, std::size_t>           alpha2edm4hep_vdzt      {  };
    std::map<int, std::size_t>           alpha2edm4hep_vdht      {  };

    data_access_t& data;
    mcparticle_t           invalid_mc_particle     {  };
#if 0
    template<typename T,typename C>
    T get_item(C& cont, std::size_t id, const std::string& err="")  {
      for( auto i=cont.begin(); i != cont.end(); ++i )  {
        if( id == i->id() )  {
          return *i;
        }
      }
      throw std::runtime_error(err.empty() ? "Non existing object" : err.c_str());
    }
#endif
    
    /// Default constructor
    event_t(data_access_t& data);
    /// Default destructor
    virtual ~event_t();
    
    using calo_deposit_t   = edm4hep::MutableCalorimeterHit;
    using vdet_hit_t       = edm4hep::MutableTrackerHit3D;
    using sim_trackerhit_t = edm4hep::MutableSimTrackerHit;

    mcparticle_t particle_mc_fkin(std::size_t itk);
    mcparticle_t particle_mc_alpha(std::size_t itk);
    
    /// Create MC particle
    void             process_mc_particles();
    /// Convert charged tracks. They enter the catalog with their FRFT row number
    void             process_charged_tracks();
    /// Link reconstructed charged tracks from FRFT to MC tracks
    void             process_pasl();

    /// Convert calorimeter objects. They enter the catalog with their CalObject number
    void             process_calorimeter_objects();
    /// Convert gammas from PGAC.
    void             process_gammas_pgac();

    /// Create VDET 3D hit from VDCO bank
    void             process_vdco();
    /// Create VDET simulation hit from VDHT bank (produced by GALEPH)
    void             process_vdht();
    /// Process VDCO relations to FKIN and create VDET simulation hits from VDHT
    void             process_vdfk();
    /// Process VDXY and VDZT relations to FKIN
    void             process_vufk();

    /// Create VDET 3D hit from VDXY banks
    void             process_vdxy();
    /// Create VDET 3D hit from VDZT banks
    void             process_vdzt();

    /// Process VCPL bank to associate VDXY/VDZT to FRFT
    void             process_vcpl();

    /// Process PIDI ITC digitisings (NR=0)
    void             process_pidi(); // To be done!
    
    /// Create ECAL cluster from PECO row
    void             process_peco();
    /// Analyze ECAL wire data from PEWI bank
    void             process_pewi();
    /// Analyze ECAL pad relations to wires from PRPW bank
    void             process_prpw();
    /// Create HCAL cluster relations from PECO to FRFT tracks: bank PFHR
    void             process_pfer();

    /// Create HCAL cluster from PHCO row
    void             process_phco();
    /// Create HCAL cluster relations from PHCO to FRFT tracks: bank PFHR
    void             process_pfhr();

    /// Print particle tables
    int print_qvec(const char* title, int first, int last)  const;
    /// Print table of Monte-Carlo Particles
    int print_fkin()  const;
    /// Print table of charged tracks
    int print_cht()  const;
    /// Print table of EFLW (energy flow objects)
    int print_eflw()  const;
    /// Print table of Calorimeter Objects
    int print_calobj()  const;
    /// Print table of GAMPEC gammas
    int print_gampec()  const;
    /// Print table of neutrals
    int print_neutrals()  const;
    /// Print table of jets
    int print_jets()  const;

    
    /// Link tracker hit to FRFT charged track
    void link_hit_to_frft(std::size_t frft_track_number, const edm4hep::TrackerHit& hit);
    /// Link tracker hit to FRFT charged track
    void link_hit_to_frft(std::size_t frft_track_number, const edm4hep::CalorimeterHit& hit);
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


  template<typename COLL, typename FROM, typename TO>
  typename COLL::mutable_type _set_link(COLL& coll, const FROM& from, const TO& to, float weight=1e0)  {
    typename COLL::mutable_type link = coll.create();
    link.setWeight(weight);
    link.setFrom(from);
    link.setTo(to);
    return link;
  }

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
  this->frft.nami = bos77::namind("FRFT");
  this->pasl.nami = bos77::namind("PASL");
  this->pitm.nami = bos77::namind("PITM");

  this->peco.nami = bos77::namind("PECO");
  this->phco.nami = bos77::namind("PHCO");
  this->pgac.nami = bos77::namind("PGAC");
  this->pewi.nami = bos77::namind("PEWI");

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
  this->event_config( this->frft );

  this->vdxy.debug = false;
  this->vdzt.debug = false;
  this->vcpl.debug = false;
  this->debug_mc_particles = false;
  this->debug_charged_tracks = false;
}

/// Default constructor
alpha::output_edm4hep::event_t::event_t(data_access_t& da)
  : data(da), invalid_mc_particle()
{
  invalid_mc_particle.unlink();
}

/// Default destructor
alpha::output_edm4hep::event_t::~event_t()  {
}

/// Print particle tables
int alpha::output_edm4hep::event_t::print_qvec(const char* title, int first, int last)  const  {
  if( last >= first )  {
    std::cout << "+++ " << title << ": first=" << first << " last:" << last << std::endl;
    for( int itk=first; itk <= last; ++itk )  {
      auto* track = this->data.qvec.row<class qvec>(itk);
      std::cout << track->to_string() << std::endl;
    }
  }
  else  {
    std::cout << "+++ " << title << ": No tracks to be displayed.   first=" << first << " last:" << last << std::endl;
  }
  return last - first + 1;
}

/// Print table of Monte-Carlo Particles
int alpha::output_edm4hep::event_t::print_fkin()  const  {
  return this->print_qvec("Monte-Carlo Tracks", qcde.KFMCT, qcde.KLMCT);
}

/// Print table of charged tracks
int alpha::output_edm4hep::event_t::print_cht()  const  {
  return this->print_qvec("Charded Tracks", qcde.KFCHT, qcde.KLCHT);
}

/// Print table of EFLW (energy flow objects)
int alpha::output_edm4hep::event_t::print_eflw()  const  {
  return this->print_qvec("Energy Flow Tracks", qcde.KFEFT, qcde.KLEFT);
}

/// Print table of Calorimeter Objects
int alpha::output_edm4hep::event_t::print_calobj()  const  {
  return this->print_qvec("Calorimeter Objects", qcde.KFIST, qcde.KLAST);
}

/// Print table of GAMPEC gammas
int alpha::output_edm4hep::event_t::print_gampec()  const  {
  return this->print_qvec("GAMPEC gammas", qcde.KFGAT, qcde.KLGAT);
}

/// Print table of neutrals
int alpha::output_edm4hep::event_t::print_neutrals()  const  {
  return this->print_qvec("Neutral particles", qcde.KFNET, qcde.KLNET);
}

/// Print table of jets
int alpha::output_edm4hep::event_t::print_jets()  const  {
  return this->print_qvec("JETS", qcde.KFJET, qcde.KLJET);
}

edm4hep::MutableMCParticle
alpha::output_edm4hep::event_t::particle_mc_alpha(std::size_t itk)  {
  auto& cont = this->alpha2edm4hep_particles;
  auto itr = cont.find(itk);
  if( itr != cont.end() )  {
    mcparticle_t p = this->particles_mc.at(itr->second);
    return p;
  }
  std::string err = "Non existing object: MutableMCParticle ALPHA MCT id: "+std::to_string(itk);
  throw std::runtime_error(err);
}

edm4hep::MutableMCParticle
alpha::output_edm4hep::event_t::particle_mc_fkin(std::size_t itk)  {
  auto& cont = this->fkin2edm4hep_particles;
  auto itr = cont.find(itk);
  if( itr != cont.end() )  {
    mcparticle_t p = this->particles_mc.at(itr->second);
    return p;
  }
  if( itk == 0 )  { // Particle got removed from MC record (ie Bremsstrahlung)
                    // Or is this an inconsistency in VDFK and VUFK ?
    return this->invalid_mc_particle;
  }
  std::string err = "Non existing object: MutableMCParticle FKIN id: "+std::to_string(itk);
  throw std::runtime_error(err);
}
/*
 ......................................................
                                        Subschema: CobjPOTBanks                 
 +------+                                                                       
 | PEMH |  Relation between ECAL and                                            
 +------+  LCAL objects and FKIN tracks
 ..............................................................
     1          I    Number of words/relation (=3)
     2          I    Number of relations
 ..............................................................
      1    CO  I    CalObjectnumber  [1,*]
                       PECO or PEOB number
      2    TN  I    TrackNumber      [1,*]
                       FKIN number
      3    SE  I    SharedEnergy     [1,*]
                       Energy FKIN track contributes to
                       this Cal Obj in MeV
 */
/// Link tracker hit to FRFT charged track
void alpha::output_edm4hep::event_t::link_hit_to_frft(std::size_t frft_track_number,
                                                      const edm4hep::TrackerHit& hit)
{
  if( frft_track_number > 0 && frft_track_number <= this->particles_reco.size() )  {
    edm4hep::MutableTrack track = this->particles_reco[frft_track_number-1];
    track.addToTrackerHits( hit );
    return;
  }
  std::string err = "FRFT track " + std::to_string(frft_track_number) +
    " Does not exist. Failed to add tracker hit.";
  std::cout << err << std::endl;
  for(const auto& c : this->alpha2edm4hep_charged )  {
    std::cout << "alpha2edm4hep_charged: FRFT: " << c.first << " --> particles_reco: " << c.second << std::endl;
  }
  //edm4hep::MutableTrack track = this->particles_reco[frft_track_number-1];
  //track.addToTrackerHits( hit );
  //return;
  //throw std::runtime_error(err);
}

/// Convert calorimeter objects. They enter the catalog with their CalObject number
void alpha::output_edm4hep::event_t::process_calorimeter_objects()  {
  for( int itk = qcde.KFCOT; itk <= qcde.KLAST; ++itk )  {
    std::size_t key   = this->calorimeter_objects.size();
    auto*       track = this->data.qvec.row<class qvec>(itk);
    int32_t cal_track = track->ktn();
    if( key && track && cal_track ) {
    }
  }
}
#include "output_edm4hep_mc_particles.h"
#include "output_edm4hep_charged_tracks.h"

#include "output_edm4hep_peco.h"
#include "output_edm4hep_pewi.h"
#include "output_edm4hep_pfer.h"
#include "output_edm4hep_pasl.h"
#include "output_edm4hep_pgac.h"

#include "output_edm4hep_phco.h"
#include "output_edm4hep_pfhr.h"

/// VDET coordinated NR=0 (POT)   
#include "output_edm4hep_vdco.h"
/// MVD hits in r-phi wafer.
#include "output_edm4hep_vdxy.h"
/// MVD hits in z wafer. (POT) 
#include "output_edm4hep_vdzt.h"
/// Vdco to FKIN truth relation       
#include "output_edm4hep_vdfk.h"
/// Vdxy/vdzt to FKIN truth 
#include "output_edm4hep_vufk.h"
/// VDet HiT list NR=0 (GAL)      
#include "output_edm4hep_vdht.h"
/// Process VCPL bank to associate VDXY/VDZT to FRFT
#include "output_edm4hep_vcpl.h"

/// Process PIDI ITC digitisings (NR=0)
#include "output_edm4hep_pidi.h"

void alpha::output_edm4hep::event_t::process_prpw()  {
}

/*
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

/// Convert the VDET hits and clusters
void alpha::output_edm4hep::convert_vdet()  {
  /// Create all VDET GALEPH hits from VDHT
  this->event->process_vdht();
  /// Create all VDET clusters from VDCO
  this->event->process_vdco();
  /// Process VDCO relations to FKIN
  this->event->process_vdfk();

  /// Process the list od VDXY bank:
  this->event->process_vdxy();
  /// Process the list od VDZT bank:
  this->event->process_vdzt();
  /// Process VDXY and VDZT relations to FKIN
  this->event->process_vufk();

  /// Process VCPL bank to associate VDXY/VDZT to FRFT
  this->event->process_vcpl();
}

/// Convert ITC hits and clusters
void alpha::output_edm4hep::convert_itc()  {
  /// Process PIDI ITC digitisings (NR=0)
  this->event->process_pidi();
}

/// Convert TPC hits and clusters
void alpha::output_edm4hep::convert_tpc()  {
}

/// Convert ECAL hits and clusters
void alpha::output_edm4hep::convert_ecal()  {  
  /// Create all ECAL clusters from PECO
  this->event->process_peco();
  /// Analyze ECAL wire data from PEWI bank
  this->event->process_pewi();
  /// Analyze ECAL pad relations to wires from PRPW bank
  this->event->process_prpw();
  /// Create HCAL cluster relations from PECO to FRFT tracks: bank PFHR
  this->event->process_pfer();
}

/// Convert HCAL hits and clusters
void alpha::output_edm4hep::convert_hcal()  {  
  /// Create all HCAL clusters from PHCO
  this->event->process_phco();
  /// Create HCAL cluster relations from PHCO to FRFT tracks: bank PFHR
  this->event->process_pfhr();
}

/// Convert event data
void alpha::output_edm4hep::convert_event()  {
  /// First create the Monte-Carlo particles and fill the properties
  this->event->process_mc_particles();
  /// Create the charged reconstructed particles and fill the properties
  this->event->process_charged_tracks();
  /// Link reconstructed charged tracks from FRFT to MC tracks
  this->event->process_pasl();

  /// Convert VDET hits and clusters
  this->convert_vdet();
  /// Convert ITC hits and clusters
  this->convert_itc();
  /// Convert TPC hits and clusters
  this->convert_tpc();
  /// Convert ECAL hits and clusters
  this->convert_ecal();
  /// Convert HCAL hits and clusters
  this->convert_hcal();

  this->event->print_fkin();
  this->event->print_cht();
  this->event->print_eflw();
  this->event->print_gampec();
  this->event->print_calobj();
  this->event->print_neutrals();
  this->event->print_jets();
  this->event->print_qvec("Standard V0s",            qcde.KFV0T, qcde.KLV0T);
  this->event->print_qvec("Tracks from V0 vertices", qcde.KFDCT, qcde.KLDCT);
  // this->event->print_qvec("Long V0 tracks",          qcde.KFLVT, qcde.KLLVT);
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
