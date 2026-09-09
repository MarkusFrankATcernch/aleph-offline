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
void alpha::edm4hep_output::event_t::process_vufk()  {
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

  Note:
      View: 1 --> Z wafer, 2 --> R/phi wafer
  */
  std::stringstream log;
  std::size_t no_fkin = 0;
  std::map<int, std::size_t>::iterator it;
  auto* tab = this->data.vufk.load<object_table<class vufk> >(true);

  if( this->data.vufk.debug )  {
    std::cout << bos77::to_string(tab) << std::endl;
  }
  for( uint32_t i=1; i <= tab->size(); ++i )  {
    trackerhit_t vdet_hit;
    auto*   rel   = tab->row(i);
    int32_t nfkin = rel->fkIN();
    int32_t nhit  = rel->hitNumber();
    int32_t nbank = rel->bankNumber();
    int32_t iview = rel->view();
    int32_t hitid = (nbank<<16) + nhit;
    auto mcp = this->particle_mc_fkin(nfkin);

    if( this->data.vufk.debug )  {
      char text[256];
      ::snprintf(text, sizeof(text),
                 "+++\tVUFK: KINE:%3d valid:%3s id:%08X bank:%6d row:%3d view:%1d",
                 nfkin, yes_no(mcp.isAvailable()), hitid, nbank, nhit, iview);
      log << text << std::endl;
    }
    if( !mcp.isAvailable() ) ++no_fkin;
    if( iview == 1 && (it=this->alpha2edm4hep_vdzt.find(hitid)) != this->alpha2edm4hep_vdzt.end() )  {
      vdet_hit = hits_vdzt[it->second];
      _set_link(percent_charge_unused_vdzt_fkin, vdet_hit, mcp, rel->percentCharge());
      _set_link(strips_count_unused_vdzt_fkin,   vdet_hit, mcp, rel->stripsCount());
    }
    else if( iview == 2 && (it=this->alpha2edm4hep_vdxy.find(hitid)) != this->alpha2edm4hep_vdxy.end() )  {
      vdet_hit = hits_vdxy[it->second];
      _set_link(percent_charge_unused_vdzt_fkin, vdet_hit, mcp, rel->percentCharge());
      _set_link(strips_count_unused_vdzt_fkin,   vdet_hit, mcp, rel->stripsCount());
    }
    else  {
      throw std::runtime_error("Failed to access VDXY/VDZT id: "+std::to_string(hitid));
    }
  }
  if( no_fkin > 0 )  {
    log << "+++ VUFK: " << no_fkin << "/" << tab->size()
        << " VDXX/VDZT hits have no FKIN information!" << std::endl;
  }
  if( this->data.vufk.debug )  {
    ::printf("%s\n", log.str().c_str());
  }
}
