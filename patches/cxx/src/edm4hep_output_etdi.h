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
#include <alpha/etdi.h>
#include <alpha/peco.h>
#include <alpha/pest.h>

/// ETDI table bank: Ecal Tower DIgits NR=0. (RAW)
void alpha::edm4hep_output::event_t::process_etdi()  {
  /**
 +------+                              Subschema: EcalRAWBanks
 | ETDI |  Ecal Tower DIgits NR=0.
 +------+  (RAW)

 ..............................................................
     1          I    Number of words/tower (=4)
     2          I    Number of towers
 ..............................................................
      1    TL  I    TowerLabel
                       tower label, see ETHT
                       tower address ( b 2-11=phi, 16-24=teta )
      2    S1  I    Stack1
                       stack 1 content ( keV )
      3    S2  I    Stack2
                       stack 2 content ( keV )
      4    S3  I    Stack3
                       stack 3 content ( keV )
 ..............................................................
  */
  bool  dbg = this->data.phco.debug;
  auto& ecal = *this->exp.ecal;
  auto* pest = this->data.pest.load<object_table<class pest> >(true);
  auto* etdi = this->data.etdi.load<object_table<class etdi> >(true);

  if( dbg )  {
    std::cout << bos77::to_string(pest) << std::endl;
  }
  /// Check PEST bank data. If nothing is saved, extract stack locations
  for( uint32_t itk=1, siz=pest->size(); itk <= siz; ++itk )  {
    auto*    bnk_pest = pest->row(itk);
    int32_t  ind_etdi = bnk_pest->etDI();
    if( ind_etdi > 0 )  { // Now digit error
      int32_t   ind_peco  = bnk_pest->peCO();
      calohit_t peco      = this->ecal_hits.at( ind_peco-1 );
      auto*     bnk_etdi  = etdi->row( ind_etdi );
      int32_t   kstack    = bnk_pest->ksto() - 1;
      float     energy[3] = { float(bnk_etdi->stack1()/1e6),
                              float(bnk_etdi->stack2()/1e6),
                              float(bnk_etdi->stack3()/1e6) };
      int32_t   addr   = bnk_etdi->towerLabel();
      int32_t   itheta = (addr>>16) & 0xFF;
      int32_t   iphi   = (addr>>2)  & 0x1FF;
      auto      center = ecal.tower_geometry.tower_center( {itheta, iphi, kstack+1} );

      edm4hep::MutableCalorimeterHit hit = this->ecal_hits_etdi.create();
      hit.setType(   kstack+1 );
      hit.setEnergy( _ENE(energy[kstack]) );
      hit.setEnergyError( _ENE(ecal.pad_energy_error(energy[kstack])) );
      hit.setPosition( edm4hep::Vector3f( center.x(), center.y(), center.z() ) );
      peco.addToHits( hit );
      if( dbg )  {
        ::printf("+++ ETDI %3d PECO %3d Stack:%1d E: %s+-%s [%s] "
                 "r: %s  rho: %s phi:%7.2f [%7.2f] theta:%7.2f [%7.2f] \n",
                 ind_etdi, ind_peco, kstack, fmt_ene(hit.getEnergy()).c_str(),
                 fmt_ene(hit.getEnergyError()).c_str(), fmt_ene(peco.getEnergy()).c_str(),
                 fmt_len(center.r()).c_str(), fmt_len(center.rho()).c_str(),
                 center.phi(), peco.getIPhi(), center.theta(), peco.getITheta() );
      }
    }
  }
}
