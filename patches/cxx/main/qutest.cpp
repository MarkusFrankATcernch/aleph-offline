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
#include <alpha/f77functions.h>
#include <iostream>
#include <cstring>
#include <cstdlib>
#include <climits>
#include <cerrno>
#include <string>
#include <vector>
#include <memory>
#include <dlfcn.h>

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

  union _callback  {
    void (*function)(void);
    void (*function_args)(void* fcn, void* com, const std::vector<std::string>& args);
    void* pointer;
  };

  _callback __init, __event, __fini;
  std::vector<std::string> arguments;
  
  void* load_func(const char* sym_name)  {
    const char* lib_name = "libalpha_userlib.so";
    ::dlerror();    /* Clear any existing error */
    void* handle = ::dlopen(lib_name, RTLD_NOW|RTLD_GLOBAL);
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
namespace alpha  {  void forward_functions();  }

using namespace alpha;

extern "C"  {
  void qminit_();
  void qmread_();
  void abruev_(int& irun, int& ievt);
  void qmevnt_(int& irun, int& ievt);

  int64_t mem_addr_(int32_t& value)  {
    int64_t val = (int64_t)&value;
    return val;
  }

  /// Initialization function
  void quinit_()  {
    alpha::forward_functions();
    __init.pointer  = load_func("init_111");
    __fini.pointer  = load_func("fini_111");
    __event.pointer = load_func("event_111");
    if( __init.pointer )  {
      (*__init.function_args)(f77::fun, f77::com, arguments);
    }
  }

  /// Finalization function
  void quterm_()  {
    if( __fini.pointer )  {
      (*__fini.function)();
    }
  }

  /// Event processing function
  void quevnt_()  {
    if( __event.pointer )  {
      (*__event.function)();
    }
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
