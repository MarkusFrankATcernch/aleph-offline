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

/// C/C++ header files
#include <cstdint>
#include <alpha/f77functions.h>

extern "C"  {
  void    bosta_();
  int32_t namind_(const char* bname, int);
  int32_t nlinc_ (int32_t&    nami,  int& nr );
  int32_t nlink_ (const char* bname, int& nr, int );
  void    nlistb_(char*, int32_t, int32_t*, uint32_t*, const char*, int);
  
  void    aublis_(const char* bank_list, int32_t len);
  void    get_particle_name_(const int32_t& pcode, int32_t& len, char* text, int32_t siz);
  void    tctglb_(int32_t& slot, float& rs, float& rphis, float& zs, float& r, float& rphi, float& z);

  bos77::bcs_t*     bcs_;
  bos77::sysbos_t*  sysbos_;

  alpha::qcde_t*    qcde_;
  alpha::qcnami_t*  qcnami_;
  alpha::tpcgeom_t* tpcgeom_;
  alpha::tpcgeop_t* tpcgeop_;
}

namespace f77 {
  extern functions* fun;
  extern commons*   com;
}

namespace alpha  {
  extern void (*tctglb)(int32_t& slot, float& rs, float& rphis, float& zs, float& r, float& rphi, float& z);
  extern void (*get_particle_name)(const int32_t& pcode, int32_t& len, char* text, int32_t siz);
  extern void (*f77_aublis)(const char* bank_list, int32_t len);

  void forward_functions()  {
    f77::fun = new f77::functions();
    
    f77::fun->bosta_   = ::bosta_;
    f77::fun->namind_  = ::namind_;
    f77::fun->nlinc_   = ::nlinc_;
    f77::fun->nlink_   = ::nlink_;
    f77::fun->nlistb_  = ::nlistb_;
    
    f77::fun->tctglb_   = ::tctglb_;
    f77::fun->aublis_   = ::aublis_;
    f77::fun->get_particle_name_ = ::get_particle_name_;


    f77::com = new f77::commons();
    f77::com->bcs     = ::bcs_;
    f77::com->qcde    = ::qcde_;
    f77::com->qcnami  = ::qcnami_;
    f77::com->tpcgeom = ::tpcgeom_;
    f77::com->tpcgeop = ::tpcgeop_;
  }
}
