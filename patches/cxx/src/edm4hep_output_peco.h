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
#include <alpha/peco.h>
#include <alpha/pest.h>
#include <alpha/pept.h>
#include <alpha/etdi.h>

/// Create ECAL hit from PECO row
void alpha::edm4hep_output::event_t::process_peco()  {
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


      PCRL (Calobject ReLations: PCOB, PECO, PFRF, PHCO, PPOB)

      -- ECAL
      PEHY (Electromagnetic calobject HYpothesis)                   --> PECO #)
      PEMH (Relation between ECAL and LCAL objects and FKIN tracks  --> PECI #)
      PEST (Pot Electromagnetic calorimeter SToreys                 --> PECO #)
      EGPC (Photons from GAMPEC                                     --> PECO #)
      PEPT (Theta/Phi of stack1/2 and stack3                   parallel PECO  )
      PHER (ECAL-HCAL relations : (PECO-PHCO) )
      EGPR (Link EGPC to PEST (PECO row #) )

      PECO ---> PCOB
      PCHY (Calobject HYpothesis                                   ---> PCOB #)

      IADD = ITABL(KETDI,ITDI,JETDTL)
      TJ = IBITS(IADD,16,8)
      FI = IBITS(IADD,2,9)
      KS = ITABL(KPEST,ISTO,JPESKS)
      

      see also:
      process_pest();
      process_etdi();
      
ETHT (GALEPH)
   |
   v
ETDI (JULIA)
   |
   v
PEST.
PEST.ET   ---------> PECO row number
   |
   +---> ETDI row number
                  |
                  v
              ETDI.TL
                  |
                  v
             Packed IADD
                  |
          +-------+-------+
          |               |
          v               v
      IBITS(16,8)     IBITS(2,9)
          |               |
          v               v
          TJ              FI
          |               |
          +-------+-------+
                  |
                  v
        CALL ESRDIR(TJ+0.5,FI+0.5,
                    THETA,PHI)
                  |
                  v
           Physical direction
             in radians

  */
  bool  dbg = this->data.phco.debug;
  //auto& ecal = *this->exp.ecal;
  auto* peco = this->data.peco.load<object_table<class peco> >(true);

  if( dbg )  {
    std::cout << bos77::to_string(peco) << std::endl;
  }
  for( uint32_t itk=1, siz=peco->size(); itk <= siz; ++itk )  {
    auto   hit  = this->ecal_hits.create();
    auto*  ah   = this->data.peco.row<class peco>(itk);
    double ecor = ah->ecorr();
    double ene  = _ENE(ecor);
    uint64_t      cell = ah->pcOB();

    //hit.setTime(   0e0 );
    hit.setEnergy( ene );
    hit.setITheta( ah->theta() );
    hit.setIPhi(   ah->phi()   );
    hit.setEnergyError( std::abs(ene - ah->eraw()) );
    hit.setType( ((0x10 & cell)) +
                 ((  0x7 & ah->rbits()) <<  5) +
                 ((  0x7 & ah->ccode()) <<  8) +
                 ((  0x7 & ah->kdrg())  << 11) +
                 ((0x3FF & int(ah->esta1()*512e0)) << 14) +
                 ((0x3FF & int(ah->esta2()*512e0)) << 23) );

    if( dbg )  {
      char text[512];
      ::snprintf(text, sizeof(text),
                 "PECO %3d %8lX Energy:%s Corr:%s [%s,%s] Theta: %4.2f Phi: %4.2f"
                 " Region:%3d CC:%1d RB:%2d CalObjID:%3d",
                 itk, uint64_t(ah),
                 fmt_ene(ah->eraw()).c_str(),  fmt_ene(ah->ecorr()).c_str(),
                 fmt_ene(ah->esta1()).c_str(), fmt_ene(ah->esta2()).c_str(),
                 ah->theta(), ah->phi(), ah->kdrg(), ah->ccode(),
                 ah->rbits(), ah->pcOB());
      std::cout << text << std::endl;
    }
  }
}
