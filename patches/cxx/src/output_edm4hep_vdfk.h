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
#include <alpha/vdfk.h>

/// Process VDCO relations to FKIN
void alpha::output_edm4hep::event_t::process_vdfk()  {
  /**
                                        Subschema: VdetGALBanks
     +------+
     | VDFK |  Vdco to FKIN truth relation
     +------+  (monte Carlo) (hits associated to reconstructed tracks)
     ..............................................................
         1          I    Number of words/relation (=6)
         2          I    Number of relations
     ..............................................................
        1-2    PC  F    PercentCharge    [0.0,2.00]
                           Fraction of the cluster charge
                           due to this track for the 2 views
        3-4    SC  I    StripsCount      [1,*]
                           Number of strips in this cluster
                           fired by this track for the 2 views
          5    FK  I    FKIN
                           Index of FKIN track
          6    VD  I    VDCO
                           Index of Vdet cluster in VDCO bank
  */
  std::size_t no_fkin = 0;
  auto* tab = this->data.vdfk.load<object_table<class vdfk> >(true);
  std::cout << bos77::to_string(tab) << std::endl;
  for( uint32_t i=1; i <= tab->size(); ++i )  {
    float   percent_deposit;
    auto*   rel   = tab->row(i);
    int32_t nfkin = rel->fkIN();
    int32_t nvdco = rel->vdCO();
    auto    it    = this->alpha2edm4hep_vdco.find(nvdco);

    if( it == this->alpha2edm4hep_vdco.end() )  {
      throw std::runtime_error("Failed to access VDCO cluster id: "+std::to_string(nvdco));
    }
    trackerhit_t vdco = this->hits_vdco[it->second];
    uint64_t cell = vdco.getCellID() & (~VDET_COORDINATE);
    auto mcp = this->particle_mc_fkin(nfkin);
    if( !mcp.isAvailable() ) ++no_fkin;
    {
      auto hit = this->simhits_vdfk_rphi.create();
      hit.setCellID( cell+VDET_WAFER_RPHI );
      _set_link(rel_vdco_vdfk_rphi, vdco, hit, percent_deposit);
      percent_deposit = rel->percentCharge()[0];
      hit.setTime( 0e0 );
      hit.setQuality( vdco.getQuality() );
      hit.setPosition( vdco.getPosition() );
      hit.setEDep( vdco.getEDep()*percent_deposit );
      hit.setParticle(mcp);
    }
    {
      auto hit = this->simhits_vdfk_z.create();
      hit.setCellID( cell+VDET_WAFER_Z );
      _set_link(rel_vdco_vdfk_z, vdco, hit, percent_deposit);
      percent_deposit = rel->percentCharge()[1];
      hit.setTime( 0e0 );
      hit.setQuality( vdco.getQuality() );
      hit.setPosition( vdco.getPosition() );
      hit.setEDep( vdco.getEDep()*percent_deposit );
      hit.setParticle(mcp);
    }
  }
  if( no_fkin > 0 )  {
    std::cout << "+++ VDFK: " << no_fkin << "/" << tab->size()
	      << " VDCO clusters have no FKIN information!" << std::endl;
  }
}
