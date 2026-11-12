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
#ifndef ALPHA_F77_FUNCTIONS_H
#define ALPHA_F77_FUNCTIONS_H

/// C/C++ header files
#include <cstdint>

namespace bos77  {
  union bcs_t;
  class sysbos_t;
}

namespace alpha  {
  class qcde_t;
  class qcnami_t;
  class tpcgeom_t;
  class tpcgeop_t;
}

/// ALPHA namespace declaration
namespace f77  {

  struct commons  {
    bos77::bcs_t*     bcs;
    bos77::sysbos_t*  sysbos;

    alpha::qcde_t*    qcde;
    alpha::qcnami_t*  qcnami;
    alpha::tpcgeom_t* tpcgeom;
    alpha::tpcgeop_t* tpcgeop;
  };
  extern commons* com;
  
  struct functions  {
    void    (*bosta_)();
    int32_t (*namind_)(const char* bname, int);
    int32_t (*nlinc_)(int32_t&    nami,  int& nr );
    int32_t (*nlink_)(const char* bname, int& nr, int );
    void    (*nlistb_)(char*, int32_t, int32_t*, uint32_t*, const char*, int);
    
    void    (*tctglb_)(int32_t& slot, float& rs, float& rphis, float& zs, float& r, float& rphi, float& z);
    void    (*get_particle_name_)(const int32_t& pcode, int32_t& len, char* text, int32_t siz);
    void    (*aublis_)(const char* bank_list, int32_t len);
  };
  extern functions* fun;

}      // End namespace alpha
#endif // ALPHA_F77_FUNCTIONS_H
