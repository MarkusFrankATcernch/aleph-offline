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

/// Process information from VCPL bank
void alpha::output_edm4hep::event_t::process_vcpl()  {
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
  */

  for( int itk = qcde.KFCHT; itk <= qcde.KLCHT; ++itk )  {
    auto* track = this->data.qvec.row<class qvec>(itk);
    int32_t julia_track = track->ktn();
    if( const auto* bank = bos77::get_bank(this->data.vcpl.nami, julia_track) )  {
      auto* table = (object_table<class vcpl>*)bank;
      std::stringstream log;

      if( this->data.vcpl.debug )  {
        log << bos77::to_string(table) << std::endl;
      }
      for( uint32_t i=0; i<table->size(); ++i )  {
        auto*   vcpl     = table->at(i);
        int32_t frft_num = table->bank_header::row();
        int32_t id_xy    = this->alpha2edm4hep_vdxy[(vcpl->nrXyBank()<<16) + vcpl->nxy()];
        int32_t id_z     = this->alpha2edm4hep_vdzt[(vcpl->nrZBank()<<16)  + vcpl->nz()];
        auto    hit_xy   = this->hits_vdxy[id_xy];
        auto    hit_z    = this->hits_vdzt[id_z];

        /// Associate VDXY/VDZT hits to reconstructed track
        this->link_hit_to_frft_track(frft_num, hit_z);
        this->link_hit_to_frft_track(frft_num, hit_xy);

        if( this->data.vcpl.debug )  {
          log << std::setw(4) << "\t" << std::left << i << ": "
              << " FRFT:"  << std::setw(3) << std::left  << frft_num
              << " #xy:"   << std::setw(8) << std::right << vcpl->nrXyBank()
              << " hit:"   << std::setw(4) << std::right << vcpl->nxy()
              << " #z:"    << std::setw(8) << std::right << vcpl->nrZBank()
              << " hit:"   << std::setw(4) << std::right << vcpl->nz()
              << " Chi2:"  << double(vcpl->chi2())
              << std::endl;
        }
      }
      if( this->data.vcpl.debug )  {
        ::printf("%s", log.str().c_str());
      }
    }
  }
}
