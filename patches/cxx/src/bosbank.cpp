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
#include <iostream>

extern "C"  {
  extern bos77::bcs_t    bcs_;
  extern bos77::sysbos_t sysbos_;
  //char* nlistb_(int32_t*, uint32_t*, const char*, int);
  void nlistb_(char*, int32_t, int32_t*, uint32_t*, const char*, int);
}

namespace bos77  {
  bcs_t&    bcs    = bcs_;
  sysbos_t& sysbos = sysbos_;
  const std::string& bos_bank_lists = "CERST";
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

  std::string nlistb(uint32_t i, char list)  {
    char ret[32];
    i = i + 1;
    //const char* ret = nlistb_(bcs.iw, &i, &list, 1);
    nlistb_(ret, sizeof(ret), bcs.iw, &i, &list, 1);
    if( ret[0] == ' ' )  return {};
    return std::string(ret, ret+4);
  }
  
  bank* get_bank(const char* bnam, int32_t num)  {
    return get_bank(std::string(bnam), num);
  }
  bank* get_bank(const std::string& bnam, int32_t num)  {
    int32_t off = nlink_(bnam.c_str(), num, 4);
    if( off )  {
      off -= bos77::bankheader_words;
      return (bank*)(bcs.iw + off);
    }
    return nullptr;
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
    const char* blist = lists.c_str();
    for( const char* c=blist; *c; ++c)  {
      int i=0;
      std::cout << "++++++ Banks of list: '" << *c << "'" << std::endl;
      for(int i=0; ; ++i )  {
	std::string bnam = nlistb(i, *c);
	if( bnam.empty() ) break;
	std::cout << bnam << " ";
	if( ((i+1)%20) == 0 ) std::cout << std::endl;
      }
      std::cout << std::endl;
    }
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

