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

  template <typename TYPE> class processor  {
  public:
    bool debug = false;
    bool print = false;
  public:
    processor(bool deb, bool prt) : debug(deb), print(prt) {}
    virtual void initialize();
    virtual void handle_event(constants_t& params);
    virtual void terminate();
  };

}      // End namespace alpha
#endif // ALPHA_ALPHA_PROCESSOR_H
