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
#include <alpha/processor.h>

extern "C"  {
  extern alpha::qcde_t   qcde_;
}

namespace alpha  {
  qcde_t& qcde = qcde_;
  const std::string& bos_bank_lists = "CEA";
}

extern "C"  {

  void quinit_()  {
    alpha::processor<alpha::event_processor>::initialize();
  }

  void quterm_()  {
    alpha::processor<alpha::event_processor>::terminate();
  }

  void quevnt_()  {
    alpha::processor<alpha::event_processor>::handle_event(alpha::params);
  }
}
