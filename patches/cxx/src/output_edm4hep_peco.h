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


/// Create ECAL hit from PECO row
void alpha::output_edm4hep::event_t::process_peco()  {
  /*
                                        Subschema: EobjPOTBanks                 
   +------+                                                                       
   | PECO |  Electromagnetic Calorimeter                                          
   +------+  Object (PEOB and PEOT are
             replaced by PECO and PEPT )
   ..............................................................
       1          I    Number of words/calobject (=10)
       2          I    Number of calobjects
   ..............................................................
        1    ER  F    ERaw             [0.0,300.000]
                         Raw Energy (Gev)
        2    E1  F    Esta1            [0.0,1.00]
                         Proportion of raw energy in stack 1
                         "Stack 1" is defined properly for the
                         overlap
                         region according to the tower number
        3    E2  F    Esta2            [0.0,1.00]
                         Proportion of raw energy in stack 2
                         "Stack 2" is defined properly for the
                         overlap
                         region according to the tower number
        4    TH  F    THeta            [0.0,3.1416]
                         Theta. The theta, phi of stacks 1 - 3
                         are weighted by the stack energy
        5    PH  F    PHi              [0.0,6.2833]
                         Phi. The theta, phi of stacks 1 - 3 are
                         weighted by the stack energy
        6    EC  F    ECorr            [0.0,300.000]
                         Energy (Gev) corrected for geometrical
                         effects (cracks, detector
                         overlap, dead storeys)
                         Set to ERaw in JULIA 224
        7    KD  I    KDrg             [0,255]
                         Region code ; see note ALEPH # 88-134
                         for a detailed description of this code
        8    CC  I    CCode            [0,255]
                         Correction code for the cluster
                         analysis method;
                         CCode =
                         0 : Standard
                         1 : Crack between modules
                         2 : Overlap between detectors
                         3 : Dead storey
        9    RB  I    RBits            [0,255]
                         Relation bits
                         RBITS =
                         0 : no relations
                         1 : charged track relation
                         2 : HCAL relation
                         3 : Both ch tk and HC relation
        10   PC  I    PCOB
                         CalObject number
  */
  auto* tab = this->data.peco.load<object_table<class peco> >();
  for( uint32_t itk=1, siz=tab->size(); itk <= siz; ++itk )  {
    std::size_t   key  = this->hits_ecal.size();
    auto          hit  = this->hits_ecal.create();
    auto*         ah   = this->data.peco.row<class peco>(itk);
    uint64_t      cell = ah->pcOB();
    PositionPolar pos(1e0, ah->theta(), ah->phi()); // Radius is unknown from bank

    this->alpha2edm4hep_peco[itk] = key;
    hit.setCellID( cell );
    hit.setTime( 0e0 );
    hit.setPosition( { pos.x(), pos.y(), pos.z() } );
    hit.setEnergy( ah->ecorr() );
    hit.setEnergyError( std::abs(ah->ecorr() - ah->eraw()) );
    hit.setPosition( { pos.x(), pos.y(), pos.z() } );
    hit.setType(((  0x7 & ah->rbits())) +
		((  0x7 & ah->ccode()) <<  3) +
		((  0x7 & ah->kdrg())  <<  6) +  // Bit 9 unused.
		((0x3FF & int(ah->esta1()*1000e0)) << 10) +
		((0x3FF & int(ah->esta2()*1000e0)) << 20));
  }
}
