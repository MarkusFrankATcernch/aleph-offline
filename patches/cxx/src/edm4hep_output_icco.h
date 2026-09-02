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
#include <alpha/icco.h>

/// Create VDET 3D hit from VDCO row
void alpha::edm4hep_output::event_t::process_icco()  {
  /**
 +------+                             Subschema: ItcJuliaBanks                
 | ICCO |  Itc Corrected COordinates for                                       
 +------+  the final track fit. This                                           
           bank is parallel to ITCO. Use                                       
           track trajectory to correct                                         
           used coords. (for Z, TOF,                                           
           signal time and entrance                                            
           angle thru cell) Used coords.                                       
           correspond to lists in IGCL                                         
           or FICL. Unused coords.                                             
           cannot be corrected and have                                        
           null entries. (JUL)                                                 
 
 ..............................................................                
     1          I    Number of words per (=5)                                  
                     coordinate                                                
     2          I    Number of coordinates.                                    
 ..............................................................                
      1    RV  F    RValue           [15.000,27.300]                           
                       radius of coord.(cm.)                                   
      2    PH  F    PHi              [0.0,6.30000]                             
                       Phi of coord.                                           
      3    ZV  F    ZValue           [-150.0,150.0]                            
                       Z of coord (cm.)                                        
      4    SR  F    SigmaRphi        [0.0,1.000000]                            
                       Sigma(r-phi)**2 (cm**2)                                 
      5    SZ  F    SigmaZ           [0.0,*]                                   
                       Sigma(z)**2 (cm**2)                                     
   */
  auto* tab = this->data.icco.load<object_table<class icco> >(true);
  if( this->data.icco.debug )  {
    std::cout << bos77::to_string(tab) << std::endl;
  }
  for( uint32_t i=1; i <= tab->size(); ++i )  {
    class icco*     ah  = tab->row(i);
    auto            hit = this->hits_icco.create();
    PositionRhoZPhi pos(_LEN(ah->rvalue()), _LEN(ah->zvalue()), ah->phi());
    PositionRhoZPhi err(std::sqrt(ah->sigmaRphi()), std::sqrt(ah->sigmaZ()), std::sqrt(ah->sigmaRphi()));
    uint64_t        cell = 1;

    hit.setCellID( cell );
    hit.setTime( _TIM(0e0) );
    hit.setEDep( _ENE(0e0) );
    hit.setEDepError( _ENE(0e0) );
    hit.setPosition( { float(pos.x()), float(pos.y()), float(pos.z()) } );
    hit.setCovMatrix( {
        err.x()*err.x(),
        err.y()*err.x(), err.y()*err.y(),
        err.z()*err.x(), err.z()*err.y(), err.z()*err.z() } );
    hit.setQuality(0);
    hit.setType(ITC_COORDINATE);
  }
}
