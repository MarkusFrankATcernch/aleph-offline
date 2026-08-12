#!/bin/tcsh
#export SHELL=/etc/tcsh
export ALEPH_ROOT=/eos/experiment/aleph/sw
#export ALEPH_ROOT=/cvmfs/aleph.cern.ch
export ALEPH=${ALEPH_ROOT}/Linux
#export FC="g77"
export FC="gfortran"
export FCOPT="-c -O -fno-automatic -fdollar-ok -fno-backslash -DUNIX -DALEPH_LINUX -I$ALEPH_ROOT/reference/cvs/inc"
export ALPOPT="-c -O -fno-automatic -fdollar-ok -fno-backslash -DUNIX -DALEPH_LINUX -I$ALEPH_ROOT/reference/cvs/inc -I$ALEPH_ROOT/shared/src/alpha/inc"
export STAGELABEL=sl
export STAGEFSEQ=1
export STAGE_USER=aleph
export STAGERECFM=F
export STAGELRECL=32040
export STAGEBLKSIZ=32040
export STAGE_HOST=stagealeph

export ALEPH_HOME=/home/frankm/Aleph/offline;
#
#
if test -e ${ALEPH_HOME}; then
    export ALEPH=${ALEPH_HOME};
    echo "ALEPH home directory: ${ALEPH}";
elif test -e /aleph; then
    export ALEPH=/aleph;
    echo "ALEPH home directory: ${ALEPH}";
else
    echo "No ALEPH home directory!";
fi;

export ALEPH_ROOT=${ALEPH}
export ALBOOK=${ALEPH_ROOT}/book

export ADBSCONS=${ALEPH}/Linux/dbase/adbscons.daf
export ADBSCONS=${ALEPH}/Linux/dbase/adbs256.daf
export ADBSTEST=${ALEPH}/Linux/dbase/adbstest.daf
export ADBS8990=${ALEPH}/Linux/dbase/adbs8990.daf

export DBASBANK=${ALEPH}/phy/dbas.bank
export BANKALFMT=${ALEPH}/DBASE/bankal.fmt
export data_base=${ALEPH}/DBASE/adbs314.daf
export GENATTR=${ALEPH}/DBASE/genattr.ddl
export BOSKEY=${ALEPH}/DBASE/boskey.ddl
export DBASBANK=${ALEPH}/phy/dbas.bank

export BEAMPOSITION=${ALEPH}/phy/beam.position
export ALPHACARDS=${ALEPH}/phy/alpha.cards
export ALPHACARDS=${ALEPH}/phy/analysis.cards
export KINGALCARDS=${ALEPH}/kin/korl08.cards
export GALEPHCARDS=${ALEPH}/Cards/GALEPH.cards
export JULIACARDS=/opt/JULIA/julia.cards

export REFERENCE=${ALEPH_ROOT}/reference
export SHARED=${ALEPH_ROOT}/shared
export CERN=/eos/experiment/aleph/sw/cern
export CERN_LEVEL=2006
export CERN_ROOT=$CERN/$CERN_LEVEL
export TAPESCONF=${ALEPH_ROOT}/shared/etc/tapes.conf
export CVSROOT=${ALEPH_ROOT}/reference/cvsmaster
export ALROOT=${ALEPH_ROOT}/reference/cvs
export ALINC=${ALEPH_ROOT}/reference/cvs/inc
export ALEDIR=${ALEPH_ROOT}/shared/edirlink
export ALDOC=${ALEPH_ROOT}/doc
export ALBOOK=${ALEPH_ROOT}/reference/book
export PHYINC=${ALEPH_ROOT}/reference/phy
export KINAGAINCARDS=${ALEPH_ROOT}/reference/kin/kinagain.cards
export LOOKCARDS=${ALEPH_ROOT}/reference/gen/look.cards
export RUNCARTSLIST=${ALEPH_ROOT}/reference/book/runcarts.list

echo "+++ ALPHACARDS:        ${ALPHACARDS}";
echo "+++ KINGALCARDS:       ${KINGALCARDS}";
echo "+++ GALEPHCARDS:       ${GALEPHCARDS}";
echo "+++ JULIACARDS:        ${JULIACARDS}";
echo "+++ ADBSCONS:          ${ADBSCONS}";
echo "+++ DBASBANK:          ${DBASBANK}";
echo "+++ BANKALFMT:         ${BANKALFMT}";
echo "+++ BEAMPOSITION:      ${BEAMPOSITION}";




export ALSTOUT=aldataout
export DPMSIZE=200
export DPMUSER=aleph
export STAGELABEL=sl
export STAGEFSEQ=1
export STAGE_HOST=stagealeph
export STAGE_USER=aleph
export STAGERECFM=F
export STAGELRECL=32040
export STAGEBLKSIZ=32040
#export STAGECLEAN=${ALEPH_ROOT}/shared/script/stage-clean-sh
export RETRYCOUNT=10
export ALDATA=${ALEPH_ROOT}/shared/data
export ALSTAGE=${ALEPH_ROOT}/shared/data
export ALPVER=126
export GALVER=309
export JULVER=313
##export HEP_ENV=/etc/tcsh
##export PATH=.:${ALEPH_ROOT}/shared/script/:{$PATH}
##export PATH=${ALEPH}/bin/:{$PATH}
##export PATH=${ALEPH_ROOT}/i386_redhat42/gal/:{$PATH}
##export PATH=${ALEPH_ROOT}/i386_redhat42/jul/:{$PATH}
##export PATH=${ALEPH_ROOT}/i386_redhat42/dali/:{$PATH}
##export PATH={$PATH}:/cern/pro/bin
