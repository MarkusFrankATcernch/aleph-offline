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
        git pull;
        cd -;
    else
        git clone ${git_dir}/${repo}.git;
        cd ${repo};
        git fetch    origin cmake-build;
        git checkout origin/cmake-build;
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
    aleph-do-checkout_package ssh://git@gitlab.cern.ch:7999/aleph/software alephlib
    aleph-do-checkout_package ssh://git@gitlab.cern.ch:7999/aleph/software dbase
    aleph-do-checkout_package ssh://git@gitlab.cern.ch:7999/aleph/software alephio
    aleph-do-checkout_package ssh://git@gitlab.cern.ch:7999/aleph/software alpha
    aleph-do-checkout_package ssh://git@gitlab.cern.ch:7999/aleph/software julia
    aleph-do-checkout_package ssh://git@gitlab.cern.ch:7999/aleph/software galeph
    aleph-do-checkout_package ssh://git@gitlab.cern.ch:7999/aleph/software look
    aleph-do-checkout_package ssh://git@gitlab.cern.ch:7999/aleph/software mini
    aleph-do-checkout_package ssh://git@gitlab.cern.ch:7999/aleph/software tpcsim
    aleph-do-checkout_package ssh://git@gitlab.cern.ch:7999/aleph/software uphy
}
#
#
# ==================================================================================================
patch-checkout()   {
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
    exec_echo cp -r ${ALEPH}/aleph-offline/patches/* ${ALEPH_BUILD_DIR}/;
    cd ${curr};
}
#
#
# ==================================================================================================
verify-checkout()  {
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
	#git push -f origin cmake-build;
        cd ..;
    done;
    cd ${curr};
}
# ==================================================================================================
#
install-cernlib()  {
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
gen-headers-aleph()  {
    curr=`pwd`;
    mkdir -p ${ALEPH_BUILD_DIR}/build64;
    cd ${ALEPH_BUILD_DIR}/build64;
    python ../dbase/scripts/gen_header.py -o ../aleph_headers/alpha -a;
    cd ${curr};
}
# ==================================================================================================
#
install-aleph()  {
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
build-aleph()  {
    mkdir -p ${ALEPH_BUILD_DIR};
    #
    aleph-do-checkout;
    #
    patch-checkout;
    #
    . ${LCG_VIEW}/setup.sh;
    #
    #
    # install-cernlib;
    #
    gen-headers-aleph;
    #
    install-aleph;
    #
    #
    cd ${ALEPH_BUILD_DIR}/build64;
    ./alpha/alpha.124;
}
# ==================================================================================================
aleph-pathes()  {
    . ${CERNLIB_DIR}/setup.sh;
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
aleph-info()  {
    echo "+++ LCG_VIEW:              ${LCG_VIEW}";
    echo "+++ CERNLIB_DIR:           ${CERNLIB_DIR}";
    echo "+++ ALEPH_BUILD_DIR:       ${ALEPH_BUILD_DIR}";
    echo "+++ type build-aleph       to build and checkout in the current working directory.";
    echo "+++      install-cernlib   to build cernlib only.";
    echo "+++      install-aleph     to build aleph stuff only.";
    echo "+++      aleph-pathes      to set PATH and LD_LIBRARY_PATH";
    echo "+++      patch-checkout    to patch checkouts";
    echo "+++      gen-headers-aleph to generate header files corresponding to BOS banks";
}
# ==================================================================================================
. /home/frankm/Aleph/offline/setaleph.sh;
export LCG_VIEW=/cvmfs/sft.cern.ch/lcg/views/LCG_110/x86_64-ubuntu2404-gcc13-opt;
export CERNLIB_DIR=/cvmfs/dphep.cern.ch/cernlib/releases/ubuntu-24-x86_64/cm/std/gcc/new;
export ALEPH_BUILD_DIR=${ALEPH}/gitlab;
aleph-info;
# ==================================================================================================
