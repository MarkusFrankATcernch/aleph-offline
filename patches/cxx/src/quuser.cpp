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
#include <alpha/qcde.h>
#include <alpha/processor.h>
#include <memory>

extern "C"  {
  extern alpha::qcde_t   qcde_;
}

namespace alpha  {
  qcde_t& qcde = qcde_;
  const std::string& bos_bank_lists = "CEA";

  class test_processor;
  class output_edm4hep;
  class event_processor;

  std::unique_ptr<processor<test_processor> >  test_proc;
  std::unique_ptr<processor<event_processor> > event_proc;
  std::unique_ptr<processor<output_edm4hep> >  edm4hep_proc;
}

using namespace alpha;

extern "C"  {

  void quinit_()  {
    bool debug = false;
    bool print = false;
    event_proc   = std::make_unique<processor<event_processor> >(debug, print);
    test_proc    = std::make_unique<processor<test_processor> >(debug, print);
    edm4hep_proc = std::make_unique<processor<output_edm4hep> >(true, true);
    if ( event_proc   ) event_proc->initialize();
    if ( test_proc    ) test_proc->initialize();
    if ( edm4hep_proc ) edm4hep_proc->initialize();
  }

  void quterm_()  {
    if ( edm4hep_proc ) edm4hep_proc->terminate();
    if ( test_proc    ) test_proc->terminate();
    if ( event_proc   ) event_proc->terminate();
    edm4hep_proc.reset();
    test_proc.reset();
    event_proc.reset();
  }

  void quevnt_()  {
    if ( event_proc   ) event_proc->handle_event(params);
    if ( test_proc    ) test_proc->handle_event(params);
    if ( edm4hep_proc ) edm4hep_proc->handle_event(params);
  }
}
