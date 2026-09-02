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
#include <alpha/tpco.h>
#include <alpha/tpcgeom.h>

/// Create VDET 3D hit from VDCO row
void alpha::output_edm4hep::event_t::process_tpco()  {
  /**
 +------+                               Subschema: TpcJULBanks                
 | TPCO |  Tpc Pad Coordinates in global                                       
 +------+  system. NR=1: raw                                                   
           coordinates. NR=0: final                                            
           coordinates, after removal of                                       
           overlapping coordinates from                                        
           track candidates and addition                                       
           of twin coordinates. The                                            
           coordinates are sorted in                                           
           order of increasing Index.                                          
 
 ..............................................................                
     1          I    Number of words/coord. (=12)                              
     2          I    Number of coordinates                                     
 ..............................................................                
      1    IN  I    Index            [101001,2136150]                          
                       100000*padrow number +                                  
                       1000*sector number + pad number                         
      2    RV  F    RValue           [30.000,180.000]                          
                       r [cm]                                                  
      3    PH  F    Phi              [0.0,6.300]                               
                       phi [rad]                                               
      4    ZV  F    ZValue           [-216.00,216.00]                          
                       z [cm]                                                  
      5    SR  F    SigRphi          [0.0,*]                                   
                       sigma**2 of R*Phi                                       
      6    SZ  F    SigZ             [0.0,*]                                   
                       sigma**2 of Z                                           
      7    OF  I    OriginFlag       [0,5]                                     
                       Origin flag:                                            
                       0=can not remember                                      
                       1=TCOOR                                                 
                       2=TWINCO                                                
                       3=TSACOR                                                
                       4=TCOORT                                                
                       5=TPCOOR Fake Coordinate                                
      8    TN  I    TrackNumber      [*,*]                                     
                       track number                                            
                       = 0 if coordinate not used,                             
                       = -(track number) if associated                         
                       but not used in fit                                     
                       Warning: which track bank this refers                   
                       to depends on what stage of the program                 
                       is being executed. At the end it refers                 
                       to FRFT.                                                
      9    CN  I    ClusterNumbe     [1,*]                                     
                       sub-cluster number (per sector)                         
      10   IT  I    IT               [0,*]                                     
                       work space                                              
      11   RR  F    RawRphi          [-55.,55.]                                
                       Uncorrected r*phi in the sector                         
                       coordinate system                                       
      12   RZ  F    RawZ             [0.0,220.]                                
                       Uncorrected z in the sector coordinate                  
                       system                                                  
   */
  std::stringstream log;
  auto& tpc = *this->exp.tpc;
  auto& dsc = tpc.descriptor;
  bool  dbg = this->data.tpco.debug;
  auto* tab = this->data.tpco.load<object_table<class tpco> >(true);
  if( dbg )  {
    log << bos77::to_string(tab) << std::endl;
  }
  for( uint32_t i=1; i <= tab->size(); ++i )  {
    class tpco* ah     = tab->row(i);
    uint64_t    indx   = ah->index();
    auto        hit    = this->hits_tpco.create();
    uint32_t    padrow = (indx/100000);
    uint32_t    sector = (indx/1000)%100;
    uint32_t    pad    = (indx%1000);
    PositionRhoZPhi pos (_LEN(ah->rvalue()), _LEN(ah->zvalue()), ah->phi());
    PositionRhoZPhi err (std::sqrt(ah->sigRphi()), std::sqrt(ah->sigZ()), std::sqrt(ah->sigRphi()));
    int32_t         trno = ah->trackNumber();
    int32_t         sign = trno != 0 ? int(trno/std::abs(trno)) : 1;
    // Encode local coordinates
    uint64_t    cell   = tpc.desc_system   + dsc.encode(tpc.field_sector, sector) +
      dsc.encode(tpc.field_padrow, padrow) + dsc.encode(tpc.field_pad,    pad);
    // cell += uint64_t(ah->rvalue())<<20 + uint64_t(ah->zvalue())<<10 + uint64_t(ah->phi());

    hit.setCellID( cell );
    hit.setTime( _TIM(0e0) );
    hit.setEDep( _ENE(0e0) );
    hit.setEDepError( _ENE(0e0) );
    hit.setPosition( { float(pos.x()), float(pos.y()), float(pos.z()) } );
    hit.setCovMatrix( {
        err.x()*err.x(),
        err.y()*err.x(), err.y()*err.y(),
        err.z()*err.x(), err.z()*err.y(), err.z()*err.z() } );
    hit.setQuality( sign*(ah->originFlag() + (ah->clusterNumbe()<<4)) );
    hit.setType( TPC_COORDINATE );
    
    if( dbg )  {
      char text[512];
      ::snprintf(text, sizeof(text),
                 "\tTPCO %3d %08lX Cell:%08lX Indx:%8ld Sector:%2d Padrow:%2d Pad:%3d Track:%3d "
                 "r:%s phi:%5.2f z:%s subclu:%3d org:%1d",
                 i, uint64_t(ah), cell, indx, sector, padrow, pad, ah->trackNumber(),
                 fmt_len(ah->rvalue()).c_str(), ah->phi(), fmt_len(ah->zvalue()).c_str(),
                 ah->clusterNumbe(), ah->originFlag() );
      log << text << std::endl;
    }
  }
  if( dbg )  {
    ::printf("%s\n", log.str().c_str());
  }
}
