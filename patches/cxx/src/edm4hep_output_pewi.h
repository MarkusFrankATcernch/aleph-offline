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
#include <alpha/pewi.h>
#include <alpha/prpw.h>
#include <alpha/peco.h>

/// Create ECAL wire data from PEWI data
void alpha::edm4hep_output::event_t::process_pewi()  {
  /**
   +------+                              Subschema: EcalPOTBanks
   | PEWI |  Ecal Wire plane Digits. NR=0  (POT)
   +------+ 
   ..............................................................
       1          I    Number of words/module (=55)
       2          I    Number of modules above
                       threshold
   ..............................................................
        1    MN  I    ModuleNumbe      [1,36]
                      module number ( 1-36 )
      2-46   PD  I    PlanDig
                      Plane digit ( keV )
     47-54   SS  I    SSumval
                      Sum sampling value (ADC count)
        55   TI  I    TIME0
                      T0 time crossing from ECAL wires(ns)

   +------+                              Subschema: EcalPOTBanks                
   | PWEI |  Pot Wire Energy Information(former PEWI) 
   +------+ 
   ..............................................................                
       1          I    Number of words per Module (=63)                          
       2          I    Number of modules                                         
   ..............................................................                
        1    MN  I    ModuleNumber     [1,36]                                    
                         Module Number [1,36]                                    
      2-46   PD  I    PD               [*,*]                                     
                         Plane Digit for each of the 45 planes                   
                         (kev)                                                   
     47-62   SS  I    SS               [*,*]                                     
                         Sum Sampling value                                      
        63   TI  I    TIme             [*,*]                                     
                         T0 time from Ecal wires                                 
   ..............................................................                

   +------+                              Subschema: EcalPOTBanks                
   | PRPW |  Pot bank for Relation Pad/Wire
   +------+ 
   ..............................................................                
       1          I    Number of words/Relation (=6)                             
       2          I    Number of Relation                                        
   ..............................................................                
        1    PN  I    PecoNumber                                                 
                         Peco number                                             
        2    MR  I    ModuleRange                                                
                         Module range in PEWI                                    
        3    MN  I    ModuleNumber                                               
                         Wire Module number                                      
        4    F1  F    F1                                                         
                         Fraction of energy contained                            
                         in this module in stack1 (%)                            
        5    F2  F    F2                                                         
                         Fraction of energy contained                            
                         in this module in stack2 (%)                            
        6    ER  F    ER                                                         
                         Raw energy contained in module                          
                         for PECO (Gev)                                          
   ..............................................................                

   
         IF(PRPW.MN < 13) Endcap - A
         IF(PRPW.MN > 24) Endcap - B
         IF(PRPW.MN > 12 .AND. PRPW.MN < 25) BARREL

         PRPW.PN      ---> Peco row index [1...n]
         PRPW.MN      ---> ECAL module number: Selector on PEWI ( Ecal Wire plane Digits )
         PRPW.MR      ---> Module range in PEWI (??)  [PEWI row index [1...n]]

         PEWI.PD      ---> Wire digits
         PEWI.SS[0]   must be <= 300
         PEWI.TI      ---> time: wire T0

  */
  bool  dbg      = this->data.pewi.debug;
  auto* tab_prpw = this->data.prpw.load<object_table<class prpw> >(true);
  auto* tab_pewi = this->data.pewi.load<object_table<class pewi> >(true);

  printout( dbg ? ALWAYS : DEBUG, __FUNCTION__,"+++ %s", bos77::to_string(tab_pewi).c_str() );
  for( uint32_t itk=1, siz=tab_prpw->size(); itk <= siz; ++itk )  {
    auto*   prpw  = tab_prpw->row(itk);
    int32_t ipewi = prpw->moduleRange();
    auto*   pewi  = tab_pewi->row(ipewi);
    int32_t adc0  = pewi->ssumval()[0];

    if( adc0 > 300 )  { /// ADC may not be saturated
      continue;
    }
    const int32_t* digits = pewi->planDig();
    int32_t        ipeco  = prpw->pecoNumber();
    calohit_t      clu    = this->ecal_hits.at(ipeco-1);

    /// Add to shape parameters: Module number/endcap, T0, ADC values
    /// 10 numbers in the shape parameters
    /// Modules:  1-12 ECA, 13-24 barrel, 25-36 ECB
    int32_t  barrel = 0;
    const char *tag = "";
    double       t0 = _TIM( double(pewi->tiME0())*1e9 );
    int32_t  module = pewi->moduleNumbe();

    if(      module < 13 ) barrel = 'A', tag = "Endcal A";
    else if( module < 25 ) barrel = 'b', tag = "Barrel";
    else                   barrel = 'B', tag = "Endcap B";
    clu.addToShapeParameters( barrel + (module << 8) );
    clu.addToShapeParameters( t0 );

    /// 45 plane numbers per relation.
    /// See parallel data in shape parameters for optional data
    for( int32_t iplane=0; iplane < 45; ++iplane )   {
      float ene = _ENE( 1e6*float(digits[iplane]) );
      clu.addToSubdetectorEnergies( ene );
      printout( dbg ? ALWAYS : DEBUG, "process_pewi",
                "+++ \tPEWI %-10s EC wire: mod:%2d plane:%2d E:%7s GeV T0:%5.0f ns",
                tag, module, iplane, fmt_ene(digits[iplane]/1e6).c_str(), t0 );
    }
  }
  ++this->data.pewi.num_process;
}
