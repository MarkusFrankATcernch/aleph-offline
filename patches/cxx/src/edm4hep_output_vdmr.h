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
#include <alpha/vfhl.h>
#include <alpha/vfph.h>
#include <alpha/vdxy.h>

/// Create VDET 3D hit from VDXY row
void alpha::edm4hep_output::event_t::process_vdxy()  {
  /**
     +------+                              Subschema: VdetPOTBanks
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
  */
  int32_t view = 1;
  for( this->data.vdxy.load(false); this->data.vdxy.data; this->data.vdxy.knext() )  {
    auto*    tab_vdxy = this->data.vdxy.table<class vdxy>();
    uint32_t row      = tab_vdxy->row();
    auto*    tab_vdmr = this->banks_vdmr[row+view];
    for( uint32_t ihit=1; ihit <= tab_vdxy->size(); ++ihit )  {
      std::size_t key = this->alpha2edm4hep_vdxy[(row<<16) + ihit];
      MutableTrackerHit3D hit = this->hits_vdxy.at(key);

    }
  }
}
