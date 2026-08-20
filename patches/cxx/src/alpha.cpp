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

namespace {
  alpha::constants_t s__params;
  const char* ws = " \t\n\r\f\v";
}

namespace alpha {
  constants_t& params = s__params;
  // trim from end of string (right)
  inline std::string& rtrim(std::string& s, const char* t = ws)  {
    s.erase(s.find_last_not_of(t) + 1);
    return s;
  }

  // trim from beginning of string (left)
  inline std::string& ltrim(std::string& s, const char* t = ws)  {
    s.erase(0, s.find_first_not_of(t));
    return s;
  }
  // trim from both ends of string (right then left)
  inline std::string& trim(std::string& s, const char* t = ws)  {
    return ltrim(rtrim(s, t), t);
  }
}

extern "C" void aublis_(const char* bank_list, int32_t len);

// trim from both ends of string (right then left)
std::string& alpha::_trim(std::string& s)  {
  return ltrim(rtrim(s, ws), ws);
}

/// Bank listing of all BOS lists
void alpha::aublis(const char bank_list)  {
  aublis_(&bank_list, 1);
}

/// Bank listing of all BOS lists
void alpha::aublis(const std::string& bank_list)  {
  for(std::size_t i=0; i<bank_list.size(); ++i)
    aublis(bank_list[i]);
}

/// Stringify hexadecimal address
std::string alpha::hex_addr(const void* addr, const char* fmt)  {
  char text[32];
  unsigned long laddr = (unsigned long)addr;
  if( !laddr )
    ::snprintf(text, sizeof(text), "%-8s", "-----");
  else if( laddr < 0xFFFFFFFF )
    ::snprintf(text, sizeof(text), fmt ? fmt : "%08lx", laddr);
  else
    ::snprintf(text, sizeof(text), fmt ? fmt : "%16lx", laddr);
  return { text };
}

/// Access QVEC table. If not initialized, initialize all necessary pointers
const alpha::object_table<class alpha::qvec>*  alpha::get_qvec()  {
  if( nullptr == params.qvec_table )  {
    if( 0 == params.naqzer )  {
      init_params();
    }
    init_event();
  }
  if( params.qvec_table )  {
    return params.qvec_table;
  }
  throw std::runtime_error("Failed to access QVEC table!");
}

/// Access QDET table. If not initialized, initialize all necessary pointers
const alpha::object_table<class alpha::qdet>*  alpha::get_qdet()  {
  if( nullptr == params.qdet_table )  {
    if( 0 == params.naqzer )  {
      init_params();
    }
    init_event();
  }
  if( params.qdet_table )  {
    return params.qdet_table;
  }
  throw std::runtime_error("Failed to access QDET table!");
}

/// Initialize event parameters (bank locations) set table pointers
void alpha::init_event()  {
  using namespace alpha;
  int32_t* iw = bos77::bcs.iw;
  params.kqzer  = iw[params.naqzer];

  int32_t kqvec  = iw[params.naqvec];
  int32_t kqvrt  = iw[params.naqvrt];
  int32_t kqdet  = iw[params.naqdet];
  int32_t kqlin  = iw[params.naqlin];

  int32_t kefol  = iw[params.naefol];

  int32_t kpeco  = iw[params.napeco];
  int32_t kphco  = iw[params.naphco];
  int32_t kpgac  = iw[params.napgac];
  int32_t kpcqa  = iw[params.napcqa];

  int32_t kpdlt  = iw[params.napdlt];
  int32_t kpmdt  = iw[params.napmdt];
  int32_t kpmlt  = iw[params.napmlt];

  params.qvec_table = params.table<object_table<class qvec> >(kqvec);
  params.qvrt_table = params.table<object_table<class qvrt> >(kqvrt);
  params.qdet_table = params.table<object_table<class qdet> >(kqdet);
  params.qlin_table = params.table<object_table<class qlin> >(kqlin);

  params.efol_table = params.table<object_table<class efol> >(kefol);

  params.peco_table = params.table<object_table<class peco> >(kpeco);
  params.phco_table = params.table<object_table<class phco> >(kphco);
  params.pgac_table = params.table<object_table<class pgac> >(kpgac);
  params.pcqa_table = params.table<object_table<class pcqa> >(kpcqa);

  params.pdlt_table = params.table<object_table<class pdlt> >(kpdlt);
  params.pmlt_table = params.table<object_table<class pmlt> >(kpmlt);
  params.pmdt_table = params.table<object_table<class pmlt> >(kpmdt);
}

/// Initialize parameters (NAMIND etc)
void alpha::init_params()  {
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
  params.naqdet = bos77::namind("QDET");
  params.naqvrt = bos77::namind("QVRT");

  params.naefol = bos77::namind("EFOL");
  params.napeco = bos77::namind("PECO");
  params.naphco = bos77::namind("PHCO");
  params.napgac = bos77::namind("PGAC");
  params.napcqa = bos77::namind("PCQA");

  params.napdlt = bos77::namind("PDLT");
  params.napmdt = bos77::namind("PMDT");
  params.napmlt = bos77::namind("PMLT");
}

