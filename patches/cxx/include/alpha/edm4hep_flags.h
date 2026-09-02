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
#ifndef ALPHA_EDM4HEP_FLAGS_H
#define ALPHA_EDM4HEP_FLAGS_H

/// ALPHA namespace declaration
namespace alpha  {

  /// Helper class to give access to subdetector identifiers
  /**
   *    \author  M.Frank
   *    \date    01/08/2026
   */
  struct  detectorid  {
    enum  subdetector  {
      VDET = 1,
      ITC  = 2,
      TPC  = 3,
      ECAL = 4,
      HCAL = 5,
      MUON = 6,
      LCAL = 7,
      SCAL = 8,
      SATR = 9,
      BOM  = 10,
      LAST
    };
  };

  /// Helper class to identify auxiliary charged track information
  /**
   *    \author  M.Frank
   *    \date    01/08/2026
   */ 
 struct charged_track_items  {
    enum track_opt_data_content  {
      FRTL_DATA = 0xFEED00,
      FRID_DATA,
      EIDT_DATA,
      HMAD_DATA,
      MCAD_DATA,
      MUID_DATA,
      LAST
    };
  };
}      // End namespace alpha
#endif // ALPHA_EDM4HEP_FLAGS_H
