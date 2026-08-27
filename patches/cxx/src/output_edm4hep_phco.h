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
#include <alpha/phco.h>

/// Create HCAL cluster from PHCO row
void alpha::output_edm4hep::event_t::process_phco()  {
  /*
                                        Subschema: HobjPOTBanks                 
   +------+                                                                       
   | PHCO |  Hadron Calorimeter Object                                            
   +------+  (PHOB and PHTO are replaced
             by PHCO )
   ..............................................................
       1          I    Number of words/calobject (=9)
       2          I    Number of calobjects
   ..............................................................
        1    ER  F    ERaw             [0.0,300.00]
                         Raw Energy (Gev)
        2    TH  F    THeta            [0.0,3.142]
                         Theta
        3    PH  F    PHi              [0.0,6.284]
                         Phi
        4    EC  F    ECorr            [0.0,300.00]
                         Energy (Gev) corrected for geometrical
                         losses
                         ( cracks , spacers and coil )
        5    KD  I    KDrg             [0,255]
                         Region code =1 in overlap region
                         = 0 otherwise
        6    CC  I    CCode            [0,255]
                         Correction code
                         (not filled in JULIA 224)
        7    RB  I    RBits            [0,255]
                         Relation bits
                         RBITS =
                         0 : no relations
                         1 : charged track relation
                         2 : ECAL relation
                         3 : Both ch tk and EC relation
        8    NF  I    NoiseFlag        [0,64]
                         Noise Flag from Topological Analysis
                         and Digital Pattern matching
                         bits 0-4 Top. An. (0=OK,1=Noise)
                         bit 5 Dig. Matching (0=OK,1=Noise)
        9    PC  I    PCOB
                         CalObject number
  */
  std::stringstream log;
  bool  dbg = this->data.phco.debug;
  auto* tab = this->data.phco.load<object_table<class phco> >();
  if( dbg )  {
    log << bos77::to_string(tab) << std::endl;
  }
  for( uint32_t itk=1, siz=tab->size(); itk <= siz; ++itk )  {
    auto          hit  = this->hits_hcal.create();
    auto*         ah   = this->data.phco.row<class phco>(itk);
    uint64_t      cell = ah->pcOB();
    PositionPolar pos(1e0, ah->theta(), ah->phi()); // HCAL Radius unknown from bank

    hit.setCellID( cell );
    hit.setTime( 0e0 );
    hit.setPosition( { pos.x(), pos.y(), pos.z() } );
    hit.setEnergy(ah->ecorr());
    hit.setEnergyError( std::abs(ah->ecorr() - ah->eraw()) );
    hit.setType(((  0x7 & ah->rbits())) +
		((  0x7 & ah->ccode()) <<  3) +
		((  0x7 & ah->noiseFlag())  <<  6));

    if( dbg )  {
      char text[512];
      ::snprintf(text, sizeof(text),
                 "  PHCO %3d %8lX Energy:%7.3f Corr:%7.3f Theta: %4.2f Phi: %4.2f"
                 " Region:%3d CC:%2d Rel:%2d CalObjID:%5d",
                 itk, uint64_t(ah), ah->eraw(), ah->ecorr(), ah->theta(), ah->phi(),
                 ah->kdrg(), ah->ccode(), ah->rbits(), ah->pcOB());
      log << text << std::endl;
    }
  }
  if( dbg )  {
    ::printf("%s\n", log.str().c_str());
  }
}

