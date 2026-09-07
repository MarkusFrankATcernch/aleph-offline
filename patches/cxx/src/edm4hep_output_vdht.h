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
#include <alpha/vdht.h>

/// Create VDET simulation hit from VDHT row (produced by GALEPH)
void alpha::edm4hep_output::event_t::process_vdht()  {
  /**
     Subschema: VdetGALBanks
     +------+
     | VDHT |  VDet HiT list NR=0 (GAL)
     +------+
     ..............................................................
     1          I    Number of words/hit (=10)
     2          I    Number of hits
     ..............................................................
      1    TN  I    TrackNumber      [1,999]
                       Galeph track number
      2    LN  I    LayerNumber
                       Layer number
      3    PN  I    PhiNumber
                       phi coordinate
      4    XE  F    XEntry
                       X-entry point
      5    YE  F    YEntry
                       Y-entry point
      6    ZE  F    ZEntry
                       Z-entry point
      7    XL  F    XLast
                       X-exit point
      8    YL  F    YLast
                       Y-exit point
      9    ZL  F    ZLast
                       Z-exit point
      10   ER  F    EnergyReleased
                       Energy released
  */
  auto* tab = this->data.vdht.load<object_table<class vdht> >();
  auto siz = tab->size();
  for( uint32_t itk=1; itk <= siz; ++itk )  {
    auto        hit = this->simhits_vdht.create();
    auto*       ah  = this->data.vdht.row<class vdht>(itk);
    int32_t    galt = ah->trackNumber();
    uint64_t   cell = ah->layerNumber() + 1000*ah->phiNumber(); // probably wrong....
    Position  pos(_LEN(ah->xentry()), _LEN(ah->yentry()), _LEN(ah->zentry()));
    Position  end(_LEN(ah->xlast()),  _LEN(ah->ylast()),  _LEN(ah->zlast()));
    Direction dir   = (end-pos);
    edm4hep::MutableMCParticle mcp;

    if( galt )  {
      // edm4hep::MutableMCParticle mcp = particle_mc_fkin(galt);
    }
    // hit.setParticle(mcp);
    hit.setEDep(_ENE(ah->energyReleased()));
    hit.setTime(_TIM(0e0));
    hit.setPosition( { pos.x(), pos.y(), pos.z() } );
    hit.setPathLength(_LEN(dir.r()));
    dir = dir.Unit() * hit.getEDep();
    hit.setMomentum( { dir.x(), dir.y(), dir.z() } );
    hit.setCellID( cell );
    hit.setQuality( ah->trackNumber() );
  }
}
