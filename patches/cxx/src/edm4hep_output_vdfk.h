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
void alpha::edm4hep_output::event_t::process_vdfk()  {
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
  std::stringstream log;
  std::size_t no_fkin = 0;
  auto* tab = this->data.vdfk.load<object_table<class vdfk> >(true);
  if( this->data.vdfk.debug )  {
    log << bos77::to_string(tab) << std::endl;
  }
  for( uint32_t i=1; i <= tab->size(); ++i )  {
    auto*   rel   = tab->row(i);
    int32_t nfkin = rel->fkIN();
    int32_t nvdco = rel->vdCO();
    auto    it    = this->alpha2edm4hep_vdco.find(nvdco);
    if( it == this->alpha2edm4hep_vdco.end() )  {
      throw std::runtime_error("Failed to access VDCO cluster id: "+std::to_string(nvdco));
    }
    auto vdco = this->hits_vdco[it->second];
    auto mcp  = this->particle_mc_fkin(nfkin);
    if( !mcp.isAvailable() ) ++no_fkin;
    _set_link(this->percent_charge_vdco_fkin_rphi, vdco, mcp, rel->percentCharge()[0]);
    _set_link(this->percent_charge_vdco_fkin_z,    vdco, mcp, rel->percentCharge()[1]);
    _set_link(this->strips_count_vdco_fkin_rphi,   vdco, mcp, rel->stripsCount()[0]);
    _set_link(this->strips_count_vdco_fkin_z,      vdco, mcp, rel->stripsCount()[1]);
  }
  if( no_fkin > 0 )  {
    log << "+++ VDFK: " << no_fkin << "/" << tab->size()
        << " VDCO clusters have no FKIN relationship!" << std::endl;
  }
  if( this->data.vdfk.debug )  {
    ::printf("%s\n", log.str().c_str());
  }
}
