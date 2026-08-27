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
#include <alpha/hpdi.h>

/// HPDI table bank: Hcal Plane DIgits (RawData)
void alpha::output_edm4hep::event_t::process_hpdi()  {
  /**
 +------+                              Subschema: HcalRAWBanks                
 | HPDI |  Hcal Plane DIgits (RawData)                                         
 +------+
 
 ..............................................................                
     1          I    Number of words / plane (=2)                              
     2          I    Number of planes                                          
 ..............................................................                
      1    PA  I    PlaneAddres      [10101,306022]                            
                       Plane address                                           
                       bits: 0- 7 : plane number                               
                       endcaps[1,12] 89-90-91 data                             
                       endcaps[1,22] 92 data                                   
                       barrel[1,23] 89-90-91-92 data                           
                       8-15 : module number                                    
                       endcaps[1,6],barrel[1,24]                               
                       16-23 : subdetector number [1,3]                        
                       endcapA=1,Barrel=2,endcapB=3                            
      2    ED  I    EnergyDepos      [0,100000]                                
                       Energy in plane ( MeV)                                  
  */
  std::stringstream log;
  bool dbg = this->data.hpdi.debug;
  for( this->data.hpdi.load(false); this->data.hpdi.data; this->data.hpdi.knext() )  {
    auto* tab = this->data.hpdi.table<class hpdi>();
    if( dbg )  {
      log << bos77::to_string(tab) << std::endl;
    }
    for( uint32_t i=0, siz=tab->size(); i < siz; ++i )  {
      const auto* plane = tab->at(i);
      const auto   edep = plane->energyDepos();
      uint64_t     cell = uint64_t(plane->planeAddres());
      this->hits_hcal_plane.create(cell, edep, 0e0);
      if( dbg )  {
        char text[256];
        int detector = (cell>>16)&0xF;
        int module   = (cell>>8)&0xFF;
        int plane    = (cell&0xFF);
        const char* det_name = "Endcap B";
        if(      detector == 1 ) det_name = "Endcap A";
        else if( detector == 2 ) det_name = "Barrel  ";
        
        ::snprintf(text, sizeof(text), "  HWDI plane hit %s add:%08lX det:%2d mod:%2d plane:%3d E:%s GeV",
                   det_name, cell, detector, module, plane, fmt_ene(edep/1000e0).c_str());
        log << text << std::endl;
      }
    }
  }
  if( dbg )  {
    ::printf("%s\n", log.str().c_str());
  }
}
