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

/// Create ECAL wire data from PEWI data
void alpha::edm4hep_output::event_t::process_pewi()  {
  /**
                                        Subschema: EcalPOTBanks
   +------+
   | PEWI |  Ecal Wire plane Digits. NR=0
   +------+  (POT)
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
  */
  std::stringstream log;
  bool dbg = this->data.pewi.debug;
  for( this->data.pewi.load(false); this->data.pewi.data; this->data.pewi.knext() )  {
    auto* tab = this->data.pewi.table<class pewi>();
    if( dbg )  {
      log << bos77::to_string(tab) << std::endl;
    }
    for( uint32_t i=0, siz=tab->size(); i < siz; ++i )  {
      const auto* plane = tab->at(i);
      const auto*  edep = plane->planDig();
      int32_t        t0 = plane->tiME0() * TIMECONV;
      int32_t    module = plane->moduleNumbe();
      const char *tag;
      if( module<=12 )      tag = "Endcal A";
      else if( module<=24 ) tag = "Barrel";
      else                  tag = "Endcap B";
      for( int32_t iplane=0; iplane < 44; ++iplane )  {
        // Modules:  1-12 ECA, 13-24 barrel, 25-36 ECB
        uint64_t     cell = (uint64_t(module) << 32) + iplane;
        this->hits_ecal_wire.create(cell, edep[iplane], t0);
        if( dbg )  {
          char text[256];
          ::snprintf(text, sizeof(text),
                     "\tPEWI %-10s EC wire: mod:%2d plane:%2d E:%7s GeV T0:%2d ns",
                     tag, module, iplane, fmt_ene(edep[iplane]/1e6).c_str(), t0);
          log << text << std::endl;
        }
      }
    }
  }
  if( dbg )  {
    ::printf("%s\n", log.str().c_str());
  }
}
