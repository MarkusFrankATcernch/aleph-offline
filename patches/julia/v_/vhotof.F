*DK vhotof
      SUBROUTINE VHOTOF(IADDR,IFLAG)
C----------------------------------------------------------------------
C!  - Flag as NOISY all strips contained in the hot strips list VHOT
C!
C?
C!======================================================================
#ifndef DOC
#include "implicit.h"
#include "vdimpl.h"
#include "vhotjj.h"
C
C  Global includes
C
#include "bcs.h"
#include "vdflgs.h"
#include "vdnamc.h"
#include "vdhotc.h"
C
C  Inputs
C
      INTEGER IFLAG(*)
      INTEGER IADDR
C
C  Functions
C
      INTEGER NLINK
C
C  Local variables
C
      INTEGER NVHOT
      INTEGER IND, ISTRP
      INTEGER NHOT, IHOT
      INTEGER NSTRP,ILAY,IWAF,IPHI,IVIEW
      INTEGER INDEX
C
C  Variables for inline functions
C
      INTEGER I, J, K, ISSTP
      INTEGER IAND,ISHFT,IOR
      INTEGER VSSTRP, VMSTRP

#include "vdbmcr.h"
#include "bmacro.h"
      ISSTP(I) = IAND(ISHFT(I,-VSSTRP),VMSTRP)
C--------------------------------------------------------------
C  Find the VHOT bank
C
      NVHOT = IW(NAVHOT)
      IF(NVHOT.EQ.0)THEN
        CALL RERROR('VHOTOF',1,'Failed to find VHOT bank')
        RETURN
      END IF
      NHOT = LROWS(NVHOT)
C
C  Find the index of this wafer
C
      CALL VADDUN(IADDR,NSTRP,ILAY,IWAF,IPHI,IVIEW,ISTRP)
C
C  Loop over all the hot strips in this wafer, flagging the ones which
C  were hit this event
C
      IF(IHOTFR(ILAY,IWAF,IPHI,IVIEW).GT.0)THEN
        DO 10 IHOT=IHOTFR(ILAY,IWAF,IPHI,IVIEW),
     &             IHOTLA(ILAY,IWAF,IPHI,IVIEW)
          ISTRP = ISSTP(ITABL(NVHOT,IHOT,JVHOHA))
C
C  Check to see if this hot strip was read out this event- IE, has a
C  flag>0.  This must be checked as opposed to PH=/0, as it's possible
C  to readout a PH of 0.
C
          IF(IFLAG(ISTRP).GT.0)THEN
C
C  Set the hot channel flag
C
            IFLAG(ISTRP) = IOR(IFLAG(ISTRP),VBHOTC)
          END IF
   10   CONTINUE
      END IF
      RETURN
      END
#endif
