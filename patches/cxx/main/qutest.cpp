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
#include <alpha/qcnami.h>
#include <alpha/processor.h>
#include <iostream>
#include <cstring>
#include <cstdlib>
#include <climits>
#include <cerrno>
#include <string>
#include <vector>
#include <memory>
#include <dlfcn.h>

#include "../src/alpha.cpp"
#include "../src/bosbank.cpp"
#include "../src/event_processor.cpp"

extern "C"  {
  extern alpha::qcde_t      qcde_;
  extern alpha::qcnami_t    qcnami_;
  void qminit_();
  void qmread_();
  void abruev_(int& irun, int& ievt);
  void qmevnt_(int& irun, int& ievt);
  int32_t namind_( const char* bname, int );
}

extern "C"  {
  extern union bos77::bcs_t    bcs_;
  extern class bos77::sysbos_t sysbos_;
}

/// bos77 namespace declaration
namespace bos77  {
  //union bcs_t&       bcs    = bcs_;
  //class sysbos_t&    sysbos = sysbos_;
}


namespace alpha  {
  qcde_t& qcde = qcde_;
  qcnami_t& qcnami = qcnami_;
  const std::string& bos_bank_lists = "CERST";

  class event_processor;
  std::unique_ptr<processor<event_processor> > event_proc;
}

namespace {
  
  union _callback  {
    void (*function)(void);
    void (*function_args)(const std::vector<std::string>& args);
    void* pointer;
  };

  _callback __init, __event, __fini;
  
  std::vector<std::string> arguments;
  
  void* load_func(const char* sym_name)  {
    const char* lib_name = "libalpha_userlib.so";
    ::dlerror();    /* Clear any existing error */
    void* handle = ::dlopen(lib_name, RTLD_LAZY);
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
using namespace alpha;

extern "C" int64_t mem_addr_(int32_t& value)  {
  int64_t val = (int64_t)&value;
  return val;
}

extern "C"  {

  /// Initialization function
  void quinit_()  {
    bool debug = false;
    bool print = false;
    event_proc   = std::make_unique<processor<event_processor> >(debug, print);
    __init.pointer  = load_func("init_111");
    __fini.pointer  = load_func("fini_111");
    __event.pointer = load_func("event_111");
    std::cout << "Namind pointer is " << (void*)namind_ << std::endl;
    if( __init.pointer )  {
      (*__init.function_args)(arguments);
    }
  }

  /// Finalization function
  void quterm_()  {
    if( __fini.pointer )  {
      (*__fini.function)();
    }
    if ( event_proc )  {
      event_proc->terminate();
    }
  }

  /// Event processing function
  void quevnt_()  {
    if( event_proc   )  {
      event_proc->handle_event(params);
    }
    if( __event.pointer )  {
      (*__event.function)();
    }
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
  void set_alpha_cards( const char* value )  {
    char resolved[PATH_MAX];
    if( nullptr == ::realpath(value, resolved) )  {
      ::printf( "+++ Non existing cards file: %s  [%s]\n",
                value, ::strerror(errno) );
      ::exit(ENOENT);
    }
    ::printf( "+++ Set ALPHACARDS=%s\n", resolved );
    ::setenv("ALPHACARDS", resolved, 1);
  }
}

int main(int argc, char** argv)   {
  int irun=0, ievt=0;
  for( int iarg=1; iarg < argc; ++iarg )  {
    const char* p = argv[iarg];
    while( *p == '-' ) ++p;
    if( strncmp(p,"help",1) == 0 ) {
      usage( argc, argv, nullptr );
    }
    else if( strncmp( p, "cards", 1 ) == 0 )  {
      set_alpha_cards( argv[++iarg] );
    }
    else  {
      arguments.push_back( argv[iarg] );
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
