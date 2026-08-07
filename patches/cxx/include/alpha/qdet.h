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
#ifndef ALPHA_BOS_QDET_H
#define ALPHA_BOS_QDET_H

/// C/C++ include files

/// Framework include files
#include <alpha/defs.h>

/// ALPHA namespace declaration
namespace alpha  {

  /// Forward declarations
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

  /// Track data from bank QDET
  /** HAC parameters and description from ALPHA manual Appendix B:
   *
   *   PARAMETER ( JQDEAF= 1,JQDEAL= 2,JQDENT= 3,JQDEAT= 4,JQDELT= 8,
   *  &  JQDEAE= 9,JQDEAH=10,JQDEAM=11,JQDECF=12,JQDEEC=13,JQDEHC=14,
   *  &  JQDEET=15,JQDEFI=16,JQDENF=17,JQDEFL=18,JQDENE=19,JQDEEL=20,
   *  &  JQDENH=21,JQDEHL=22,JQDELH=23,JQDEEF=24,JQDEPC=25,JQDEEG=26,
   *  &  JQDEMU=27,JQDEDX=28,JQDEPG=29,JQDEPD=30,JQDEPM=31)
   *
   *  *------*
   *  | QDET |          DETECTOR INFORMATION
   *  *------*
   *
   *     1    number of words / track
   *     2    maximum allowed number of tracks
   *
   *     1  AF  I   offset for corresponding row in bank FRFT (NOT row number !)
   *     2  AL  I   offset for corresponding row in bank FRTL
   *     3  NT  I   number of segments in bank TEXS
   *     4  AT  I   offset for corresponding rows in bank TEXS (MAX : 5)
   *     8  LT  I   last allowed AT address
   *
   *     9  AE  I   offset for corresponding row in bank EIDT
   *    10  AH  I   offset for corresponding row in bank HMAD
   *    11  AM  I   offset for corresponding row in bank MCAD
   *
   *    12  CF  I   calorimeter flag (<0 : ass, > 0 : isol, abs=1 : ECAL; =2 : HCAL)
   *    13  EC  I   offset for corresponding row in bank PECO
   *    14  HC  I   offset for corresponding row in bank PHCO
   *    15  ET  I   offset for corresponding row in bank PEPT
   *    16  FI  I   offset for corresponding row in bank FRID
   *    17  NF  I   number of associated charged tracks
   *    18  FL  I   offset of 1st ass. ch. track in list QLIN
   *    19  NE  I   number of associated ECAL objects
   *    20  EL  I   offset of 1st ass. ECAL object in list QLIN
   *    21  NH  I   number of associated HCAL objects
   *    22  HL  I   offset of 1st ass. HCAL object in list QLIN
   *    23  LH  I   overlap - associated
   *    24  EF  I   offset for corresponding row in bank EFOL
   *    25  PC  I   offset for corresponding row in bank PCQA
   *    26  EG  I   offset for corresponding row in bank EGPC
   *    27  MU  I   offset for corresponding row in bank MUID
   *    28  DX  I   offset for corresponding row in bank
   *    29  PG  I   offset for corresponding row in bank PGPC
   *    30  PD  I   offset for corresponding row in bank PDLT
   *    31  PM  I   offset for corresponding row in bank PMLT
   *
   *
   *   \author M.Frank
   *   \version 1.0
   */
  class qdet  {
  public:
    // Note: HAC parameters in C are one lower to F77
    constexpr static const int32_t JQDEAF  =  0;
    constexpr static const int32_t JQDEAL  =  1;
    constexpr static const int32_t JQDENT  =  2;
    constexpr static const int32_t JQDEAT  =  3;
    constexpr static const int32_t JQDELT  =  7;
     
    constexpr static const int32_t JQDEAE  =  8;
    constexpr static const int32_t JQDEAH  =  9;
    constexpr static const int32_t JQDEAM  = 10;
    constexpr static const int32_t JQDECF  = 11;
    constexpr static const int32_t JQDEEC  = 12;
    constexpr static const int32_t JQDEHC  = 13;

    constexpr static const int32_t JQDEET  = 14;
    constexpr static const int32_t JQDEFI  = 15;
    constexpr static const int32_t JQDENF  = 16;
    constexpr static const int32_t JQDEFL  = 17;
    constexpr static const int32_t JQDENE  = 18;
    constexpr static const int32_t JQDEEL  = 19;
     
    constexpr static const int32_t JQDENH  = 20;
    constexpr static const int32_t JQDEHL  = 21;
    constexpr static const int32_t JQDELH  = 22;
    constexpr static const int32_t JQDEEF  = 23;
    constexpr static const int32_t JQDEPC  = 24;
    constexpr static const int32_t JQDEEG  = 25;

    constexpr static const int32_t JQDEMU  = 26;
    constexpr static const int32_t JQDEDX  = 27;
    constexpr static const int32_t JQDEPG  = 28;
    constexpr static const int32_t JQDEPD  = 29;
    constexpr static const int32_t JQDEPM  = 30;

  public:
    int32_t   offset_frft;                                     // JQDEAF  =  0;
    int32_t   offset_frtl;                                     // JQDEAL  =  1;
    uint32_t  num_segment_texs;                                // JQDENT  =  2;
    int32_t   offset_texs[5];                                  // JQDEAT  =  3;
    /* int32_t   offset_last_texs;   */                        // JQDELT  =  7;
    int32_t   offset_eidt;                                     // JQDEAE  =  8;
    int32_t   offset_hmad;                                     // JQDEAH  =  9;
    int32_t   offset_mcad;                                     // JQDEAM  = 10;
    int32_t   calorimeter_flag;                                // JQDECF  = 11;
    int32_t   offset_peco;                                     // JQDEEC  = 12;
    int32_t   offset_phco;                                     // JQDEHC  = 13;
    int32_t   offset_pept;                                     // JQDEET  = 14;
    int32_t   offset_frid;                                     // JQDEFI  = 15;

    uint32_t  num_associated_charged_tracks;                   // JQDENF  = 16;
    int32_t   offset_first_associated_track;  // QLIN          // JQDEFL  = 17;

    uint32_t  num_associated_ecal;                             // JQDENE  = 18;
    int32_t   offset_first_ecal;                               // JQDEEL  = 19;

    uint32_t  num_associated_hcal;                             // JQDENH  = 20;
    int32_t   offset_first_hcal;                               // JQDEHL  = 21;

    int32_t   verlap_associated;                               // JQDELH  = 22;

    int32_t   offset_efol;                                     // JQDEEF  = 23;
    int32_t   offset_pcqa;                                     // JQDEPC  = 24;
    int32_t   offset_egpc;                                     // JQDEEG  = 25;
    int32_t   offset_muid;                                     // JQDEMU  = 26;
    int32_t   offset_xxxx;                                     // JQDEDX  = 27;
    int32_t   offset_pgac;                                     // JQDEPG  = 28;
    int32_t   offset_pdlt;                                     // JQDEPD  = 29;
    int32_t   offset_pmlt;                                     // JQDEPM  = 30;

  protected:
    /// Default constructor inhibited: object lives in the BOS common block
    qdet() = delete;
    /// Default destructor inhibited: object lives in the BOS common block
    ~qdet() = delete;

  public:
    /// Check if track fit data are available for track I
    bool              xfrf()  const        {  return this->offset_frft != params.kqzer;          }
    /// Access to the track's corresponding bank FRFT: Global geometrical track fit
    const class frft* frft()  const        {  return bcs_offset<class frft>(this->offset_frft);  }

    /// Access to the track's corresponding bank FRTL: Number of coordinates used for the global fit
    const class frtl* frtl()  const        {  return bcs_offset<class frtl>(this->offset_frtl);  }
    /// Access to the track's corresponding bank FRID: Charged−particle identification
    const class frid* frid()  const        {  return bcs_offset<class frid>(this->offset_frid);  }

    /// Check if dE/dx is available for track I
    bool              xtexs()  const       {  return this->num_segment_texs != 0;                }
    /// Number of TPC sectors on track I (max: 5 sectors accessible)
    uint32_t          kntexs() const       {  return this->num_segment_texs;                     }
    /// Access dE/dX information for each sector
    const class texs* texs(uint32_t i) const { return bcs_offset<class texs>(this->offset_texs[i]); }

    /// Check if ECAL data (PECO) are available for calorimeter object “track”
    bool              xpeco()  const       {  return this->offset_peco != params.kqzer;          }
    /// number of associated ECAL objects
    uint32_t          knecal()  const      {  return this->num_associated_ecal;                  }
    /// Access corresponding ECAL data (PECO) for calorimeter object “track”
    const class peco* peco(uint32_t i)  const;
    /// Access ith associated ECAL bank row object
    uint32_t          peco_rownum(uint32_t i)  const;
    /// Reference to the ECAL track object
    const class qvec* ecal_track(uint32_t i)  const;
    /// QVEC index of the ECAL track object
    uint32_t          ecal_track_rownum(uint32_t i)  const;

    /// Check if ECAL data (PEPT) are available for calorimeter object “track”
    bool              xpept()  const       {  return this->offset_pept != params.kqzer;          }

    /// Check if HCAL data (PHCO) are available for calorimeter object “track”
    bool              xphco()  const       {  return this->offset_phco != params.kqzer;          }
    /// number of associated HCAL objects
    uint32_t          knhcal()  const      {  return this->num_associated_hcal;                  }
    /// Access corresponding HCAL data (PHCO) for calorimeter object “track”
    const class phco* phco(uint32_t i)  const;
    /// Access ith associated HCAL bank row object
    uint32_t          phco_rownum(uint32_t i)  const;
    /// Reference to the HCAL track object
    const class qvec* hcal_track(uint32_t i)  const;
    /// QVEC index of the HCAL track object
    uint32_t          hcal_track_rownum(uint32_t i)  const;

    /// Check if electron identification (bank EIDT) is available for this track
    bool              xeidt()  const       {  return this->offset_eidt != params.kqzer;          }
    /// Access bank with electron identification information (bank EIDT)
    const class eidt* eidt()  const        {  return bcs_offset<class eidt>(this->offset_eidt);  }

    /// Check if HCAL data (bank HMAD) are available for this track
    bool              xhmad()  const       {  return this->offset_hmad != params.kqzer;          }
    /// Access bank HCAL data (bank HMAD) available for this track
    const class hmad* hmad()  const        {  return bcs_offset<class hmad>(this->offset_hmad);  }

    /// Check if muon chamber data (bank MCAD) are available for this track
    bool              xmcad()  const       {  return this->offset_mcad != params.kqzer;          }
    /// Access bank with muon chamber data (bank MCAD) for this track
    const class mcad* mcad()  const        {  return bcs_offset<class mcad>(this->offset_mcad);  }

    /// Check if QMUIDO information (bank MUID) is available for this track
    bool              xmuid()  const       {  return this->offset_muid != params.kqzer;          }
    /// Access QMUIDO information (bank MUID) for this track
    const class muid* muid()  const        {  return bcs_offset<class muid>(this->offset_muid);  }

    /// Check if energy flow (EFOL) data are available for track” I ( of the EFT section )
    bool              xefol()  const       {  return this->offset_efol != params.kqzer;          }
    /// Access energy flow (EFOL) data are available for track” I ( of the EFT section )
    const class efol* efol()  const        {  return bcs_offset<class efol>(this->offset_efol);  }

    /// Check if V0 data are available for track I
    bool              xyv0v()  const       {  return false;   } // this->offset_yv0v != params.kqzer; }
    /// Access V0 data are available for track I
    const class efol* yv0v()  const        {  return nullptr; } // bcs_offset<class yv0v>(this->offset_yv0v);  }

    /// Check if PCQA data are available for track I
    bool              xpcqa()  const       {
      return params.pcqa_table && this->offset_pcqa != params.kqzer;
    }
    /// Access PCQA data are available for track I
    const class pcqa* pcqa()  const        {
      return params.pcqa_table ? params.pcqa_table->at(this->offset_pcqa+1) : nullptr;
    }

    /// Check if GAMPECK data are available for “track” I of the GAT section
    bool              xpgac()  const       {
      return params.pgac_table && this->offset_pgac != params.kqzer;
    }
    /// Access GAMPECK data are available for “track” I of the GAT section
    const class pgac* pgac()  const        {  return bcs_offset<class pgac>(this->offset_pgac);  }

    /// Check if PDLT data are available for “track”
    bool              xpdlt()  const       {
      return params.pdlt_table && this->offset_pdlt != params.kqzer;
    }
    /// Access PDLT data are available for “track”
    const class pdlt* pdlt()  const        {
      if( params.pdlt_table )  {
        return params.pdlt_table->at(this->offset_pdlt+1);
      }
      return nullptr;
    }
  //const class pdlt* pdlt()  const        {  return bcs_offset<class pdlt>(this->offset_pdlt);  }
    
    /// Check if PMLT data are available for “track”
    bool              xpmlt()  const       {
      return params.pmlt_table && this->offset_pmlt != params.kqzer;
    }
    /// Access PMLT data are available for “track”
    const class pmlt* pmlt()  const        {
      if( params.pmlt_table )  {
        return params.pmlt_table->at(this->offset_pmlt+1);
      }
      return nullptr;
    }
    //const class pmlt* pmlt()  const        {  return bcs_offset<class pmlt>(this->offset_pmlt);  }
  };
}      // End namespace alpha

/// Framework include files
#include <alpha/qvec.h>

/// ALPHA namespace declaration
namespace alpha  {

  inline const class qvec* qdet::ecal_track(uint32_t i)  const  {
    const auto* qlin = params.qlin_table->at(i + this->offset_first_ecal + 1);
    return params.qvec_table->at(qlin->link);
  }

  inline uint32_t qdet::ecal_track_rownum(uint32_t i)  const  {
    return params.qlin_table->at(i + this->offset_first_ecal + 1)->link;
  }

  /// Access ith associated ECAL bank row object
  inline uint32_t qdet::peco_rownum(uint32_t i)  const  {
    return this->ecal_track(i)->ktn();
  }

  /// Access corresponding ECAL data (PECO) for calorimeter object “track”
  inline const class peco* qdet::peco(uint32_t i)  const {
    const auto* track = this->ecal_track(i);
    return params.peco_table->at(track->ktn());
  }
    
  /// Reference to the HCAL track object
  inline const class qvec* qdet::hcal_track(uint32_t i)  const  {
    const auto* qlin = params.qlin_table->at(i + this->offset_first_hcal + 1);
    return params.qvec_table->at(qlin->link);
  }

  /// Access corresponding HCAL data (PHCO) for calorimeter object “track”
  inline const class phco* qdet::phco(uint32_t i)  const  {
    const auto* track = this->hcal_track(i);
    return params.phco_table->at(track->ktn());
  }

  /// Access ith associated HCAL bank row object
  inline uint32_t           qdet::phco_rownum(uint32_t i)  const  {
    return this->hcal_track(i)->ktn();
  }

  /// QVEC index of the HCAL track object
  inline uint32_t           qdet::hcal_track_rownum(uint32_t i)  const  {
    return params.qlin_table->at(i + this->offset_first_hcal + 1)->link;
  }

}      // End namespace alpha
#endif // ALPHA_BOS_QDET_H
