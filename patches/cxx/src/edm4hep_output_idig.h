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
#include <alpha/idig.h>
#include <alpha/itco.h>

/// Process IDIG ITC digitisings (NR=0)
void alpha::edm4hep_output::event_t::process_idig()  {
  /**

     +------+                               Subschema: ItcPOTBanks
     | IDIG |  Packed Itc DIgitisings (NR=0)
     +------+  (POT)
     ..............................................................
     1          I    Number of words/digit. (=1)
     2          I    Number of digits
     ..............................................................
      1    PD  I    PackedDigit      [*,*]
                       ITC digits. info. packed into
                       a 32 bit word.
                       10 bits wire number - bits 0 - 9,
                       9 bits r-phi TDC - bits 10 - 18,
                       9 bits Z TDC - bits 19 - 27,
                       2 bits quality flag - bits 28 - 29,
                       1 bit ambiguity flag - bit 30,
                       1 bit version flag - bit 31 (not used
                       from 1991 onwards
  */
  auto* tab_itco  = this->data.itco.load<object_table<class itco> >(true);
  std::map<int32_t, std::set<int32_t> > rel_coord_wire;
  std::map<int32_t, std::set<int32_t> > rel_wire_coord;

  for( uint32_t ind_coord=1; ind_coord < tab_itco->size(); ++ind_coord )  {
    int32_t num   = tab_itco->row(ind_coord)->wireNumber();
    int32_t wire  = num / 1000;
    // int32_t layer = num % 1000;
    rel_coord_wire[ind_coord].insert( wire );
    rel_wire_coord[wire].insert( ind_coord-1 );
  }

  /**
   *  Decoding not really obvious. See alephlib/itc/icrcoo.F
   *  ITCO contains 1000*layer + wire
   *  IDIG contains wire only. How to resolve possible ambiguities
   *  when multiple IDIG entries match the coordinate in multiple layers ?
   */
#if 0
  auto* tab_idig  = this->data.idig.load<object_table<class idig> >(true);
  for( uint32_t itk=1; itk <= tab_idig->size(); ++itk )  {
    const idig* ah = tab_idig->row(itk);
    int32_t     digi = ah->packedDigit();
    int32_t     wire = (digi & 0x3FF);
    auto        irel = rel_wire_coord.find(wire);
    
    if( irel != rel_wire_coord.end() )  {
      for( int32_t ind_coord : irel->second )  {
        MutableTrackerHit3D hit = this->hits_itco1.at(ind_coord);
      }
    }
  }
  ++this->data.idig.num>_process;
  #endif
}
