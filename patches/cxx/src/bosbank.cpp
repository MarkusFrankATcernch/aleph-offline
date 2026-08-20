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
#include <iomanip>
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

  /// Access to next bank of the same type: Works only if the offset work is correct (Aleph offline)
  const bank_header* bank_header::next_bank_offset()  const  {
    const void* cptr = (const void*)this;
    if( cptr != nullptr && this->_offset_next != 0 )  {
      auto* ptr = &bcs.iw[_offset_next - bankheader_words];
      return (const bank_header*)ptr;
    }
    return nullptr;
  }
  
  /// String representation for printouts
  std::string bank::to_string(const std::string& prefix)  const  {
    std::stringstream str;
    const auto* nam = (char*)&this->_name;
    const auto* nb  = this->next_bank_offset();
    std::size_t nw  = this->total_num_words();
    std::size_t pl  = this->payload_columns()*this->payload_rows();

    str << prefix;
    if( !prefix.empty() ) str << " ";
    str << "Bank: " << nam[0] << nam[1] << nam[2] << nam[3]
        << " row:"  << std::setw(6) << std::left  << this->row()
        << " Len:"  << std::setw(5) << std::right << this->total_length()
        << "/"      << std::setw(5) << std::left  << this->data_length();
    if( nw > 4 && pl+subheader_words == nw )  {
      str << " Words/row:" << std::setw(4) << std::right << this->payload_columns()
          << " #row:"      << std::setw(4) << std::right << this->payload_rows();
    }
    if( nb )  {
      str << " KNext:" << std::setw(8) << this->offset2next()
          << " "       << std::setw(4) << std::left
          << (const char*)(nb ? nb->name().c_str() : " ");
    }
    return str.str();
  }

  /// String represntation for printouts
  std::string format::to_string()  const  {
    auto res = this->bank::to_string();
    return res;
  }

  int32_t* absolute_offset( std::size_t offset )  {
    return bcs.iw + offset;
  }

  /// Resolve hashed name index of the bank
  int32_t namind(const char* bank)   {
    // Offset here call to iw[namind(xxx))] which starts in F77 with 1
    return namind_(bank,4) - 1;
  }

  /// Resolve hashed name index of the bank
  int32_t namind(const std::string& bank)   {
    return ::bos77::namind(bank.c_str());
  }

  /// Get bank instance 'num' of bank type 'bnam'
  int32_t nlink(const char* bnam, int32_t num)  {
    int32_t off = ::nlink_(bnam, num, 4);
    if( off )  {
      return off - bankheader_words;
    }
    return 0;
  }

  /// Get bank instance 'num' of bank type 'bnam'
  int32_t nlink(const std::string& bnam, int32_t num)  {
    return ::bos77::nlink(bnam.c_str(), num);
  }

  /// Access bank names in bank list
  std::string nlistb(uint32_t i, char list)  {
    char ret[32];
    i = i + 1;
    ::nlistb_(ret, sizeof(ret), bcs.iw, &i, &list, 1);
    if( ret[0] == ' ' )  return {};
    return std::string(ret, ret+4);
  }
  
  /// Get bank instance 'num' of bank type 'bnam'
  bank* get_bank(const char* bnam, int32_t num)  {
    int32_t off = bnam ? ::nlink_(bnam, num, 4) : 0;
    if( off )  {
      off -= bos77::bankheader_words;
      return (bank*)(bcs.iw + off);
    }
    return nullptr;
  }

  /// Get bank instance 'num' of bank type 'bnam'
  bank* get_bank(const std::string& bnam, int32_t num)  {
    return ::bos77::get_bank(bnam.c_str(), num);
  }

  /// Access BOS bank com BOS common by hashed index
  int32_t* get_bank_pointer_from_namind( int32_t nami )  {
    if( nami > 0 )  {
      int32_t knami = bcs.iw[nami];
      if( knami != 0 )  {
        int32_t* bank = bcs.iw + knami - bankheader_words;
        return bank;
      }
    }
    return nullptr;
  }

  /// Access BOS bank com BOS common by name
  int32_t* get_bank_pointer_from_name( const char* bank )  {
    auto indx = ::bos77::namind(bank);
    return ::bos77::get_bank_pointer_from_namind(indx);
  }
  
  /// Print bank names of all known BOS bank lists
  void print_bank_lists( const char* blist )  {
    if( blist )  {
      for( const char* c=blist; *c; ++c)  {
        std::cout << "++++++ Banks of list: '" << *c << "'" << std::endl;
        for(int i=0; ; ++i )  {
          std::string bnam = nlistb(i, *c);
          if( bnam.empty() ) break;
          std::cout << bnam << " ";
          if( ((i+1)%20) == 0 ) std::cout << std::endl;
        }
        std::cout << std::endl;
      }
      return;
    }
    throw std::runtime_error("print_bank_lists: Invalid bank list (nullptr)");
  }
  
  /// Print bank names of all known BOS bank lists
  void print_bank_lists(const std::string& lists)  {
    print_bank_lists(lists.c_str());
  }

  /// Print all banks identified by 'bnam'
  std::size_t print_banks_of_type(const std::string& bnam)  {
    std::size_t total_mem = 0;
    int i = 0;
    const auto* bank = get_bank(bnam, 0);
    for( ; bank != nullptr; ++i )  {
      if( !bank )  {
        break;
      }
      const auto* next  = bank->next();
      const auto* knext = bank->knext();
      total_mem += bank->total_num_words();
      ::printf("%4s: %-4d BANK: %-8ld %p  %-60s ",
               bnam.c_str(), i, ((uint8_t*)bank) - (uint8_t*)bcs.iw,
               (void*)bank, bank->to_string().c_str());
      
      if( next )  {
        ::printf("NEXT: %-8ld %p %4s ",
                 ((uint8_t*)next)   - (uint8_t*)bcs.iw,  (void*)next,  next  ? next->name().c_str() : "");
      }
      ::printf("\n");
      bank = knext;
    }
    return total_mem;
  }

}
