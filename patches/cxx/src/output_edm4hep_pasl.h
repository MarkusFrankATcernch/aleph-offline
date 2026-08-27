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
#include <alpha/pasl.h>

/// Link reconstructed charged tracks from FRFT to MC tracks
void alpha::output_edm4hep::event_t::process_pasl()  {
  /**
     +------+                               Subschema: ItcPOTBanks                
     | PASL |  JULIA track to FKIN track                                           
     +------+  association list (accessed                                          
               via PITM bank). (ordered                                            
               according to increasing FRFT                                        
               track number) (POT)                                                 

    ..............................................................                
     1          I    Number of words/associated (=2) FKIN track.                                               
     2          I    Number of associated FKIN tracks.                                                   
     ..............................................................                
      1    FK  I    FKin             [1,*]                                     
                       No. of FKIN track associated with the FRFT track                                          
      2    NH  I    NumberHits       [1,*]                                     
                       No. of hits shared with this FKIN track                                                   
   */
  auto* frft = this->data.frft.load<object_table<class frft> >();
  auto* pitm = this->data.pitm.load<object_table<class pitm> >();
  auto* pasl = this->data.pasl.load<object_table<class pasl> >();
  for( uint32_t itk=0, frft_size = frft->size(); itk < frft_size; ++itk )  {
    edm4hep::MutableTrack track = this->particles_reco[itk];
    for( uint32_t itm=0, ipa=0, pitm_size = pitm->size(); itm < pitm_size; ++itm )  {
      uint32_t nass = pitm->at(itm)->numberAssociated();
      for( ; nass > 0; --nass,++ipa )  {
	const class pasl* pa = pasl->at(ipa);
	int32_t fkin = pa->fkin();
	int32_t nhit = pa->numberHits(); // Number of shared hits. Save as weight
	auto mcpart  = this->particle_mc_fkin(fkin);
	_set_link(rel_part_reco_mc, track, mcpart, nhit);
      }
    }
  }
}
