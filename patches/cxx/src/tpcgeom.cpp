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

/// Framework include files
#include <alpha/tpcgeom.h>

extern "C"  {
  extern alpha::tpcgeom_t tpgeom_;
  extern alpha::tpcgeop_t tpgeop_;
}

namespace alpha  {
  tpcgeom_t& tpcgeom = tpgeom_;
  tpcgeop_t& tpcgeop = tpgeop_;
}
