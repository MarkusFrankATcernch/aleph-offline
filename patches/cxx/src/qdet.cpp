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
#include <alpha/qdet.h>

/// C/C++ include files
#include <cstring>


/// Check if V0 data are available for track I
bool alpha::qdet::xyv0v()  const  {
  // return this->offset_yv0v != 0 && this->offset_yv0v != params.kqzer;
  return false;
}

/// Access V0 data are available for track I
const class alpha::yv0v* alpha::qdet::yv0v()  const  {
  //return this->xyv0v() ? bcs_offset<class yv0v>(this->offset_yv0v) : nullptr;
  return nullptr;
}
