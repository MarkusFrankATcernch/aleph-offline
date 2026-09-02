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
#include <alpha/itco.h>

/// Create ITC 3D hit from ITCO row
void alpha::edm4hep_output::event_t::process_itco()  {
  /**
 +------+                             Subschema: ItcJuliaBanks                
 | ITCO |  ITc COordinates (Recon. Bank)                                       
 +------+  (Banks 0 (corrected coords.)                                        
           and 1 (raw coords.). If only                                        
           bank 0 exists, it gives the                                         
           raw coords.) (Data is ordered                                       
           by increasing wire number).                                         
           (JUL)                                                               
 
 ..............................................................                
     1          I    Number of words/coordinate (=8)                           
     2          I    Number of coordinates                                     
 ..............................................................                
      1    WN  I    WireNumber       [1001,8960]                               
                       1000*Layer + Wire No.                                   
      2    RA  F    RAdius           [16.000,26.300]                           
                       radius (cm.)                                            
      3    P1  F    Phi1             [0.0,6.30000]                             
                       Phi of hit (or ambiguity)(rads.)                        
      4    P2  F    Phi2             [0.0,6.30000]                             
                       Phi of ambiguity (or hit)(rads.)                        
      5    ZH  F    ZHit             [-150.0,150.0]                            
                       Z of hit (cm.)                                          
      6    SR  F    SigmaRphi        [0.0,1.000000]                            
                       Sigma(r-phi)**2 (cm**2)                                 
      7    SZ  F    SigmaZ           [0.0,*]                                   
                       Sigma(z)**2 (cm**2)                                     
      8    DT  F    DriftTime        [0.0,512.0]                               
                       Drift Time calc. from TDC (ns.)                         
   */
  char text[512];
  std::stringstream log;
  PositionRhoZPhi   pos;
  auto& itc  = *this->exp.itc;
  auto& desc = itc.descriptor;
  bool  dbg  = this->data.itco.debug;
  auto* tab  = this->data.itco.load<object_table<class itco> >(true);
  if( dbg )  {
    log << bos77::to_string(tab) << std::endl;
  }
  
  for( uint32_t i=1; i <= tab->size(); ++i )  {
    class itco* ah   = tab->row(i);
    int32_t     layer = (ah->wireNumber()/1000);
    int32_t     wire  = (ah->wireNumber()%1000);
    uint64_t    cell  = itc.desc_system + 
      desc.encode(itc.field_layer,  layer) + desc.encode(itc.field_wire, wire);
    PositionRhoZPhi err(std::sqrt(ah->sigmaRphi()), std::sqrt(ah->sigmaZ()), std::sqrt(ah->sigmaRphi()));

    /// Process first hit (or ambiguity)
    auto hit1 = this->hits_itco1.create();
    pos = PositionRhoZPhi(_LEN(ah->radius()), _LEN(ah->zhit()), ah->phi1());
    hit1.setCellID( cell );
    hit1.setTime( _TIM(ah->driftTime()) );
    hit1.setEDep( _ENE(0e0) );
    hit1.setEDepError( _ENE(0e0) );
    hit1.setPosition( { float(pos.x()), float(pos.y()), float(pos.z()) } );
    hit1.setCovMatrix( {
        err.x()*err.x(),
        err.y()*err.x(), err.y()*err.y(),
        err.z()*err.x(), err.z()*err.y(), err.z()*err.z() } );
    hit1.setQuality(0);
    hit1.setType(ITC_COORDINATE);

    /// Process second hit (or ambiguity)
    auto hit2 = this->hits_itco2.create();
    pos = PositionRhoZPhi(_LEN(ah->radius()), _LEN(ah->zhit()), ah->phi2());
    hit2.setCellID( cell );
    hit2.setTime( _TIM(ah->driftTime()) );
    hit2.setEDep( _ENE(0e0) );
    hit2.setEDepError( _ENE(0e0) );
    hit2.setPosition( { float(pos.x()), float(pos.y()), float(pos.z()) } );
    hit2.setCovMatrix( {
        err.x()*err.x(),
        err.y()*err.x(), err.y()*err.y(),
        err.z()*err.x(), err.z()*err.y(), err.z()*err.z() } );
    hit2.setQuality(1);
    hit2.setType(ITC_COORDINATE);

    if( dbg )  {
      ::snprintf(text, sizeof(text),
                 "\tITCO %3d %8lX Cell:%08lX Wire:%6d r:%s phi:%5.1f/%5.1f z:%s drift:%3.0f ns",
                 i  , uint64_t(ah), cell, ah->wireNumber(),
                 fmt_len(ah->radius()).c_str(), ah->phi1(),
                 ah->phi2(), fmt_len(ah->zhit()).c_str(),
                 ah->driftTime() );
      log << text << std::endl;
    }
  }
  if( dbg )  {
    ::printf("%s\n", log.str().c_str());
  }
}
