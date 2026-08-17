      SUBROUTINE USKRIN(EI)                                             USKRIN 2
C                                                                       USKRIN 3
C! Reinitialise the generator with energy  EI and reinit some quantitiesUSKRIN 4
C! LUND04 version  B.Bloch march  1994                                  USKRIN 5
C                                                                       USKRIN 6
      INTEGER LMHLEN, LMHCOL, LMHROW  ,LBCS                             BCS    2
      PARAMETER (LMHLEN=2, LMHCOL=1, LMHROW=2, LBCS=1000)               BCS    3
C                                                                       BCS    4
      COMMON /BCS/   IW(LBCS )                                          BCS    5
      INTEGER IW                                                        BCS    6
      REAL RW(LBCS)                                                     BCS    7
      EQUIVALENCE (RW(1),IW(1))                                         BCS    8
C                                                                       BCS    9
      COMMON / GLUPAR / IFL,IPRI,SVERT(3),ECM                           GLUCOM 2
C     IFL      : LUND flavour , set to 0 by default, can be changed     GLUCOM 3
C     IPRI     : PRINT level flag                                       GLUCOM 4
C     ECM      : nominal cms energy                                     GLUCOM 5
C     SVERT    : vertex smearing, set to 0. by default, can be changed  GLUCOM 6
      COMMON / GLUSTA / ICOULU(10)                                      GLUCOM 7
      DIMENSION TABL(25)                                                USKRIN 9
      INTEGER ALTABL                                                    USKRIN10
      EXTERNAL ALTABL                                                   USKRIN11
      ECM = EI                                                          USKRIN12
C Get KPAR bank, modify                                                 USKRIN13
      IKPAR= NLINK('KPAR',0)                                            USKRIN14
      IF (IKPAR.GT.0) THEN                                              USKRIN15
         NCO = IW(IKPAR+LMHCOL)                                         USKRIN16
         CALL UCOPY(RW(IKPAR+LMHLEN+1),TABL(1),NCO)                     USKRIN17
         CALL BDROP(IW ,'KPAR')                                         USKRIN18
         TABL(2) = EI                                                   USKRIN19
         IND = ALTABL('KPAR',NCO,1,TABL,'2I,(F)','C')                   USKRIN20
         CALL PRTABL('KPAR',0)                                          USKRIN21
      ENDIF                                                             USKRIN22
      CALL VZERO(ICOULU,10)                                             USKRIN23
      RETURN                                                            USKRIN24
      END                                                               USKRIN25
