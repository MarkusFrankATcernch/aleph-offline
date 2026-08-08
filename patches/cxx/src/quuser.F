C ======================================================================
      SUBROUTINE get_tracks(values)
      INTEGER*4 values(4)
#include "qcde.h"
      IF( values(1) .EQ.1 )THEN
        values(2) = KFCHT;
        values(3) = KLCHT;
        values(4) = KNCHT;
        RETURN
      ELSE IF( values(1) .EQ. 2 )THEN
        values(2) = KFV0T;
        values(3) = KLV0T;
        values(4) = KNV0T;
        RETURN
      ENDIF

      values(2) = -1;
      values(3) = -1;
      values(4) = -1;
      RETURN
      END

      SUBROUTINE GET_NLIST(RES, JW, IND, NAM)
      INTEGER JW(*), IND
      CHARACTER*4 RES
      CHARACTER*4 NAM
      CHARACTER*4 NLIST
      RES = NLIST(JW, IND, NAM) 
      RETURN
      END

      INTEGER*4 FUNCTION DO_NLINK(JW, NAM, IND)
      INTEGER*4 JW(*), IND
      INTEGER*4 NLINK, NAMIND, AIND
      CHARACTER*4 NAM
      AIND = NAMIND(NAM)
      GET_NLINK = NLINK(NAM, IND)
      RETURN
      END

C ======================================================================
      SUBROUTINE GET_PARTICLE_NAME(CODE, LEN, NAME)
#include "qcdesh.h"
      INTEGER*4       CODE
      INTEGER*4       LENOCC
      CHARACTER* 12   CQPART
      CHARACTER* (*)  NAME
      NAME = CQPART(CODE)
      LEN = LENOCC(NAME)
      RETURN
      END

C ======================================================================
      INTEGER*4 FUNCTION GET_IDNAM()
#include "boscom.h"
      GET_IDNAM = IDNAM
      RETURN
      END

C ======================================================================
      INTEGER*4 FUNCTION GET_NSYST()
#include "boscom.h"
      GET_NSYST = NSYST
      RETURN
      END

      SUBROUTINE SET_ALPHA_OPTS()
      CALL QJOPTR('EF',' ')
      RETURN
      END
      
