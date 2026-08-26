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


/// Create ECAL wire data from PEWI data
void alpha::output_edm4hep::event_t::process_pewi()  {
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
  for( this->data.pewi.load(false); this->data.pewi.data; this->data.pewi.knext() )  {
    auto* tab = this->data.pewi.table<class pewi>();
    std::cout << bos77::to_string(tab) << std::endl;
    for( uint32_t i=0, siz=tab->size(); i <= siz; ++i )  {
      const auto* plane = tab->at(i);
      const auto*  edep = plane->planDig();
      int32_t      t0   = plane->tiME0() * TIMECONV;
      for( int32_t j=0; j < 44; ++j )  {
	// Modules:  1-12 ECA, 13-24 barrel, 25-36 ECB
        uint64_t     cell = (uint64_t(plane->moduleNumbe()) << 32) + j;
	this->hits_ecal_wire.create(cell, edep[j], t0);
      }
    }
  }
}
