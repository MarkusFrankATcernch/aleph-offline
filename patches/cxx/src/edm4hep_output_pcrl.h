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
#include <alpha/pcrl.h>

/// Create HCAL cluster relations from PECO to FRFT tracks: bank PFHR
void alpha::edm4hep_output::event_t::process_pcrl()  {
  /**
 +------+                              Subschema: CobjPOTBanks
 | PCRL |  Calobject ReLations
 +------+
 ..............................................................
     1          I    Number of words/calobject (=5)
                     relation
     2          I    Number of calobject relations
 ..............................................................
      1    PC  I    PCOB
                       Calobject number
      2    PE  I    PECO
                       Row index of Electromagnetic Calobject
      3    PF  I    PFRF
                       Row index of Fitted Track
      4    PH  I    PHCO
                       Row index of Hadronic Calobject
      5    PP  I    PPOB
                       Row index of Hadronic Digital Pattern
  */
  // Do not know how to resolve this: Calorimeter hits cannot attached to edm4hep::MutableTrack
}
