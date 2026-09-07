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
#include <alpha/pfer.h>

/// Create HCAL cluster relations from PECO to FRFT tracks: bank PFHR
void alpha::edm4hep_output::event_t::process_pfer()  {
  /**
     +------+                              Subschema: CobjPOTBanks
     | PFER |  Fitted track-Ecalobject
     +------+  Relations

     ..............................................................
     1          I    Number of words/relation (=2)
     2          I    Number of relations
     ..............................................................
      1    PE  I    PECO
                       Row index of EcalObject
      2    PF  I    PFRF
                       Row index of Fitted Track
  */
  // Do not know how to resolve this: Calorimeter hits cannot attached to edm4hep::MutableTrack
}
