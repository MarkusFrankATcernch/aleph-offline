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

/// Create ITC 3D hit from ITCO row
void alpha::edm4hep_output::event_t::process_itco()  {
  char text[512];
  std::stringstream log;
  auto& itc  = *this->exp.itc;
  auto& desc = itc.descriptor;
  bool  dbg  = this->data.itco.debug;
  auto* tab  = this->data.itco.load<object_table<class itco> >(true);
  if( dbg )  {
    log << bos77::to_string(tab) << std::endl;
  }

  for( uint32_t i=1; i <= tab->size(); ++i )  {
    class itco*  ah   = tab->row(i);
    double       r        = _LEN(ah->radius());
    double       z        = _LEN(ah->zhit());
    double       phi      = ah->phi1();
    double       sinPhi   = std::sin(phi);
    double       cosPhi   = std::cos(phi);
    double       sigrphi2 = _LEN2(ah->sigmaRphi());   //
    double       sigz2    = _LEN2(ah->sigmaZ());      //
    
    int32_t      layer = (ah->wireNumber()/1000);
    int32_t      wire  = (ah->wireNumber()%1000);
    uint64_t     cell  = itc.desc_system +
      desc.encode(itc.field_layer,  layer) +
      desc.encode(itc.field_wire, wire);

    /// Process first hit (or ambiguity)
    auto hit1 = this->hits_itco1.create();
    hit1.setCellID( cell );
    hit1.setQuality(0);
    hit1.setType(ITC_COORDINATE);
    hit1.setEDep(      _ENE(0e0) );       // Not available
    hit1.setEDepError( _ENE(0e0) );       // Not available
    hit1.setTime(      _TIM(ah->driftTime()) );
    hit1.setPosition( { r * cosPhi, r * sinPhi, z } );
    hit1.setCovMatrix( sigrphi2 * sinPhi * sinPhi, edm4hep::Cartesian::x, edm4hep::Cartesian::x);
    hit1.setCovMatrix(-sigrphi2 * sinPhi * cosPhi, edm4hep::Cartesian::y, edm4hep::Cartesian::x);
    hit1.setCovMatrix( sigrphi2 * cosPhi * cosPhi, edm4hep::Cartesian::y, edm4hep::Cartesian::y);
    hit1.setCovMatrix( 0.0f,  edm4hep::Cartesian::z, edm4hep::Cartesian::x);
    hit1.setCovMatrix( 0.0f,  edm4hep::Cartesian::z, edm4hep::Cartesian::y);
    hit1.setCovMatrix( sigz2, edm4hep::Cartesian::z, edm4hep::Cartesian::z);

    /// Process second hit (or ambiguity)
    auto hit2 = this->hits_itco2.create();
    phi      = ah->phi2();
    sinPhi   = std::sin(phi);
    cosPhi   = std::cos(phi);
    hit2.setCellID( cell );
    hit2.setQuality(1);
    hit2.setType(ITC_COORDINATE);
    hit2.setTime(      _TIM(ah->driftTime()) );
    hit2.setEDep(      _ENE(0e0) );       // Not available
    hit2.setEDepError( _ENE(0e0) );       // Not available
    hit2.setPosition(  { r * cosPhi, r * sinPhi, z } );
    hit2.setCovMatrix( sigrphi2 * sinPhi * sinPhi, edm4hep::Cartesian::x, edm4hep::Cartesian::x);
    hit2.setCovMatrix(-sigrphi2 * sinPhi * cosPhi, edm4hep::Cartesian::y, edm4hep::Cartesian::x);
    hit2.setCovMatrix( sigrphi2 * cosPhi * cosPhi, edm4hep::Cartesian::y, edm4hep::Cartesian::y);
    hit2.setCovMatrix( 0.0f,  edm4hep::Cartesian::z, edm4hep::Cartesian::x);
    hit2.setCovMatrix( 0.0f,  edm4hep::Cartesian::z, edm4hep::Cartesian::y);
    hit2.setCovMatrix( sigz2, edm4hep::Cartesian::z, edm4hep::Cartesian::z);

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

/// Create ITC hit planes from ITCO row
void alpha::edm4hep_output::event_t::process_itco_planes()  {
  constexpr double halfPi = M_PI / 2.0;
  char text[512];
  std::stringstream log;
  auto& itc  = *this->exp.itc;
  auto& desc = itc.descriptor;
  bool  dbg  = this->data.itco.debug;
  auto* tab  = this->data.itco.load<object_table<class itco> >(true);
  
  if( dbg )  {
    log << bos77::to_string(tab) << std::endl;
  }

  for( uint32_t i=1; i <= tab->size(); ++i )  {
    class itco*  ah       = tab->row(i);
    double       r        = _LEN(ah->radius());
    double       z        = _LEN(ah->zhit());
    double       phi      = ah->phi1();
    double       sinPhi   = std::sin(phi);
    double       cosPhi   = std::cos(phi);
    double       sigrphi2 = _LEN2(ah->sigmaRphi());   //
    double       sigz2    = _LEN2(ah->sigmaZ());      //
    
    int32_t      layer = (ah->wireNumber()/1000);
    int32_t      wire  = (ah->wireNumber()%1000);
    uint64_t     cell  = itc.desc_system +
      desc.encode(itc.field_layer,  layer) +
      desc.encode(itc.field_wire, wire);

    /// Process first hit (or ambiguity)
    auto hit1 = this->hitplanes_itco1.create();    
    hit1.setCellID( cell );
    hit1.setQuality(0);
    hit1.setType(ITC_COORDINATE);
    hit1.setEDep(      _ENE(0e0) );       // Not available
    hit1.setEDepError( _ENE(0e0) );       // Not available
    hit1.setTime(      _TIM(ah->driftTime()) );
    hit1.setPosition( { r * cosPhi, r * sinPhi, z } );
    hit1.setU(edm4hep::Vector2f(halfPi, phi + halfPi));
    hit1.setV(edm4hep::Vector2f(0.0f, 0.0f));
    hit1.setDu(std::sqrt(sigrphi2));
    hit1.setDv(std::sqrt(sigz2));
    hit1.setCovMatrix( sigrphi2 * sinPhi * sinPhi, edm4hep::Cartesian::x, edm4hep::Cartesian::x);
    hit1.setCovMatrix(-sigrphi2 * sinPhi * cosPhi, edm4hep::Cartesian::y, edm4hep::Cartesian::x);
    hit1.setCovMatrix( sigrphi2 * cosPhi * cosPhi, edm4hep::Cartesian::y, edm4hep::Cartesian::y);
    hit1.setCovMatrix( 0.0f,  edm4hep::Cartesian::z, edm4hep::Cartesian::x);
    hit1.setCovMatrix( 0.0f,  edm4hep::Cartesian::z, edm4hep::Cartesian::y);
    hit1.setCovMatrix( sigz2, edm4hep::Cartesian::z, edm4hep::Cartesian::z);

    /// Process second hit (or ambiguity)
    auto hit2 = this->hitplanes_itco2.create();
    phi      = ah->phi2();
    sinPhi   = std::sin(phi);
    cosPhi   = std::cos(phi);
    hit2.setCellID( cell );
    hit2.setQuality(1);
    hit2.setType(ITC_COORDINATE);
    hit2.setTime(      _TIM(ah->driftTime()) );
    hit2.setEDep(      _ENE(0e0) );       // Not available
    hit2.setEDepError( _ENE(0e0) );       // Not available
    hit2.setPosition(  { r * cosPhi, r * sinPhi, z } );
    hit2.setU(edm4hep::Vector2f(halfPi, phi + halfPi));
    hit2.setV(edm4hep::Vector2f(0.0f, 0.0f));
    hit2.setDu(std::sqrt(sigrphi2));
    hit2.setDv(std::sqrt(sigz2));
    hit2.setCovMatrix( sigrphi2 * sinPhi * sinPhi, edm4hep::Cartesian::x, edm4hep::Cartesian::x);
    hit2.setCovMatrix(-sigrphi2 * sinPhi * cosPhi, edm4hep::Cartesian::y, edm4hep::Cartesian::x);
    hit2.setCovMatrix( sigrphi2 * cosPhi * cosPhi, edm4hep::Cartesian::y, edm4hep::Cartesian::y);
    hit2.setCovMatrix( 0.0f,  edm4hep::Cartesian::z, edm4hep::Cartesian::x);
    hit2.setCovMatrix( 0.0f,  edm4hep::Cartesian::z, edm4hep::Cartesian::y);
    hit2.setCovMatrix( sigz2, edm4hep::Cartesian::z, edm4hep::Cartesian::z);

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

