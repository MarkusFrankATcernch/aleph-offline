//==========================================================================
//  Software for the ALEPH experiment
//--------------------------------------------------------------------------
// Copyright (C) Organisation europeenne pour la Recherche nucleaire (CERN)
// All rights reserved.
//
// For the licensing terms see ALSOFTINSTALL/LICENSE.
// For the list of contributors see ALSOFTINSTALL/doc/CREDITS.
//
// Author     : M.Frank
//
//==========================================================================
#ifndef ALPHA_BOS_QVEC_H
#define ALPHA_BOS_QVEC_H

/// C/C++ include files
#include <cmath>

/// Framework include files
#include <alpha/defs.h>

/// ALPHA namespace declaration
namespace alpha  {

  /// Forward declarations
  class qdet;
  class frft;
  class frtl;
  class frid;
  class texs;
  class peco;
  class phco;
  class eidt;
  class hmad;
  class muid;
  class yv0v;
  class pcqa;
  class pgac;

  /// Track data from bank QVEC
  /** HAC parameters and description from ALPHA manual Appendix B:
   *
   *   PARAMETER (JQVEQX= 1,JQVEQY= 2,JQVEQZ= 3,JQVEQE= 4,JQVEQM= 5,
   *  & JQVEQP= 6,JQVECH= 7,JQVETN= 8,JQVESC= 9,JQVEKS=10,JQVECL=11,
   *  & JQVEPA=12,JQVEQD=13,JQVENP=14,JQVESP=15,JQVEOV=16,JQVEEV=17,
   *  & JQVEND=18,JQVEDL=19,JQVENO=20,JQVEOL=21,JQVENM=22,JQVEML=23,
   *  & JQVEBM=24,JQVELK=38,JQVEDB=39,JQVEZB=40,JQVESD=41,JQVESZ=42,
   *  & JQVECB=43,JQVEEM=44,JQVECF=54,JQVEEW=55,JQVEUS=56)
   *
   *   \author M.Frank
   *   \version 1.0
   */
  class qvec  {
  public:
    // Note: HAC parameters in C are one lower to F77
    constexpr static const int32_t JQVEQX  = 0;
    constexpr static const int32_t JQVEQY  = 1;
    constexpr static const int32_t JQVEQZ  = 2;
    constexpr static const int32_t JQVEQE  = 3;
    constexpr static const int32_t JQVEQM  = 4;
    constexpr static const int32_t JQVEQP  = 5;
    constexpr static const int32_t JQVEQCH = 6;
    constexpr static const int32_t JQVETN  = 7;
    constexpr static const int32_t JQVESC  = 8;
    constexpr static const int32_t JQVEKS  = 9;
    constexpr static const int32_t JQVECL  = 10;

    constexpr static const int32_t JQVEPA  = 11;
    constexpr static const int32_t JQVEQD  = 12;

    constexpr static const int32_t JQVEOV  = 15;
    constexpr static const int32_t JQVEEV  = 16;

    constexpr static const int32_t JQVEND  = 17;
    constexpr static const int32_t JQVEDL  = 18;

    constexpr static const int32_t JQVENO  = 19;
    constexpr static const int32_t JQVEOL  = 20;

    constexpr static const int32_t JQVENM  = 21;
    constexpr static const int32_t JQVENL  = 22;

    constexpr static const int32_t JQVEBM  = 23;
    
    constexpr static const int32_t JQVELK  = 37;

    constexpr static const int32_t JQVEDB  = 38;

    constexpr static const int32_t KCQVEC  = 73;

  public:
    float    px                    { 0e0 };              // JQVEQX= 0
    float    py                    { 0e0 };              // JQVEQY= 1
    float    pz                    { 0e0 };              // JQVEQZ= 2
    float    energy                { 0e0 };              // JQVEQE= 3
    float    mass                  { 0e0 };              // JQVEQM= 4
    float    momentum              { 0e0 };              // JQVEQP= 5
    float    charge                {   0 };              // JQVECH= 6
    uint32_t frft_row              {   0 };              // JQVETN= 7
    int32_t  stability_code        {   0 };              // JQVESC= 8
    int32_t  lund_status           {   0 };              // JQVEKS= 9
    int32_t  track_class           {   0 };              // JQVECL=10
    int32_t  particle_code         {   0 };              // JQVEPA=11
    int32_t  offset_qdet           {   0 };              // JQVEQD=12
    int32_t  pointer_next          {   0 };              // JQVENP=13
    int32_t  pointer_same          {   0 };              // JQVESP=14

    int32_t  pointer_origin_vtx    {   0 };              // JQVEOV=15
    int32_t  pointer_end_vtx       {   0 };              // JQVEEV=16
    
    uint32_t number_daughters      {   0 };              // JQVEND=17
    int32_t  offset_first_daughter {   0 };              // JQVEDL=18

    uint32_t number_mothers        {   0 };              // JQVENO=19
    int32_t  offset_first_mother   {   0 };              // JQVEOL=20

    uint32_t number_matches        {   0 };              // JQVENM=21
    int32_t  offset_matches        {   0 };              // JQVEML=22

    int32_t  lock_mask[14];                              // JQVEBM=23
    int32_t  flag_qlitk            {   0 };              // JQVELK=37
    float    d0                    { 0e0 };              // JQVEDB=38
    float    z0                    { 0e0 };              // JQVEZB=39
    float    err2_d0               { 0e0 };              // iQVESD=40
    float    err2_z0               { 0e0 };              // JQVESZ=41
    float    chi2_track            { 0e0 };              // JQVECB=42
    float    cov[10];                                    // JQVEEM=43
    float    chi2_kinematic        { 0e0 };              // JQVECF=53
    float    weight_energy_flow    { 0e0 };              // JQVEEW=54
    float    user_space[17];                             // JQVEUS=55

  protected:
    /// Default constructor inhibited: object lives in the BOS common block
    qvec() = delete;
    /// Default destructor inhibited: object lives in the BOS common block
    ~qvec() = delete;

  public:
    /// Ease switching from single to double precision
    using real_t = float;

    /** Alpha like access functions   */
    /// x momentum component
    real_t  qx()       const   {  return this->px;                               }
    /// y momentum component
    real_t  qy()       const   {  return this->py;                               }
    /// z momentum component
    real_t  qz()       const   {  return this->pz;                               }
    /// Mass (use QMASV0 for V0 mass; see below)
    real_t  qm()       const   {  return this->mass;                             }
    /// Energy
    real_t  qe()       const   {  return this->energy;                           }
    /// CHarge
    real_t  qch()      const   {  return this->charge;                           }
    /// distance of closest approach to beam axis
    real_t  qdb()      const   {  return this->d0;                               }
    /// z coordinate of track point where QDB is measured
    real_t  qzb()      const   {  return this->z0;                               }
    /// Error squared on QDB
    real_t  qdbs2()    const   {  return this->err2_d0;                          }
    /// Error squared on QZB
    real_t  qzbs2()    const   {  return this->err2_z0;                          }
    /// χ2 due to QDB and QZB.
    real_t  qbc2()     const   {  return this->chi2_track;                       }
    real_t  qmchif()   const   {  return this->chi2_kinematic;                   }

    bool   xsig()      const   {  return (this->cov[0] > 0e0);                   }

    real_t  qx2()      const   {  return this->px * this->px;                    }
    real_t  qy2()      const   {  return this->py * this->py;                    }
    real_t  qz2()      const   {  return this->pz * this->pz;                    }
    real_t  qp2()      const   {  return this->qx2()+this->qy2()+this->qz2();    }
    real_t  qe2()      const   {  return this->energy * this->energy;            }

    real_t  qpt()      const   {  return std::sqrt(this->qx2()+this->qy2());     }
    real_t  qp()       const   {  return std::sqrt(this->qp2());                 }

    /// Access to coveariance matrix from particle fit
    real_t  qsmat(int i) const {  return cov[i];                                 }
    /// Error squared on particle x-momentum from particle fit
    real_t  qsigqx2()  const   {  return this->cov[0];                           }
    /// Error on particle x-momentum from particle fit
    real_t  qsigqx()   const   {  return std::sqrt(this->cov[0]);                }
    /// Error squared on particle y-momentum from particle fit
    real_t  qsigqy2()  const   {  return this->cov[2];                           }
    /// Error on particle y-momentum from particle fit
    real_t  qsigqy()   const   {  return std::sqrt(this->cov[2]);                }
    /// Error squared on particle z-momentum from particle fit
    real_t  qsigqz2()  const   {  return this->cov[5];                           }
    /// Error on particle z-momentum from particle fit
    real_t  qsigqz()   const   {  return std::sqrt(this->cov[5]);                }

    /// Error squared on particle energy from particle fit
    real_t  qsige2()   const   {  return this->cov[9];                           }
    /// Error on particle energy from particle fit
    real_t  qsige()   const    {  return std::sqrt(this->cov[9]);                }

    /// Error squared on total momentum from particle fit
    real_t  qsigp2()  const;
    /// Error on total momentum from particle fit
    real_t  qsigp()   const    {  return std::sqrt(this->qsigp2());              }
    /// Error squared on mass from particle fit
    real_t  qsigm2()  const;
    /// Error on mass from particle fit
    real_t  qsigm()   const    {  return std::sqrt(this->qsigm2());              }   

    /// Polar angle of the track
    real_t  qphi()    const    {  return std::atan2(this->qy(), this->qx());     }

    /// Particle class (see ALPHA user's manual for details)
    int32_t kclass()  const    {  return this->track_class;                      }
    /// Check if track a MC particle
    bool    xmc()  const       {  return this->track_class != 1;                 }

    /// Stability code (see ALPHA user's manual for details)
    int32_t kstabc()  const    {  return this->stability_code;                   }
    /// Track's particle code (see ALPHA user's manual for details)
    int32_t ktpcod()  const    {  return this->particle_code;                    }
    /// Track's particle name (see ALPHA user's manual for details)
    std::string cqtpn() const;
    
    /// GALEPH/ JULIA/ ENFLW track number (see ALPHA user's manual for details)
    uint32_t ktn()     const    {  return this->frft_row;                         }

    /// Number of mother tracks
    uint32_t knmoth()   const  {  return this->number_mothers;                   }
    /// Access index of the charged mother track
    uint32_t kmoth(uint32_t i) const {
      const auto* qlin = params.qlin_table->at(this->offset_first_mother + i + 1);
      return qlin ? qlin->link : 0;
    }
    /// Direct access to the charged mother track
    const class qvec* mother(uint32_t i)  const;

    /// Number of daughter tracks
    uint32_t kndau()   const   {  return this->number_daughters;                 }
    /// Access index of the charged daughter track
    uint32_t kdau(uint32_t i) const  {
      const auto* qlin = params.qlin_table->at(this->offset_first_daughter + i + 1);
      return qlin ? qlin->link : 0;
    }
    /// Direct access to the charged daughter track
    const class qvec* daughter(uint32_t i)  const;

    /// Pointer to origin vertex
    uint32_t koriv()  const     {  return this->pointer_origin_vtx;              }
    /// Pointer to start vertex
    const class qvrt* origin_vtx()  const;
    /// Pointer to end vertex
    uint32_t kendv()  const     {  return this->pointer_end_vtx;                 }
    /// Pointer to end vertex
    const class qvrt* end_vtx()  const;

    /// Access to detector information
    const class qdet* qdet() const { return bcs_offset<class qdet>(this->offset_qdet); }

    /// Check if track fit data are available for track I
    bool              xfrf()  const;
    /// Access to the track's corresponding bank FRFT: Global geometrical track fit
    const class frft* frft()  const;

    /// Access to the track's corresponding bank FRTL: Number of coordinates used for the global fit
    const class frtl* frtl()  const;
    /// Access to the track's corresponding bank FRID: Charged−particle identification
    const class frid* frid()  const;

    /// Check if dE/dx is available for track I
    bool              xtexs()  const;
    /// Number of TPC sectors on track I (max: 5 sectors accessible)
    uint32_t          kntexs() const;
    /// Access dE/dX information for each sector
    const class texs* texs(uint32_t i)  const;

    /// Check if ECAL data (PECO) are available for calorimeter object “track”
    bool              xpeco()  const;
    /// number of associated ECAL objects
    uint32_t          knecal()  const;
    /// Access ith associated ECAL object
    const class peco* peco(uint32_t i)  const;
    /// Reference to the ECAL track object
    const class qvec* ecal_track(uint32_t i)  const;

    /// Check if ECAL data (PEPT) are available for calorimeter object “track”
    bool              xpept()  const;

    /// Check if HCAL data (PHCO) are available for calorimeter object “track”
    bool              xphco()  const;
    /// number of associated HCAL objects
    uint32_t          knhcal()  const;
    /// Access ith associated HCAL object
    const class phco* phco(uint32_t i)  const;
    /// Reference to the HCAL track object
    const class qvec* hcal_track(uint32_t i)  const;

    /// Check if electron identification (bank EIDT) is available for this track
    bool              xeidt()  const;
    /// Access bank with electron identification information (bank EIDT)
    const class eidt* eidt()  const;

    /// Check if HCAL data (bank HMAD) are available for this track
    bool              xhmad()  const;
    /// Access bank HCAL data (bank HMAD) available for this track
    const class hmad* hmad()  const;

    /// Check if muon chamber data (bank MCAD) are available for this track
    bool              xmcad()  const;
    /// Access bank with muon chamber data (bank MCAD) for this track
    const class mcad* mcad()  const;

    /// Check if QMUIDO information (bank MUID) is available for this track
    bool              xmuid()  const;
    /// Access QMUIDO information (bank MUID) for this track
    const class muid* muid()  const;

    /// Check if energy flow (EFOL) data are available for track” I ( of the EFT section )
    bool              xefol()  const;
    /// Access energy flow (EFOL) data are available for track” I ( of the EFT section )
    const class efol* efol()  const;

    /// Check if V0 data are available for track I
    bool              xyv0v()  const;
    /// Access V0 data are available for track I
    const class efol* yv0v()  const;

    /// Check if PCQA data are available for track I
    bool              xpcqa()  const;
    /// Access PCQA data are available for track I
    const class pcqa* pcqa()  const;

    /// Check if GAMPECK data are available for “track” I of the GAT section
    bool              xpgac()  const;
    /// Access GAMPECK data are available for “track” I of the GAT section
    const class pgac* pgac()  const;

    /// Check if track is a Lepton tagged by QSELEP
    bool              xleptg() const;
    /// Properties of selected tagged Leptons (See ALPHA manual)
    int32_t           kleppa() const;
    /// ALPHA ”track” Number of nearest jet in LJET section
    int32_t           klepjt() const;
    /// Transverse momentum respect to the jet , lepton included
    real_t            qleppi() const;
    /// Transverse momentum respect to the jet , lepton excluded
    real_t            qleppe() const;
    /// Flag giving the validity of the Pt calculation (See ALPHA manual)
    int32_t           klepvp() const;
    /// Access results for track as a Lepton tagged by QSELEP
    const class pdlt* pdlt()  const;
    
    /// Check for results from the QTRUTH part of QSELEP
    bool              xlepth() const;
    /// Primary quark Flavour of the event
    int32_t           klepfl() const;
    /// Flavour of the quark from gluon splitting
    int32_t           kleppo() const;
    /// H.F. process originating the decay chain
    int32_t           klepch() const;
    /// End of the decay chain
    int32_t           klepsp() const;
    /// Track identity
    int32_t           kleple() const;
    /// Identity of the decaying b particle (if any)
    int32_t           klepme() const;
    /// FKIN track number of the lepton
    int32_t           klepkt() const;
    /// Access results from the QTRUTH part of QSELEP
    const class pmlt* pmlt()  const;
  };

  using track_data = bos_wrap_data<qvec>;
}       // End namespace alpha


#include <alpha/qdet.h>
#include <alpha/pdlt.h>
#include <alpha/pmlt.h>

/// ALPHA namespace declaration
namespace alpha  {
  
  /// Pointer to start vertex
  inline const class qvrt* qvec::origin_vtx()  const  {
    int32_t idx = this->koriv();
    return (idx > 0) ? params.qvrt_table->at(idx) : nullptr;
  }

  /// Pointer to end vertex
  inline const class qvrt* qvec::end_vtx()  const  {
    int32_t idx = this->kendv();
    return (idx > 0) ? params.qvrt_table->at(idx) : nullptr;
  }
  
  /// Check if track fit data are available for track I
  inline bool              qvec::xfrf() const       {  return this->qdet()->xfrf();      }
  /// Access to the track's corresponding bank FRFT: Global geometrical track fit
  inline const class frft* qvec::frft()  const      {  return this->qdet()->frft();      }

  /// Access to the track's corresponding bank FRTL: Number of coordinates used for the global fit
  inline const class frtl* qvec::frtl()  const      {  return this->qdet()->frtl();      }
  /// Access to the track's corresponding bank FRID: Charged−particle identification
  inline const class frid* qvec::frid()  const      {  return this->qdet()->frid();      }

  /// Check if dE/dx is available for track I
  inline bool              qvec::xtexs()  const     {  return this->qdet()->xtexs();     }
  /// Number of TPC sectors on track I (max: 5 sectors accessible)
  inline uint32_t          qvec::kntexs() const     {  return this->qdet()->kntexs();    }
  /// Access dE/dX information for each sector
  inline const class texs* qvec::texs(uint32_t i) const {  return this->qdet()->texs(i);     }

  /// Check if ECAL data (PECO) are available for calorimeter object “track”
  inline bool              qvec::xpeco()  const     {  return this->qdet()->xpeco();     }
  /// number of associated ECAL objects
  inline uint32_t          qvec::knecal()  const    {  return this->qdet()->knecal();    }
  /// Access ith associated ECAL object
  inline const class peco* qvec::peco(uint32_t i)  const {  return this->qdet()->peco(i);     }
  /// Reference to the ECAL track object
  inline const class qvec* qvec::ecal_track(uint32_t i)  const
  {  return this->qdet()->ecal_track(i);                                                 }

  /// Check if ECAL data (PEPT) are available for calorimeter object “track”
  inline bool              qvec::xpept()  const     {  return this->qdet()->xpept();     }

  /// Check if HCAL data (PHCO) are available for calorimeter object “track”
  inline bool              qvec::xphco()  const     {  return this->qdet()->xphco();     }
  /// number of associated HCAL objects
  inline uint32_t          qvec::knhcal()  const    {  return this->qdet()->knhcal();    }
  /// Access ith associated HCAL object
  inline const class phco* qvec::phco(uint32_t i)  const {  return this->qdet()->phco(i);}
  /// Reference to the HCAL track object
  inline const class qvec* qvec::hcal_track(uint32_t i)  const
  {  return this->qdet()->hcal_track(i);                                                 }

  /// Check if electron identification (bank EIDT) is available for this track
  inline bool              qvec::xeidt()  const     {  return this->qdet()->xeidt();     }
  /// Access bank with electron identification information (bank EIDT)
  inline const class eidt* qvec::eidt()  const      {  return this->qdet()->eidt();      }

  /// Check if HCAL data (bank HMAD) are available for this track
  inline bool              qvec::xhmad()  const     {  return this->qdet()->xhmad();     }
  /// Access HCAL data (bank HMAD) available for this track
  inline const class hmad* qvec::hmad()  const      {  return this->qdet()->hmad();      }

  /// Check if muon chamber data (bank MCAD) are available for this track
  inline bool              qvec::xmcad()  const     {  return this->qdet()->xmcad();     }
  /// Access bank with muon chamber data (bank MCAD) for this track
  inline const class mcad* qvec::mcad()  const      {  return this->qdet()->mcad();      }

  /// Check if QMUIDO information (bank MUID) is available for this track
  inline bool              qvec::xmuid()  const     {  return this->qdet()->xmuid();     }
  /// Access QMUIDO information (bank MUID) for this track
  inline const class muid* qvec::muid()  const      {  return this->qdet()->muid();      }

  /// Check if energy flow (EFOL) data are available for track” I ( of the EFT section )
  inline bool              qvec::xefol()  const      {  return this->qdet()->xefol();    }
  /// Access energy flow (EFOL) data are available for track” I ( of the EFT section )
  inline const class efol* qvec::efol()  const       {  return this->qdet()->efol();     }

  /// Check if V0 data are available for track I
  inline bool              qvec::xyv0v()  const      {  return this->qdet()->xyv0v();    }
  /// Access V0 data are available for track I
  inline const class efol* qvec::yv0v()  const       {  return this->qdet()->yv0v();     }

  /// Check if PCQA data are available for track I
  inline bool              qvec::xpcqa()  const      {  return this->qdet()->xpcqa();    }
  /// Access PCQA data are available for track I
  inline const class pcqa* qvec::pcqa()  const       {  return this->qdet()->pcqa();     }

  /// Check if GAMPECK data are available for “track” I of the GAT section
  inline bool              qvec::xpgac()  const      {  return this->qdet()->xpgac();    }
  /// Access GAMPECK data are available for “track” I of the GAT section
  inline const class pgac* qvec::pgac()  const       {  return this->qdet()->pgac();     }

  /// Check if track is a Lepton tagged by QSELEP
  inline bool              qvec::xleptg() const      {  return this->qdet()->xpdlt();    }
  /// Access results for track as a Lepton tagged by QSELEP
  inline const class pdlt* qvec::pdlt()  const       {  return this->qdet()->pdlt();     }
  /// Properties of selected tagged Leptons (See ALPHA manual)
  inline int32_t           qvec::kleppa() const      {  return this->qdet()->pdlt()->particletype(); }
  /// ALPHA ”track” Number of nearest jet in LJET section
  inline int32_t           qvec::klepjt() const      {  return this->qdet()->pdlt()->jeTnumber();    }
  /// Transverse momentum respect to the jet , lepton included
  inline qvec::real_t      qvec::qleppi() const      {  return this->qdet()->pdlt()->ptInclusif();   }
  /// Transverse momentum respect to the jet , lepton excluded
  inline qvec::real_t      qvec::qleppe() const      {  return this->qdet()->pdlt()->ptExclusif();   }
  /// Flag giving the validity of the Pt calculation (See ALPHA manual)
  inline int32_t           qvec::klepvp() const      {  return this->qdet()->pdlt()->validityPt();   }
  
  /// Check for results from the QTRUTH part of QSELEP
  inline bool              qvec::xlepth() const      {  return this->qdet()->xpmlt();    }
  /// Access results from the QTRUTH part of QSELEP
  inline const class pmlt* qvec::pmlt()  const       {  return this->qdet()->pmlt();     }
  /// Primary quark Flavour of the event
  inline int32_t           qvec::klepfl() const      {  return this->qdet()->pmlt()->flavorofquark();   }
  /// Flavour of the quark from gluon splitting
  inline int32_t           qvec::kleppo() const      {  return this->qdet()->pmlt()->poppingflag();     }
  /// H.F. process originating the decay chain
  inline int32_t           qvec::klepch() const      {  return this->qdet()->pmlt()->flagCHain();       } 
  /// End of the decay chain
  inline int32_t           qvec::klepsp() const      {  return this->qdet()->pmlt()->flagSPlit();       }
  /// Track identity
  inline int32_t           qvec::kleple() const      {  return this->qdet()->pmlt()->flagLEpton();      }
  /// Identity of the decaying b particle (if any)
  inline int32_t           qvec::klepme() const      {  return this->qdet()->pmlt()->flagMEss();        }
  /// FKIN track number of the lepton
  inline int32_t           qvec::klepkt() const      {  return this->qdet()->pmlt()->kineTrack();       }
  
}      // End namespace alpha
#endif // ALPHA_BOS_QVEC_H
