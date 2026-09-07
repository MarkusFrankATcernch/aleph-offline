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
#include <alpha/etdi.h>

/// ETDI table bank: Ecal Tower DIgits NR=0. (RAW)
void alpha::edm4hep_output::event_t::process_etdi()  {
  /**
 +------+                              Subschema: EcalRAWBanks
 | ETDI |  Ecal Tower DIgits NR=0.
 +------+  (RAW)

 ..............................................................
     1          I    Number of words/tower (=4)
     2          I    Number of towers
 ..............................................................
      1    TL  I    TowerLabel
                       tower label, see ETHT
                       tower address ( b 2-11=phi, 16-24=teta )
      2    S1  I    Stack1
                       stack 1 content ( keV )
      3    S2  I    Stack2
                       stack 2 content ( keV )
      4    S3  I    Stack3
                       stack 3 content ( keV )
 ..............................................................
  */
}
