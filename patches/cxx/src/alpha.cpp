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
#include <alpha/alpha.h>

/// C/C++ include files
#include <cstring>


extern "C" void aublis_(const char* bank_list, int32_t len);

void alpha::aublis(const char bank_list)  {
  aublis_(&bank_list, 1);
}

void alpha::aublis(const std::string& bank_list)  {
  for(std::size_t i=0; i<bank_list.size(); ++i)
    aublis(bank_list[i]);
}

