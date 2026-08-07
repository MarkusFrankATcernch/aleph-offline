C ======================================================================
      SUBROUTINE F77_PRINT_LOOP_VARS(PREF, NAM, N, F, LL)
      IMPLICIT NONE
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
      
      WRITE(6,*) PREF//'  KF'//NAM//'=', F,
     &  ' KL'//NAM//'=', LL,
     &  ' KN'//NAM//'=', N,
     &  ' F77   KF'//NAM//'=', I,
     &  ' KL'//NAM//'=', J,
     &  ' KN'//NAM//'=', K
      RETURN
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
      RETURN
      END

C ======================================================================
      SUBROUTINE F77_PRINT_TRACK(WHICH)
      IMPLICIT NONE
#include "qdecl.h"
      INTEGER WHICH
#include "qcde.h"
#include "qmacro.h"
      PRINT 1000, WHICH, QX(WHICH), QY(WHICH), QZ(WHICH)
 1000 FORMAT('F77 Track[',I3,']',4x,F8.3,2x,F8.3,2x,F8.3)
      RETURN
      END

C ======================================================================
      SUBROUTINE F77_PRINT_TRACK_OPTS1(WHICH)
      IMPLICIT NONE
#include "qdecl.h"
      INTEGER WHICH
#include "qcde.h"
#include "qmacro.h"
      IF( KNTEX(WHICH) .EQ. 0 ) THEN
        WRITE(6,1000) WHICH, KTN(WHICH),
     &    QFRFD0(WHICH), QFRFZ0(WHICH),
     &    KFRTNV(WHICH), KFRTNI(WHICH), KFRTNT(WHICH),
     &    QFRIPE(WHICH), QFRIPI(WHICH),
     &    XTEX(WHICH),   KNTEX(WHICH)
      ELSE IF( KNTEX(WHICH) .EQ. 1 ) THEN
        WRITE(6,2000) WHICH, KTN(WHICH),
     &    QFRFD0(WHICH),   QFRFZ0(WHICH),
     &    KFRTNV(WHICH),   KFRTNI(WHICH), KFRTNT(WHICH),
     &    QFRIPE(WHICH),   QFRIPI(WHICH),
     &    XTEX(WHICH),     KNTEX(WHICH),
     &    KTEXSI(WHICH,1), MOD(KTEXNS(WHICH,1),99)
      ELSE IF( KNTEX(WHICH) .EQ. 2 ) THEN
        WRITE(6,3000) WHICH, KTN(WHICH),
     &    QFRFD0(WHICH),   QFRFZ0(WHICH),
     &    KFRTNV(WHICH),   KFRTNI(WHICH), KFRTNT(WHICH),
     &    QFRIPE(WHICH),   QFRIPI(WHICH),
     &    XTEX(WHICH),     KNTEX(WHICH),
     &    KTEXSI(WHICH,1), MOD(KTEXNS(WHICH,1),99),
     &    KTEXSI(WHICH,2), MOD(KTEXNS(WHICH,2),99)
      ELSE IF( KNTEX(WHICH) .EQ. 3 ) THEN
        WRITE(6,4000) WHICH, KTN(WHICH),
     &    QFRFD0(WHICH),   QFRFZ0(WHICH),
     &    KFRTNV(WHICH),   KFRTNI(WHICH), KFRTNT(WHICH),
     &    QFRIPE(WHICH),   QFRIPI(WHICH),
     &    XTEX(WHICH),     KNTEX(WHICH),
     &    KTEXSI(WHICH,1), MOD(KTEXNS(WHICH,1),99),
     &    KTEXSI(WHICH,2), MOD(KTEXNS(WHICH,2),99),
     &    KTEXSI(WHICH,3), MOD(KTEXNS(WHICH,3),99)
      ENDIF
 1000 FORMAT('F77 TRACK[',I3,'] ',
     &  'FRFT:', I2,1X,
     &  'D0:',       F7.2,1x'Z0:',F7.2,1X,
     &  'FRTL:',     1X,I1,1X,I1,1X,I2,1X,
     &  'FRID:',15x,'e-:',  F5.3,1X,'pi:',F5.3,1X,
     &  'TEXS:',     L1,1X,I2,1X
     &  )
 2000 FORMAT('F77 TRACK[',I3,'] ',
     &  'FRFT:', I2,1X,
     &  'D0:',       F7.2,1x'Z0:',F7.2,1X,
     &  'FRTL:',     1X,I1,1X,I1,1X,I2,1X,
     &  'FRID:',15x,'e-:',  F5.3,1X,'pi:',F5.3,1X,
     &  'TEXS:',     L1,1X,I2,1X,
     &  '0: SEG:',   I2,1X,'#S:',I2,1X
     &  )
 3000 FORMAT('F77 TRACK[',I3,'] ',
     &  'FRFT:', I2,1X,
     &  'D0:',       F7.2,1x'Z0:',F7.2,1X,
     &  'FRTL:',     1X,I1,1X,I1,1X,I2,1X,
     &  'FRID:',15x,'e-:',  F5.3,1X,'pi:',F5.3,1X,
     &  'TEXS:',     L1,1X,I2,1X,
     &  '0: SEG:',   I2,1X,'#S:',I2,1X,
     &  '1: SEG:',   I2,1X,'#S:',I2,1X)
 4000 FORMAT('F77 TRACK[',I3,'] ',
     &  'FRFT:', I2,1X,
     &  'D0:',       F7.2,1x'Z0:',F7.2,1X,
     &  'FRTL:',     1X,I1,1X,I1,1X,I2,1X,
     &  'FRID:',15x,'e-:',  F5.3,1X,'pi:',F5.3,1X,
     &  'TEXS:',     L1,1X,I2,1X,
     &  '0: SEG:',   I2,1X,'#S:',I2,1X,
     &  '1: SEG:',   I2,1X,'#S:',I2,1X,
     &  '2: SEG:',   I2,1X,'#S:',I2,1X
     &  )
      RETURN
      END

C ======================================================================
      SUBROUTINE F77_PRINT_VERTEX_1(WHICH)
      IMPLICIT NONE
#include "qdecl.h"
      INTEGER WHICH, K1, K2
#include "qcde.h"
#include "qmacro.h"
      K1 = -1
      K2 = -1
      IF( KVYV0V(WHICH) .NE. 0 ) THEN
        K1 = IW(KVYV0V(WHICH)+JYV0K1)
        K2 = IW(KVYV0V(WHICH)+JYV0K2)
      END IF
      WRITE(6,1000) WHICH, KVN(WHICH), KVTYPE(WHICH),
     &  QVX(WHICH), QVY(WHICH),QVZ(WHICH),
     &  KVYV0V(WHICH), KVFVER(WHICH),
     &  KVINCP(WHICH), K1, K2, KVNDAU(WHICH),
     &  QVEM(WHICH,1,1), QVEM(WHICH,1,2), QVEM(WHICH,1,3), 
     &  QVEM(WHICH,2,1), QVEM(WHICH,2,2), QVEM(WHICH,2,3), 
     &  QVEM(WHICH,3,1), QVEM(WHICH,3,2), QVEM(WHICH,3,3)

 1000 FORMAT('F77 VERTEX[',I2,'] ',I2, ' TYP:', I1,
     &  ' X:', F7.2, ' Y:', F7.2, ' Z:', F7.2,
     &  ' YV0V:', I16, ' FVER:', I16,
     &  ' IN:', I3, I3, I3, ' #OUT:', I2,
     &  F8.2,F8.2,F8.2, F8.2,F8.2,F8.2, F8.2,F8.2,F8.2
     &    )
      RETURN
      END

C ======================================================================
      SUBROUTINE F77_PRINT_VERTEX_2(WHICH)
      IMPLICIT NONE
#include "qdecl.h"
      INTEGER WHICH
#include "qcde.h"
#include "qmacro.h"
      WRITE(6,1000) WHICH

 1000 FORMAT('F77 VERTEX[',I2,'] ',I2
     &  )
      RETURN
      END

C ======================================================================
      SUBROUTINE F77_PRINT_TRACK_TRACKFIT(WHICH)
      IMPLICIT NONE
#include "qdecl.h"
      INTEGER WHICH, LENOCC
      CHARACTER* 12 NAME
#include "qcde.h"
#include "qmacro.h"
      IF( KNDAU(WHICH) .EQ. 0 ) THEN
        RETURN
      ENDIF
      NAME = CQTPN(WHICH)
      IF( KNDAU(WHICH) .EQ. 1 ) THEN
        WRITE(6,2000) WHICH, KTN(WHICH), NAME(1:LENOCC(NAME)),
     &    ABS(QSIGMM(WHICH)), QSIGE(WHICH), QSIGP(WHICH),
     &    KNDAU(WHICH),KDAU(WHICH,1),
     &    QSMAT(WHICH,0), QSMAT(WHICH,1), QSMAT(WHICH,2),
     &    QSMAT(WHICH,3), QSMAT(WHICH,4), QSMAT(WHICH,5),
     &    QSMAT(WHICH,6), QSMAT(WHICH,6), QSMAT(WHICH,8),
     &    QSMAT(WHICH,9)
      ELSE IF( KNDAU(WHICH) .EQ. 2 ) THEN
        WRITE(6,3000) WHICH, KTN(WHICH), NAME(1:LENOCC(NAME)),
     &    ABS(QSIGMM(WHICH)), QSIGE(WHICH), QSIGP(WHICH),
     &    KNDAU(WHICH),KDAU(WHICH,1),KDAU(WHICH,2),
     &    QSMAT(WHICH,0), QSMAT(WHICH,1), QSMAT(WHICH,2),
     &    QSMAT(WHICH,3), QSMAT(WHICH,4), QSMAT(WHICH,5),
     &    QSMAT(WHICH,6), QSMAT(WHICH,6), QSMAT(WHICH,8),
     &    QSMAT(WHICH,9)
      ELSE IF( KNDAU(WHICH) .EQ. 3 ) THEN
        WRITE(6,4000) WHICH, KTN(WHICH), NAME(1:LENOCC(NAME)),
     &    ABS(QSIGMM(WHICH)), QSIGE(WHICH), QSIGP(WHICH),
     &    KNDAU(WHICH),KDAU(WHICH,1),KDAU(WHICH,2),KDAU(WHICH,3),
     &    QSMAT(WHICH,0), QSMAT(WHICH,1), QSMAT(WHICH,2),
     &    QSMAT(WHICH,3), QSMAT(WHICH,4), QSMAT(WHICH,5),
     &    QSMAT(WHICH,6), QSMAT(WHICH,6), QSMAT(WHICH,8),
     &    QSMAT(WHICH,9)
      ELSE IF( KNDAU(WHICH) .EQ. 4 ) THEN
        WRITE(6,5000) WHICH, KTN(WHICH), NAME(1:LENOCC(NAME)),
     &    ABS(QSIGMM(WHICH)), QSIGE(WHICH), QSIGP(WHICH),
     &    KNDAU(WHICH),KDAU(WHICH,1),KDAU(WHICH,2),KDAU(WHICH,3),
     &    KDAU(WHICH,4),
     &    QSMAT(WHICH,0), QSMAT(WHICH,1), QSMAT(WHICH,2),
     &    QSMAT(WHICH,3), QSMAT(WHICH,4), QSMAT(WHICH,5),
     &    QSMAT(WHICH,6), QSMAT(WHICH,6), QSMAT(WHICH,8),
     &    QSMAT(WHICH,9)
      ENDIF
 2000 FORMAT('F77 TRACK[',I3,']',
     &  ' FRFT:', I2, 1X, (A),' SIGM:',F6.2,' SIGE:',F6.2,' SIGP:',F6.2,
     &  ' KNDAU:', I2, I3,
     &  ' COV:', F7.2,1X, F7.2,1X, F7.2,1X, F7.2,1X, F7.2,1X,
     &  F7.2,1X, F7.2,1X, F7.2,1X, F7.2,1X, F7.2,1X, F7.2
     &  )
 3000 FORMAT('F77 TRACK[',I3,']',
     &  ' FRFT:', I2, 1X, (A),' SIGM:',F6.2,' SIGE:',F6.2,' SIGP:',F6.2,
     &  ' KNDAU:',I2, I3, I3,
     &  ' COV:', F7.2,1X, F7.2,1X, F7.2,1X, F7.2,1X, F7.2,1X,
     &  F7.2,1X, F7.2,1X, F7.2,1X, F7.2,1X, F7.2,1X, F7.2
     &  )
 4000 FORMAT('F77 TRACK[',I3,']',
     &  ' FRFT:', I2, 1X, (A),' SIGM:',F6.2,' SIGE:',F6.2,' SIGP:',F6.2,
     &  ' KNDAU:',I2, I3, I3, I3,
     &  ' COV:', F7.2,1X, F7.2,1X, F7.2,1X, F7.2,1X, F7.2,1X,
     &  F7.2,1X, F7.2,1X, F7.2,1X, F7.2,1X, F7.2,1X, F7.2
     &  )
 5000 FORMAT('F77 TRACK[',I3,']',
     &  ' FRFT:', I2, 1X, (A),' SIGM:',F6.2,' SIGE:',F6.2,' SIGP:',F6.2,
     &  ' KNDAU:',I2, I3, I3, I3, I3,
     &  ' COV:', F7.2,1X, F7.2,1X, F7.2,1X, F7.2,1X, F7.2,1X,
     &  F7.2,1X, F7.2,1X, F7.2,1X, F7.2,1X, F7.2,1X, F7.2
     &  )
      RETURN
      END

C ======================================================================
      SUBROUTINE F77_PRINT_TRACK_MOTHERS(WHICH)
      IMPLICIT NONE
#include "qdecl.h"
      INTEGER WHICH, ITK
#include "qcde.h"
#include "qmacro.h"
      IF( KNMOTH(WHICH) .GT. 0 ) THEN
        ITK = KMOTH(WHICH,1)
        WRITE(6,1000) WHICH, QX(WHICH), QY(WHICH), QZ(WHICH),
     &    KNMOTH(WHICH), ITK, QX(ITK), QY(ITK), QZ(ITK)
      END IF
 1000 FORMAT('F77 TRACK[',I3,'] MOTHERS  ',F7.3,1x,F7.3,1x,F7.3,2X,
     &       '#MOT:',I1,2X,'TRK:',I3,1x,F7.3,1x,F7.3,1x,F7.3,1X)
      RETURN
      END

C ======================================================================
      SUBROUTINE F77_PRINT_TRACK_DAUGHTERS(WHICH)
      IMPLICIT NONE
#include "qdecl.h"
      INTEGER WHICH, ITK1, ITK2, ITK3, ITK4, NDAU
#include "qcde.h"
#include "qmacro.h"

      NDAU = KNDAU(WHICH)
      IF( NDAU .LE. 0 ) THEN
        RETURN
      END IF
      
      IF( NDAU .EQ.1 ) THEN
        ITK1 = KDAU(WHICH,1)
        WRITE(6,1000) WHICH, QX(WHICH), QY(WHICH), QZ(WHICH),
     &    NDAU, ITK1, QX(ITK1), QY(ITK1), QZ(ITK1)
      ELSE IF( NDAU .EQ.2 ) THEN
        ITK1 = KDAU(WHICH,1)
        ITK2 = KDAU(WHICH,2)
        WRITE(6,2000) WHICH, QX(WHICH), QY(WHICH), QZ(WHICH),
     &    NDAU,
     &    ITK1, QX(ITK1), QY(ITK1), QZ(ITK1),
     &    ITK2, QX(ITK2), QY(ITK2), QZ(ITK2)
      ELSE IF( NDAU .EQ.3 ) THEN
        ITK1 = KDAU(WHICH,1)
        ITK2 = KDAU(WHICH,2)
        ITK3 = KDAU(WHICH,3)
        WRITE(6,3000) WHICH, QX(WHICH), QY(WHICH), QZ(WHICH),
     &    NDAU,
     &    ITK1, QX(ITK1), QY(ITK1), QZ(ITK1),
     &    ITK2, QX(ITK2), QY(ITK2), QZ(ITK2),
     &    ITK3, QX(ITK3), QY(ITK3), QZ(ITK3)
      ELSE IF( NDAU .GE.4 ) THEN
        ITK1 = KDAU(WHICH,1)
        ITK2 = KDAU(WHICH,2)
        ITK3 = KDAU(WHICH,3)
        ITK4 = KDAU(WHICH,4)
        WRITE(6,4000) WHICH, QX(WHICH), QY(WHICH), QZ(WHICH),
     &    NDAU,
     &    ITK1, QX(ITK1), QY(ITK1), QZ(ITK1),
     &    ITK2, QX(ITK2), QY(ITK2), QZ(ITK2),
     &    ITK3, QX(ITK3), QY(ITK3), QZ(ITK3),
     &    ITK4, QX(ITK4), QY(ITK4), QZ(ITK4)
      END IF

 1000 FORMAT('F77 TRACK[',I3,'] DAUGHTERS',F7.3,1x,F7.3,1x,F7.3,1X,
     &  '#NDAU:',I1,2X,
     &  'TRK:',I3,1x,F7.3,1x,F7.3,1x,F7.3,1X)
 2000 FORMAT('F77 TRACK[',I3,'] DAUGHTERS',F7.3,1x,F7.3,1x,F7.3,1X,
     &  '#NDAU:',I1,2X,
     &  'TRK:',I3,1x,F7.3,1x,F7.3,1x,F7.3,2X,
     &  'TRK:',I3,1x,F7.3,1x,F7.3,1x,F7.3,1X)
 3000 FORMAT('F77 TRACK[',I3,'] DAUGHTERS',F7.3,1x,F7.3,1x,F7.3,1X,
     &  '#NDAU:',I1,2X,
     &  'TRK:',I3,1x,F7.3,1x,F7.3,1x,F7.3,2X,
     &  'TRK:',I3,1x,F7.3,1x,F7.3,1x,F7.3,2X,
     &  'TRK:',I3,1x,F7.3,1x,F7.3,1x,F7.3,1X)
 4000 FORMAT('F77 TRACK[',I3,'] DAUGHTERS',F7.3,1x,F7.3,1x,F7.3,1X,
     &  '#NDAU:',I1,2X,
     &  'TRK:',I3,1x,F7.3,1x,F7.3,1x,F7.3,2X,
     &  'TRK:',I3,1x,F7.3,1x,F7.3,1x,F7.3,2X,
     &  'TRK:',I3,1x,F7.3,1x,F7.3,1x,F7.3,2X,
     &  'TRK:',I3,1x,F7.3,1x,F7.3,1x,F7.3,1X)
      RETURN
      END

C ======================================================================
      SUBROUTINE F77_PRINT_TRACK_ECAL(WHICH)
      IMPLICIT NONE
#include "qdecl.h"
      INTEGER WHICH, NECAL, ITK
#include "qcde.h"
#include "qmacro.h"

      NECAL = KNECAL(WHICH)
      IF( NECAL .LE. 0 ) THEN
        RETURN
      ENDIF
      ITK = KECAL(WHICH,1)
      IF( NECAL .EQ.1 ) THEN
        WRITE(6,1000,advance='no') WHICH, NECAL, ITK,
     .    KTN(ITK), QE(ITK), QE(ITK)
      ELSE IF( NECAL .EQ.2 ) THEN
        WRITE(6,2000,advance='no') WHICH, NECAL, ITK, KECAL(WHICH,2)
      ELSE IF( NECAL .EQ.3 ) THEN
        WRITE(6,3000,advance='no') WHICH, NECAL, ITK,
     &    KECAL(WHICH,2), KECAL(WHICH,3)
      ELSE IF( NECAL .GE.4 ) THEN
        WRITE(6,4000,advance='no') WHICH, NECAL, ITK,
     &    KECAL(WHICH,2), KECAL(WHICH,3), KECAL(WHICH,4)
      END IF

 1000 FORMAT('F77 TRACK[',I3,'] ECAL OBJS',1x,
     &  '#NEC:',I1,1X,'ECOBJ:',I3,1X,
     &  'PECO:',I3,1x,'E:',F7.3,1X,F7.3,1X)
 2000 FORMAT('F77 TRACK[',I3,'] ECAL OBJS',1x,
     &  '#NEC:',I1,1X,'ECOBJ:',I3,1x,I3)
 3000 FORMAT('F77 TRACK[',I3,'] ECAL OBJS',1x,
     &  '#NEC:',I1,1X,'ECOBJ:',I3,1x,I3,1x,I3,1x)
 4000 FORMAT('F77 TRACK[',I3,'] ECAL OBJS',1x,
     &  '#NEC:',I1,1X,'ECOBJ:',I3,1x,I3,1x,I3,1x,I3,1x)
      RETURN
      END

C ======================================================================
      SUBROUTINE F77_PRINT_TRACK_HCAL(WHICH)
      IMPLICIT NONE
#include "qdecl.h"
      INTEGER WHICH, ITK, NHCAL
#include "qcde.h"
#include "qmacro.h"

      NHCAL = KNHCAL(WHICH)
      IF( NHCAL .LE. 0 ) THEN
        RETURN
      END IF
      ITK = KHCAL(WHICH,1)
      IF( NHCAL .EQ.1 ) THEN
        WRITE(6,1000) NHCAL, ITK, KTN(ITK), QE(ITK)
      ELSE IF( NHCAL .EQ.2 ) THEN
        WRITE(6,2000) NHCAL, ITK, KHCAL(WHICH,2)
      ELSE IF( NHCAL .EQ.3 ) THEN
        WRITE(6,3000) NHCAL, ITK,
     &    KHCAL(WHICH,2), KHCAL(WHICH,3)
      ELSE IF( NHCAL .GE.4 ) THEN
        WRITE(6,4000) NHCAL, ITK,
     &    KHCAL(WHICH,2), KHCAL(WHICH,3), KHCAL(WHICH,4)
      END IF

 1000 FORMAT('HCAL OBJS #NHC:',I1,1X,'HCOBJ:',I3,1X,
     &  'PHCO:',I3,1x,'E:',F7.3)
 2000 FORMAT('HCAL OBJS #NHC:',I1,1X,'HCOBJ:',I3,1x,I3)
 3000 FORMAT('HCAL OBJS #NHC:',I1,1X,'HCOBJ:',I3,1x,I3,1x,I3,1x)
 4000 FORMAT('HCAL OBJS #NHC:',I1,1X,'HCOBJ:',I3,1x,I3,1x,I3,1x,I3,1x)
      RETURN
      END

C ======================================================================
      SUBROUTINE F77_PRINT_PECO(WHICH)
      IMPLICIT NONE
#include "bosext.h"
#include "qdecl.h"
      INTEGER WHICH, KPECO
#include "qcde.h"
#include "qmacro.h"
      KPECO = NLINK('PECO',1)
      WRITE(6,1000) WHICH,
     &  RTABL(KPECO,WHICH,JPECEC),
     &  RTABL(KPECO,WHICH,JPECTH),
     &  RTABL(KPECO,WHICH,JPECPH)

 1000 FORMAT('F77[',I2,']',1x,
     &  'CORR:',F7.3,1X,'THETA:',F5.2,1x,'PHI:',F5.2)
      RETURN
      END

C ======================================================================
      SUBROUTINE F77_PRINT_PHCO(WHICH)
      IMPLICIT NONE
#include "bosext.h"
#include "qdecl.h"
      INTEGER WHICH, KPHCO
#include "qcde.h"
#include "qmacro.h"
      KPHCO = NLINK('PHCO',1)
      WRITE(6,1000) WHICH,
     &  RTABL(KPHCO,WHICH,JPHCEC),
     &  RTABL(KPHCO,WHICH,JPHCTH),
     &  RTABL(KPHCO,WHICH,JPHCPH)

 1000 FORMAT('F77[',I2,']',1x,
     &  'CORR:',F7.3,1X,'THETA:',F5.2,1x,'PHI:',F5.2)
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
