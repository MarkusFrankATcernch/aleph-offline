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

#include <cstdint>

#if 0
namespace bos77 {
  int32_t* bcs = nullptr;
}

extern "C" void set_bcs(int32_t* ptr)  {
  bos77::bcs = ptr;
}
#endif

extern "C"  {
  extern int bcs_[1];
  extern int sysbos_[1];
}

namespace bos77  {

  union bcs_t;
  class sysbos_t;
  
  union bcs_t& get_bcs()  {
    auto* ptr = (bcs_t*)&bcs_[0];
    return *ptr;
  }
  class sysbos_t& get_sysbos()  {
    auto* ptr = (sysbos_t*)&sysbos_[0];
    return *ptr;
  }

}

