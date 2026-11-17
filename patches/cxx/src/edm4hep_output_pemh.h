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
#include <alpha/pemh.h>

/// Create ECAL wire data from PEWI data
void alpha::edm4hep_output::event_t::process_pemh()  {
  /**
 +------+                              Subschema: CobjPOTBanks
 | PEMH |  Relation between ECAL and   
 +------+  LCAL objects and FKIN tracks
 
 ..............................................................                
     1          I    Number of words/relation (=3)
     2          I    Number of relations  
 ..............................................................                
      1    CO  I    CalObjectnumber  [1,*]
                       PECO or PEOB number
      2    TN  I    TrackNumber      [1,*]
                       FKIN number        
      3    SE  I    SharedEnergy     [1,*]
                       Energy FKIN track contributes to
                       this Cal Obj in MeV
 ..............................................................
  */
  auto* tab = this->data.pemh.load<object_table<class pemh> >(true);

  for( uint32_t itk=1, siz=tab->size(); itk <= siz; ++itk )  {
    auto*   pemh   = tab->row(itk);
    int32_t ifkin  = pemh->trackNumber()-1;
    int32_t ipeco  = pemh->calObjectnumber()-1;
    if( ifkin >= 0 && ipeco >= 0 )  {
      double  shared = pemh->sharedEnergy();
      double  energy = this->ecal_hits.at(ipeco).getEnergy();
      double  weight = (energy > 1e-8) ? shared/energy : 1e0;
      _set_link( this->ecal_hits2mcp, this->ecal_hits.at(ipeco), this->particles_mc.at(ifkin), weight );
    }
  }
}
