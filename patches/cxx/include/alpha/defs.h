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
#ifndef ALPHA_BOS_DEFS_H
#define ALPHA_BOS_DEFS_H

/// C/C++ include files
#include <cstdint>
#include <stdexcept>

/// Framework include files
#include <bos/bosbank.h>

/// ALPHA namespace declaration
namespace alpha  {

  template<typename T> const T* bcs_offset(uint32_t offset)  {
    if( offset > 0 )  {
      return (T*)bos77::absolute_offset(offset);
    }
    return nullptr;
  }

  class qlin  {
  public:
    int32_t  link;
  };
  
  /// Track data for direct use in ALPHA
  /*
   *
   *   \author M.Frank
   *   \version 1.0
   */
  template <typename T> struct bos_wrap_data  {
  public:
    using data_t = T;
  public:
    data_t*               wrap  { nullptr };
  public:
    bos_wrap_data(T* p) : wrap(p) {}
    /// Default destructor
    ~bos_wrap_data();
    data_t* operator->()             {  return wrap; }
    const data_t* operator->() const {  return wrap; }
  };
  
  /// Aleph bos bank: standard bos header followed by object table
  /*
   *   Object table is defined by the
   *   -  number of words per object (num_words) and
   *   -  the number of objects in the table (num_objects)
   *   \author M.Frank
   *   \version 1.0
   */
  template <typename POINTER, typename VALUE=POINTER*>
  class object_table : public bos77::bank_header  {
  public:
    using const_pointer_type = const POINTER*;
    using pointer_type       = POINTER*;
    using value_type         = VALUE;
    using table_type         = object_table<POINTER, VALUE>;

    /// Payload table: Number of words per object (columns)
    int32_t  _obj_words    { 0 };
    /// Payload table: Number of objects (rows)
    int32_t  _num_objects  { 0 };

  public:
    int32_t  _row[1];

    bool check_bounds(uint32_t /* idx */)  const {
      return true;//(idx >= 0 && idx <= this->_num_objects) ? true : false;
    }
    int32_t* _access_pointer(uint32_t idx)  const  {
      if ( check_bounds(idx) )  {
        const auto* p = this->_row + (this->_obj_words * idx);
        return (int32_t*)p;
      }
      return nullptr;
    }
    value_type _access_object(int idx)  const  {
      value_type v = { pointer_type(_access_pointer(idx)) };
      return v;
    }

  public:
    /// Default constructor inhibited: object lives in the BOS common block
    object_table() = delete;
    /// Default destructor inhibited: object lives in the BOS common block
    ~object_table() = delete;

    /// Access table by name index [aka NAMIND('QVEC') ]
    static object_table& get(uint32_t nam_indx)  {
      auto* ptr = (object_table*)bos77::get_bank_pointer_from_namind( nam_indx );
      if( ptr )  {
        return ptr;
      }
      throw std::runtime_error( "Failed to access bank from index" );
    }

    /// Access table by name index [aka NAMIND('QVEC') ]
    static object_table& get(const char* name)  {
      auto* ptr = (object_table*)bos77::get_bank_pointer_from_name( name );
      if( ptr )  {
        return *ptr;
      }
      throw std::runtime_error( "Failed to access bank by name: "+std::string(name) );
    }

    /// Access table by name index [aka NAMIND('QVEC') ]
    static object_table* ptr(uint32_t namidx)  {
      return (object_table*)bos77::get_bank_pointer_from_namind( namidx );
    }

    /// Check if the container is empty
    bool     empty()  const                {  return this->_num_objects == 0;    }

    /// Get object index in table (starting from NULL!). -1 if not in table
    int32_t index(const_pointer_type obj)  const;
    /** Direct object access  */
    /// Number of objects
    uint32_t size()  const                    {  return this->_num_objects;      }
    /// Number of words per object
    uint32_t words_per_object()  const        {  return this->_obj_words;        }
    /// Direct access by row to object entry in the table 
    value_type operator[](uint32_t idx)       {  return _access_object(idx);     }
    /// Direct access by row to object entry in the table  (CONST)
    value_type operator[](uint32_t idx) const {  return _access_object(idx);     }
    /// Direct access by row to object entry in the table 
    value_type at(uint32_t idx)               {  return _access_object(idx);     }
    /// Direct access by row to object entry in the table  (CONST)
    const value_type at(uint32_t idx) const   {  return _access_object(idx);     }

    /// Direct access by row to object entry in the table: USE for loops with QCDE ranges
    value_type row(uint32_t idx)              {  return _access_object(idx-1);   }
    /// Direct access by row to object entry in the table  (CONST): USE for loops with QCDE ranges
    const value_type row(uint32_t idx) const  {  return _access_object(idx-1);   }
#if 0
    /** Iterative object access  */
    /// Bank iterator: start iteration
    pointer_type       begin()         {  return (pointer_type*)_table._access_pointer(_first);   }
    /// Bank iterator: start iteration (CONST) 
    const pointer_type begin()  const  {  return (pointer_type*)_table._access_pointer(_first);   }
    /// Bank iterator: iteration stop marker
    pointer_type       end()           {  return (pointer_type*)_table._access_pointer(_last+1);  }
    /// Bank iterator: iteration stop marker (CONST)
    const pointer_type end()  const    {  return (pointer_type*)_table._access_pointer(_last+1);  }
    /// Bank iterator: start iteration
    pointer_type       rbegin()        {  return (pointer_type*)_table._access_pointer(_last);    }
    /// Bank iterator: start iteration (CONST)
    const pointer_type rbegin()  const {  return (pointer_type*)_table._access_pointer(_last);    }
    /// Bank iterator: iteration stop marker
    pointer_type       rend()          {  return (pointer_type*)_table._access_pointer(_first-1); }
    /// Bank iterator: iteration stop marker (CONST)
    const pointer_type rend()  const   {  return (pointer_type*)_table._access_pointer(_first-1); }
#endif

    class iterator  {
      pointer_type  _ptr;
      iterator(pointer_type p) : _ptr(p)      {      }
      iterator& operator++()     {  ++this->_ptr; return *this; }
      iterator& operator++(int)  {  ++this->_ptr; return *this; }
      iterator& operator--()     {  --this->_ptr; return *this; }
      iterator& operator--(int)  {  --this->_ptr; return *this; }
    };
    using iterator_type = iterator;
  };
  
  /// Get object index in table (starting from NULL!). -1 if not in table
  template <typename POINTER, typename VALUE>
  int32_t object_table<POINTER,VALUE>::index(const_pointer_type obj)  const  {
    const int32_t* ptr = (const int32_t*)obj;
    int64_t offset = (ptr - &this->_row[0]);
    int32_t which = offset/this->_obj_words;
    if( (offset%this->_obj_words) != 0 )   // Wrong table pointer. This must be NULL
      return -1;
    else if( which < 0 )                  // negative offset impossible: wrong table pointer
      return -1;
    else if( which > this->_num_objects ) // outside of object table: wrong table pointer
      return -1;
    return which;
  }
}      // End namespace alpha

/// Default destructor
template <typename T> inline alpha::bos_wrap_data<T>::~bos_wrap_data()  {
}

#endif // ALPHA_BOS_DEFS_H
