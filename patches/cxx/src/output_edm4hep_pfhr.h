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
#include <alpha/pfhr.h>

/// Create HCAL cluster relations from PHCO to FRFT tracks: bank PFHR
void alpha::output_edm4hep::event_t::process_pfhr()  {
  /**
     +------+                              Subschema: CobjPOTBanks                
     | PFHR |  Fitted track-Hcalobject                                             
     +------+  Relations                                                           
 
     ..............................................................                
     1          I    Number of words/relation (=2)                             
     2          I    Number of relations                                       
     ..............................................................                
      1    PF  I    PFRF                                                       
                       Row index of Fitted Track                               
      2    PH  I    PHCO                                                       
                       Row index of HcalObject                                 
  */
  // Do not know how to resolve this: Calorimeter hits cannot attached to edm4hep::MutableTrack
}

