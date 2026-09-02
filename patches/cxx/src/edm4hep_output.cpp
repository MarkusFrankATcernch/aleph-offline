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
  
  +++ AUBLIS +++    R list contains the following banks: 
  RUNR RUNH KRUN KRAN KJOB PART KCAR RLEP KLUN KPAR KLIN MSTU PARU MSTJ PARJ PMA1 PMA2 PMA3 PMA4 MDM1
  GADM GRPL GMOB KORL TSIM LALI LCAL EZTH VOLU ACUT AFID AJOB ARUN APRO ASIM ILIV ECHE EHGF SMPD RHAH
  ZPFR JCON PTUN

  +++ AUBLIS +++    S list contains the following banks: 
  FRFT FRTL FTCL FICL FVCL ITMA IDIG ITCO IWCR ILIV ICCO TPCO TBCO TEXS T2XS HSDA HSTO PCQA EFOL EAUX
  EJET
  ;
#endif

/// Framework include files
#include <alpha/edm4hep_output.h>

#include <alpha/alpha.h>
#include <alpha/qcde.h>
#include <alpha/qvec.h>
#include <alpha/qvrt.h>

/// C/C++ include files
#include <cstdio>
#include <atomic>

/// edm4hep include files
#include <edm4hep/RecDqdxCollection.h>
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

/// podio include files
#include <podio/Frame.h>
#include <podio/CollectionBase.h>
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

#include <TGeoSystemOfUnits.h>
#include <TGeant4SystemOfUnits.h>

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
  class edm4hep_output::io_t  {
  public:
#if PODIO_BUILD_VERSION >= PODIO_VERSION(1, 0, 0)
    using writer_t = podio::Writer;
    std::string type = "default"; // "rntuple" : "default"
#else
    using writer_t = podio::ROOTWriter;
#endif
    std::unique_ptr<writer_t>     podio_file  {       };
    podio::Frame                  podio_frame {       };
    bool                          debug       { false };
  public:
    /// Default constructor
    io_t(bool dbg);
    /// Default destructor
    ~io_t();
    /// Open PODIO file
    bool open(const std::string& fname);
    /// Close event output stream
    void close();
    /// Start frame trannsaction
    void begin();
    /// Commit frame to event stream
    void commit();
    /// Add emd4hep object to output frame
    template <typename T> void put(T& container, const std::string& name);
  };

  /// Helper class to access BOS data structures from ALEPH
  /**
   *    \author  M.Frank
   *    \date    01/08/2026
   */
  class edm4hep_output::data_access_t  {

  public:
    bank_access_t qvec;  // QVEC table bank
    bank_access_t qvrt;  // QVRT table bank
    bank_access_t qdet;  // QDET table bank
    bank_access_t frft;  // FRFT table bank
    bank_access_t frtl;  // FRFT table bank: Tpc+Itc+Vdet Geometry Track point List
    bank_access_t pitm;  // PITM table bank: JULIA track to FKIN track association (using ITC + TPC coordinate information)
    bank_access_t pasl;  // PASL table bank: JULIA track to FKIN track association list (accessed via PITM bank).

    bank_access_t vdco;  // VDCO table bank: VDET coordinated NR=0 (POT) --> FRFT
    bank_access_t fvcl;  // FVCL table bank: Vdet Geometry track Coordinate List
    bank_access_t vdxy;  // VDXY table bank: MVD hits in r-phi wafer.       NR=ILAYER*10000+IZED*1000+IPH-I*10+(IVIEW=1)
    bank_access_t vdzt;  // VDZT table bank: MVD hits in z wafer. (POT)     NR=ILAYER*10000+IZED*1000+IPH-I*10+(IVIEW=1)
    bank_access_t vcpl;  // VCPL table bank: association of a track NR=NTRACK (POT)

    bank_access_t vfhl;  // VFHL table bank: VDET final hit list bank,      NR=VHLS number (POT)
    bank_access_t vfph;  // VFPH table bank: VDET final pulse height bank,  NR=VHLS number (POT)
    bank_access_t vflg;  // VFLG table bank: VDET strip flag bank,          NR=VHLS number (POT)
    bank_access_t vufk;  // VUFK table bank: Vdxy/vdzt to FKIN truth relation (monte Carlo)
    bank_access_t vdfk;  // VDFK table bank: VDCO      to FKIN truth relation
    bank_access_t vdht;  // VDHT table bank: VDet HiT list NR=0 (GAL)
    bank_access_t vdgc;  // VDGC table bank: VDET global clusters    
    
    bank_access_t itco;  // ITCO table bank: ITc COordinates (Recon. Bank) 
    bank_access_t ficl;  // FICL table bank: Itc Geometry track Coordinate  
    bank_access_t pidi;  // PIDI table bank: Packed Itc DIgitisings (NR=0)
    bank_access_t icco;  // ICCO table bank: Itc Corrected COordinates

    bank_access_t ptun;  // PTUN table bank: Units
    bank_access_t ptnc;  // PTNC table bank: Production output Tpc pad Coordinates (NR=0)
    bank_access_t texs;  // TEXS table bank: Production output Tpc track pad dE/dX (NR=0)
    bank_access_t t2xs;  // T2XS table bank: Tpc dE/dX Segment for Overlapping Tracks (NR=0)

    bank_access_t tpco;  // TPCO table bank: Tpc Pad Coordinates in global system (NR=0: final coordinates)
    bank_access_t ftcl;  // FTCL table bank: Tpc Geometry track Coordinate List

    bank_access_t peco;  // PECO table bank: Electromagnetic Calorimeter Object (Mini: DECO)

    bank_access_t pewi;  // PEWI table bank: Ecal Wire plane Digits Raw bank: EWDI
    bank_access_t etdi;  // ETDI table bank: Ecal Tower DIgits NR=0. (RAW)
    bank_access_t pgac;  // PGAC table bank: Photons from GAMPEX (oct 94) (Mini: DGAC)

    bank_access_t phco;  // PHCO table bank: Hadron Calorimeter Object (Mini: DHCO)
    bank_access_t hpdi;  // HPDI table bank: Hcal Plane DIgits (RawData)
    bank_access_t hphr;  // HPHR table bank: 
    
    bool debug_mc_particles   = false;
    bool debug_charged_tracks = false;
    
  public:
    /// Default constructor
    data_access_t(const std::string& debug_banks="");
    /// Default destructor
    ~data_access_t();
    /// Configure for new event
    void event_config();
    /// Configure data access
    void config_data(bank_access_t& acc, const char* nam, const std::string& debug_banks);
  };

  
  /// Helper class to convert event data from ALEPH to EDM4HEP
  /**
   *    \author  M.Frank
   *    \date    01/08/2026
   */
  class edm4hep_output::edm4hep_output::event_t  {
  public:

    enum data_types_t  {
      VDET_COORDINATE         = 0,
      VDET_HIT_RPHI           = 1<<0,
      VDET_HIT_Z              = 1<<1,
      VDET_HIT_XY             = 1<<2,
      VDET_HIT_ZT             = 1<<3,
      ITC_COORDINATE          = 1<<4,

      
      TPC_COORDINATE          = 1<<4,
      TPC_RAW_PAD_COORDINATE  = 1<<5,

      
      VDET_WAFER_RPHI         = VDET_HIT_RPHI,
      VDET_WAFER_Z            = VDET_HIT_Z,

      
      DEDX_TRUNCATED_MEAN     = 1<<0,
      DEDX_AVERAGE_DRIFT      = 1<<1,
      DEDX_NUM_SAMPLES        = 1<<2,
      DEDX_TRACKLENGTH        = 1<<3,
      DEDX_TRACK_OVERLAP      = 1<<4,
      
      NONE
    };

    
    using track_t             = edm4hep::MutableTrack;
    using mcparticle_t        = edm4hep::MutableMCParticle;
    using trackerhit_t        = edm4hep::MutableTrackerHit3D;
    using trackerhits_t       = edm4hep::TrackerHit3DCollection;
    using simtrackerhits_t    = edm4hep::SimTrackerHitCollection;
    using track2mctrack_t     = edm4hep::TrackMCParticleLinkCollection;
    using track_hit2mchit_t   = edm4hep::TrackerHitSimTrackerHitLinkCollection;

    using calohit_t           = edm4hep::MutableCalorimeterHit;
    using calohits_t          = edm4hep::CalorimeterHitCollection;

    using rawcalohit_t        = edm4hep::MutableRawCalorimeterHit;
    using rawcalohits_t       = edm4hep::RawCalorimeterHitCollection;
    using dedxdata_t          = edm4hep::RecDqdxCollection;
    
  public:
    
    experiment_t&                        exp;
    data_access_t&                       data;
    
    edm4hep::EventHeader                 event_header            {  };

    /// Container withg Monte-Carlo tracks
    edm4hep::MCParticleCollection        particles_mc            {  };
    /// Container with reconstructed tracks
    edm4hep::TrackCollection             particles_reco          {  };
    /// Relationship between reconstructed tracks and Monte-Carlo tracks
    track2mctrack_t                      rel_part_reco_mc        {  };

    edm4hep::TrackCollection             calorimeter_objects     {  };

    /// VDET XY Monte-Carlo hits
    trackerhits_t                        hits_vdxy               {  };
    /// VDET ZT Monte-Carlo hits
    trackerhits_t                        hits_vdzt               {  };

    /// Reconstructed VDET coordinates
    trackerhits_t                        hits_vdco               {  };

    /// Sim tracker hits as produced by Galeph
    simtrackerhits_t                     simhits_vdht            {  };

    /// Sim tracker hits projected to xy wafers
    simtrackerhits_t                     simhits_vufk_xy         {  };
    /// Sim tracker hits on xy wafers to FKIN
    track_hit2mchit_t                    rel_vdxy_vufk           {  };

    /// Sim tracker hits projected to z wafers
    simtrackerhits_t                     simhits_vufk_z          {  };
    /// Sim tracker hits on z wafers to FKIN
    track_hit2mchit_t                    rel_vdzt_vufk           {  };

    simtrackerhits_t                     simhits_vdfk_rphi       {  };
    simtrackerhits_t                     simhits_vdfk_z          {  };
    track_hit2mchit_t                    rel_vdco_vdfk_z         {  };
    track_hit2mchit_t                    rel_vdco_vdfk_rphi      {  };

    trackerhits_t                        hits_icco               {  };
    /// ITC R-Phi hits
    trackerhits_t                        hits_itco1              {  };
    /// ITC R-Phi hits (ambiguity)
    trackerhits_t                        hits_itco2              {  };
    /// TPC corrected coordinates
    trackerhits_t                        hits_tpco               {  };
    /// Raw TPC pad hits transformed to global coordinate system
    trackerhits_t                        hits_ptnc               {  };
    /// dEdx data: Truncated mean of dE/dx measurements 
    dedxdata_t                           dedx_trunc_mean         {  };
    /// dEdx data: Average drift length of samples  
    dedxdata_t                           dedx_avg_drift          {  };
    /// dEdx data: Useful length of track for dE/dx
    dedxdata_t                           dedx_tracklen           {  };
    /// dEdx data: Number of samples used for dE/dx
    dedxdata_t                           dedx_num_samples        {  };

    /// ECAL hits from PECO
    calohits_t                           hits_ecal               {  };
    /// ECAL wire hits
    rawcalohits_t                        hits_ecal_wire          {  };

    /// HCAL hits from PHCO
    calohits_t                           hits_hcal               {  };
    /// 
    rawcalohits_t                        hits_hcal_plane         {  };

    /// Relationship between ALEPH MC particles and edm4hep::MCParticles
    std::map<int, std::size_t>           alpha2edm4hep_particles {  };
    /// Relationship between ALEPH MC particles from FKIN and edm4hep::MCParticles
    std::map<int, std::size_t>           fkin2edm4hep_particles  {  };
    /// Relationship between ALEPH FRFT track rows and edm4hep::Tracks
    std::map<int, std::size_t>           alpha2edm4hep_charged   {  };

    /// Relationship between reconstructed VDET hits and VDET tracker hits in hits_vdco
    std::map<int, std::size_t>           alpha2edm4hep_vdco      {  };
    /// Relationship between reconstructed VDET hits from VDXY and tracker hits in hits_vdxy
    std::map<int, std::size_t>           alpha2edm4hep_vdxy      {  };
    /// Relationship between reconstructed VDET hits from VDZT and tracker hits in hits_vdzt
    std::map<int, std::size_t>           alpha2edm4hep_vdzt      {  };

    /// Invalid MC particle
    mcparticle_t                         invalid_mc_particle     {  };

    /// Default constructor
    event_t(experiment_t& experiment, data_access_t& data);
    /// Default destructor
    virtual ~event_t();
    
    using calo_deposit_t   = edm4hep::MutableCalorimeterHit;
    using vdet_hit_t       = edm4hep::MutableTrackerHit3D;
    using sim_trackerhit_t = edm4hep::MutableSimTrackerHit;


    /// Main event conversion entry point
    void convert_event();

    /// Convert VDET hits and clusters
    void convert_vdet();
    /// Convert ITC hits and clusters
    void convert_itc();
    /// Convert TPC hits and clusters
    void convert_tpc();
    /// Convert ECAL hits and clusters
    void convert_ecal();
    /// Convert HCAL hits and clusters
    void convert_hcal();
    
    /// Access Monte-Carlo particle by the FKIN bank number
    mcparticle_t particle_mc_fkin(std::size_t itk);
    /// Access Monte-Carlo particle by the alpha  MC track number
    mcparticle_t particle_mc_alpha(std::size_t itk);
    /// Access charged track by the FRFT track number
    track_t      particle_frft(std::size_t itk);
    
    /// Create MC particle
    void         process_mc_particles();
    /// Convert charged tracks. They enter the catalog with their FRFT row number
    void         process_charged_tracks();
    /// Link reconstructed charged tracks from FRFT to MC tracks
    void         process_pasl();

    /// Convert calorimeter objects. They enter the catalog with their CalObject number
    void         process_calorimeter_objects();

    /// Create VDET 3D hit from VDCO bank
    void         process_vdco();
    /// Create VDET simulation hit from VDHT bank (produced by GALEPH)
    void         process_vdht();
    /// Process VDCO relations to FKIN and create VDET simulation hits from VDHT
    void         process_vdfk();

    /// Create VDET 3D hit from VDXY banks
    void         process_vdxy();
    /// Create VDET 3D hit from VDZT banks
    void         process_vdzt();
    /// Process VCPL bank to associate VDXY/VDZT to FRFT
    void         process_vcpl();
    /// Process VDXY and VDZT relations to FKIN
    void         process_vufk();

    /// Process PIDI ITC digitisings (NR=0)
    void         process_pidi(); // To be done!
    /// Process ICCO table bank: Itc Corrected COordinates
    void         process_icco();
    /// ITCO table bank: ITc COordinates (Recon. Bank) 
    void         process_itco();

    /// Process TPC coordinates from TPCO
    void         process_tpco();
    /// Create Production output Tpc pad Coordinates (NR=0)
    void         process_ptnc();
    /// Production output Tpc track pad dE/dX (NR=0)
    void         process_texs();
    void         process_t2xs();

    /// Process relationships between charged tracks and VDCO, ITCO and TPCO
    void         process_frtl();
    
    /// Create ECAL cluster from PECO row
    void         process_peco();
    /// Analyze ECAL wire data from PEWI bank
    void         process_pewi();
    /// Create HCAL cluster relations from PECO to FRFT tracks: bank PFHR
    void         process_pfer();
    /// ETDI table bank: Ecal Tower DIgits NR=0. (RAW)
    void         process_etdi();

    /// Create HCAL cluster from PHCO row
    void         process_phco();
    /// HPDI table bank: Hcal Plane DIgits (RawData)
    void         process_hpdi();
    /// Create HCAL cluster relations from PHCO to FRFT tracks: bank PFHR
    void         process_pfhr();

    /// Convert gammas from PGAC.
    void         process_gammas_pgac();
    
    /// Print particle tables
    int          print_qvec(const char* title, int first, int last)  const;
    /// Print table of Monte-Carlo Particles
    int          print_fkin()  const;
    /// Print table of charged tracks
    int          print_cht()  const;
    /// Print table of EFLW (energy flow objects)
    int          print_eflw()  const;
    /// Print table of Calorimeter Objects
    int          print_calobj()  const;
    /// Print table of GAMPEC gammas
    int          print_gampec()  const;
    /// Print table of neutrals
    int          print_neutrals()  const;
    /// Print table of jets
    int          print_jets()  const;

    /// Link tracker hit to FRFT charged track
    void link_hit_to_frft_track(std::size_t frft_track_number, const edm4hep::TrackerHit& hit);
    /// Link tracker hit to FRFT charged track
    void link_hit_to_frft_track(std::size_t frft_track_number, const edm4hep::CalorimeterHit& hit);
  };
}

#include <alpha/processor.h>
namespace {
  std::unique_ptr<alpha::edm4hep_output> conv;
}

/// Framework event callback
template <> void alpha::processor<alpha::edm4hep_output>::handle_event(constants_t& /* par */)  {
  if( debug ) ::printf("edm4hep_output:  +++++++  Calling %s   KNEVT:%d\n", __FUNCTION__, qcde.KNEVT);
  ::printf("+++++++  Calling %s\n", __FUNCTION__);
  auto evt = conv->begin_event();
  conv->convert_event(*evt);
  conv->end_event(*evt);
}

/// Framework termination callback
template <> void alpha::processor<alpha::edm4hep_output>::terminate()  {
  ::printf("+++++++  Calling %s\n", __FUNCTION__);
  conv.reset();
}

/// Framework termination callback
template <> void alpha::processor<alpha::edm4hep_output>::initialize()  {
  ::printf("+++++++  Calling %s\n", __FUNCTION__);
  conv = std::make_unique<alpha::edm4hep_output>();
}

/// Default constructor
alpha::edm4hep_output::io_t::io_t(bool dbg) : debug(dbg)  {
}

/// Default destructor
alpha::edm4hep_output::io_t::~io_t()  {
  if ( this->podio_file )  {
    this->close();
  }
}

/// Open PODIO file
bool alpha::edm4hep_output::io_t::open(const std::string& fname)  {
#if PODIO_BUILD_VERSION >= PODIO_VERSION(1, 0, 0)
  this->podio_file = std::make_unique<writer_t>( podio::makeWriter(fname, this->type) );
#else
  this->podio_file = std::make_unique<podio::ROOTWriter>(fname);
#endif
  if( this->debug ) ::printf("+++ Open file %s\n", fname.c_str());
  return this->podio_file.get() != nullptr;
}

/// Close event output stream
void alpha::edm4hep_output::io_t::close()  {
  if ( this->podio_file )  {
    this->podio_file->finish();
    this->podio_file.reset();
    if( this->debug ) ::printf("+++ Closed event output stream\n");
  }
}

/// Add emd4hep object to output frame
template <typename T>
void alpha::edm4hep_output::io_t::put(T& container, const std::string& name)  {
  if( this->debug ) ::printf("+++ \tPut container %s\n", name.c_str());
  this->podio_frame.put( std::move(container),  name);    
}

/// Start frame trannsaction
void alpha::edm4hep_output::io_t::begin()  {
  if( this->debug ) ::printf("+++ Begin transaction\n");
  this->podio_frame = { };
}
#include <TDirectory.h>
/// Commit frame to event stream
void alpha::edm4hep_output::io_t::commit()  {
  TDirectory::TContext context;
  if( this->debug ) ::printf("+++ Commit frame to file\n");
  this->podio_file->writeFrame( this->podio_frame, "ALEPH" );
}

/// Default constructor
alpha::edm4hep_output::edm4hep_output()   {
  this->exp = std::make_unique<experiment_t>();
}

/// Default destructor
alpha::edm4hep_output::~edm4hep_output()   {
  this->exp.reset();
  this->data.reset();
}

/// Set option value
bool alpha::edm4hep_output::set_option(const char* name, const char* value)  {
  const char* p = name;
  while( *p == '-' ) ++p;

  if( strcmp(p,"debug-all") == 0 )  {
    this->data = std::make_unique<data_access_t>("ALL");
    return true;
  }
  else if( strcmp(p,"debug-banks") == 0 )  {
    std::string debug_banks = value;
    for( std::size_t i=0; i < debug_banks.length(); ++i )
      debug_banks[i] = ::toupper(debug_banks[i]);
    this->data = std::make_unique<data_access_t>(debug_banks);
    return true;
  }
  else if( strcmp(p,"output") == 0 )  {
    this->output_file = value;
    return true;
  }
  else if( strncmp(p,"save-mc-part",12) == 0 )  {
    return this->save_mc_particles = true;
  }
  else if( strncmp(p,"save-part",9) == 0 )  {
    return this->save_particles = true;
  }
  else if( strcmp(p,"save-vdet") == 0 )  {
    this->save_vdet_reco = true;
    this->save_vdet_sim  = true;
    return true;
  }
  else if( strcmp(p,"save-vdet-mc") == 0 )  {
    return this->save_vdet_sim  = true;
  }
  else if( strcmp(p,"save-vdet-rec") == 0 )  {
    return this->save_vdet_reco = true;
  }
  else if( strcmp(p,"save-tracker") == 0 )  {
    return this->save_tracker = true;
  }
  else if( strcmp(p,"save-calorimeter") == 0 )  {
    return this->save_calorimeter = true;
  }
  else if( strcmp(p,"save-all") == 0 )  {
    return this->save_all = true;
  }
  return false;
}

/// Initialize conversion. Open file
bool alpha::edm4hep_output::initialize()  {
  this->io   = std::make_unique<io_t>(this->debug_io);
  this->data = std::make_unique<data_access_t>("");

  if( !this->output_file.empty() )  {
    if( !this->io->open(this->output_file) )  {
      ::printf("+++ Failed to open output file: %s\n", this->output_file.c_str());
      return false;
    }
    ::printf("+++ Opened successfully %s for output.\n", this->output_file.c_str() ) ;
  }
  this->save_particles    |=  this->save_all;
  this->save_mc_particles |= (this->save_all | this->save_particles);
  
  this->save_tracker      |=  this->save_all;
  this->save_vdet_reco    |=  this->save_tracker;
  this->save_vdet_sim     |=  this->save_tracker;
  
  this->save_itc_reco     |=  this->save_tracker;
  this->save_itc_sim      |=  this->save_tracker;

  this->save_tpc_reco     |=  this->save_tracker;
  this->save_tpc_sim      |=  this->save_tracker;

  this->save_calorimeter  |=  this->save_all;
  this->save_ecal_reco    |=  this->save_calorimeter;
  this->save_hcal_reco    |=  this->save_calorimeter;
  
  return true;
}

/// Finalize conversion. Close file
bool alpha::edm4hep_output::finalize()  {
  if( this->io )  {
    this->io->close();
  }
  this->io.reset();
  this->data.reset();
  return true;
}

/// Framework event callback
bool alpha::edm4hep_output::handle_event(constants_t& /* par */)  {
  auto event = this->begin_event();
  this->convert_event(*event);
  this->end_event(*event);
  return true;
}

/// Start event saving
std::unique_ptr<alpha::edm4hep_output::event_t>
alpha::edm4hep_output::begin_event()  {
  this->data->event_config();
  auto event = std::make_unique<edm4hep_output::event_t>(*this->exp, *this->data);
  return event;
}

/// End event saving
void alpha::edm4hep_output::end_event(event_t& event)  {
  this->io->begin();
  if( this->save_mc_particles   ) this->io->put(event.particles_mc,     "MCParticles" );
  if( this->save_charged_tracks ) this->io->put(event.particles_reco,   "ChargedTracks" );
  if( this->save_mc_particles && this->save_charged_tracks )
    this->io->put(event.rel_part_reco_mc,   "TrackMCRelations" );

  if( this->save_vdet_reco ) {
    this->io->put(event.hits_vdco,          "VDCO_Hits" );
  }
  if( this->save_vdet_sim )  {
    this->io->put(event.hits_vdxy,          "VDXY_Hits" );
    this->io->put(event.hits_vdzt,          "VDZT_Hits" );
    this->io->put(event.simhits_vdht,       "VDHT_SimHits" );
    this->io->put(event.simhits_vufk_xy,    "VUFK_XY_SimHits" );
    this->io->put(event.rel_vdxy_vufk,      "VDXY_VUFK_Relations" );

    this->io->put(event.simhits_vufk_z,     "VUFK_Z_SimHits" );
    this->io->put(event.rel_vdzt_vufk,      "VDZT_VUFK_Relations" );

    this->io->put(event.simhits_vdfk_rphi,  "VDFK_RPHI_SimHits" );
    this->io->put(event.rel_vdco_vdfk_rphi, "VDCO_VDFK_RPHI_Relations" );

    this->io->put(event.simhits_vdfk_z,     "VDFK_Z_SimHits" );
    this->io->put(event.rel_vdco_vdfk_z,    "VDCO_VDFK_Z_Relations" );
  }
  if( this->save_itc_reco )  {
    this->io->put(event.hits_itco1,         "ITCO1_Hits" );
    this->io->put(event.hits_itco2,         "ITCO2_Hits" );
  }
  if( this->save_tpc_reco )  {
    this->io->put(event.hits_tpco,          "TPCO_Hits" );
    this->io->put(event.hits_ptnc,          "PTNC_PadHits" );
    this->io->put(event.dedx_trunc_mean,    "DEDX_TruncatedMean" );
    this->io->put(event.dedx_avg_drift,     "DEDX_AverageDrivt" );
    this->io->put(event.dedx_tracklen,      "DEDX_TrackLength" );
    this->io->put(event.dedx_num_samples,   "DEDX_NumberSamples" );
  }
  if( this->save_ecal_reco )  {
    this->io->put(event.hits_ecal,          "ECAL_Clusters" );
  }
  if( this->save_hcal_reco )  {
    this->io->put(event.hits_hcal,          "HCAL_Clusters" );
  }

  this->io->commit();
}

/// Convert event data
void alpha::edm4hep_output::convert_event(event_t& event)  {
  event.convert_event();
}

void alpha::edm4hep_output::event_t::convert_event()  {
  /// First create the Monte-Carlo particles and fill the properties
  this->process_mc_particles();
  /// Create the charged reconstructed particles and fill the properties
  this->process_charged_tracks();
  /// Link reconstructed charged tracks from FRFT to MC tracks
  this->process_pasl();

  /// Convert VDET hits and clusters
  this->convert_vdet();
  /// Convert ITC hits and clusters
  this->convert_itc();
  /// Convert TPC hits and clusters
  this->convert_tpc();
  /// Connect coordinates to frft charged tracks
  this->process_frtl();

  /// Convert ECAL hits and clusters
  this->convert_ecal();
  /// Convert HCAL hits and clusters
  this->convert_hcal();

  //aublis("CERST");
#if 0
  this->print_fkin();
  this->print_cht();
  this->print_eflw();
  this->print_gampec();
  this->print_calobj();
  this->print_neutrals();
  this->print_jets();
  this->print_qvec("Standard V0s",            qcde.KFV0T, qcde.KLV0T);
  this->print_qvec("Tracks from V0 vertices", qcde.KFDCT, qcde.KLDCT);
#endif
  // this->print_qvec("Long V0 tracks",          qcde.KFLVT, qcde.KLLVT);
}

/// Convert the VDET hits and clusters
void alpha::edm4hep_output::event_t::convert_vdet()  {
  /// Create all VDET GALEPH hits from VDHT
  this->process_vdht();
  /// Create all VDET clusters from VDCO
  this->process_vdco();
  /// Process VDCO relations to FKIN
  this->process_vdfk();

  /// Process the list od VDXY bank:
  this->process_vdxy();
  /// Process the list od VDZT bank:
  this->process_vdzt();
  /// Process VDXY and VDZT relations to FKIN
  this->process_vufk();

  /// Process VCPL bank to associate VDXY/VDZT to FRFT
  this->process_vcpl();
}

/// Convert ITC hits and clusters
void alpha::edm4hep_output::event_t::convert_itc()  {
  /// Process PIDI ITC digitisings (NR=0)
  this->process_pidi();
  /// Process TPC coordinates from ICCO
  this->process_icco();
  /// ITCO table bank: ITc COordinates (Recon. Bank) 
  this->process_itco();
}

/// Convert TPC hits and clusters
void alpha::edm4hep_output::event_t::convert_tpc()  {
  /// Process TPC coordinates from TPCO
  this->process_tpco();
  /// CreateProduction output Tpc pad Coordinates (NR=0)
  this->process_ptnc();
  /// Production output Tpc track pad dE/dX (NR=0)
  this->process_texs();
  this->process_t2xs();
}

/// Convert ECAL hits and clusters
void alpha::edm4hep_output::event_t::convert_ecal()  {  
  /// Create all ECAL clusters from PECO
  this->process_peco();
  /// Create HCAL cluster relations from PECO to FRFT tracks: bank PFHR
  this->process_pfer();
  /// Analyze ECAL wire data from PEWI bank
  this->process_pewi();
  /// ETDI table bank: Ecal Tower DIgits NR=0. (RAW)
  this->process_etdi();
}

/// Convert HCAL hits and clusters
void alpha::edm4hep_output::event_t::convert_hcal()  {  
  /// Create all HCAL clusters from PHCO
  this->process_phco();
  /// HPDI table bank: Hcal Plane DIgits (RawData)
  this->process_hpdi();
  /// Create HCAL cluster relations from PHCO to FRFT tracks: bank PFHR
  this->process_pfhr();
}

/// Default constructor
alpha::edm4hep_output::data_access_t::data_access_t(const std::string& debug_banks)  {

  this->config_data(this->qvec, "QVEC", debug_banks);
  this->config_data(this->qvrt, "QVRT", debug_banks);
  this->config_data(this->qdet, "QDET", debug_banks);
  this->config_data(this->frft, "FRFT", debug_banks);
  this->config_data(this->frtl, "FRTL", debug_banks);
  this->config_data(this->pasl, "PASL", debug_banks);
  this->config_data(this->pitm, "PITM", debug_banks);

  this->config_data(this->peco, "PECO", debug_banks);
  this->config_data(this->pgac, "PGAC", debug_banks);
  this->config_data(this->pewi, "PEWI", debug_banks);
  this->config_data(this->etdi, "ETDI", debug_banks);

  this->config_data(this->phco, "PHCO", debug_banks);
  this->config_data(this->hpdi, "HPDI", debug_banks);
  
  this->config_data(this->vdco, "VDCO", debug_banks);
  this->config_data(this->fvcl, "FVCL", debug_banks);
  this->config_data(this->vdxy, "VDXY", debug_banks);
  this->config_data(this->vdzt, "VDZT", debug_banks);
  this->config_data(this->vcpl, "VCPL", debug_banks);

  this->config_data(this->vfhl, "VFHL", debug_banks);
  this->config_data(this->vfph, "VFPH", debug_banks);
  this->config_data(this->vflg, "VFLG", debug_banks);
  this->config_data(this->vufk, "VUFK", debug_banks);
  this->config_data(this->vdfk, "VDFK", debug_banks);
  this->config_data(this->vdht, "VDHT", debug_banks);
  this->config_data(this->vdgc, "VDGC", debug_banks);

  this->config_data(this->itco, "ITCO", debug_banks);
  this->config_data(this->ficl, "FICL", debug_banks);
  this->config_data(this->pidi, "PIDI", debug_banks);
  this->config_data(this->icco, "ICCO", debug_banks);

  this->config_data(this->ptun, "PTUN", debug_banks);
  this->config_data(this->tpco, "TPCO", debug_banks);
  this->config_data(this->ftcl, "FTCL", debug_banks);
  this->config_data(this->ptnc, "PTNC", debug_banks);
  this->config_data(this->texs, "TEXS", debug_banks);
  this->config_data(this->t2xs, "T2XS", debug_banks);

  this->debug_mc_particles = false;
  this->debug_charged_tracks = false;
}

/// Default destructor
alpha::edm4hep_output::data_access_t::~data_access_t()  {
}
void alpha::edm4hep_output::data_access_t::config_data(bank_access_t& acc, const char* nam, const std::string& debug_banks)  {
  acc.nami = bos77::namind(nam);
  acc.debug = (debug_banks == "ALL") || (debug_banks.find(nam) != std::string::npos);
}

void alpha::edm4hep_output::data_access_t::event_config()  {
  this->qvec.load();
  this->qdet.load();
  this->qvrt.load();
  this->frft.load();
  this->ptun.load();
}

/// Default constructor
alpha::edm4hep_output::event_t::event_t(experiment_t& ex, data_access_t& dat)
  : exp(ex), data(dat), invalid_mc_particle()
{
  invalid_mc_particle.unlink();
}

/// Default destructor
alpha::edm4hep_output::event_t::~event_t()  {
}

/// Print particle tables
int alpha::edm4hep_output::event_t::print_qvec(const char* title, int first, int last)  const  {
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
int alpha::edm4hep_output::event_t::print_fkin()  const  {
  return this->print_qvec("Monte-Carlo Tracks", qcde.KFMCT, qcde.KLMCT);
}

/// Print table of charged tracks
int alpha::edm4hep_output::event_t::print_cht()  const  {
  return this->print_qvec("Charded Tracks", qcde.KFCHT, qcde.KLCHT);
}

/// Print table of EFLW (energy flow objects)
int alpha::edm4hep_output::event_t::print_eflw()  const  {
  return this->print_qvec("Energy Flow Tracks", qcde.KFEFT, qcde.KLEFT);
}

/// Print table of Calorimeter Objects
int alpha::edm4hep_output::event_t::print_calobj()  const  {
  return this->print_qvec("Calorimeter Objects", qcde.KFIST, qcde.KLAST);
}

/// Print table of GAMPEC gammas
int alpha::edm4hep_output::event_t::print_gampec()  const  {
  return this->print_qvec("GAMPEC gammas", qcde.KFGAT, qcde.KLGAT);
}

/// Print table of neutrals
int alpha::edm4hep_output::event_t::print_neutrals()  const  {
  return this->print_qvec("Neutral particles", qcde.KFNET, qcde.KLNET);
}

/// Print table of jets
int alpha::edm4hep_output::event_t::print_jets()  const  {
  return this->print_qvec("JETS", qcde.KFJET, qcde.KLJET);
}

/// Access Monte-Carlo particle by the alpha  MC track number
edm4hep::MutableMCParticle
alpha::edm4hep_output::event_t::particle_mc_alpha(std::size_t itk)  {
  auto& cont = this->alpha2edm4hep_particles;
  auto itr = cont.find(itk);
  if( itr != cont.end() )  {
    mcparticle_t p = this->particles_mc.at(itr->second);
    return p;
  }
  std::string err = "Non existing object: MutableMCParticle ALPHA MCT id: "+std::to_string(itk);
  throw std::runtime_error(err);
}

/// Access Monte-Carlo particle by the FKIN bank number
edm4hep::MutableMCParticle
alpha::edm4hep_output::event_t::particle_mc_fkin(std::size_t itk)  {
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

/// Access charged track by the FRFT track number
edm4hep::MutableTrack
alpha::edm4hep_output::event_t::particle_frft(std::size_t itk)  {
  auto& cont = this->alpha2edm4hep_charged;
  auto itr = cont.find(itk);
  if( itr != cont.end() )  {
    track_t p = this->particles_reco.at(itr->second);
    return p;
  }
  std::string err = "Non existing object: MutableTrack id: "+std::to_string(itk);
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
void alpha::edm4hep_output::event_t::link_hit_to_frft_track(std::size_t frft_track_number,
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
void alpha::edm4hep_output::event_t::process_calorimeter_objects()  {
  for( int itk = qcde.KFCOT; itk <= qcde.KLAST; ++itk )  {
    std::size_t key   = this->calorimeter_objects.size();
    auto*       track = this->data.qvec.row<class qvec>(itk);
    int32_t cal_track = track->ktn();
    if( key && track && cal_track ) {
    }
  }
}


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

/// Alpha include files
#include <alpha/frft.h>
#include <alpha/frtl.h>
#include <alpha/frid.h>
#include <alpha/eidt.h>
#include <alpha/hmad.h>
#include <alpha/mcad.h>
#include <alpha/muid.h>
#include <alpha/pitm.h>

#include "edm4hep_output_mc_particles.h"
#include "edm4hep_output_charged_tracks.h"

#include "edm4hep_output_pasl.h"

/// VDET coordinated NR=0 (POT)   
#include "edm4hep_output_vdco.h"
/// MVD hits in r-phi wafer.
#include "edm4hep_output_vdxy.h"
/// MVD hits in z wafer. (POT) 
#include "edm4hep_output_vdzt.h"
/// Vdco to FKIN truth relation       
#include "edm4hep_output_vdfk.h"
/// Vdxy/vdzt to FKIN truth 
#include "edm4hep_output_vufk.h"
/// VDet HiT list NR=0 (GAL)      
#include "edm4hep_output_vdht.h"
/// Process VCPL bank to associate VDXY/VDZT to FRFT
#include "edm4hep_output_vcpl.h"

/// Process PIDI ITC digitisings (NR=0)
#include "edm4hep_output_pidi.h"
/// Process ITC coordinates from ICCO
#include "edm4hep_output_icco.h"
/// ITCO table bank: ITc COordinates (Recon. Bank) 
#include "edm4hep_output_itco.h"

/// Process TPC coordinates from TPCO
#include "edm4hep_output_tpco.h"
/// Process raw TPC pad coordinates from PTNC
#include "edm4hep_output_ptnc.h"
/// Process Production output Tpc track pad dE/dX (NR=0)
#include "edm4hep_output_texs.h"
#include "edm4hep_output_t2xs.h"

/// Process FRTL table: Tpc+Itc+Vdet Geometry Track point List
#include "edm4hep_output_frtl.h"

/// Create ECAL cluster from PECO row
#include "edm4hep_output_peco.h"
/// Analyze ECAL wire data from PEWI bank
#include "edm4hep_output_pewi.h"
/// ETDI table bank: Ecal Tower DIgits NR=0. (RAW)
#include "edm4hep_output_etdi.h"
/// Create HCAL cluster relations from PECO to FRFT tracks: bank PFHR
#include "edm4hep_output_pfer.h"

/// PHCO table bank: Hadron Calorimeter Object (Mini: DHCO)
#include "edm4hep_output_phco.h"
/// PFHR tabel bank: Create HCAL cluster relations from PHCO to FRFT tracks
#include "edm4hep_output_pfhr.h"
/// HPDI table bank: Hcal Plane DIgits (RawData)
#include "edm4hep_output_hpdi.h"

/// Convert gammas from PGAC.
#include "edm4hep_output_pgac.h"

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

