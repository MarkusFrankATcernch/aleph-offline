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
#include <alpha/qcde.h>
#include <bos/bosbank.h>

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

std::string alpha::fmt_ene(float v)  {
  char  text[128];
  float vv = (v < 0e0) ? -110e0*v : v;
  if(      vv >=   10 ) ::snprintf(text,sizeof(text),"%5.0f ",v);
  else if( vv >=    1 ) ::snprintf(text,sizeof(text),"%5.1f ",v);
  else if( vv >=  0.1 ) ::snprintf(text,sizeof(text),"%5.2f ",v);
  else if( vv >= 0.01 ) ::snprintf(text,sizeof(text),"%5.3f ",v);
  else                  ::snprintf(text,sizeof(text),"%5.3f ",v);
  return { text };
}

std::string alpha::fmt_len(float v)  {
  char  text[128];
  float vv = (v < 0e0) ? -110e0*v : v;
  if(      vv >=   10 ) ::snprintf(text,sizeof(text),"%5.0f ",v);
  else if( vv >=    1 ) ::snprintf(text,sizeof(text),"%5.1f ",v);
  else if( vv >=  0.1 ) ::snprintf(text,sizeof(text),"%5.1f ",v);
  else if( vv >= 0.01 ) ::snprintf(text,sizeof(text),"%5.2f ",v);
  else                  ::snprintf(text,sizeof(text),"%5.2f ",v);
  return { text };
}

/// Access BOS bank com BOS common by hashed index (NR=0)
int32_t* alpha::bank_access_t::get_bank_first()   {
  bos77::bank* bank = bos77::get_bank_pointer_from_namind( this->nami );
  if( bank ) bos77::verify_bank_type(bank, this->nami);
  return (int32_t *)bank;
}

/// Access next BOS bank from BOS common as indicted in the bank header
int32_t* alpha::bank_access_t::get_bank_next()   {
  auto* bank = (bos77::bank*)this->data;
  auto* next = bank->next_bank_offset();
  if( next ) bos77::verify_bank_type(next, this->nami);
  this->data = (int32_t*)next;
  return this->data;
}

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
  if( params.current_event_number != qcde.KNEVT )  {
    int32_t* iw = bos77::bcs.iw;
    params.current_event_number = qcde.KNEVT;

    params.kqzer  = iw[params.naqzer-1];

    int32_t kqvec  = iw[params.naqvec-1];
    int32_t kqvrt  = iw[params.naqvrt-1];
    int32_t kqdet  = iw[params.naqdet-1];
    int32_t kqlin  = iw[params.naqlin-1];

    int32_t kfrft  = iw[params.nafrft-1];
    int32_t kefol  = iw[params.naefol-1];

    int32_t kpeco  = iw[params.napeco-1];
    int32_t kphco  = iw[params.naphco-1];
    int32_t kpgac  = iw[params.napgac-1];
    int32_t kpcqa  = iw[params.napcqa-1];

    int32_t kpdlt  = iw[params.napdlt-1];
    int32_t kpmdt  = iw[params.napmdt-1];
    int32_t kpmlt  = iw[params.napmlt-1];

    params.qvec_table = params.table<object_table<class qvec> >(kqvec);
    params.qvrt_table = params.table<object_table<class qvrt> >(kqvrt);
    params.qdet_table = params.table<object_table<class qdet> >(kqdet);
    params.qlin_table = params.table<object_table<class qlin> >(kqlin);

    params.frft_table = params.table<object_table<class frft> >(kfrft);
    params.efol_table = params.table<object_table<class efol> >(kefol);

    params.peco_table = params.table<object_table<class peco> >(kpeco);
    params.phco_table = params.table<object_table<class phco> >(kphco);
    params.pgac_table = params.table<object_table<class pgac> >(kpgac);
    params.pcqa_table = params.table<object_table<class pcqa> >(kpcqa);

    params.pdlt_table = params.table<object_table<class pdlt> >(kpdlt);
    params.pmlt_table = params.table<object_table<class pmlt> >(kpmlt);
    params.pmdt_table = params.table<object_table<class pmlt> >(kpmdt);
  }
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

  params.nafrft = bos77::namind("FRFT");
  params.naefol = bos77::namind("EFOL");
  params.napeco = bos77::namind("PECO");
  params.naphco = bos77::namind("PHCO");
  params.napgac = bos77::namind("PGAC");
  params.napcqa = bos77::namind("PCQA");

  params.napdlt = bos77::namind("PDLT");
  params.napmdt = bos77::namind("PMDT");
  params.napmlt = bos77::namind("PMLT");
}

