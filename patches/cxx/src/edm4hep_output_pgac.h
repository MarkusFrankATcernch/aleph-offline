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
#include <alpha/pgac.h>

/// Convert gammas from PGAC.
void alpha::edm4hep_output::event_t::process_gammas_pgac()  {
  /**
     +------+                              Subschema: EcalJULBanks                
     | PGAC |  Photons from GAMPEX (oct 94)                                        
     +------+ 
     ..............................................................                
     1          I    Number of words per photon (=25)                          
     2          I    Number of photons                                         
     ..............................................................                
      1    EC  F    EnergyCorrected  [*,*]                                     
                       Energy corrected for photon hypothesis                  
      2    TC  F    ThetaCorrected   [*,*]                                     
                       Theta corrected for photon hypothesis                   
      3    PC  F    PhiCorrected     [*,*]                                     
                       Phi corrected for photon hypothesis                     
      4    R1  F    R1               [0.0,1.]                                  
                       Energy fraction in stack 1                              
      5    R2  F    R2               [0.0,1.]                                  
                       Energy fraction in stack 2                              
      6    F4  F    F4               [0.0,1.]                                  
                       Energy fraction in the 4 central towers                 
      7    DM  F    DistanceMinimun  [0.0,*]                                   
                       Distance to the closest track (cm)                      
      8    ST  I    SToreyflag                                                 
                       NST1+100*NST2+10000*NST3                                
                       NSTi = number of storey stack i                         
      9    QU  I    QUalityflag                                                
                       CRCK+10*DST1+100*DST2+1000*DST3                         
                       DSTi = 1 if dead storey(s) stacki                       
                       CRCK = 1 if photon in crack region                      
      10   Q1  F    Q1                                                         
                       Quality estimator for photon 1                          
      11   Q2  F    Q2                                                         
                       Quality estimator for photon 2                          
      12   M1  F    Moment1                                                    
                       1st moment from CLMONS analysis                         
      13   M2  F    Moment2                                                    
                       2nd moment from CLMONS analysis                         
      14   MA  F    MAss                                                       
                       Pi0 mass estmated from clmoms                           
      15   ER  F    EnergyRaw                                                  
                       Raw energy for photon                                   
      16   TR  F    ThetaRaw                                                   
                       Uncorrected theta for photon                            
      17   PR  F    PhiRaw                                                     
                       Uncorrected phi for photon                              
      18   EF  F    ExpectedFraction                                           
                       Expected fraction in 4 towers                           
      19   GC  F    GeometricalCorr                                            
                       Geomtrical correction                                   
      20   ZS  F    ZeroSupression                                             
                       Zero suppresion correction from Coradoc                 
      21   PL  F    ProbafakeeLm                                               
                       Probability to be a fake photon from                    
                       Electromagnetic origine                                 
      22   PH  F    ProbafakeHad                                               
                       Probability to be a fake photon from                    
                       Hadronic origine                                        
      23   PN  I    ParentNumber                                               
                       Row number(PGAC) of parent giving a                     
                       fake photon                                             
      24   FA  I    FAkequality                                                
                       Flag for fake determination                             
      25   PE  I    PECO                                                       
                       pointeur to PECO bank                                   
   */
  auto* tab = this->data.pgac.load<object_table<class pgac> >(false);
  if( tab )  {
    for( uint32_t itk=1, siz=tab->size(); itk <= siz; ++itk )  {
    }
  }
}

