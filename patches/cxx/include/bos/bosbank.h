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
#ifndef ALEPH_BOS_BANK_H
#define ALEPH_BOS_BANK_H

/// Framework include files
#include <cstdint>
#include <string>

extern "C"  {
  void    bosta_();
  int32_t namind_(const char* bname, int);
  int32_t nlink_(const char* bname, int& which, int);
}

namespace bos77  {

  /// Forward declarations
  class bank;
  class record;

    
  constexpr static const int32_t bankheader_words = 4;
  constexpr static const int32_t subheader_words = 2;
  constexpr static const int32_t index_payload_columns = 1;
  constexpr static const int32_t index_payload_rows = 2;

    
  /// Definition of the BOS event record
  /**
   *   \author M.Frank
   *   \version 1.0
   */
  class record  {
  public:
    int32_t  _num_bytes;
    int32_t  _num_words;
      
  public:
    /// Default constructor
    record() = default;
      
    /// Total bank length in bytes
    std::size_t total_length()  const  {
      return sizeof(record) + this->_num_bytes;
    }

    /// Data length in bytes
    std::size_t data_length()  const  {
      return this->_num_bytes;
    }

    /// Offset to next record
    int32_t offset2next()  const  {
      return this->total_length();
    }

    /// Move to the next record
    const record* next()  const  {
      const uint8_t* ptr = (const uint8_t*)this;
      return (record*)(ptr + this->total_length());
    }

    /// Bank iteration on the record: start
    const bank* begin()  const  {
      const uint8_t* ptr = (const uint8_t*)this;
      return (const bank*)(ptr + sizeof(record));
    }

    /// Bank iteration on the record: end marker
    const bank* end()  const  {
      const uint8_t* ptr = (const uint8_t*)this;
      return (const bank*)(ptr + this->total_length() - sizeof(record));
    }

    /// String represntation for printouts
    std::string to_string()  const;
  };


  /// Definition of the event in the bos structure
  /**
   *   \author M.Frank
   *   \version 1.0
   */
  class event  {
  public:
    /// Default constructor
    event() = delete;
    /// Default destructor
    ~event() = delete;
  };
    

  /// Definition of the BOS bank header
  /**
   *   \author M.Frank
   *   \version 1.0
   */
  class bank_header  {
  public:
    int32_t  _name         { 0 };
    int32_t  _row          { 0 };
    int32_t  _offset_next  { 0 };
    int32_t  _num_words    { 0 };

  public:
    /// Default constructor inhibited: object lives in the BOS common block
    bank_header() = delete;
    /// Default destructor inhibited: object lives in the BOS common block
    ~bank_header() = delete;

    /// Access bank name is 4 characters
    const std::string name()  const  {
      const char* nam = (const char*)&this->_name;
      return std::string(nam, nam+sizeof(this->_name));
    }

    /// Bank name as unique type identifier
    int32_t id()  const  {
      return this->_name;
    }

    /// Bank row number
    int32_t row()  const  {
      return this->_row;
    }

    /// Data length in uint32 words
    std::size_t total_num_words()  const  {
      return this->_num_words;
    }

    /// Total bank length in bytes
    std::size_t total_length()  const  {
      return sizeof(bank_header) + this->_num_words*sizeof(uint32_t);
    }

    /// Data payload length in bytes
    std::size_t data_length()  const  {
      return this->_num_words*sizeof(uint32_t);
    }

    /// Data payload length in bytes
    std::size_t data_num_words()  const  {
      return this->_num_words;
    }

    /// Offset to next bank (in bytes)
    int32_t offset2next()  const  {
      return this->_offset_next*sizeof(int32_t);
    }

    /// Access to next bank of the same type: Works only if the offset work is correct (Aleph offline)
    const bank_header* next_bank_offset()   const;
  };

  /// Definition of the BOS bank
  /**
   *   \author M.Frank
   *   \version 1.0
   */
  class bank : public bank_header {

  protected:
    /// Default constructor inhibited: object lives in the BOS common block
    bank() = delete;
    /// Default destructor inhibited: object lives in the BOS common block
    ~bank() = delete;

  public:

    /// Pointer to next bank: Works only if banks are contiguous (Aleph online)
    const bank* next()  const  {
      const uint8_t* ptr = (const uint8_t*)this;
      return (const bank*)(ptr + this->total_length());
    }

    /// Pointer to next bank: Works only if banks are contiguous (Aleph online)
    template <typename T> const T* next()  const  {
      const uint8_t* ptr = (const uint8_t*)this;
      return (const T*)(ptr + this->total_length());
    }
      
    /// Access to next bank of the same type: Works only if the offset work is correct (Aleph offline)
    template <typename T=bank> const T* knext()  const  {
      return (T*)this->bank_header::next_bank_offset();
    }
    
    const bank* end()  const  {
      const uint8_t* ptr = (const uint8_t*)this;
      return (const bank*)(ptr + this->total_length() - sizeof(uint32_t));
    }
      
    /// Access to the data array
    const uint8_t* data()  const  {
      const uint8_t* ptr = ((const uint8_t*)this) + sizeof(bank);
      return ptr;
    }

    /// Access to typed data 
    template <typename T> const T* data()  const  {
      const uint8_t* ptr = ((const uint8_t*)this) + sizeof(bank);
      return (const T*)ptr;
    }

    /// Access to the data array
    const uint8_t* data(std::size_t offset)  const  {
      const uint8_t* ptr = ((const uint8_t*)this) + sizeof(bank) + offset;
      return ptr;
    }

    /// Access to typed data 
    template <typename T> const T* data(std::size_t offset)  const  {
      const uint8_t* ptr = ((const uint8_t*)this) + sizeof(bank) + offset;
      return (const T*)ptr;
    }

    /// String representation for printouts
    std::string to_string(const std::string& prefix="BOS")  const;

    /// Number of columns defined in the sub-header
    int32_t payload_columns()   const   {
      const int32_t* iptr = (const int32_t*)this;
      return iptr[4];
    }
    /// Number of rows defined in the sub-header
    int32_t payload_rows()   const   {
      const int32_t* iptr = (const int32_t*)this;
      return iptr[5];
    }
  };

  /// Definition of the BOS bank "+FMT"
  /**
   *   \author M.Frank
   *   \version 1.0
   */
  class format : public bank  {
  public:
    char _bname[4];

  public:
    /// Access bank name
    std::string bank_name()  const  {
      return std::string(this->_bname, this->_bname+4);
    }
    /// String represntation for printouts
    std::string to_string()  const;
  };

  /// Number of 40byte header words
  inline int32_t header_words()   {
    return bos77::bankheader_words;
  }

  /// Access BOS common with abolute offset
  int32_t* absolute_offset( std::size_t offset );
 
  /// Resolve hashed name index of the bank
  int32_t     namind(const std::string& bname);
  /// Resolve hashed name index of the bank
  int32_t     namind(const char* bank);
  /// Link to bank data offset by index
  int32_t     nlink(const std::string& bank, int which);
  /// Link to bank data offset by index
  int32_t     nlink(const char* bank, int which);
    
  /// Access bank names in bank list
  std::string nlistb(uint32_t index, char list);
  /// Access bank names in bank list
  std::string nlist(uint32_t index, const std::string& list);
  /// Access bank names in bank list
  std::string nlist(uint32_t index, const char* list);

  /// Access bank from BOS common by index: Get bank instance 'num' of bank type 'bnam'
  bank*       get_bank(const std::string& bank, int which);
  /// Access bank from BOS common by index: Get bank instance 'num' of bank type 'bnam'
  bank*       get_bank(const char* bank, int which);

  /// Access BOS bank com BOS common by name
  int32_t*    get_bank_pointer_from_name( const char* name );

  /// Access BOS bank com BOS common by hashed index
  int32_t*    get_bank_pointer_from_namind( int32_t nami );


  /// Print bank names of a single bank list
  void        print_bank_list(char list);
  /// Print bank names of all known BOS bank lists
  void        print_bank_lists(const char* lists);
  /// Print bank names of all known BOS bank lists
  void        print_bank_lists(const std::string& lists);

  /// Print all banks identified by 'bnam'
  std::size_t print_banks_of_type(const std::string& bnam);
    
  /// Definition of the BOS system data structure from inc/sysbos.h
  /**
   *   \author M.Frank
   *   \version 1.0
   */
  struct sysbos_t  {
    int32_t  NSYST;
    int32_t  NAMES;
    int32_t  NPRIM;
    int32_t  IDNAM;
    int32_t  IDPTR;
    int32_t  IDFMT;
    int32_t  NDUMM;
    int32_t  NRESR;
    int32_t  NLPLM;
    int32_t  NARR;
    int32_t  IARR[10];
    int32_t  IEFMT;
    int32_t  TLEFT;
    int32_t  LEPIO;
    int32_t  NAMI;
    int32_t  INDI;
    int32_t  INDJ;
    int32_t  IBC;
    int32_t  DUMMI[73];
    int32_t  INTA[200];
    int32_t  NPTR;
    int32_t  NRUN;
    int32_t  NEVT;
    int32_t  LUNDAT;
    int32_t  LUNSEL;
    int32_t  LUNSE2;
    int32_t  LUTDAT;
    int32_t  MASKR;
    int32_t  LMASK;
    int32_t  NRE;
    int32_t  NAMERE[3];
    int32_t  NUMMRE[3];
    int32_t  IRUNRE[3];
    int32_t  IEVTRE[3];
  };


  union bcs_t  {
    int32_t   iw[1000];
    float     rw[1000];
  };


#ifndef HAVE_BOS_EXTERNS
  extern bcs_t&       bcs;
  extern sysbos_t&    sysbos;
  extern const std::string& bos_bank_lists;
#endif

} // End namespace bos


#endif // ALEPH_BOS_BANK_H
