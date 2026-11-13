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
#include <alpha/peco.h>
#include <alpha/pest.h>
#include <alpha/pept.h>
#include <alpha/etdi.h>

/// Create ECAL hit from PECO row
void alpha::edm4hep_output::event_t::process_pest()  {
  /*

                                        Subschema: EobjPOTBanks                 
 +------+                                                                       
 | PEST |  Pot Electromagnetic                                                  
 +------+  calorimeter SToreys
 ..............................................................
     1          I    Number of words/storey (=5)
     2          I    Number of storeys
 ..............................................................
      1    KS  I    KSto             [1,3]
                       Stack level
      2    ER  F    EcoR             [0.0,300.000]
                       Energy in Gev, eventually corrected for
                       the slow control factor (this
                       factor comes from gas variations and is
                       the same throughout one ECAL
                       module)
      3    ED  I    EDDB             [1,10000]
                       Row index of EcalDeadStorey
      4    ET  I    ETDI
                       Row index of Ecaltower
      5    PE  I    PECO
                       Row index of Electromagnetic CalObject
..............................................................
    
ETHT (GALEPH)
   |
   v
ETDI (JULIA)
   |
   v
PEST.
PEST.ET   ---------> PECO row number
   |
   +---> ETDI row number
                  |
                  v
              ETDI.TL
                  |
                  v
             Packed IADD
                  |
          +-------+-------+
          |               |
          v               v
      IBITS(16,8)     IBITS(2,9)
          |               |
          v               v
          TJ              FI
          |               |
          +-------+-------+
                  |
                  v
        CALL ESRDIR(TJ+0.5,FI+0.5,
                    THETA,PHI)
                  |
                  v
           Physical direction
             in radians

  */
}
