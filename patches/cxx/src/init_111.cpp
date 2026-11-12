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
#include <alpha/edm4hep_output.h>
#include <alpha/f77functions.h>

#include <iostream>
#include <cstring>
#include <cstdlib>
#include <climits>
#include <cerrno>
#include <string>
#include <memory>
#include <dlfcn.h>

using namespace alpha;

namespace alpha  {
  class test_processor;
  class edm4hep_output;
  class event_processor;
}

namespace  {

  //std::unique_ptr<processor<test_processor> >  test_proc;
  std::unique_ptr<processor<event_processor> > event_proc;
  std::unique_ptr<edm4hep_output> edm4hep_proc;
  void* load_func(const char* lib_name, const char* sym_name)  {
    ::dlerror();    /* Clear any existing error */
    void* handle = ::dlopen(lib_name, RTLD_LAZY);
    if( !lib_name ) lib_name = "<exe>";
    if( handle )  {
      void *pointer = ::dlsym(handle, sym_name);
      if( nullptr == pointer )  {
        char* error = ::dlerror();
        std::cout << "+++ Loading library: " << lib_name
                  << " symbol: " << sym_name
                  << "  [" << error << "] "
                  << std::endl;
        return nullptr;
      } 
      std::cout << "+++ Loaded: " << lib_name
                << " symbol: " << sym_name
                << std::endl;
      return pointer;
    }
    char* error = ::dlerror();
    std::cout << "+++ Loading library: " << lib_name
              << "  [" << error << "] "
              << std::endl;
    return nullptr;
  }
}

namespace f77 { void load(void*, void*); }

extern "C"  {

  int32_t* textio_;

  /// Initialization function
  void init_111(void* funcs, void* com, const std::vector<std::string>& argv)  {
    bool debug = false;
    bool print = true;
    int  ret;

    f77::load(funcs, com);

    for( std::size_t iarg=0, argc = argv.size(); iarg < argc; ++iarg )  {
      const char* p = argv[iarg].c_str();
      while( *p == '-' ) ++p;
      if( strcmp( p, "debug" ) == 0 ) {
        debug = true;
      }
      else if( strncmp( p, "test", 1 ) == 0 ) {
        //test_proc = std::make_unique<processor<test_processor> >( debug, true );
      }
      else if( strncmp( p, "edm4hep", 1 ) == 0 ) {
        edm4hep_proc = std::make_unique<edm4hep_output>();
      }
      else {
        const char* arg1 = argv[iarg].c_str();
        const char* arg2 = (iarg+1 < argc) ? argv[iarg+1].c_str() : "";
        ret = edm4hep_proc->set_option( arg1, arg2 );
        if( ret == 2 ) ++iarg;
      }
    }
    event_proc   = std::make_unique<processor<event_processor> >(debug, print);
    //if ( test_proc    ) test_proc->initialize();
    if ( edm4hep_proc ) edm4hep_proc->initialize();
  }
  
  void event_111()  {
    if( event_proc   )  {
      event_proc->handle_event();
    }
    //if ( test_proc    ) test_proc->handle_event();
    if ( edm4hep_proc ) edm4hep_proc->handle_event();
  }
  
  void fini_111()  {
    //if ( test_proc    ) test_proc->terminate();
    if ( edm4hep_proc ) edm4hep_proc->finalize();
    if ( event_proc )  event_proc->terminate();
  }
}
