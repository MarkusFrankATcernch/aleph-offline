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
#include <alpha/ptnc.h>
#include <alpha/ptun.h>
#include <alpha/tpcgeom.h>

extern "C" void tctglb_(int32_t& slot, float& rs, float& rphis, float& zs, float& r, float& rphi, float& z);

/// Create Production output Tpc pad Coordinates (NR=0)
void alpha::edm4hep_output::event_t::process_ptnc()  {
  /**

     Be careful: the resulting pads only contain the geometrical trafo
     from the sector local frame to the ALEPH global frame by calling tctglb_.
     Corrections other than alignment is not contained.

     See: alephlib/tdes/tctglb.F
     and: alephlib/ptoj/tun1nc.F (Full conversion to TPCO)

     M.Frank

  */

  /**
 +------+                               Subschema: TpcPOTBanks
 | PTNC |  Production output Tpc pad
 +------+  Coordinates (NR=0). The
           coordinates here are in the
           TPC sector reference frame
           and contain no drift or
           alignment corrections. This
           bank replaces the older
           version named PTCO, which
           contained corrected
           coordinates.

 ..............................................................
     1          I    Number of words/coord. (=6)
     2          I    Number of coordinates
 ..............................................................
      1    SL  I    SLot             [1,36]
                       Sector slot number
      2    SR  I    SectorRow        [1,12]
                       Padrow number in sector system
      3    RP  I    RPhisector       [-32000,32000]
                       R*Phi in sector system in units of 16
                       microns
      4    ZV  I    ZValue           [0,65000]
                       z in sector system in units of 40
                       microns
      5    SP  I    SigrPhi          [0,255]
                       sigma of R*Phi in units of 40 microns
      6    SZ  I    SigZ             [0,255]
                       sigma of Z in units of 80 microns
   */
  std::stringstream log;
  auto& tpc = *this->exp.tpc;
  auto& dsc = tpc.descriptor;
  bool  dbg  = this->data.ptnc.debug;
  auto* ptun = this->data.ptun.table<class ptun>()->at(0);
  auto* tab  = this->data.ptnc.load<object_table<class ptnc> >(true);
  if( dbg )  {
    log << bos77::to_string(tab) << std::endl;
  }
  for( uint32_t i=1; i <= tab->size(); ++i )  {
    class ptnc* ah     = tab->row(i);
    auto        hit    = this->hits_ptnc.create();
    uint32_t    padrow = ah->sectorRow();
    int32_t     slot   = ah->slot();
    // int32_t  endcap = tpcgeom.IENDTP[slot-1]; // 1 -> +z, 2 -> -z
    int32_t     sector = tpcgeom.ITPSEC[slot-1];
    int32_t     type   = tpcgeom.ITPTYP[slot-1];
    /// Global padrow
    int32_t    padrowg = (type == 1) ? padrow : (padrow+tpcgeop.NTPDRW[0]);
    /// Cell ID
    uint64_t    cell   = tpc.desc_system +
      dsc.encode(tpc.field_sector, sector) + dsc.encode(tpc.field_padrow, padrowg);
    /// Rphi and z in units as given in PTUN
    float       rphis  = float(ah->rphisector()) * ptun->rphi();
    float       zs     = float(ah->zvalue())     * ptun->zsector();
    float       rs     = tpcgeom.TPPROW[padrow-1];
    float       sigrphi= float(ah->sigrPhi()) * 40e-4; // sigrphi in units of 40 microns. Note: cm here!
    float       sigz   = float(ah->sigZ())    * 80e-4; // sigz in units of 80 microns.    Note: cm here!
    float       r      = 0e0, z = 0e0, rphi = 0e0;
    ///
    /// Transform TPC coordinates to global frame, including
    /// -- associated alignment corrections
    /// -- changes in drift velocity
    /// -- time-of-flight
    /// -- drift field distortions
    ::tctglb_(slot, rs, rphis, zs, r, rphi, z);

    float           phi = rphi/r;
    PositionRhoZPhi pos (_LEN(r), _LEN(z), phi);
    PositionRhoZPhi err (_LEN(sigrphi), _LEN(sigz), sigrphi/r);

    hit.setCellID( cell );
    hit.setTime( _TIM(0e0) );
    hit.setEDep( _ENE(0e0) );
    hit.setEDepError( _ENE(0e0) );
    hit.setPosition( { float(pos.x()), float(pos.y()), float(pos.z()) } );
    hit.setCovMatrix( {
        err.x()*err.x(),
        err.y()*err.x(), err.y()*err.y(),
        err.z()*err.x(), err.z()*err.y(), err.z()*err.z() } );
    hit.setQuality( 0 );
    hit.setType( TPC_RAW_PAD_COORDINATE );

    if( dbg )  {
      char text[512];
      ::snprintf(text, sizeof(text),
                 "\tPTNC %3d %08lX Cell:%08lX Sector:%2d Padrow:%2d "
                 "r:%s phi:%5.2f z:%s",
                 i, uint64_t(ah), cell, sector, padrow,
                 fmt_len(rs).c_str(), phi, fmt_len(zs).c_str());
      log << text << std::endl;
    }
  }
  if( dbg )  {
    ::printf("%s\n", log.str().c_str());
  }
}
