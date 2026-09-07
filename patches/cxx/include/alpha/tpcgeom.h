//==========================================================================
//  Software for the ALEPH experiment
//--------------------------------------------------------------------------
// Copyright (C) Organisation europeenne pour la Recherche nucleaire (CERN)
// All rights reserved.
//
// For the licensing terms see ALSOFTINSTALL/LICENSE.
// For the list of contributors see ALSOFTINSTALL/doc/CREDITS.
//
// Author     : M.Frank
//
//==========================================================================
#ifndef ALPHA_ALPHA_TPCGEOM_H
#define ALPHA_ALPHA_TPCGEOM_H

/// Framework include files

/// C/C++ include files
#include <cstdint>

namespace alpha {

  /// TPC Geometry dimensions and parameters
  /**
   *  NOTE: Parameters are defined in comdeck TPGPAR:
   *
   * Parameter definitions for TPC geometry
   *
   *   LTSLOT  = number of sectors of each type
   *   LTSECT  = maximum number of sectors in TPC
   *   LTCORN  = maximum number of corners in a sector
   *   LTPDRO  = maximum number of padrows in TPC
   *   LTTROW  = maximum number of trigger padrows in TPC
   *   LTTSRW  = maximum number of trigger padrows per sector
   *   LTTPAD  = maximum number of pads on trigger padrow
   *   LTSTYP  = maximum number of TPC sector types
   *   LTWIRE  = maximum number of wires on TPC sector
   *   LTSROW  = maximum number of rows on sector
   *   LMXPDR  = maximum number of pads in a single row
   */
  struct tpgpar  {
    constexpr static std::size_t LTPDRO = 21;
    constexpr static std::size_t LTTROW = 19;
    constexpr static std::size_t LTSROW = 12;
    constexpr static std::size_t LTWIRE = 200;
    constexpr static std::size_t LTSTYP = 3;
    constexpr static std::size_t LTSLOT = 12;
    constexpr static std::size_t LTCORN = 6;
    constexpr static std::size_t LTSECT = LTSLOT*LTSTYP;
    constexpr static std::size_t LTTPAD = 4;
  };

  ///  Pad readout geometry for TPC
  /**
   *   NTPDRW(is)      = Number of padrows in each sector type
   *   NTPDPR(ir,is)   = No of equivalent full pads on each padrow
   *   TPDRBG(is)      = First padrow radius of each sector type
   *   TPDRST(is)      = Radial step between padrow centres
   *   TPDHGT(is)      = Pad height
   *   TPDSEP(is)      = Spacing between pad centres (along r-phi)
   *   TPDWID(is)      = Pad width (along r-phi)
   *   TPDHWD(is)      = Half-pad width
   *   TPDPHF(irs,is)  =  Half angle subtended by frame at each padrow
   *   TPDPHW(irs,is)  =  Phi at leading edge of 1st pad on row irs
   *   TPDPHS(irs,is)  =  Phi width of single pad on row irs
   *
   */
  struct tpcgeop_t  {
    int32_t           NTPDRW[tpgpar::LTSTYP];
    int32_t           NTPDPR[tpgpar::LTSROW][tpgpar::LTSTYP];
    float             TPDRBG[tpgpar::LTSTYP];
    float             TPDRST[tpgpar::LTSTYP];
    float             TPDHGT[tpgpar::LTSTYP];
    float             TPDSEP[tpgpar::LTSTYP];
    float             TPDWID[tpgpar::LTSTYP];
    float             TPDHWD[tpgpar::LTSTYP];
    float             TPDPHF[tpgpar::LTSROW][tpgpar::LTSTYP];
    float             TPDPHW[tpgpar::LTSROW][tpgpar::LTSTYP];
    float             TPDPHS[tpgpar::LTSROW][tpgpar::LTSTYP];
  };

  struct tpcgeom_t  {
    /**----------------------------------------------------------------------
     *
     *  Global geometry for the TPC
     *  ===========================
     *
     *  RTPCMN          =  Min radius of active volume
     *  RTPCMX          =  Maximum radius of active volume
     *  ZTPCMX          =  Half length of active volume
     *  DRTPMN          =  Inner wall thickness
     *  DRTPMX          =  Outer wall thickness
     *  DZTPMX          =  Endplate thickness
     *  TPFRDZ          =  Thickness of Al sector support frame
     *  TPFRDW          =  Width of Al sector support frame
     *  TPAVDZ          =  Thickness of equivalent endplate av. material
     *  TPFOF1          =  Offset of frame "kink" (see drawings)
     *  TPFOF2          =  Offset of frame "kink" (see drawings)
     *  TPFOF3          =  Offset of frame "kink" (see drawings)
     *  TPPROW(ipr)     =  Nominal padrow radii in master system
     *  TPTROW(itr)     =  Nominal trig. padrow radii in master system
     *  NTSECT          =  Number of sectors in TPC
     *  NTPROW          =  Total number of rows in TPC
     *
     *  TPC Sector geometry
     *  ===================
     *
     *               LTSTYP is number of sector types
     *               LTSLOT is number of sector slots of each type
     *               LTCORN is max number of corners for describing
     *                      sector geometry
     *
     *  NTPCRN(is)      =  No of corners in each sector type
     *                     (1=K, 2=M, 3=W)
     *  TPCORN(2,ic,is) =  {x,y} coordinates of corners
     *  TPPHI0(ntsec)   =  Global phi positions of sectors
     *  TPCPH0(iloc)    =  Cosines of sector phi positions
     *  TPSPH0(iloc)    =  Sines of sector phi positions
     *  ITPTYP(iloc)    =  Type number of sector at each slot
     *  ITPSEC(iloc)    =  Sector number (of given type) at each slot
     *  IENDTP(iloc)    =  Endplate number of each sector
     *                     Endplate 1 is at +z, Endplate 2 is at -z
     */

    float    RTPCMN;
    float    RTPCMX, ZTPCMX, DRTPMN, DRTPMX, DZTPMX;
    float    TPFRDZ, TPFRDW, TPAVDZ, TPFOF1, TPFOF2, TPFOF3;
    float    TPPROW[tpgpar::LTPDRO];
    float    TPTROW[tpgpar::LTTROW];
    int32_t  NTSECT, NTPROW;
    int32_t  NTPCRN[tpgpar::LTSTYP];
    float    TPCORN[2][tpgpar::LTCORN][tpgpar::LTSTYP];
    float    TPPHI0[tpgpar::LTSECT];
    float    TPCPH0[tpgpar::LTSECT];
    float    TPSPH0[tpgpar::LTSECT];
    int32_t  ITPTYP[tpgpar::LTSECT];
    int32_t  ITPSEC[tpgpar::LTSECT];
    int32_t  IENDTP[tpgpar::LTSECT];
  };

  extern tpcgeom_t& tpcgeom;
  extern tpcgeop_t& tpcgeop;
}
#endif //  ALPHA_ALPHA_TPCGEOM_H
