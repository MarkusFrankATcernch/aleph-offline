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
#include <bos/bosbank.h>

#include <sstream>

extern "C"  {
  extern bos77::bcs_t    bcs_;
  extern bos77::sysbos_t sysbos_;
  //extern int32_t namind_(int32_t& name);
}

namespace bos77  {
  bcs_t&    bcs    = bcs_;
  sysbos_t& sysbos = sysbos_;
  const std::string& bos_bank_lists = "CEA";
}


namespace bos77  {

  int32_t* absolute_offset( std::size_t offset )  {
    return bcs.iw + offset;
  }

  int32_t namind(const char* bank)   {
    // Offset here call to iw[namind(xxx))] which starts in F77 with 1
    return namind_(bank,4) - 1;
  }
  int32_t namind(const std::string& bank)   {
    return namind(bank.c_str());
  }
  
  int32_t* get_bank_pointer_from_namind( int32_t namindex )  {
    int32_t* bank = bcs.iw + namindex;
    return bank;
  }
  int32_t* get_bank_pointer_from_name( const char* bank )  {
    auto indx = ::bos77::namind(bank);
    return ::bos77::get_bank_pointer_from_namind(indx);
  }
  void print_bank_lists(const std::string& lists)  {
  }
  std::string bank::to_string(const std::string& prefix)  const  {
    std::stringstream str;
    auto nam = this->name();
    str << prefix;
    if( !prefix.empty() ) str << " ";
    str << "Bank: " << nam[0] << nam[1] << nam[2] << nam[3]
	<< " row:" << this->row()
	<< " Len:" << this->total_length() << "/" << this->data_length()
	<< " Next:" << this->offset2next();
    return str.str();
  }

}

