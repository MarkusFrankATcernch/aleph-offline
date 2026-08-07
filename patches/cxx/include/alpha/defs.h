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
    using pointer_type = POINTER*;
    using value_type   = VALUE;
    using table_type   = object_table<POINTER, VALUE>;

    /// Payload table: Number of words per object (columns)
    int32_t  _obj_words    { 0 };
    /// Payload table: Number of objects (rows)
    int32_t  _num_objects  { 0 };

  public:
    int32_t  row[1];

    bool check_bounds(int idx)  const {
      return true;//(idx >= 0 && idx <= this->_num_objects) ? true : false;
    }
    int32_t* _access_pointer(int idx)  const  {
      if ( check_bounds(idx) )  {
        const auto* p = row + (this->_obj_words * (idx-1));
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
    static object_table& get(uint32_t naqvec)  {
      return *( object_table* ) bos77::get_bank_pointer_from_namind( naqvec );
    }

    /// Access table by name index [aka NAMIND('QVEC') ]
    static object_table& get(const char* name)  {
      return *( object_table* ) bos77::get_bank_pointer_from_name( name );
    }

    /// Check if the container is empty
    bool     empty()  const                {  return this->_num_objects == 0;    }

    /** Direct object access  */
    /// Number of objects
    uint32_t size()  const                 {  return this->_num_objects;         }
    /// Direct access by row to object entry in the table 
    value_type operator[](int idx)         {  return _access_object(idx);        }
    /// Direct access by row to object entry in the table  (CONST)
    value_type operator[](int idx) const   {  return _access_object(idx);        }
    /// Direct access by row to object entry in the table 
    value_type at(int idx)                 {  return _access_object(idx);        }
    /// Direct access by row to object entry in the table  (CONST)
    const value_type at(int idx) const     {  return _access_object(idx);        }
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
  
  inline char true_false(bool value)  {  return value ? 'T' : 'F';  }

  class qvec;
  class qvrt;
  class qdet;
  class peco;
  class phco;
  class pgac;
  class pcqa;
  class pdlt;
  class pmlt;

  struct constants_t   {
    int32_t naqzer;
    int32_t kqzer;

    int32_t naqvec;
    int32_t naqvrt;
    int32_t naqdet;
    int32_t naqlin;
    int32_t napeco;
    int32_t naphco;
    int32_t napgac;
    int32_t napcqa;
    int32_t napmdt;
    int32_t napdlt;
    int32_t napmlt;

    int32_t kmatix[6][6];
    
    template <typename T> const T* table(int32_t offset)  const {
      return (T*)(&bos77::bcs.iw[0] + offset - bos77::bankheader_words);
    }

    const object_table<class qvec>* qvec_table;
    const object_table<class qvrt>* qvrt_table;
    const object_table<class qdet>* qdet_table;
    const object_table<class qlin>* qlin_table;
    const object_table<class peco>* peco_table;
    const object_table<class phco>* phco_table;
    const object_table<class pgac>* pgac_table;
    const object_table<class pcqa>* pcqa_table;
    const object_table<class pdlt>* pdlt_table;
    const object_table<class pmlt>* pmlt_table;
    
  };
  extern constants_t&  params;
}      // End namespace alpha

/// Default destructor
template <typename T> inline alpha::bos_wrap_data<T>::~bos_wrap_data()  {
}

#endif // ALPHA_BOS_DEFS_H
