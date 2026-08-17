      FUNCTION XKSECT(ECMI)                                             XKSECT 2
C                                                                       XKSECT 3
C! Returns the cross section value in nb for energy ECM                 XKSECT 4
C! LUND04 version  B.Bloch march  1994                                  XKSECT 5
C                                                                       XKSECT 6
      COMMON / GLUPAR / IFL,IPRI,SVERT(3),ECM                           GLUCOM 2
C     IFL      : LUND flavour , set to 0 by default, can be changed     GLUCOM 3
C     IPRI     : PRINT level flag                                       GLUCOM 4
C     ECM      : nominal cms energy                                     GLUCOM 5
C     SVERT    : vertex smearing, set to 0. by default, can be changed  GLUCOM 6
      COMMON / GLUSTA / ICOULU(10)                                      GLUCOM 7
      CALL LUXTOT(IFL,ECMI,XTOT)                                        XKSECT 8
      XKSECT = XTOT                                                     XKSECT 9
      RETURN                                                            XKSECT10
      END                                                               XKSECT11
