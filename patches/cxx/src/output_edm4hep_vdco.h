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
#include <alpha/vdco.h>

/// Create VDET 3D hit from VDCO row
void alpha::output_edm4hep::event_t::process_vdco()  {
  /**
     Subschema: VdetPOTBanks                 
     +------+                                                                       
     | VDCO |  VDET coordinated NR=0 (POT)                                          
     +------+
   ..............................................................
     1          I    Number of words/coord (=8)
     2          I    Number of coords
   ..............................................................
     1    WI  I    WaferIdent       [10000,29999]
                   wafer identifier
     2    R0  F    R                [0.0,20.00000]
                   R coordinate
     3    PH  F    PHi              [0.0,6.3000002]
                   phi coordinate
     4    Z0  F    Z                [-20.0000,20.0000]
                   Z coordinate
     5    SR  F    SigRphi2         [0.0,*]
                   sigma of R-phi squared
     6    SZ  F    SigZ2            [0.0,*]
                   sigma of z squared
     7    QF  I    QualityFlag      [0,100000]
                   Qualtity flag
     8    TN  I    TrackNumber      [0,100000]
                   associated track in FRFT
  */
  std::stringstream log;
  auto* tab = this->data.vdco.load<object_table<class vdco> >(true);
  if( this->data.vdco.debug )  {
    log << bos77::to_string(tab) << std::endl;
  }
  for( uint32_t itk=1, siz=tab->size(); itk <= siz; ++itk )  {
    std::size_t     key = this->hits_vdco.size();
    auto            hit = this->hits_vdco.create();
    auto*           ah  = this->data.vdco.row<class vdco>(itk);
    double          sigrphi2 = ah->sigRphi2();   // 
    double          sigz2    = ah->sigZ2();      // 
    PositionRhoZPhi pos(_LEN(ah->r()), _LEN(ah->z()), ah->phi());
    PositionRhoZPhi err(std::sqrt(sigrphi2), std::sqrt(sigz2), std::sqrt(sigrphi2));
    uint64_t        cell  = (uint64_t(ah->waferIdent()) << 32) + VDET_COORDINATE;

    hit.setCellID( cell );
    hit.setTime( _TIM(0e0) );
    hit.setEDep( _ENE(0e0) );
    hit.setEDepError( _ENE(0e0) );
    hit.setPosition( { float(pos.x()), float(pos.y()), float(pos.z()) } );
    hit.setCovMatrix( {
        err.x()*err.x(),
        err.y()*err.x(), err.y()*err.y(),
        err.z()*err.x(), err.z()*err.y(), err.z()*err.z() } );
    hit.setQuality(ah->qualityFlag());
    hit.setType(VDET_COORDINATE);

    this->alpha2edm4hep_vdco[itk] = key;

    if( this->data.vdco.debug )  {
      char text[512];
      ::snprintf(text, sizeof(text), "  VDCO %3d %8lX Wafer:%8d r:%s phi:%5.1f z:%s quality:%6d track:%2d",
                 itk, uint64_t(ah), ah->waferIdent(),
                 fmt_len(ah->r()).c_str(), ah->phi(), fmt_len(ah->z()).c_str(),
                 ah->qualityFlag(), ah->trackNumber() );
      log << text << std::endl;
    }
  }
  if( this->data.vdco.debug )  {
    ::printf("%s\n", log.str().c_str());
  }
}
