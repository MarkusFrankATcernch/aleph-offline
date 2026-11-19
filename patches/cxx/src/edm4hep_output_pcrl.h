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
#include <alpha/pcrl.h>

/// Create HCAL cluster relations from PECO to FRFT tracks: bank PFHR
void alpha::edm4hep_output::event_t::process_pcrl()  {
  /**
     +------+                              Subschema: CobjPOTBanks
     | PCRL |  Calobject ReLations
     +------+
     ..............................................................
         1          I    Number of words/calobject (=5)
                         relation
         2          I    Number of calobject relations
     ..............................................................
          1    PC  I    PCOB
                           Calobject number
          2    PE  I    PECO
                           Row index of Electromagnetic Calobject
          3    PF  I    PFRF
                           Row index of Fitted Track
          4    PH  I    PHCO
                           Row index of Hadronic Calobject
          5    PP  I    PPOB
                           Row index of Hadronic Digital Pattern
  */
  // Do not know how to resolve this: Calorimeter hits cannot attached to edm4hep::MutableTrack
  bool  dbg      = this->data.pcrl.debug;
  auto* tab_pcrl = this->data.pcrl.load<object_table<class pcrl> >( true );

  printout( dbg ? ALWAYS : DEBUG, __FUNCTION__,"+++ %s", bos77::to_string(tab_pcrl).c_str() );
  for( uint32_t itk=1, siz=tab_pcrl->size(); itk <= siz; ++itk )  {
    auto*   pcrl  = tab_pcrl->row(itk);
    int32_t ipcob = pcrl->pcOB();
    int32_t ipeco = pcrl->peCO();
    int32_t iphco = pcrl->phCO();
    int32_t ippob = pcrl->ppOB();
    int32_t ifrft = pcrl->pfRF();

    if( ifrft > 0 )  {
      auto track = this->particle_frft( ifrft );
      if( ipeco > 0 )  {
        _set_link( this->ecal_hits2frft, this->ecal_hits.at(ipeco-1), track );
      }
      if( iphco > 0 )  {
        _set_link( this->hcal_hits2frft, this->hcal_hits.at(iphco-1), track );
      }
      if( ipcob > 0 )  {
        this->frft2calobj[ifrft].insert( ipcob );
        this->calobj2frft[ipcob].insert( ifrft );
      }
    }
    if( iphco > 0 && ippob > 0 )  {
      this->hcal_hit2pattern[iphco].insert( ippob );
    }
    if( ipcob > 0 )  {
      if( ipeco > 0 ) this->calobj2ecal_hits[ipcob].insert( ipeco );
      if( iphco > 0 ) this->calobj2hcal_hits[ipcob].insert( iphco );
      if( ippob > 0 ) this->calobj2hcal_pattern[ipcob].insert( ippob );
    }
  }
  ++this->data.pcrl.num_process;
}
