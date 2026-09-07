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

/// C/C++ include files
#include <sstream>
#include <iomanip>
#include <iostream>

extern "C"  {
  extern union bos77::bcs_t    bcs_;
  extern class bos77::sysbos_t sysbos_;
  void nlistb_(char*, int32_t, int32_t*, uint32_t*, const char*, int);
}

/// bos77 namespace declaration
namespace bos77  {
  union bcs_t&       bcs    = bcs_;
  class sysbos_t&    sysbos = sysbos_;
  const std::string& bos_bank_lists = "CERST";
}

/// bos77 namespace declaration
namespace bos77  {

  /// Access to next bank of the same type: Works only if the offset work is correct (Aleph offline)
  const class bank_header* bank_header::next_bank_offset()  const  {
    const void* cptr = (const void*)this;
    if( cptr != nullptr && this->_offset_next != 0 )  {
      auto* ptr = &bcs.iw[_offset_next - bankheader_words];
      return (const bank_header*)ptr;
    }
    return nullptr;
  }

  /// Access BOS common block with offset
  int32_t* absolute_offset( std::size_t offset )  {
    return bcs.iw + offset;
  }

  /// Resolve hashed name index of the bank
  int32_t namind(const char* bank)   {
    // Offset here call to iw[namind(xxx))] which starts in F77 with 1
    return namind_(bank,4);
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

  /// Get bank instance 'num' of bank type 'bnam'
  int32_t nlinc(int32_t nami, int32_t num)  {
    return ::nlinc_(nami, num);
  }

  /// Access bank names in bank list
  std::string nlistb(uint32_t i, char list)  {
    char ret[32];
    i = i + 1;
    ::nlistb_(ret, sizeof(ret), bcs.iw, &i, &list, 1);
    if( ret[0] == ' ' )  return {};
    return std::string(ret, ret+4);
  }

  /// Access bank from BOS common by index: Get bank instance 'num' of bank name identifier
  class bank* get_bank(int32_t nami, int32_t num)  {
    int32_t off = ::nlinc_(nami, num);
    if( off )  {
      off -= bos77::bankheader_words;
      class bank* bank = (class bank*)(bcs.iw + off);
      verify_bank_type(bank, nami);
      return bank;
    }
    return nullptr;
  }

  /// Get bank instance 'num' of bank type 'bnam'
  class bank* get_bank(const char* bnam, int32_t num)  {
    int32_t off = bnam ? ::nlink_(bnam, num, 4) : 0;
    if( off )  {
      off -= bos77::bankheader_words;
      class bank* bank = (class bank*)(bcs.iw + off);
      verify_bank_type(bank, bnam);
      return bank;
    }
    return nullptr;
  }

  /// Get bank instance 'num' of bank type 'bnam'
  class bank* get_bank(const std::string& bnam, int32_t num)  {
    return ::bos77::get_bank(bnam.c_str(), num);
  }

  /// Access BOS bank com BOS common by hashed index
  class bank* get_bank_pointer_from_namind( int32_t name_index )  {
    if( name_index > 0 )  {
      int32_t knami = bcs.iw[name_index-1];
      if( knami != 0 )  {
        int32_t* ptr = bcs.iw + knami - bos77::bankheader_words;
        class bank* bank = (class bank*)ptr;
        verify_bank_type(bank, name_index);
        return bank;
      }
    }
    return nullptr;
  }

  /// Access BOS bank com BOS common by name
  bank* get_bank_pointer_from_name( const char* bank )  {
    auto indx = ::bos77::namind(bank);
    return ::bos77::get_bank_pointer_from_namind(indx);
  }

  /// As a temporary measure chack if we really got the bank in question
  bool verify_bank_type(const class bank_header* hdr, int32_t name_index)  {
    if( hdr )  {
      int32_t nami = bos77::namind((const char*)&hdr->_name);
      if( nami != name_index )  {
        throw std::runtime_error( "verify_bank_type: Failed to access proper bank by index" );
      }
      return true;
    }
    throw std::runtime_error( "verify_bank_type: Invalid bank!" );
  }

  /// As a temporary measure chack if we really got the bank in question
  bool verify_bank_type(const class bank_header* hdr, const char* name)  {
    if( hdr )  {
      const char* b = (const char*)&hdr->_name;
      if( b[0] != name[0] || b[1] != name[1] || b[2] != name[2] || b[3] != name[3] )  {
        throw std::runtime_error( "verify_bank_type: Failed to access proper bank by index" );
      }
      return true;
    }
    throw std::runtime_error( "verify_bank_type: Invalid bank!" );
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
    auto total_mem = print_banks_of_type(namind(bnam.c_str()));
    if( total_mem == 0 )  {
      std::cout << "BOS Bank: " << bnam << " No banks found!" << std::endl;
    }
    return total_mem;
  }

  /// Print all banks identified by resolved bank name
  std::size_t print_banks_of_type(int32_t name_index)  {
    std::size_t total_mem = 0;
    for( const auto* b=get_bank(name_index, 0); b != nullptr; b=b->knext() )  {
      if( !b )
        break;
      total_mem += b->total_num_words();
      std::cout << to_string(b) << std::endl;
    }
    return total_mem;
  }

  /// String representation for printouts
  std::string to_string(const class bank_header* data, const std::string& prefix)  {
    std::stringstream str;
    const auto* bnk = (const class bank*)data;

    str << prefix;
    if( !prefix.empty() ) str << " ";

    if( bnk )  {
      const auto* nam = (char*)&data->_name;
      const auto* nb  = data->next_bank_offset();
      std::size_t nw  = bnk->total_num_words();
      std::size_t pl  = bnk->payload_columns()*bnk->payload_rows();

      str << "Bank: " << nam[0] << nam[1] << nam[2] << nam[3]
          << " row:"  << std::setw(6) << std::left  << data->row()
          << " Len:"  << std::setw(5) << std::right << data->total_length()
          << "/"      << std::setw(5) << std::left  << data->data_length();
      if( nw >= 2 && pl+subheader_words == nw )  {
        str << " Words/row:" << std::setw(4) << std::right << bnk->payload_columns()
            << " #row:"      << std::setw(4) << std::right << bnk->payload_rows();
      }
      if( nb )  {
        str << " KNext:" << std::setw(8) << data->offset2next()
            << "/"       << std::setw(4) << std::left
            << (const char*)(nb ? nb->name().c_str() : " ");
      }
    }
    else  {
      str << "[Invalid bank reference]";
    }
    return str.str();
  }

  /// String representation for printouts
  std::string to_string(const class bank* data, const std::string& prefix)  {
    return to_string((const class bank_header*)data, prefix);
  }

  /// String representation for printouts
  std::string to_string(const class format* data, const std::string& prefix)  {
    return to_string((const class bank_header*)data, prefix);
  }
}
