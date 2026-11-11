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


/// Create VDET 3D hit from VDCO row
void alpha::edm4hep_output::event_t::process_vdco()  {
  std::stringstream     log;
  auto*                 tab  = this->data.vdco.load<object_table<class vdco> >(true);
  detector_vdet_t&      vdet = *this->exp.vdet;
  dd4hep::IDDescriptor& desc = vdet.descriptor;
  if( this->data.vdco.debug )  {
    log << bos77::to_string(tab) << std::endl;
  }
  for( uint32_t itk=1, siz=tab->size(); itk <= siz; ++itk )  {
    std::size_t  key      = this->hits_vdco.size();
    trackerhit_t hit      = this->hits_vdco.create();
    const vdco*  ah       = this->data.vdco.row<class vdco>(itk);
    double       r        = _LEN(ah->r());
    double       z        = _LEN(ah->z());
    double       phi      = ah->phi();
    double       sinPhi   = std::sin(phi);
    double       cosPhi   = std::cos(phi);
    double       sigrphi2 = _LEN2(ah->sigRphi2());   //
    double       sigz2    = _LEN2(ah->sigZ2());      //
    int32_t      wafid    = ah->waferIdent();
    int32_t      layer    = (wafid/10000);    // See vadewa.F    ILAY  / I  Layer index of this wafer
    int32_t      iz       = (wafid/1000)%10;  // See vadewa.F    IWFF  / I  Local wafer-in-face index + VDXY bank doc
    int32_t      iphi     = (wafid/10)%100;   // See vadewa.F    IFAC  / I  Local face index of this wafer + VDXY bank doc
    int32_t      iview    = (wafid%10);       // See vadewa.F    IVIEW / I  View number (=1 for z, =2 for r-phi)
    uint64_t     cell     = vdet.desc_system +
      desc.encode(vdet.field_layer,  layer)  +
      desc.encode(vdet.field_z,      iz)     +
      desc.encode(vdet.field_phi,    iphi)   +
      desc.encode(vdet.field_view,   iview);

    hit.setCellID( cell );
    hit.setType(VDET_COORDINATE);
    hit.setQuality(ah->qualityFlag());
    
    hit.setTime( _TIM(0e0) );
    hit.setEDep( _ENE(0e0) );
    hit.setEDepError( _ENE(0e0) );
    hit.setPosition( { r * cosPhi, r * sinPhi, z } );
    hit.setCovMatrix( sigrphi2 * sinPhi * sinPhi, edm4hep::Cartesian::x, edm4hep::Cartesian::x);
    hit.setCovMatrix(-sigrphi2 * sinPhi * cosPhi, edm4hep::Cartesian::y, edm4hep::Cartesian::x);
    hit.setCovMatrix( sigrphi2 * cosPhi * cosPhi, edm4hep::Cartesian::y, edm4hep::Cartesian::y);
    hit.setCovMatrix(0.0f,  edm4hep::Cartesian::z, edm4hep::Cartesian::x);
    hit.setCovMatrix(0.0f,  edm4hep::Cartesian::z, edm4hep::Cartesian::y);
    hit.setCovMatrix(sigz2, edm4hep::Cartesian::z, edm4hep::Cartesian::z);

    this->alpha2edm4hep_vdco[itk] = key;

    if( this->data.vdco.debug )  {
      char text[512];
      ::snprintf(text, sizeof(text), "  VDCO %3d %8lX Cell:%08lX Wafer:%8d r:%s phi:%5.1f z:%s quality:%6d track:%2d",
                 itk, uint64_t(ah), cell, ah->waferIdent(),
                 fmt_len(ah->r()).c_str(), ah->phi(), fmt_len(ah->z()).c_str(),
                 ah->qualityFlag(), ah->trackNumber() );
      log << text << std::endl;
    }
  }
  if( this->data.vdco.debug )  {
    ::printf("%s\n", log.str().c_str());
  }
}

/// Create VDET 3D hit from VDCO row
void alpha::edm4hep_output::event_t::process_vdco_planes()  {
  std::stringstream     log;
  auto*                 tab  = this->data.vdco.load<object_table<class vdco> >(true);
  detector_vdet_t&      vdet = *this->exp.vdet;
  dd4hep::IDDescriptor& desc = vdet.descriptor;

  if( this->data.vdco.debug )  {
    log << bos77::to_string(tab) << std::endl;
  }
  constexpr double halfPi = M_PI / 2.0;
  for( uint32_t itk=1, siz=tab->size(); itk <= siz; ++itk )  {
    trackerplanehit_t hit = this->hitplanes_vdco.create();
    const vdco* ah       = this->data.vdco.row<class vdco>(itk);
    uint32_t    wafid    = ah->waferIdent();
    double      r        = _LEN(ah->r());
    double      z        = _LEN(ah->z());
    double      phi      = ah->phi();
    double      sinPhi   = std::sin(phi);
    double      cosPhi   = std::cos(phi);
    double      sigrphi2 = _LEN2(ah->sigRphi2());
    double      sigz2    = _LEN2(ah->sigZ2());
    int32_t     layer    = (wafid/10000);    // See vadewa.F    ILAY  / I  Layer index of this wafer
    int32_t     iz       = (wafid/1000)%10;  // See vadewa.F    IWFF  / I  Local wafer-in-face index + VDXY bank doc
    int32_t     iphi     = (wafid/10)%100;   // See vadewa.F    IFAC  / I  Local face index of this wafer + VDXY bank doc
    int32_t     iview    = (wafid%10);       // See vadewa.F    IVIEW / I  View number (=1 for z, =2 for r-phi)
    uint64_t    cell     = vdet.desc_system +
      desc.encode(vdet.field_layer,  layer)  +
      desc.encode(vdet.field_z,      iz)     +
      desc.encode(vdet.field_phi,    iphi)   +
      desc.encode(vdet.field_view,   iview);

    hit.setCellID(cell);
    hit.setType(VDET_COORDINATE);
    hit.setQuality(ah->qualityFlag());
    hit.setPosition( { r * cosPhi, r * sinPhi, z } );

    hit.setTime( _TIM(0e0) );       // Not available
    hit.setEDep( _ENE(0e0) );       // Not available
    hit.setEDepError( _ENE(0e0) );  // Not available
    
    hit.setU( edm4hep::Vector2f(halfPi, phi + halfPi) );
    hit.setV( edm4hep::Vector2f(0.0f, 0.0f) );
    hit.setDu(std::sqrt(sigrphi2));
    hit.setDv(std::sqrt(sigz2));

    // Cartesian covariance (cm^2)
    hit.setCovMatrix( sigrphi2 * sinPhi * sinPhi, edm4hep::Cartesian::x, edm4hep::Cartesian::x);
    hit.setCovMatrix(-sigrphi2 * sinPhi * cosPhi, edm4hep::Cartesian::y, edm4hep::Cartesian::x);
    hit.setCovMatrix( sigrphi2 * cosPhi * cosPhi, edm4hep::Cartesian::y, edm4hep::Cartesian::y);
    hit.setCovMatrix(0.0f,  edm4hep::Cartesian::z, edm4hep::Cartesian::x);
    hit.setCovMatrix(0.0f,  edm4hep::Cartesian::z, edm4hep::Cartesian::y);
    hit.setCovMatrix(sigz2, edm4hep::Cartesian::z, edm4hep::Cartesian::z);
    if( this->data.vdco.debug )  {
      char text[512];
      ::snprintf(text, sizeof(text), "  VDCO %3d %8lX Cell:%08lX Wafer:%8d r:%s phi:%5.1f z:%s quality:%6d track:%2d",
                 itk, uint64_t(ah), cell, ah->waferIdent(),
                 fmt_len(ah->r()).c_str(), ah->phi(), fmt_len(ah->z()).c_str(),
                 ah->qualityFlag(), ah->trackNumber() );
      log << text << std::endl;
    }
  }
  if( this->data.vdco.debug )  {
    ::printf("%s\n", log.str().c_str());
  }
}
