//==========================================================================
//  ALEPH software suite
//--------------------------------------------------------------------------
//  Copyright (C) Organisation europeenne pour la Recherche nucleaire (CERN)
//  All rights reserved.
//
//  For the licensing terms see OnlineSys/LICENSE.
//
//--------------------------------------------------------------------------
//
//  Author     : Markus Frank
//==========================================================================
#ifndef ALPHA_ALPHA_ALPHA_H
#define ALPHA_ALPHA_ALPHA_H

/// C/C++ include files
#include <cstdint>

/// Framework include files
#include <alpha/defs.h>

/// ALPHA namespace declaration
namespace alpha  {

  /// Forward declarations of the most imnportant BOS banks
  class qcde;
  class qvec;
  class qvrt;
  class qdet;
  class peco;
  class phco;
  class pgac;
  class pcqa;
  class pdlt;
  class pmlt;
  class efol;
  class frft;

  /// Helper class to access BOS data structures from ALEPH
  /**
   *    \author  M.Frank
   *    \date    01/08/2026
   */
  class bank_access_t  {
  protected:
    /// Access BOS bank from BOS common by hashed index (NR=0)
    int32_t* get_bank_first();
    /// Access next BOS bank from BOS common as indicted in the bank header
    int32_t* get_bank_next();

  public:
    int32_t  nami  {       0 };
    int32_t  kq    {       0 };
    int32_t* data  { nullptr };
    bool     debug {   false };

    /// Load bank for event: must be called for EVERY event. Implicit call to NLINKC.
    template<typename T=int32_t> T* load(bool throw_if_not=true)  {
      if( 0 != nami )  {
        this->data = this->get_bank_first();
        return (T*)this->data;
      }
      if( throw_if_not )  {
        throw std::runtime_error("Error: BOS bank access failed NAMI not resolved!");
      }
      return (T*)this->data;
    }

    /// Load bank for event: must be called for EVERY event. Implicit call to NLINKC.
    template<typename T=int32_t> T* knext()  {
      return (T*)this->get_bank_next();
    }

    /// Access BOS bank as partitioned object table
    template<typename T=int32_t> class object_table<T>* table()  const  {
      return (object_table<T>*)(this->data);
    }

    /// Access single row from object table. NOTE: C type: offsets start with 0!!!
    template<typename T> T* at( int32_t idx )  {
      auto* table = this->table<T>();
      if( idx < 0 || idx >= int32_t(table->size()) )  {
        throw std::runtime_error("Error: BOS table: Index out of range!");
      }
      T* row = table->at(idx);
      if( row )  {
        return row;
      }
      throw std::runtime_error("Error: non existing cluster in table!");
    }

    /// Access single row from object table. NOTE: FORTRAN type: offsets start with 1!!!
    template<typename T> T* row( int32_t idx )  {
      auto* table = this->table<T>();
      if( idx < 0 || idx > int32_t(table->size()) )  {
        throw std::runtime_error("Error: BOS table: Index out of range!");
      }
      T* row = table->row(idx);
      if( row )  {
        return row;
      }
      throw std::runtime_error("Error: non existing cluster in table!");
    }
  };

  std::string fmt_ene(float v);
  std::string fmt_len(float v);

  struct constants_t   {
    int32_t current_event_number { -1 };
    class bank_access_t zero;
    class bank_access_t bqvec;
    class bank_access_t bqvrt;
    class bank_access_t bqdet;

    int32_t naqzer   { 0 };
    int32_t kqzer    { 0 };

    int32_t naqvec   { 0 };
    int32_t naqvrt   { 0 };
    int32_t naqdet   { 0 };
    int32_t naqlin   { 0 };

    int32_t nafrft   { 0 };
    int32_t napeco   { 0 };
    int32_t naphco   { 0 };
    int32_t napgac   { 0 };
    int32_t napcqa   { 0 };

    int32_t napdlt   { 0 };
    int32_t napmdt   { 0 };
    int32_t napmlt   { 0 };
    int32_t naefol   { 0 };

    int32_t kmatix[6][6];

    template <typename T> const T* table(int32_t offset)  const {
      if( offset != 0 )  {
        T* ret = (T*)(bos77::bcs.iw + offset - bos77::bankheader_words);
        return ret;
      }
      return nullptr;
    }

    const class object_table<class qvec>* qvec_table   { nullptr };
    const class object_table<class qvrt>* qvrt_table   { nullptr };
    const class object_table<class qdet>* qdet_table   { nullptr };
    const class object_table<class qlin>* qlin_table   { nullptr };

    const class object_table<class frft>* frft_table   { nullptr };
    const class object_table<class efol>* efol_table   { nullptr };

    const class object_table<class peco>* peco_table   { nullptr };
    const class object_table<class phco>* phco_table   { nullptr };

    const class object_table<class pgac>* pgac_table   { nullptr };
    const class object_table<class pcqa>* pcqa_table   { nullptr };

    const class object_table<class pdlt>* pdlt_table   { nullptr };
    const class object_table<class pmlt>* pmdt_table   { nullptr };
    const class object_table<class pmlt>* pmlt_table   { nullptr };

  };
  extern constants_t&  params;

  template<typename T> std::string to_string(const T& val, const char* fmt)  {
    char text[256];
    ::snprintf(text, sizeof(text), fmt, val);
    return { text };
  }
  // trim from both ends of string (right then left)
  std::string& _trim(std::string& s);

  /// Bank listing of all BOS lists
  void aublis(const char list);
  /// Bank listing of all BOS lists
  void aublis(const std::string& list);
  /// Stringify hexadecimal address
  std::string hex_addr(const void* addr, const char* fmt=nullptr);
  /// Convenience helper
  inline char true_false(bool value)     {  return value ? 'T' : 'F';      }
  /// Convenience helper
  inline const char* yes_no(bool value)  {  return value ? "YES" : "NO ";  }


  /// Initialize parameters (NAMIND etc)
  void init_params();
  /// Initialize event parameters (bank locations) set table pointers
  void init_event();
  /// Access QVEC table. If not initialized, initialize all necessary pointers
  const object_table<class qvec>* get_qvec();
  /// Access QVEC table. If not initialized, initialize all necessary pointers
  const object_table<class qdet>* get_qdet();

}      // End namespace alpha
#endif // ALPHA_ALPHA_ALPHA_H
