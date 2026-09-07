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
#include <alpha/iasl.h>

/// Link reconstructed charged tracks from FRFT to MC tracks
void alpha::edm4hep_output::event_t::process_iasl()  {
  /**
 +------+                             Subschema: ItcJuliaBanks
 | IASL |  JULIA track to FKIN track
 +------+  association list (accessed via ITMA bank). (ordered
           according to increasing FRFT track number) (JUL)

     ..............................................................
     1          I    Number of words/associated (=3)
                     FKIN track.
     2          I    Number of associated FKIN
                     tracks.
     ..............................................................
      1    FK  I    FKin             [1,*]
                       No. of FKIN track associated with
                       the FRFT track
      2    NH  I    NumberHits       [1,*]
                       No. of hits shared with this FKIN
                       track
      3    CS  F    ChiSquared       [0.0,*]
                       Chi**2 of comparison of helix
                       params. of FRFT and FKIN track
   */
  auto* frft = this->data.frft.load<object_table<class frft> >();
  auto* itma = this->data.itma.load<object_table<class itma> >();
  auto* iasl = this->data.iasl.load<object_table<class iasl> >();
  for( uint32_t itk=0, frft_size = frft->size(); itk < frft_size; ++itk )  {
    edm4hep::MutableTrack track = this->particles_reco[itk];
    for( uint32_t itm=0, ipa=0, itma_size = itma->size(); itm < itma_size; ++itm )  {
      uint32_t nass = itma->at(itm)->numberAssociated();
      for( ; nass > 0; --nass,++ipa )  {
        const class iasl* pa = iasl->at(ipa);
        int32_t fkin = pa->fkin();
        int32_t nhit = pa->numberHits(); // Number of shared hits. Save as weight
        auto mcpart  = this->particle_mc_fkin(fkin);
        _set_link(rel_part_reco_mc, track, mcpart, nhit);
      }
    }
  }
}
