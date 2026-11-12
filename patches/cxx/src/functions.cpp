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

/// C/C++ header files
#include <string>
#include <alpha/f77functions.h>

#pragma pack(push)
#pragma align(32)
namespace bos77  {
  union bcs_t*    bcs;
  class sysbos_t* sysbos;
}

namespace alpha  {
  class qcde_t*     qcde;
  class qcnami_t*   qcnami;
  class tpcgeom_t*  tpcgeom;
  class tpcgeop_t*  tpcgeop;
  const std::string& bos_bank_lists = "CERST";
}
#pragma pack(pop)
  
namespace f77 {
  functions* fun = nullptr;
  commons*   com = nullptr;

  void load(void* funcs, void* com)  {
    f77::fun = (f77::functions*)funcs;
    f77::com = (f77::commons*)com;
    bos77::bcs     = f77::com->bcs;
    bos77::sysbos  = f77::com->sysbos;
    alpha::qcde    = f77::com->qcde;
    alpha::qcnami  = f77::com->qcnami;
    alpha::tpcgeom = f77::com->tpcgeom;
    alpha::tpcgeop = f77::com->tpcgeop;
  }
}
