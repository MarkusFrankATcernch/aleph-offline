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
#ifndef ALPHA_BOS_QCDE_H
#define ALPHA_BOS_QCDE_H

/// C/C++ include files
#include <cstdint>

/// Framework include files
#include <bos/bosbank.h>

/// ALPHA namespace declaration
namespace alpha  {

  constexpr static const int32_t KCQFPA = 8;
  constexpr static const int32_t KMONTE=-2;
  constexpr static const int32_t KRECO=-1;
  constexpr static const int32_t KLOCKM=14;
  constexpr static const int32_t KSOVT=1;
  constexpr static const int32_t KSCHT=2;
  constexpr static const int32_t KSIST=3;
  
  class qcde_t  {
  public:    
    float QELEP, QMFLD ,QMFLDC, QTIME, QTIMEI, QTIMEL,
      QTIMES, QTIMET, QDHEEC, QDHEEL, QDHEPF, QDHETH, QDHEPH, QDHEEF,
      QDHEET, QDHET1, QDHEP1, QDHET2, QDHEP2, QDHEE1, QDHEE2, QDHEE3,
      QKEVRN, QKEVWT, QVXNOM, QVYNOM, QVZNOM, QVXNSG, QVYNSG, QVZNSG,
      QINLUM, QRINLN, QRINLU, QDBOFS, QEECWI[36], QVXBOM, QVYBOM,
      QSILUM, QRSLLU, QRSLBK, QRSLEW , QVTXBP[3], QVTEBP[3], QVTSBP[3];
    
   int32_t KFOVT, KLOVT, KNOVT, KFCHT, KLCHT, KNCHT, KFIST,
      KLIST, KNIST, KFAST, KLAST, KNAST, KFEHT, KLEHT, KNEHT, KFV0T,
      KLV0T, KNV0T, KFDCT, KLDCT, KNDCT, KFEFT, KLEFT, KNEFT, KFNET,
      KLNET, KNNET, KFGAT, KLGAT, KNGAT, KFJET, KLJET, KNJET, KFMCT,
      KLMCT, KNMCT, KFREV, KLREV, KNREV, KFMCV, KLMCV, KNMCV, KLUST,
      KLUSV, KFFRT, KLFRT, KFFRV, KLFRV, KNRET, KNCOT, KFFRD,
      KLFJET,KLLJET,KLNJET,
      KBIT[32], KCLACO[KCQFPA], KCLAFR[KCQFPA], KCLARM[KCQFPA],
      KDEBUG, KEVT, KEXP, KFFILL, KFEOUT, KJOPTN[2][2],
      KLEOUT, KLROUT, KLOCK0[KLOCKM][2], KLOCK1[KLOCKM][2], KLOCK2[KLOCKM][2],
      KMATIX[6][6], KMQFPA, KNEFIL, KNEOUT, KNEVT, KNPAST,
      KNQDET, KNQFPA, KNQLIN, KNQMTX, KNQPAR, KNQPST, KNREIN, KNREOU,
      KOQDET, KOQFPA, KOQLIN, KOQMTL, KOQMTS, KOQPAR, KOQPBT, KOQPLI,
      KOQTRA, KOQVEC, KOQVRT, KQPAR, KQVEC, KQVRT, KQWRK, KQZER, KRUN,
      KSTATU, KTDROP, KUCARD, KUCONS, KUHIST, KUINPU, KUOUTP, KUPRNT,
      KUPTER, KDEBU1, KDEBU2, KNWRLM, KEFOPT, KUEDIN, KUEDOU, KURTOX,
      KUCAR2, KNHDRN, KNBHAB, KSBHAB, KRSLLQ, KRSLNB,
      INDATA,
      KRINNE, KRINNF, KRINDC, KRINDQ, KRINNZ, KRINNB,
      KRINBM, KRINFR, KRINLR, KRINLF,
      KEVERT, KEVEDA, KEVETI, KEVEMI[4], KEVETY, KEVEES,
      KDHEFP, KDHENP, KDHENM, KKEVNT, KKEVNV, KKEVID, KDHENX, KDHENV,
      KDHENJ, KREVDS, KXTET1, KXTET2, KXTEL2, KXTCGC, KXTCLL, KXTCBN,
      KXTCCL, KXTCHV, KXTCEN, KCLASW, KERBOM, KBPSTA;
#if 0
      DIMENSION KLOCUS(3,14)
      EQUIVALENCE (KLOCUS(1,1),KFOVT), (KFOVT,KFRET), (KLIST,KLRET),
     & (KFIST,KFCOT), (KLAST,KLCOT)
      
      COMMON /QCDE/ XCOPYJ, XFLIAC, XHISTO, XLREV(2), XLREV2(2), XMCEV,
     & XMINI, XSYNTX, XWREVT, XWRRUN, XFILMC, XFILCH, XFILV0, XFILCO,
     & XFILEF, XFILPC, XFILGA, XFILJE,
     & XPRHIS, XFILL, XVITC, XVTPC, XVECAL, XVLCAL, XVTPCD,
     & XVSATR, XVHCAL, XHVTRG, XSREC, XWMINI, XIOKLU, XIOKSI, XFRF2,
     & XNSEQ, XVDOK, XFRF0, XFMUID, XFILEM, XWNANO, XROKSI, XGETBP,
     & XJTHRU
#endif
  };

#ifndef DEFINE_QCDE_STORAGE
  extern qcde_t& qcde;
#endif
}      // End namespace alpha
#endif // ALPHA_BOS_QCDE_H
