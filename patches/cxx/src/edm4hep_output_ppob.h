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
#include <alpha/ppob.h>
#include <alpha/htub.h>

/// Create ECAL wire data from PEWI data
void alpha::edm4hep_output::event_t::process_ppob()  {
  /**
 +------+                              Subschema: HobjPOTBanks                
 | PPOB |  hadronic digital Pattern OBject
 +------+ 
 ..............................................................                
     1          I    Number of words/Digital (=9)                              
                     Pattern                                                   
     2          I    Number of Digital Patterns                                
 ..............................................................                
      1    DI  I    DIrect           [-1,5]                                    
                       Direction index                                         
      2    DE  F    DEnerg           [0.0,300.00]                              
                       Digital Energy (Gev)                                    
      3    C1  F    Cor1             [-400.,400.]                              
                       (Cor1,DirectionIndex)                                   
                       (R,1) (Z,2-5)                                           
      4    C2  F    Cor2             [-500.,500.]                              
                       (Cor2,DirectionIndex)                                   
                       (PHI,1) (Y,2-3)                                         
                       (S-up,4) (S-down,5)                                     
      5    IP  I    IPlane           [0,24]                                    
                       Interaction Plane                                       
      6    FP  I    FPlane           [0,24]                                    
                       First Plane                                             
      7    LP  I    LPlane           [0,24]                                    
                       Last Plane                                              
      8    MD  F    MDista           [0.0,500.0]                               
                       Maximum Distance                                        
      9    PD  F    PDensi           [0.0,100.0]                               
                       Pattern Density                                         
 ..............................................................

 +------+                              Subschema: HcalJULBanks                
 | HTUB |  Hcal TUBes data (from Julia                                         
 +------+  prepare data)                                                       
 ..............................................................                
     1          I    Number of words/tube (=7)                                 
     2          I    Number of fired tubes                                     
 ..............................................................                
      1    SN  I    SubcompNumbe     [1,3]                                     
                       subcomponent number                                     
      2    MN  I    ModuleNumber     [1,24]                                    
                       module number (1-6 endcaps, 1-24 barrel)                
      3    LN  I    LayerNumber      [1,23]                                    
                       layer number (1-23)                                     
      4    LC  F    LocalCoordinate  [0.0,400.]                                
                       local coordinate of center of cluster                   
      5    CW  F    ClusterWidth     [0.0,20.]                                 
                       cluster width ( cm )                                    
      6    PR  I    PointerRawdata   [0,*]                                     
                       Pointer to raw data bank HWDI                           
      7    PP  I    PointerPattern   [0,*]                                     
                       Pointer to Pattern bank HPCO/PPOB                       
 ..............................................................

-- requires process_phco run before!
  */
  auto* tab_ppob = this->data.ppob.load<object_table<class ppob> >(true);
  for( const auto& lnk_ppob : this->hcal_hit2pattern )  {
    edm4hep::MutableCluster hit = hcal_hits.at(lnk_ppob.first-1);
    for( int32_t ippob : lnk_ppob.second )  {
      const auto* ppob  = tab_ppob->row(ippob);
      int32_t fired_planes = ( (ppob->direct()+1)<<3 ) +
        ( ppob->iplane() <<  5 ) +
        ( ppob->fplane() << 10 ) +
        ( ppob->lplane() << 15 );
      hit.addToShapeParameters( fired_planes );
      hit.addToSubdetectorEnergies( _ENE(ppob->denerg()) );
    }
  }
  ++this->data.ppob.num_process;
}
