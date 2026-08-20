C ======================================================================
      SUBROUTINE F77_PRINT_LOOP_VARS(PREF, NAM, N, F, LL)
      IMPLICIT NONE
#include "textio.h"
#include "qdecl.h"
#include "qcde.h"
      INTEGER N, F, LL, I, J, K
      CHARACTER*3 NAM
      CHARACTER*3 PREF
      IF ( NAM .EQ. 'CHT' ) THEN
        I=KFCHT
        J=KLCHT
        K=KNCHT
      ELSE IF ( NAM .EQ. 'JET' ) THEN
        I=KFJET
        J=KLJET
        K=KNJET
      ELSE IF ( NAM .EQ. 'FRT' ) THEN
        I=KFFRT
        J=KLFRT
        K=-1
      ELSE IF ( NAM .EQ. 'IST' ) THEN
        I=KFIST
        J=KLIST
        K=KNIST
      ELSE IF ( NAM .EQ. 'AST' ) THEN
        I=KFAST
        J=KLAST
        K=KNAST
      ELSE IF ( NAM .EQ. 'V0T' ) THEN
        I=KFV0T
        J=KLV0T
        K=KNV0T
      ELSE IF ( NAM .EQ. 'MCT' ) THEN
        I=KFMCT
        J=KLMCT
        K=KNMCT
      ELSE IF ( NAM .EQ. 'MCV' ) THEN
        I=KFMCV
        J=KLMCV
        K=KNMCV
      ELSE IF ( NAM .EQ. 'GAT' ) THEN
        I=KFGAT
        J=KLGAT
        K=KNGAT
      ELSE
        I=-1
        J=-1
        K=-1
      END IF
      
      WRITE(TEXT,*) PREF//'  KF'//NAM//'=', F,
     &  ' KL'//NAM//'=', LL,
     &  ' KN'//NAM//'=', N,
     &  ' F77   KF'//NAM//'=', I,
     &  ' KL'//NAM//'=', J,
     &  ' KN'//NAM//'=', K
      LEN = LENOCC(TEXT)
      WRITE(6, 100) TEXT
 100  FORMAT((a))
      END
      
C ======================================================================
      SUBROUTINE F77_PRINT_QCDE
      IMPLICIT NONE
#include "qdecl.h"
#include "qcde.h"
      CALL F77_PRINT_LOOP_VARS('F77', 'CHT', KNCHT, KFCHT, KLCHT)
      CALL F77_PRINT_LOOP_VARS('F77', 'JET', KNJET, KFJET, KLJET)
      CALL F77_PRINT_LOOP_VARS('F77', 'FRT',    -1, KFFRT, KLFRT)
      CALL F77_PRINT_LOOP_VARS('F77', 'IST', KNIST, KFIST, KLIST)
      CALL F77_PRINT_LOOP_VARS('F77', 'AST', KNAST, KFAST, KLAST)
      CALL F77_PRINT_LOOP_VARS('F77', 'V0T', KNV0T, KFV0T, KLV0T)
      CALL F77_PRINT_LOOP_VARS('F77', 'NET', KNNET, KFNET, KLNET)
      CALL F77_PRINT_LOOP_VARS('F77', 'REV', KNREV, KFREV, KLREV)
      CALL F77_PRINT_LOOP_VARS('F77', 'OVT', KNOVT, KFOVT, KLOVT)
      CALL F77_PRINT_LOOP_VARS('F77', 'MCT', KNMCT, KFMCT, KLMCT)
      CALL F77_PRINT_LOOP_VARS('F77', 'MCV', KNMCV, KFMCV, KLMCV)
      CALL F77_PRINT_LOOP_VARS('F77', 'GAT', KNGAT, KFGAT, KLGAT)
      END

C ======================================================================
      SUBROUTINE F77_PRINT_TRACK(ITK)
      IMPLICIT NONE
#include "qdecl.h"
#include "textio.h"
      INTEGER ITK
#include "qcde.h"
#include "qmacro.h"
      WRITE(TEXT, 1000) ITK, CQTPN(ITK),
     &  QX(ITK), QY(ITK), QZ(ITK)
      LEN = LENOCC(TEXT)
 1000 FORMAT('F77 Track[',I3,']',1x,(A),1x,F8.3,2x,F8.3,2x,F8.3)
      END

C ======================================================================
      SUBROUTINE F77_PRINT_MC_TRACK(ITK)
      IMPLICIT NONE
#include "qdecl.h"
#include "textio.h"
      INTEGER     ITK, IOVX, IEVX
      INTEGER*8   LOC, MEM_ADDR
      CHARACTER*8 CVAL
      CHARACTER*4 CHAINT
#include "qcde.h"
#include "qmacro.h"

      IOVX = KORIV(ITK)
      IEVX = KENDV(ITK)
      WRITE(TEXT, 10)
     &  ITK, KCLASS(ITK), KTPCOD(ITK), CQTPN(ITK),
     &  QX(ITK), QY(ITK), QZ(ITK), IOVX, IEVX

      IF( IOVX .NE. 0 ) THEN
        WRITE(TEXT(LENOCC(TEXT)+1:), 11)
     &    'Org', KVN(IOVX), KVINCP(IOVX), KVNDAU(IOVX),
     &    QVX(IOVX), QVY(IOVX), QVZ(IOVX)
C       Address of first bank element
        LOC = 0
        IF( KVFVER(IOVX).NE.0 ) THEN
          LOC = MEM_ADDR(IW(KVFVER(IOVX)+1))
        END IF
        WRITE(TEXT(LENOCC(TEXT)+1:), 13) 'fver', LOC
        IF( KVFVER(IOVX).NE.0 ) THEN
          CVAL = CHAINT(IW(KVFVER(IOVX)+8))
          IF( CVAL(1:1) .NE. ' ' ) THEN
            WRITE(TEXT(LENOCC(TEXT)+1:), 12)'vol',CVAL(1:4)
          END IF
          CVAL = CHAINT(IW(KVFVER(IOVX)+9))
          IF( CVAL(1:1) .NE. ' ' ) THEN
            WRITE(TEXT(LENOCC(TEXT)+1:), 12)'mech',CVAL(1:4)
          END IF
        END IF
      END IF
      IF( IEVX .NE. 0 ) THEN
        WRITE(TEXT(LENOCC(TEXT)+1:), 11)
     &    'End', KVN(IEVX), KVINCP(IEVX), KVNDAU(IEVX),
     &    QVX(IEVX), QVY(IEVX), QVZ(IEVX)
C       Address of first bank element
        LOC = 0
        IF( KVFVER(IEVX).NE.0 ) THEN
          LOC = MEM_ADDR(IW(KVFVER(IEVX)+1))
        END IF
        WRITE(TEXT(LENOCC(TEXT)+1:), 13) 'fver', LOC
        IF( KVFVER(IEVX).NE.0 ) THEN
          CVAL = CHAINT(IW(KVFVER(IEVX)+8))
          IF( CVAL(1:1) .NE. ' ' ) THEN
            WRITE(TEXT(LENOCC(TEXT)+1:), 12)'vol',CVAL(1:4)
          END IF
          CVAL = CHAINT(IW(KVFVER(IEVX)+9))
          IF( CVAL(1:1) .NE. ' ' ) THEN
            WRITE(TEXT(LENOCC(TEXT)+1:), 12)'mech',CVAL(1:4)
          END IF
        END IF
      END IF
      LEN = LENOCC(TEXT)

 10   FORMAT('MCT[',I3,']',1X,'cl:',I2,1X,I6,1X,(A),1X,
     &  F6.1,1X,F6.1,1X,F6.1,1X,'koriv:',I3,1X,'kendv:',I3)
 11   FORMAT(1X,(A),':kvn:',I3,1X,'in:',I3,1X,'out:',I3,1X,
     &  F7.2,1X,F7.2,1X,F7.2)
 12   FORMAT(1X,(A),':',A4)
 13   FORMAT(1X,(A),':',Z8)
      END

C ======================================================================
      SUBROUTINE F77_PRINT_EIDT(ITK, TEXT)
      IMPLICIT NONE
#include "qdecl.h"
      CHARACTER*(*) TEXT
      INTEGER ITK, LENOCC
      INTEGER*8  LOC, MEM_ADDR
#include "qcde.h"
#include "qmacro.h"
      WRITE(TEXT(LENOCC(TEXT)+1:), 100) XEID(ITK)
      IF( XEID(ITK) ) THEN
        LOC = MEM_ADDR(IW(IW(KJQDET(ITK)+JQDEAE)+JEIDIF))
        WRITE(TEXT(LENOCC(TEXT)+1:), 101) LOC,
     &    IW(IW(KJQDET(ITK)+JQDEAE)+JEIDFR),
     &    IW(IW(KJQDET(ITK)+JQDEAE)+JEIDPE)
      END IF
 100  FORMAT(1X,'EIDT:', L1)
 101  FORMAT(1X,Z8,1X,'T:',I2,1X,'EC:',I2,1X)
      END

C ======================================================================
      SUBROUTINE F77_PRINT_HMAD(ITK, TEXT)
      IMPLICIT NONE
#include "qdecl.h"
      CHARACTER*(*) TEXT
      INTEGER ITK, LENOCC
      INTEGER*8  LOC, MEM_ADDR
#include "qcde.h"
#include "qmacro.h"
      WRITE(TEXT(LENOCC(TEXT)+1:), 100) XHMA(ITK)
      IF( XHMA(ITK) ) THEN
        LOC = MEM_ADDR(IW(IW(KJQDET(ITK)+JQDEAH)+JHMANF))
        WRITE(TEXT(LENOCC(TEXT)+1:), 101)
     &    LOC, IW(IW(KJQDET(ITK)+JQDEAH)+JHMATN), KHMANF(ITK)
      END IF
 100  FORMAT(1X,'HMAD:', L1)
 101  FORMAT(1X,Z8,1X,'T:',I2,1X,'PL:',I2,1X)
      END

C ======================================================================
      SUBROUTINE F77_PRINT_MCAD(ITK, TEXT)
      IMPLICIT NONE
#include "qdecl.h"
      CHARACTER*(*) TEXT
      INTEGER ITK, LENOCC
      INTEGER*8  LOC, MEM_ADDR
#include "qcde.h"
#include "qmacro.h"
      WRITE(TEXT(LENOCC(TEXT)+1:), 100) XMCA(ITK)
      IF( XMCA(ITK) ) THEN
        LOC = MEM_ADDR(IW(IW(KJQDET(ITK)+JQDEAM)+JMCANH))
        WRITE(TEXT(LENOCC(TEXT)+1:), 101)
     &    LOC, IW(IW(KJQDET(ITK)+JQDEAH)+JMCATN), KMCANH(ITK,1)
      END IF
 100  FORMAT(1X,'MCAD:', L1)
 101  FORMAT(1X,Z8,1X,'T:',I2,1X,'ID:',I2,1X)
      END

C ======================================================================
      SUBROUTINE F77_PRINT_MUID(ITK, TEXT)
      IMPLICIT NONE
#include "qdecl.h"
      CHARACTER*(*) TEXT
      INTEGER ITK, LENOCC
      INTEGER*8  LOC, MEM_ADDR
#include "qcde.h"
#include "qmacro.h"
      WRITE(TEXT(LENOCC(TEXT)+1:), 100) XMUI(ITK)
      IF( XMUI(ITK) ) THEN
        LOC = MEM_ADDR(IW(IW(KJQDET(ITK)+JQDEMU)+JMUIIF))
        WRITE(TEXT(LENOCC(TEXT)+1:), 101) 
     &    LOC, IW(IW(KJQDET(ITK)+JQDEAH)+JMUITN), KMUIIF(ITK)
      END IF
 100  FORMAT(1X,'MUID:', L1)
 101  FORMAT(1X,Z8,1X,'T:',I2,1X,'ID:',I2,1X)
      END

C ======================================================================
      SUBROUTINE F77_PRINT_TRACK_OPTS1(ITK)
      IMPLICIT NONE
#include "qdecl.h"
#include "textio.h"
      INTEGER I, ITK
      INTEGER*8  LOC, MEM_ADDR
#include "qcde.h"
#include "qmacro.h"
      WRITE(TEXT, 101) ITK, QDB(ITK), QZB(ITK),
     &  KTN(ITK),      QFRFD0(ITK), QFRFZ0(ITK)

      LOC = MEM_ADDR(IW(IW(KJQDET(ITK)+JQDEAL)+JFRTIV))
      WRITE(TEXT(LENOCC(TEXT)+1:), 104)
     &  LOC, KFRTNV(ITK), KFRTNI(ITK), KFRTNT(ITK)

      LOC = MEM_ADDR(IW(IW(KJQDET(ITK)+JQDEFI)+JFRIBP))
      WRITE(TEXT(LENOCC(TEXT)+1:), 105)
     &  LOC, QFRIPE(ITK),   QFRIPI(ITK)

      CALL F77_PRINT_EIDT(ITK, TEXT)
      CALL F77_PRINT_HMAD(ITK, TEXT)
      CALL F77_PRINT_MCAD(ITK, TEXT)
      CALL F77_PRINT_MUID(ITK, TEXT)
      WRITE(TEXT(LENOCC(TEXT)+1:), 102) XTEX(ITK),     KNTEX(ITK)
      
      DO I=1, KNTEX(ITK)
        WRITE(TEXT(LENOCC(TEXT)+1:), 103)
     &    I-1, KTEXSI(ITK,I), MOD(KTEXNS(ITK,I),99)
      END DO
      LEN = LENOCC(TEXT)
      
 99   FORMAT(1X,(A),':',F5.3)
 100  FORMAT(1X,(A),':',Z8)
 101  FORMAT('Track[',I3,'] ',
     &  'd0:',       F6.1,1x, 'z0:',F6.1,1X,
     &  'FRFT:',     I2,1X,
     &  'd0:',       F6.1,1x, 'z0:',F6.1,1X)
 102  FORMAT(1X,'TEXS:',     L1,1X,I2,1X )

 103  FORMAT(1X,I1,': SEG:',I2,1X,'#S:',I2,1X)
 104  FORMAT(1X,'FRTL:',Z8,1X,I1,1X,I1,1X,I2);
 105  FORMAT(1X,'FRID:',Z8,1x,'e-:',  F5.3,1X,'pi:',F5.3)
      END

C ======================================================================
      SUBROUTINE F77_PRINT_VERTEX_1(ITK)
      IMPLICIT NONE
#include "qdecl.h"
      INTEGER ITK, K1, K2
#include "textio.h"
#include "qcde.h"
#include "qmacro.h"
      K1 = -1
      K2 = -1
      IF( KVYV0V(ITK) .NE. 0 ) THEN
        K1 = IW(KVYV0V(ITK)+JYV0K1)
        K2 = IW(KVYV0V(ITK)+JYV0K2)
      END IF
      WRITE(TEXT, 1000) ITK, KVN(ITK), KVTYPE(ITK),
     &  QVX(ITK), QVY(ITK),QVZ(ITK),
     &  KVYV0V(ITK), KVFVER(ITK),
     &  KVINCP(ITK), K1, K2, KVNDAU(ITK),
     &  QVEM(ITK,1,1), QVEM(ITK,1,2), QVEM(ITK,1,3), 
     &  QVEM(ITK,2,1), QVEM(ITK,2,2), QVEM(ITK,2,3), 
     &  QVEM(ITK,3,1), QVEM(ITK,3,2), QVEM(ITK,3,3)
      LEN = LENOCC(TEXT)
 1000 FORMAT('F77 VERTEX[',I2,'] ',I2, ' TYP:', I1,
     &  ' X:', F7.2, ' Y:', F7.2, ' Z:', F7.2,
     &  ' YV0V:', I16, ' FVER:', I16,
     &  ' IN:', I3, I3, I3, ' #OUT:', I2,
     &  F8.2,F8.2,F8.2, F8.2,F8.2,F8.2, F8.2,F8.2,F8.2
     &    )
      RETURN
      END

C ======================================================================
      SUBROUTINE F77_PRINT_VERTEX_2(ITK)
      IMPLICIT NONE
#include "qdecl.h"
#include "textio.h"
      INTEGER ITK
#include "qcde.h"
#include "qmacro.h"
      WRITE(TEXT, 1000) ITK
      LEN = LENOCC(TEXT)

 1000 FORMAT('F77 VERTEX[',I2,'] ',I2
     &  )
      RETURN
      END

C ======================================================================
      SUBROUTINE F77_PRINT_TRACK_TRACKFIT(ITK)
      IMPLICIT NONE
#include "qdecl.h"
#include "textio.h"
      INTEGER       I, ITK
      CHARACTER* 12 NAME
#include "qcde.h"
#include "qmacro.h"
      TEXT = ''
      LEN = 0
      IF( KNDAU(ITK) .EQ. 0 ) THEN
        RETURN
      ENDIF
      NAME = CQTPN(ITK)
      WRITE(TEXT, 100) ITK, KTN(ITK), NAME(1:LENOCC(NAME)),
     &  ABS(QSIGMM(ITK)), QSIGE(ITK), ABS(QSIGP(ITK)), KNDAU(ITK)
      DO I = 1, KNDAU(ITK)
        WRITE(TEXT(LENOCC(TEXT)+1:), 101) KDAU(ITK, I)
      END DO
      WRITE(TEXT(LENOCC(TEXT)+1:), 102) 
     &  QSMAT(ITK,0), QSMAT(ITK,1), QSMAT(ITK,2),
     &  QSMAT(ITK,3), QSMAT(ITK,4), QSMAT(ITK,5),
     &  QSMAT(ITK,6), QSMAT(ITK,6), QSMAT(ITK,8),
     &  QSMAT(ITK,9)
      LEN = LENOCC(TEXT)
 100  FORMAT('F77 TRACK[',I3,']',
     &  ' FRFT:', I2, 1X, (A),' SIGM:',F6.2,' SIGE:',F6.2,' SIGP:',F6.2,
     &  ' KNDAU:', I2, 1X)
 101  FORMAT(I3,1X)
 102  FORMAT(' COV:', F7.2,1X, F7.2,1X, F7.2,1X, F7.2,1X, F7.2,1X,
     &  F7.2,1X, F7.2,1X, F7.2,1X, F7.2,1X, F7.2,1X, F7.2 )
      RETURN
      END

C ======================================================================
      SUBROUTINE F77_PRINT_TRACK_MOTHERS_DAUGHTERS(ITK, PREF)
      IMPLICIT NONE
#include "qdecl.h"
#include "textio.h"
      CHARACTER*(*) PREF
      INTEGER ITK, I, J, NMOT, NDAU
#include "qcde.h"
#include "qmacro.h"
      NMOT = KNMOTH(ITK)
      NDAU = KNDAU(ITK)
      TEXT = ''
      IF( (NDAU+NMOT) .GT. 0 ) THEN
        WRITE(TEXT, 100) PREF, ITK
        IF( NMOT .GT. 0 ) THEN
          WRITE(TEXT(LENOCC(TEXT)+1:), 101) '#MOT', NMOT
          DO I=1,NMOT
            J = KMOTH(ITK,I)
            WRITE(TEXT(LENOCC(TEXT)+1:), 102) 'M', J, QX(J)
          END DO
        END IF
        IF( NDAU .GT. 0 ) THEN
          WRITE(TEXT(LENOCC(TEXT)+1:), 101) '#DAU', NDAU
          DO I=1,NDAU
            J = KDAU(ITK,I)
            WRITE(TEXT(LENOCC(TEXT)+1:), 102) 'D', J, QX(J)
          END DO
        END IF
      END IF
      LEN = LENOCC(TEXT)

 100  FORMAT((A),'[',I3,']')
 101  FORMAT(1X,(A), ':', I2)
 102  FORMAT(1X,(A), ':', I3, 1X, 'px:', F7.3)
      END

C ======================================================================
      SUBROUTINE F77_PRINT_TRACK_CALO(WHICH)
      IMPLICIT NONE
#include "qdecl.h"
#include "textio.h"
      INTEGER WHICH, NECAL, NHCAL, ITK, KTK, I
#include "qcde.h"
#include "qmacro.h"
      TEXT = ''
      LEN  = 0
      NECAL = KNECAL(WHICH)
      NHCAL = KNHCAL(WHICH)
      IF( NECAL+NHCAL .LE. 0 ) THEN
        RETURN
      ENDIF

      WRITE(TEXT(LENOCC(TEXT)+1:), 100) WHICH
      DO I=1, NECAL
        IF( I .EQ. 1 ) THEN
          WRITE(TEXT(LENOCC(TEXT)+1:), 101) NECAL
        END IF
        ITK = KECAL(WHICH,I)
        KTK = KTN(ITK)
        WRITE(TEXT(LENOCC(TEXT)+1:), 102) ITK, KTK, QPECEC(ITK), QE(ITK)
      END DO

      DO I=1, NHCAL
        IF( I .EQ. 1 ) THEN
          WRITE(TEXT(LENOCC(TEXT)+1:), 201) NHCAL
        END IF
        ITK = KHCAL(WHICH,I)
        KTK = KTN(ITK)
        WRITE(TEXT(LENOCC(TEXT)+1:), 202) ITK, KTK, QPHCEC(ITK), QE(ITK)
      END DO
      LEN = LENOCC(TEXT)

 100  FORMAT('Track[',I3,'] CALO info')
 101  FORMAT(1X, 'ECAL',1x,    '#EC:', I1)
 102  FORMAT(1X, 'OBJ:',I3,1X, 'PECO:',I3,1x, 'E:',F7.3,1X, 'TE:',F7.3)
 201  FORMAT(1X, 'HCAL',1x,    '#HC:', I1)
 202  FORMAT(1X, 'OBJ:',I3,1X, 'PHCO:',I3,1x, 'E:',F7.3,1X, 'TE:',F7.3)
      END

C ======================================================================
      SUBROUTINE F77_PRINT_PECO(WHICH)
      IMPLICIT NONE
#include "bosext.h"
#include "qdecl.h"
#include "textio.h"
      INTEGER*8 LOC, MEM_ADDR
      INTEGER WHICH, ITK, KPECO, NW, NR
#include "qcde.h"
#include "qmacro.h"
      ITK = WHICH + 1
      KPECO = IW(NAPECO) + LMHLEN
      NW = IW(KPECO - LMHLEN + 1)
      NR = IW(KPECO - LMHLEN + 2)
      LOC = MEM_ADDR(IW(KPECO+WHICH*NW+JPECER))
      WRITE(TEXT,1000) WHICH, LOC,
     &  RW(KPECO+WHICH*NW+JPECER),
     &  RW(KPECO+WHICH*NW+JPECEC),
     &  RW(KPECO+WHICH*NW+JPECTH),
     &  RW(KPECO+WHICH*NW+JPECPH),
     &  IW(KPECO+WHICH*NW+JPECKD),
     &  IW(KPECO+WHICH*NW+JPECCC),
     &  IW(KPECO+WHICH*NW+JPECRB),
     &  IW(KPECO+WHICH*NW+JPECPC)
      LEN = LENOCC(TEXT)
 1000 FORMAT('PECO',1X,I3,1X,Z8,1X,
     &  'ENERGY:', F7.3,1X,  'CORR:', F7.3,1X,
     &  'THETA:',  F5.2,1X,  'PHI:',  F5.2,1X, 'REGION:',I2,1X,
     &  'CC:',     I2,1X,    'REL:',  I2,1X,   'ID:',I5)
      RETURN
      END

C ======================================================================
      SUBROUTINE F77_PRINT_PHCO(WHICH)
      IMPLICIT NONE
#include "bosext.h"
#include "qdecl.h"
#include "textio.h"
      INTEGER*8 LOC, MEM_ADDR
      INTEGER WHICH, ITK, KPHCO, NR, NW
#include "qcde.h"
#include "qmacro.h"
      ITK = WHICH + 1
      KPHCO = IW(NAPHCO)+LMHLEN
      NW = IW(KPHCO - LMHLEN + 1)
      NR = IW(KPHCO - LMHLEN + 2)
      LOC = MEM_ADDR(IW(KPHCO+WHICH*NW+JPHCER))
      WRITE(TEXT,1000) WHICH, LOC,
     &  RW(KPHCO+WHICH*NW+JPHCER),
     &  RW(KPHCO+WHICH*NW+JPHCEC),
     &  RW(KPHCO+WHICH*NW+JPHCTH),
     &  RW(KPHCO+WHICH*NW+JPHCPH),
     &  IW(KPHCO+WHICH*NW+JPHCKD),
     &  IW(KPHCO+WHICH*NW+JPHCCC),
     &  IW(KPHCO+WHICH*NW+JPHCRB),
     &  IW(KPHCO+WHICH*NW+JPHCNF),
     &  IW(KPHCO+WHICH*NW+JPHCPC)
      LEN = LENOCC(TEXT)

 1000 FORMAT('PHCO',1X,I3,1X,Z8,1X,
     &  'ENERGY:',  F7.3,1X, 'CORR:', F7.3,1X,
     &  'THETA: ',  F4.2,1x, 'PHI: ', F4.2,1X,
     &  'REGION:',  I1,1X,   'CC:',   I1,1X,
     &  'RB:',      I2,1X,   'NOISE:',I2,1X,
     &  'ID:',      I3)
      RETURN
      END

C ======================================================================
      SUBROUTINE F77_PRINT_TRACKS(FIRST, LAST)
      IMPLICIT NONE
      INTEGER I, FIRST, LAST
      DO I=FIRST, LAST
        CALL F77_PRINT_TRACK(I)
      END DO
      RETURN
      END
      
C ======================================================================
      SUBROUTINE USER_EVENT()
#include "qcde.h"
#include "qmacro.h"
      CHARACTER*4  BNAME
      INTEGER I, J, K
      INTEGER PRINT_TRACKS;
      PARAMETER( PRINT_TRACKS=0, PRINT1=0, PRINT2=0, PRINT3=0 )
      CHARACTER*4 NLIST


      INAM = NAMIND('QVEC')
      IF( PRINT_TRACKS.GT.0 ) THEN
        PRINT *,' --- ',INAM, IW(INAM), KOQVEC, KCQVEC, JQVEQX
        DO I=KFCHT, KLCHT
          PRINT *, 'TRACK ', I, ' = ',
     &      KOQVEC+I*KCQVEC+JQVEQX,
     &      QX(I), QY(I), QZ(I)
        END DO
      END IF

      IF( PRINT1.GT.0 ) THEN
        DO I=1,100000
          BNAME = NLIST(IW, I, 'CERST')
          IF( BNAME(1:1).EQ.' ' )THEN
            GOTO 100
          END IF
          PRINT *,'   ',BNAME
        END DO
      END IF
      IF( PRINT2.GT.0 ) THEN
        DO I=1,100000
          BNAME = NLIST(IW, I, '')
          IF( BNAME(1:1).EQ.' ' )THEN
            GOTO 100
          END IF
          PRINT *,'   ',BNAME
        END DO
      END IF
      
 100  CONTINUE
      
      RETURN
      END
