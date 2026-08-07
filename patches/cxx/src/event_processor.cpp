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
#include <cstdint>
#include <alpha/alpha.h>

#include <alpha/defs.h>
#include <alpha/qcde.h>
#include <alpha/qvec.h>
#include <tests/tests.h>

#include <bos/bosbank.h>

namespace {
  bool have_tests = true;
}
namespace {
  alpha::constants_t s__params;
}

namespace alpha {
    constants_t& params = s__params;
}

static void init_event()  {
  using namespace alpha;
  params.kqzer  = bos77::bcs.iw[params.naqzer];

  int32_t kqvec  = bos77::bcs.iw[params.naqvec];
  int32_t kqvrt  = bos77::bcs.iw[params.naqvrt];
  int32_t kqdet  = bos77::bcs.iw[params.naqdet];
  int32_t kqlin  = bos77::bcs.iw[params.naqlin];
  int32_t kpeco  = bos77::bcs.iw[params.napeco];
  int32_t kphco  = bos77::bcs.iw[params.naphco];
  int32_t kpgac  = bos77::bcs.iw[params.napgac];
  int32_t kpcqa  = bos77::bcs.iw[params.napcqa];
  int32_t kpdlt  = bos77::bcs.iw[params.napdlt];
  int32_t kpmlt  = bos77::bcs.iw[params.napmlt];
  int32_t kefol  = bos77::bcs.iw[params.naefol];

  params.qvec_table = params.table<object_table<class qvec> >(kqvec);
  params.qvrt_table = params.table<object_table<class qvrt> >(kqvrt);
  params.qdet_table = params.table<object_table<class qdet> >(kqdet);
  params.qlin_table = params.table<object_table<class qlin> >(kqlin);
  params.peco_table = params.table<object_table<class peco> >(kpeco);
  params.phco_table = params.table<object_table<class phco> >(kphco);
  params.pgac_table = params.table<object_table<class pgac> >(kpgac);
  params.pcqa_table = params.table<object_table<class pcqa> >(kpgac);
  params.pdlt_table = params.table<object_table<class pdlt> >(kpdlt);
  params.pmlt_table = params.table<object_table<class pmlt> >(kpmlt);
  params.efol_table = params.table<object_table<class efol> >(kefol);
}

/// Framework initialization callback
template <> void alpha::processor<alpha::event_processor>::initialize()  {
  params.kmatix[0][0] = 0;
  params.kmatix[1][0] =     1;
  params.kmatix[2][0] =         3;
  params.kmatix[3][0] =             6;
  params.kmatix[4][0] =                10;
  params.kmatix[5][0] =                    15;
  params.kmatix[0][1] = 1;
  params.kmatix[1][1] =     2;
  params.kmatix[2][1] =         4;
  params.kmatix[3][1] =             7;
  params.kmatix[4][1] =                11;
  params.kmatix[5][1] =                    16;
  params.kmatix[0][2] = 3;
  params.kmatix[1][2] =     4;
  params.kmatix[2][2] =         5;
  params.kmatix[3][2] =             8;
  params.kmatix[4][2] =                12;
  params.kmatix[5][2] =                    17;
  params.kmatix[0][3] = 6;
  params.kmatix[1][3] =     7;
  params.kmatix[2][3] =         8;
  params.kmatix[3][3] =             9;
  params.kmatix[4][3] =                13;
  params.kmatix[5][3] =                    18;
  params.kmatix[0][4] = 10;
  params.kmatix[1][4] =    11;
  params.kmatix[2][4] =        12;
  params.kmatix[3][4] =            13;
  params.kmatix[4][4] =                14;
  params.kmatix[5][4] =                    19;
  params.kmatix[0][5] = 15;
  params.kmatix[1][5] =    16;
  params.kmatix[2][5] =        17;
  params.kmatix[3][5] =            18;
  params.kmatix[4][5] =                19;
  params.kmatix[5][5] =                    20;

  params.naqzer = bos77::namind("QZER");
  params.naqlin = bos77::namind("QLIN");
  params.naqvec = bos77::namind("QVEC");
  params.naqvrt = bos77::namind("QVRT");
  params.naqdet = bos77::namind("QDET");
  params.napeco = bos77::namind("PECO");
  params.naphco = bos77::namind("PHCO");
  params.napgac = bos77::namind("PGAC");
  params.napcqa = bos77::namind("PCQA");
  params.napdlt = bos77::namind("PDLT");
  params.napmlt = bos77::namind("PMLT");
  params.napmlt = bos77::namind("PMLT");

  if( have_tests ) tests::initialize();
}

/// Framework event callback
template <> void alpha::processor<alpha::event_processor>::handle_event()  {
  ::printf("+++++++  Calling %s\n", __FUNCTION__);
  init_event();
  if( have_tests ) tests::process_event();
}

/// Framework termination callback
template <> void alpha::processor<alpha::event_processor>::terminate()  {
  ::printf("+++++++  Calling %s\n", __FUNCTION__);
  if( have_tests ) tests::finalize();
  //::bosta_();
}
