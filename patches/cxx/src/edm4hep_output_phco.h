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
void alpha::edm4hep_output::event_t::process_phco()  {
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

                         
      PCRL (Calobject ReLations: PCOB, PECO, PFRF, PHCO, PPOB)
      
      -- HCAL
      PFHR ---> PFRF, PHCO
      PHER (ECAL-HCAL relations : (PECO-PHCO) )
      PHHY (Hadronic calobject HYpothesis)                         ---> PHCO #) (redundant PCRL ?)
      PHMH (Relation between HCAL objects and FKIN tracks)         ---> PHCO #)
      PHPR (Hcalobject-digital Pattern Relations)                  ---> PHCO #) (redundant PCRL ?)
      PHST (P.o.t. Hcal STorey data, theta, phi, corr.energy)      ---> PHCO #)
      PHTO (Topology of Hadronic calObject)                        ---> PHOB #) ???
      HLTU (True MC longit. coordinate of Hcal hit)                ---> HTUB #)
      HTUB (Hcal TUBes data (from Julia prepare data)              ---> HCAL coordinates(barrel, module, layer)
                                                                   ---> HWDI
                                                                   ---> PPOB (=HPCO)
      HPDI (Hcal Plane DIgits (RawData))                           ---> Planeaddress (HCAL coordinates) ---> HTUB
      HWDI (Hcal Wire (tube) DIgits(RawData) )                     ---> Cluster address: tube, layer, module, barrel
      HROA (Hcal road data for current track)                      ---> HTUB #)

  -- Clusters get updated in               process_pept()
     with stack 1/2/3 sub-clusters and tower positions of stacks
  -- Clusters get updated in               process_ppob()
     with digital patterns and digital streamer energies
     
  -- Clusters get linked to FRFT tracks in process_pcrl()
                         to CalObjects ( this->calobj2frft and this->frft2calobj )
  -- Clusters get linked to MC   tracks in process_phmh()

  */
  bool  dbg = this->data.phco.debug;
  auto* tab = this->data.phco.table<class phco>();

  printout( dbg ? ALWAYS : DEBUG, __FUNCTION__,"+++ %s", bos77::to_string(tab).c_str() );
  for( uint32_t itk=1, siz=tab->size(); itk <= siz; ++itk )  {
    auto          hit  = this->hcal_hits.create();
    auto*         ah   = this->data.phco.row<class phco>(itk);
    // uint64_t      cell = ah->pcOB();
    // hit.setCellID( cell );
    // hit.setTime(   0e0 );

    hit.setITheta( ah->theta() );
    hit.setIPhi(   ah->phi()   );
    hit.setEnergy( ah->ecorr() );
    hit.setEnergyError( std::abs(ah->ecorr() - ah->eraw()) );
    hit.setType(((  0x7 & ah->rbits())) +
		((  0x7 & ah->ccode()) <<  3) +
		((  0x7 & ah->noiseFlag())  <<  6));

    printout( dbg ? ALWAYS : DEBUG, __FUNCTION__,
              "+++ \tPHCO %3d %8lX Energy:%7.3f Corr:%7.3f Theta: %4.2f Phi: %4.2f"
              " Region:%3d CC:%2d Rel:%2d CalObjID:%5d",
              itk, uint64_t(ah), ah->eraw(), ah->ecorr(), ah->theta(), ah->phi(),
              ah->kdrg(), ah->ccode(), ah->rbits(), ah->pcOB());
  }
}

