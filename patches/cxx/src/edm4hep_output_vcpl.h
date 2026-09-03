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

/// Alpha include files
#include <alpha/vcpl.h>
#include <alpha/vdco.h>
#include <alpha/vdxy.h>
#include <alpha/vdzt.h>

/// Process information from VCPL bank
void alpha::edm4hep_output::event_t::process_vcpl()  {
  /**
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

  from julia/vglob/vgstor.F we have the following relationship
  between VCPL, VDXY, VDZT, VDCO. VDXY and VDZT are parallel
  
  VCPL.JVCP-(XB+NX) == VDXY (wafer).(NR+row) \       /  NR match VDCO.JVDC-WI
  and                                         >>>>>> 
  VCPL.JVCP-(ZB+NZ) == VDZT (wafer).(NR+row) /       \ 
         
  */
  std::stringstream log;
  /// From the code in julia/vglob/vgstor.F the VCPL banks(+rows)
  /// should be created 1:1 with VDCO rows.
  auto* table_vdco = this->data.vdco.load<object_table<vdco> >();

  for( this->data.vcpl.load(false); this->data.vcpl.data; this->data.vcpl.knext() )  {
    auto*   table_vcpl = this->data.vcpl.table<vcpl>();
    int32_t track_vcpl = table_vcpl->row();
    int32_t indx_vdco  = -99999;

    for( uint32_t ivdco=1; ivdco <= table_vdco->size(); ++ivdco )  {
      if( table_vdco->row(ivdco)->trackNumber() == track_vcpl )  {
        indx_vdco = ivdco;
        break;
      }
    }
    if( this->data.vcpl.debug )  {
      log << bos77::to_string(table_vcpl) << " VDCO row: " << indx_vdco << std::endl;
    }
    for( uint32_t ivcpl=0; ivcpl<table_vcpl->size(); ++ivcpl )  {
      auto*   vcpl      = table_vcpl->at(ivcpl);
      int32_t row_vdxy  = vcpl->nrXyBank();
      int32_t hit_vdxy  = vcpl->nxy();
      int32_t ilay_vdxy = ((row_vdxy/10000));
      int32_t iz_vdxy   = ((row_vdxy/1000)%10);
      int32_t iphi_vdxy = ((row_vdxy/10)%100);

      int32_t row_vdzt  = vcpl->nrZBank();
      int32_t hit_vdzt  = vcpl->nz();
      int32_t ilay_vdzt = ((row_vdzt/10000));
      int32_t iz_vdzt   = ((row_vdzt/1000)%10);
      int32_t iphi_vdzt = ((row_vdzt/10)%100);

      int32_t hid_vdxy  = this->alpha2edm4hep_vdxy[(row_vdxy<<16) + hit_vdxy];
      int32_t hid_vdzt  = this->alpha2edm4hep_vdzt[(row_vdzt<<16) + hit_vdzt];
      auto    ehit_vdxy = this->hits_vdxy[hid_vdxy];
      auto    ehit_vdzt = this->hits_vdzt[hid_vdzt];
      auto    energy    = std::max(1e-6,double(ehit_vdxy.getEDep()+ehit_vdzt.getEDep()));
      int32_t trk_vdco  = -99999;

      /// Associate VDXY/VDZT hits to reconstructed track
      this->link_hit_to_frft_track(track_vcpl, ehit_vdzt);
      this->link_hit_to_frft_track(track_vcpl, ehit_vdxy);

      if( indx_vdco > 0 )  {
        auto vdco = this->hits_vdco[indx_vdco-1];
        /// Update VDCO energy (not present in VDCO bank)
        vdco.setEDep(energy);
        _set_link(rel_hits_vdco_vdxy, vdco, ehit_vdxy, ehit_vdxy.getEDep()/energy);
        _set_link(rel_hits_vdco_vdzt, vdco, ehit_vdzt, ehit_vdzt.getEDep()/energy);

        trk_vdco = table_vdco->row(indx_vdco)->trackNumber();
        ++indx_vdco;
      }
      // Do some checks if the hit links are valid:
      // - VCPL and VDCO must refer to the same FTFT tracks
      // - for VDXY and VDZT the layer must be the same
      // - typically iphi and iz are also the same,
      //   but is not always if the track is at low tan(lambda)
      // 
      // Checking VDCO is not so simple: we only have global coordinates
      bool problem = (track_vcpl != trk_vdco) || (ilay_vdxy != ilay_vdzt);
      if( this->data.vcpl.debug || problem )  {
        log << "\t";
        if( problem ) log << " LINK Problem:";
        log << " event: "      << std::setw(4) << std::left  << qcde.KNEVT
            << " FRFT VCPL/VDCO: " << std::setw(3) << std::right << trk_vdco
            << "/" << std::setw(3) << std::left  << track_vcpl
            << std::setw(4) << std::left    << ivcpl << ": "
            << " #XY:"      << std::setw(8) << std::right << row_vdxy
            << "."          << std::setw(2) << std::left  << hit_vdxy
            << " #ZT:"      << std::setw(8) << std::right << row_vdzt
            << "."          << std::setw(2) << std::left  << hit_vdzt
            << " Chi2:"     << double(vcpl->chi2())
            << " layer: "   << std::setw(1) << ilay_vdxy
            << "/"          << std::setw(1) << std::left << ilay_vdzt
            << " phi: "     << std::setw(2) << iphi_vdxy
            << "/"          << std::setw(2) << std::left << iphi_vdzt
            << " z: "       << std::setw(1) << iz_vdxy
            << "/"          << std::setw(1) << std::left << iz_vdzt
            << std::endl;
      }
    }
  }
  if( !log.str().empty() )  {
    ::printf("%s", log.str().c_str());
  }
}
