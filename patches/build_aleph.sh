# ==================================================================================================
#
#   ALEPH library build
#
#   \author  M.Frank
#   \date    09/07/2026
#   \version 1.0
#
# ==================================================================================================
#
set +x;
exec_echo()  {
    echo "+++ Executing command: $*";
    $*;
}
#
# ==================================================================================================
aleph-do-checkout_package()  {
    curr=`pwd`;
    cd ${ALEPH_BUILD_DIR};
    git_dir=${1}
    repo=${2};
    if test -d ${repo}; then
        cd ${repo};
        git pull origin cmake-build;
	git checkout -b cmake-build;
        cd -;
    else
        git clone ${git_dir}/${repo}.git;
        cd ${repo};
        git fetch       origin cmake-build;
        git checkout    origin/cmake-build;
        git checkout -b cmake-build;
        cd -;
    fi;
    cd ${curr};
}
#
#
# ==================================================================================================
aleph-do-checkout()  {
    aleph-do-checkout_package ssh://git@gitlab.cern.ch:7999/aleph/software inc;
    aleph-do-checkout_package ssh://git@gitlab.cern.ch:7999/aleph/software bos77;
    aleph-do-checkout_package ssh://git@gitlab.cern.ch:7999/aleph/software alephio
    aleph-do-checkout_package ssh://git@gitlab.cern.ch:7999/aleph/software alephlib
    aleph-do-checkout_package ssh://git@gitlab.cern.ch:7999/aleph/software dbase
    aleph-do-checkout_package ssh://git@gitlab.cern.ch:7999/aleph/software kin
    aleph-do-checkout_package ssh://git@gitlab.cern.ch:7999/aleph/software galeph
    aleph-do-checkout_package ssh://git@gitlab.cern.ch:7999/aleph/software julia
    aleph-do-checkout_package ssh://git@gitlab.cern.ch:7999/aleph/software alpha
    aleph-do-checkout_package ssh://git@gitlab.cern.ch:7999/aleph/software phy
    aleph-do-checkout_package ssh://git@gitlab.cern.ch:7999/aleph/software uphy
    aleph-do-checkout_package ssh://git@gitlab.cern.ch:7999/aleph/software look
    aleph-do-checkout_package ssh://git@gitlab.cern.ch:7999/aleph/software mini
    aleph-do-checkout_package ssh://git@gitlab.cern.ch:7999/aleph/software tpcsim
}
#
#
# ==================================================================================================
aleph-patch()   {
    curr=`pwd`;
    cd ${ALEPH_BUILD_DIR};
    if test -f ${ALEPH_BUILD_DIR}/alephlib/ldes/lcalmv.F; then
        mv ${ALEPH_BUILD_DIR}/alephlib/ldes/lcalmv.F ${ALEPH_BUILD_DIR}/alephlib/ldes/lcalmv.F.exclude;
    fi;
    if test -f ${ALEPH_BUILD_DIR}/alephlib/gam/intif4.F; then
        mv ${ALEPH_BUILD_DIR}/alephlib/gam/intif4.F ${ALEPH_BUILD_DIR}/alephlib/ldes/intif4.F.exclude;
    fi;
    if test -f ${ALEPH_BUILD_DIR}/julia/l_/lbdlba.F; then
        mv ${ALEPH_BUILD_DIR}/julia/l_/lbdlba.F ${ALEPH_BUILD_DIR}/julia/l_/lbdlba.F.exclude;
    fi;
    if test -f ${ALEPH_BUILD_DIR}/tpcsim/F77/tpmain.F; then
        mv ${ALEPH_BUILD_DIR}/tpcsim/F77/tpmain.F ${ALEPH_BUILD_DIR}/tpcsim/F77/tpmain.F.exclude;
    fi;
    if test -f ${ALEPH_BUILD_DIR}/galeph/draw/guinti.F; then
        mv ${ALEPH_BUILD_DIR}/galeph/draw/guinti.F ${ALEPH_BUILD_DIR}/galeph/draw/guinti.F.exclude;
    fi;
    if test -f ${ALEPH_BUILD_DIR}/galeph/vdet/agvdet.F; then
        mv ${ALEPH_BUILD_DIR}/galeph/vdet/agvdet.F ${ALEPH_BUILD_DIR}/galeph/vdet/agvdet.F.exclude;
    fi;
    if test -f ${ALEPH_BUILD_DIR}/alpha/pack/qfget_bp.F; then
        mv ${ALEPH_BUILD_DIR}/alpha/pack/qfget_bp.F ${ALEPH_BUILD_DIR}/alpha/pack/qfget_bp.F.exclude;
    fi;
    if test -f ${ALEPH_BUILD_DIR}/alpha/qfn/fit_dmin.F; then
        mv ${ALEPH_BUILD_DIR}/alpha/qfn/fit_dmin.F ${ALEPH_BUILD_DIR}/alpha/qfn/fit_dmin.F.exclude;
    fi;
    if test -f ${ALEPH_BUILD_DIR}/alpha/qfn/btag_fit.F; then
        mv ${ALEPH_BUILD_DIR}/alpha/qfn/btag_fit.F ${ALEPH_BUILD_DIR}/alpha/qfn/btag_fit.F.exclude;
    fi;
    exec_echo cp -r ${ALEPH_BUILD_DIR}/../aleph-offline/patches/* ${ALEPH_BUILD_DIR}/;
    cd ${curr};
}
#
#
# ==================================================================================================
aleph-verify-checkout()  {
    curr=`pwd`;
    cd ${ALEPH_BUILD_DIR};
    echo "+++ Check software base at `pwd`";
    for i in alephio alephlib alpha bos77 dbase galeph inc julia look mini tpcsim uphy; do
        cd $i;
        printf "================================= %s \n" "`pwd`";
        #rm -rf $i;
        
        git status;
        #git checkout -b cmake-build;
	#git commit -a -m "Use CERNLIB from git@gitlab.cern.ch:7999/DPHEP/cernlib";
	#git push origin cmake-build;
        cd ..;
    done;
    cd ${curr};
}
# ==================================================================================================
#
cernlib-install()  {
    curr=`pwd`;
    cernlib_dir=${ALEPH_BUILD_DIR}/cernlib;
    #
    cd ${ALEPH_BUILD_DIR};
    aleph-do-checkout_package ssh://git@gitlab.cern.ch:7999/DPHEP/cernlib cernlib;
    mkdir -p ${cernlib_dir}/build64;
    cd ${cernlib_dir}/build64;
    #
    echo "+++ may need: sudo apt-get install libxaw7-dev";
    echo "+++ may need: sudo apt-get install libnsl2";
    #
    #
    exec_echo \
        cmake -DCMAKE_INSTALL_PREFIX=${cernlib_dir}/install64 \
        -DCERNLIB_BUILD_SHARED=ON \
        -DCERNLIB_POSITION_INDEPENDENT_CODE=ON \
        -DCERNLIB_USE_INTERNAL_LAPACK=ON \
        -DCERNLIB_2022=ON \
        -DLINUX=ON ..;
    make -j 33 install;
    cd ${curr};
}
# ==================================================================================================
#
aleph-gen-headers()  {
    curr=`pwd`;
    mkdir -p ${ALEPH_BUILD_DIR}/build64;
    cd ${ALEPH_BUILD_DIR}/build64;
    LBF_OPT=;
    if test -n "${SBANK_LBF}"; then
	LBF_OPT="-l ${SBANK_LBF}";
    fi;
    python ../dbase/scripts/gen_header.py -o ../aleph_headers/alpha -a ${LBF_OPT};
    cd ${curr};
}
# ==================================================================================================
#
aleph-install()  {
    curr=`pwd`;
    mkdir -p ${ALEPH_BUILD_DIR}/build64;
    cd ${ALEPH_BUILD_DIR}/build64;
    #
    install=`pwd`/../install64;
    exec_echo \
    cmake  -DCMAKE_MODULE_PATH=${LCG_VIEW}                  \
	   -DCERNLIB_DIR=${CERNLIB_DIR}/share/cernlib/cmake \
	   -DCMAKE_INSTALL_PREFIX=`realpath ${install}`     \
	   ..;
    make -j 33 install;
    cd ${curr};
}
# ==================================================================================================
#
aleph-build()  {
    mkdir -p ${ALEPH_BUILD_DIR};
    #
    aleph-do-checkout;
    #
    aleph-patch;
    #
    . ${LCG_VIEW}/setup.sh;
    #
    #
    # cernlib-install;
    #
    aleph-gen-headers;
    #
    aleph-install;
    #
    #
    aleph-cards;
    cd ${ALEPH_BUILD_DIR}/build64;
    ./alpha/alpha;
}
# ==================================================================================================
aleph-pathes()  {
    . ${LCG_VIEW}/setup.sh;
    if test -z "${LD_LIBRARY_PATH}"; then
	export LD_LIBRARY_PATH=${ALEPH_BUILD_DIR}/install64/lib:${CERNLIB_DIR}/lib;
    else
	export LD_LIBRARY_PATH=${LD_LIBRARY_PATH}:${ALEPH_BUILD_DIR}/install64/lib:${CERNLIB_DIR}/lib;
    fi;
    #
    if test -z "${PATH}"; then
	export PATH=${ALEPH_BUILD_DIR}/install64/lib:${CERNLIB_DIR}/bin;
    else
	export PATH=${PATH}:${ALEPH_BUILD_DIR}/install64/bin:${CERNLIB_DIR}/bin;
    fi;
}
# ==================================================================================================
#
aleph-info()  {
    echo "+++ LCG_VIEW:                   ${LCG_VIEW}";
    echo "+++ CERNLIB_DIR:                ${CERNLIB_DIR}";
    echo "+++ ALEPH_BUILD_DIR:            ${ALEPH_BUILD_DIR}";
    echo "+++ ALEPH_SOFT:                 ${ALEPH_SOFT}";
    echo "+++ ALEPH_DBASE:                ${ALEPH_DBASE}";
    echo "+++ ";
    
    echo "+++ Shortcut commands:";
    echo "+++      aleph-build            to build and checkout in the current working directory.";
    echo "+++      aleph-install          to build aleph stuff only.";
    echo "+++      aleph-pathes           to set PATH and LD_LIBRARY_PATH";
    echo "+++      aleph-patch            to patch checkouts";
    echo "+++      aleph-gen-headers      to generate header files corresponding to BOS banks";
    echo "+++      aleph-verify-checkout  check sttaus all subdirectories known.";
    echo "+++      cernlib-install        to build cernlib standalone with cmake build.";
}
# ==================================================================================================
#
aleph-cards()  {
    export ALPHACARDS=${ALEPH_BUILD_DIR}/cards/analysis.cards;
    export KINGALCARDS=${ALEPH_BUILD_DIR}/cards/pyth05.cards;
    export GALEPHCARDS=${ALEPH_BUILD_DIR}/cards/galeph.cards;
    export JULIACARDS=${ALEPH_BUILD_DIR}/cards/julia.cards;
    echo   "+++ ALPHACARDS:        ${ALPHACARDS}";
    echo   "+++ KINGALCARDS:       ${KINGALCARDS}";
    echo   "+++ GALEPHCARDS:       ${GALEPHCARDS}";
    echo   "+++ JULIACARDS:        ${JULIACARDS}";
}
#
# ==================================================================================================
aleph-tests()  {
    aleph-cards;
    #
    #
    rm -f pyth05-test.epio galeph-test.epio julia-test.epio;
    rm -f pyth05.log       galeph.log       julia.log        alpha.log;
    #
    #
    KINGALCARDS=${ALEPH_BUILD_DIR}/cards/pyth05.cards ${ALEPH_BUILD_DIR}/build64/kin/pyth05 2>&1 > pyth05.log;
    if test -z "`grep 'Kingal start event      2000' pyth05.log`"; then
        echo "+++ KINGAL step with pythia 5 FAILED. Log file: pyth05.log";
        return;
    fi;
    echo "+++ KINGAL step with pythia 5 SUCCEEDED. Log file: pyth05.log";
    if test -e pyth05-test.epio; then
        GALEPHCARDS=${ALEPH_BUILD_DIR}/cards/galeph.cards ${ALEPH_BUILD_DIR}/build64/galeph/galeph 2>&1 > galeph.log;
    fi;
    if test -z "`grep '+++ASIEVE+++ EVENT#  1000' galeph.log`"; then
        echo "+++ GALEPH simulation step FAILED. Log file: galeph.log";
        return;
    fi;
    echo "+++ GALEPH simulation step SUCCEEDED. Log file: galeph.log";

    if test -e galeph-test.epio; then
        JULIACARDS=${ALEPH_BUILD_DIR}/cards/julia.cards ${ALEPH_BUILD_DIR}/build64/julia/julia 2>&1 > julia.log;
    fi;
    if test -z "`grep 'RLOOPR-Evt  1000' julia.log`"; then
        echo "+++ JULIA reconstruction step FAILED. Log file: julia.log";
        return;
    fi;
    echo "+++ JULIA reconstruction step SUCCEEDED. Log file: julia.log";
    if test -e julia-test.epio; then
        ALPHACARDS=${ALEPH_BUILD_DIR}/cards/alpha.cards ${ALEPH_BUILD_DIR}/build64/alpha/alpha 2>&1 > alpha.log;
    fi;
    if test -z "`grep 'KNEVT :      3' alpha.log`"; then
        echo "+++ ALPHA analysis step FAILED. Log file: alpha.log";
        return;
    fi;
    echo "+++ ALPHA analysis step SUCCEEDED. Log file: alpha.log";
}
#
# ==================================================================================================
#
if test -n "`uname -a | grep -e 'lxplus.*cern.ch'`"; then
    #
    export LCG_VIEW=/cvmfs/sft.cern.ch/lcg/views/LCG_110/x86_64-el9-gcc15-opt;
    export CERNLIB_DIR=/cvmfs/dphep.cern.ch/cernlib/releases/almalinux-9-x86_64/cm/std/gcc/new;
    export SBANK_LBF=/eos/experiment/aleph/sw/reference/doc/sbank.lbf;
    export ALEPH_SOFT=/afs/cern.ch/work/f/frankb/frankm/Aleph/offline;
    export ALEPH_DBASE=/eos/experiment/aleph/sw/Linux;
    #
elif test -n "`uname -a | grep Ubuntu`"; then
    #
    export LCG_VIEW=/cvmfs/sft.cern.ch/lcg/views/LCG_110/x86_64-ubuntu2404-gcc13-opt;
    export CERNLIB_DIR=/cvmfs/dphep.cern.ch/cernlib/releases/ubuntu-24-x86_64/cm/std/gcc/new;
    export ALEPH_SOFT=${HOME}/Aleph/offline;
    export ALEPH_DBASE=${ALEPH_SOFT}/Linux;
    #
fi;
#
#
#
export ALEPH_BUILD_DIR=${ALEPH_SOFT}/gitlab;
#
export ADBSCONS=${ALEPH_DBASE}/dbase/adbs314.daf;
export BANKALFMT=${ALEPH_DBASE}/../reference/dbase/bankal.fmt;
export BOSKEY=${ALEPH_DBASE}/../reference/dbase/boskey.ddl;
export DBASBANK=${ALEPH_DBASE}/../reference/phy/dbas.bank;
export BEAMPOSITION=${ALEPH_DBASE}/../reference/phy/boskey.ddl;
#
aleph-info;
# ==================================================================================================
