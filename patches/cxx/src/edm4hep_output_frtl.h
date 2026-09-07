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

/// Alpha include files
#include <alpha/frtl.h>
#include <alpha/fvcl.h>
#include <alpha/ficl.h>
#include <alpha/ftcl.h>

/// Process information from FRTL bank
void alpha::edm4hep_output::event_t::process_frtl()  {
  /**      Subschema: JULPOTFitTrack
 +------+
 | FRTL |  Tpc+Itc+Vdet Geometry Track
 +------+  point List. Rows correspond
           with rows in FRFT bank. NR=0.
           (JUL)
 ..............................................................
     1          I    Number of words per track (=8)
     2          I    Number of Tpc+Itc+Vdet
                     geometry tracks
 ..............................................................
      1    IV  I    IoffV            [0,10000]
                       offset in FVCL
      2    NV  I    NarcV            [0,4]
                       numb. coord. in Vdet
      3    II  I    IoffI            [0,10000]
                       offset in FICL
      4    NI  I    NarcI            [0,8]
                       numb. coord. in Itc
      5    NE  I    NrEsti           [0,1000]
                       numb. coord. in following spirals in ITC
      6    IT  I    IoffT            [0,10000]
                       offset in FTCL
      7    NT  I    NarcT            [0,21]
                       numb. coord. in first arc in Tpc
      8    NR  I    NRestt           [0,1000]
                       numb. coord. in following spirals, in Tpc
 ..............................................................

 +------+                            Subschema: JULPOTFitTrack
 | FVCL |  Vdet Geometry track
 +------+  Coordinate List. Use FRTL to
           index into this bank NR=0.
           (JUL)
 ..............................................................
     1          I    Number of words per (=1)
                     coordinate
     2          I    Number of coordinates
                     associated with tracks
 ..............................................................
      1    IV  I    Ivdco            [1,10000]
                       coordinate number in VDCO
 ..............................................................

 +------+                            Subschema: JULPOTFitTrack
 | FICL |  Itc Geometry track Coordinate
 +------+  List. Use FRTL to index into
           this bank NR=0. (JUL)
 ..............................................................
     1          I    Number of words per (=1)
                     coordinate
     2          I    Number of coordinates
                     (signed) associated with
                     tracks (-ve means 2nd Phi hit
                     used)
 ..............................................................
      1    II  I    Iitco            [-10000,10000]
                       coordinate number in ITCO
 ..............................................................

 +------+                            Subschema: JULPOTFitTrack
 | FTCL |  Tpc Geometry track Coordinate
 +------+  List. Use FRTL to index into
           this bank NR=0. (JUL)
 ..............................................................
     1          I    Number of words per (=1)
                     coordinate
     2          I    Number of coordinates
                     associated with tracks
 ..............................................................
      1    IT  I    Itpco            [1,10000]
                       coordinate number in TPCO
 ..............................................................
  */
  bool  dbg = this->data.frtl.debug;
  auto* tab_frtl = this->data.frtl.load<object_table<class frtl> >();
  auto* tab_fvcl = this->data.fvcl.load<object_table<class fvcl> >();
  auto* tab_ficl = this->data.ficl.load<object_table<class ficl> >();
  auto* tab_ftcl = this->data.ftcl.load<object_table<class ftcl> >();
  std::stringstream log;
  if( dbg )  {
    log << bos77::to_string(tab_frtl) << std::endl;
  }

  for( int itk = qcde.KFCHT; itk <= qcde.KLCHT; ++itk )  {
    std::size_t itr    = itk - qcde.KFCHT + 1;
    auto*       frtl   = tab_frtl->row(itr);
    auto        nvdet  = frtl->narcV();
    auto        nitc   = frtl->narcI();
    auto        ntpc   = frtl->narcT();
    const char* tag_vd = nvdet>0 ? " VDCO:" : "";
    const char* tag_it = nitc>0 ? " ITCO:" : "";
    const char* tag_tp = ntpc>0 ? " TPCO:" : "";

    if( dbg && (nvdet+nitc+ntpc)>0 )  {
      log << "  FRTL/FRFT: " << std::setw(2) << itr << " ";
    }
    log << tag_vd;
    for( int32_t i=0; i < nvdet; ++i )  {
      int32_t idx = frtl->ioffV()+i;
      int32_t num = tab_fvcl->at(idx)->ivdco();
      auto    hit = this->hits_vdco[num-1];
      this->link_hit_to_frft_track( itr, hit );
      if( dbg ) log << std::setw(2) << num << " ";
    }

    log << tag_it;
    for( int32_t i=0; i < nitc; ++i )  {
      int32_t idx = frtl->ioffI()+i;
      int32_t num = tab_ficl->at(idx)->iitco();
      // See ITCO bank description for handling of negative indices
      auto    hit = (num > 0) ? this->hits_itco1.at(num-1) : this->hits_itco2.at(-num-1);
      this->link_hit_to_frft_track( itr, hit );
      if( dbg ) log << std::setw(2) << num << " ";
    }

    log << tag_tp;
    for( int32_t i=0; i < ntpc; ++i )  {
      uint32_t idx = frtl->ioffT()+i;
      if( idx >= tab_ftcl->size() )  {
        ::printf("TPC Coordinate %d outside limit: %d\n", idx, tab_ftcl->size());
        continue;
      }
      int32_t num = tab_ftcl->at(idx)->itpco();
      auto    hit = this->hits_tpco[num-1];
      this->link_hit_to_frft_track( itr, hit );
      if( dbg ) log << std::setw(2) << num << " ";
    }
    if( dbg ) log << std::endl;
  }
  if( dbg )  {
    ::printf("%s\n", log.str().c_str());
  }
}
