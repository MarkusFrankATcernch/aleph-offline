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
#include <alpha/pept.h>
#include <alpha/pest.h>
#include <alpha/etdi.h>
#include <alpha/peco.h>

/// ETDI table bank: Ecal Tower DIgits NR=0. (RAW)
void alpha::edm4hep_output::event_t::process_pept()  {
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
  auto& ecal = *this->exp.ecal;
  auto* pest = this->data.pest.load<object_table<class pest> >(true);
  auto* peco = this->data.peco.load<object_table<class peco> >(true);
  auto* pept = this->data.pept.load<object_table<class pept> >(true);
  auto* etdi = this->data.etdi.load<object_table<class etdi> >(true);

  for( uint32_t itk=1, siz=pest->size(); itk <= siz; ++itk )  {
    auto*     bnk_pest = pest->row(itk);
    int32_t   ind_etdi = bnk_pest->etDI();
    if( ind_etdi > 0 )  {
      auto*     bnk_peco = peco->row(bnk_pest->peCO());
      auto*     bnk_pept = pept->row(bnk_pest->peCO());
      auto*     bnk_etdi = etdi->row(bnk_pest->etDI());
      int32_t   kstack   = bnk_pest->ksto();
      int32_t   addr     = bnk_etdi->towerLabel();
      int32_t   itheta   = (addr>>16) & 0xFF;
      int32_t   iphi     = (addr>>2)  & 0x1FF;
      double    energy   = _ENE(bnk_pest->ecoR());
      calohit_t hpeco    = this->ecal_hits.at(bnk_pest->peCO()-1);
      Position  pos      = ecal.tower_geometry.tower_center({itheta, iphi, kstack});
      edm4hep::Vector3f center( pos.x(), pos.y(), pos.z() );

      /// Get the sub-hits for stacks 1/2 and stack 3
      if( bnk_peco->esta1() > limits::small_number )  {
        edm4hep::MutableCluster hit1 = this->ecal_hits_pept.create();
        hit1.setType(     1 );
        hit1.setIPhi(     bnk_pept->phi12() );
        hit1.setITheta(   bnk_pept->testa12() );
        hit1.setPosition( center );
        hit1.setEnergy(   bnk_peco->esta1()*energy );
        hit1.setEnergyError( _ENE(ecal.pad_energy_error(bnk_peco->esta2()*energy)) );
        hpeco.addToClusters(  hit1 );
      }
      if( bnk_peco->esta2() > limits::small_number )  {
        edm4hep::MutableCluster hit2 = this->ecal_hits_pept.create();
        hit2.setType(     2 );
        hit2.setIPhi(     bnk_pept->phi12() );
        hit2.setITheta(   bnk_pept->testa12() );
        hit2.setPosition( center );
        hit2.setEnergy( bnk_peco->esta2()*energy );
        hit2.setEnergyError( _ENE(ecal.pad_energy_error(bnk_peco->esta2()*energy)) );
        hpeco.addToClusters(  hit2 );
      }
      if( (1e0-bnk_peco->esta1()-bnk_peco->esta2()) > limits::small_number )  {
        edm4hep::MutableCluster hit3 = this->ecal_hits_pept.create();
        hit3.setType(     3 );
        hit3.setIPhi(     bnk_pept->phi3() );
        hit3.setITheta(   bnk_pept->testa3() );
        hit3.setPosition( center );
        hit3.setEnergy(   energy );
        energy = (1e0 - bnk_peco->esta1() - bnk_peco->esta2())*bnk_pest->ecoR();
        hit3.setEnergyError( _ENE(ecal.pad_energy_error(energy)) );
        hpeco.addToClusters(  hit3 );
      }
    }
  }
}
