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
#include <alpha/vufk.h>
#include <alpha/vdzt.h>
#include <alpha/vdxy.h>

/// Process VDXY/VDZT relations to FKIN
void alpha::output_edm4hep::event_t::process_vufk()  {
  /**
              Subschema: VdetGALBanks
    +------+
    | VUFK |  Vdxy/vdzt to FKIN truth
    +------+  relation (monte Carlo) (hits unused by pattern reconstruction)
 ..............................................................
     1          I    Number of words/relation (=6)
     2          I    Number of relations
 ..............................................................
      1    BN  I    BankNumber       [0,29999]
                       VDXY or VDZT bank number
      2    HN  I    HitNumber        [0,*]
                       VDXY or VDZT hit number
      3    VI  I    View             [1,2]
                       View
      4    PC  F    PercentCharge    [0.0,1.00]
                       Fraction of the cluster charge
                       due to this track
      5    SC  I    StripsCount      [1,*]
                       Number of strips in this cluster
                       fired by this track
      6    FK  I    FKIN
                       Index of FKIN track
  */
  std::size_t no_fkin = 0;
  auto* tab = this->data.vufk.load<object_table<class vufk> >(true);
  if( this->data.vufk.debug )  {
    std::cout << bos77::to_string(tab) << std::endl;
  }
  for( uint32_t i=1; i <= tab->size(); ++i )  {
    auto*   rel   = tab->row(i);
    int32_t nfkin = rel->fkIN();
    int32_t nhit  = rel->hitNumber();
    int32_t nbank = rel->bankNumber();
    int32_t hitid = (nbank<<16) + nhit;
    edm4hep::MutableTrackerHit3D  vdet_hit;
    edm4hep::MutableSimTrackerHit hit;
    auto it = this->alpha2edm4hep_vdxy.find(hitid);

    if( it != this->alpha2edm4hep_vdxy.end() )  {
      hit = this->simhits_vufk_xy.create();
      vdet_hit = hits_vdxy[it->second];
      hit.setCellID( vdet_hit.getCellID()+VDET_WAFER_RPHI );
    }
    else if( (it=this->alpha2edm4hep_vdzt.find(hitid)) != this->alpha2edm4hep_vdzt.end() )  {
      auto        hit = this->simhits_vufk_z.create();
      vdet_hit = hits_vdzt[it->second];
      hit.setCellID( vdet_hit.getCellID()+VDET_WAFER_Z );
    }
    else  {
      throw std::runtime_error("Failed to access VDXY/VDZT id: "+std::to_string(hitid));
    }

    float percent_deposit = rel->percentCharge();
    _set_link(rel_vdzt_vufk, vdet_hit, hit, percent_deposit);

    hit.setTime( 0e0 );
    hit.setQuality( vdet_hit.getQuality() );
    hit.setPosition( vdet_hit.getPosition() );
    hit.setEDep( vdet_hit.getEDep()*percent_deposit );
    auto mcp = this->particle_mc_fkin(nfkin);
    if( !mcp.isAvailable() ) ++no_fkin;
    hit.setParticle(mcp);
  }
  if( no_fkin > 0 || this->data.vufk.debug )  {
    std::cout << "+++ VUFK: " << no_fkin << "/" << tab->size()
              << " VDXX/VDZT hits have no FKIN information!" << std::endl;
  }
}
