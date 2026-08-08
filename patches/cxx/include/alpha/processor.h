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
#ifndef ALPHA_ALPHA_PROCESSOR_H
#define ALPHA_ALPHA_PROCESSOR_H

/// C/C++ include files
#include <cstdint>

/// Framework include files
#include <alpha/alpha.h>

/// ALPHA namespace declaration
namespace alpha  {

  class event_processor;
  
  
  template <typename TYPE> class processor  {
  public:
    static void initialize();
    static void handle_event(constants_t& params);
    static void terminate();
  };

}      // End namespace alpha
#endif // ALPHA_ALPHA_PROCESSOR_H
