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
#include <alpha/phmh.h>

/// Create ECAL wire data from PEWI data
void alpha::edm4hep_output::event_t::process_phmh()  {
  /**
 +------+                              Subschema: CobjPOTBanks
 | PHMH |  Relation between HCAL objects and FKIN tracks
 +------+
 ..............................................................                
     1          I    Number of words/relation (=2)
     2          I    Number of relations
 ..............................................................                
      1    CO  I    CalObjectnumber  [1,*]
                       PHCO or PHOB number
      2    TN  I    TrackNumber      [1,*]
                       FKIN number        
 .............................................................. */
  auto* tab = this->data.phmh.load<object_table<class phmh> >( false );
  if( tab )  {
    for( uint32_t itk=1, siz=tab->size(); itk <= siz; ++itk )  {
      auto*   phmh   = tab->row(itk);
      double  weight = 1e0;
      int32_t ifkin  = phmh->trackNumber()-1;
      int32_t iphco  = phmh->calObjectnumber()-1;
      if( ifkin >= 0 && iphco >= 0 )  {
        _set_link( this->hcal_hits2mcp, this->hcal_hits.at(iphco), this->particles_mc.at(ifkin), weight );
      }
    }
    ++this->data.phmh.num_process;
  }
}
