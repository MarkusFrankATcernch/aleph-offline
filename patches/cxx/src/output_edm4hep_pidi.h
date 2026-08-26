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


/// Process PIDI ITC digitisings (NR=0)
void alpha::output_edm4hep::event_t::process_pidi()  {
  /**

     +------+                               Subschema: ItcPOTBanks                
     | PIDI |  Packed Itc DIgitisings (NR=0)                                       
     +------+  (POT)                                                               
 
     ..............................................................                
     1          I    Number of words/digit. (=1)                               
     2          I    Number of digits                                          
     ..............................................................                
      1    PD  I    PackedDigit      [*,*]                                     
                       ITC digits. info. packed into                           
                       a 32 bit word.                                          
                       10 bits wire number - bits 0 - 9,                       
                       9 bits r-phi TDC - bits 10 - 18,                        
                       9 bits Z TDC - bits 19 - 27,                            
                       2 bits quality flag - bits 28 - 29,                     
                       1 bit ambiguity flag - bit 30,                          
                       1 bit version flag - bit 31 (not used                   
                       from 1991 onwards                                       
  */
}
