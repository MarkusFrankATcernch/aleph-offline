//==========================================================================
//  ALEPH software suite
//--------------------------------------------------------------------------
//  Copyright (C) Organisation europeenne pour la Recherche nucleaire (CERN)
//  All rights reserved.
//
//  For the licensing terms see OnlineSys/LICENSE.
//
//--------------------------------------------------------------------------
//
//  Author     : Markus Frank
//==========================================================================

/// C/C++ include files
#include <cstdio>
#include <alpha/qcde.h>
#include <alpha/alpha.h>
#include <alpha/processor.h>

namespace alpha  {
  void (*bosta) ();
}
namespace alpha {  class event_processor;   }

/// Framework initialization callback
template <> void alpha::processor<alpha::event_processor>::initialize()  {
  alpha::init_params();
}

/// Framework event callback
template <> void alpha::processor<alpha::event_processor>::handle_event()  {
  if( debug ) ::printf("event_processor: +++++++  Calling %s   KNEVT:%d\n",
                       __FUNCTION__, qcde.KNEVT);
  alpha::init_event();
}

/// Framework termination callback
template <> void alpha::processor<alpha::event_processor>::terminate()  {
  if( debug )  {
    ::printf("event_processor: +++++++  Calling %s\n", __FUNCTION__);
    ::alpha::bosta();
  }
}
