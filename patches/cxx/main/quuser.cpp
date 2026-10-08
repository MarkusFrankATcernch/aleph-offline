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
#include <alpha/edm4hep_output.h>
#include <iostream>
#include <cstring>
#include <string>
#include <memory>

extern "C"  {
  extern alpha::qcde_t    qcde_;
  void qminit_();
  void qmread_();
  void abruev_(int& irun, int& ievt);
  void qmevnt_(int& irun, int& ievt);
}

namespace alpha  {
  qcde_t& qcde = qcde_;
  const std::string& bos_bank_lists = "CERST";

  class test_processor;
  class edm4hep_output;
  class event_processor;

  std::unique_ptr<processor<test_processor> >  test_proc;
  std::unique_ptr<processor<event_processor> > event_proc;
  std::unique_ptr<edm4hep_output> edm4hep_proc;

}

using namespace alpha;

extern "C"  {

  void quinit_()  {
    bool debug = false;
    bool print = false;
    event_proc   = std::make_unique<processor<event_processor> >(debug, print);
    if ( event_proc   ) event_proc->initialize();
    if ( test_proc    ) test_proc->initialize();
    if ( edm4hep_proc ) edm4hep_proc->initialize();
  }

  void quterm_()  {
    if ( edm4hep_proc ) edm4hep_proc->finalize();
    if ( test_proc    ) test_proc->terminate();
    if ( event_proc   ) event_proc->terminate();
    test_proc.reset();
    event_proc.reset();
  }

  void quevnt_()  {
    if ( event_proc   ) event_proc->handle_event(params);
    if ( test_proc    ) test_proc->handle_event(params);
    if ( edm4hep_proc ) edm4hep_proc->handle_event(params);
  }
}

namespace  {
  void usage(int argc, char** argv, const char* msg)  {
    ::printf("Arguments: ");
    for(int i=0; i<argc; ++i)  {
      ::printf("%s ", argv[i]);
    }
    ::printf("\n");
    if( msg )  {
      ::printf("%s\n", msg);
    }
    ::exit(0);
  }
}

#include <climits>

int main(int argc, char** argv)   {
  int irun, ievt;
  bool debug = false;
  for(int iarg=1; iarg<argc; ++iarg)  {
    const char* p = argv[iarg];
    while( *p == '-' ) ++p;
    if( strncmp(p,"help",1) == 0 ) {
      usage(argc, argv, nullptr);
    }
    else if( strcmp(p,"debug") == 0 ) {
      debug = true;
    }
    else if( strncmp(p,"test",1) == 0 ) {
      test_proc = std::make_unique<processor<test_processor> >(debug, true);
    }
    else if( strncmp(p,"edm4hep",1) == 0 ) {
      edm4hep_proc = std::make_unique<edm4hep_output>();
    }
    else {
      int ret = edm4hep_proc->set_option(argv[iarg], argv[iarg+1]);
      if( ret == 2 ) ++iarg;
    }
  }

  qminit_();
  while( true )  {
    qmread_();
    abruev_(irun, ievt);
    qmevnt_(irun, ievt);
  }
  return 0;
}
