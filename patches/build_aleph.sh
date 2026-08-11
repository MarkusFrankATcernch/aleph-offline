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
do_checkout_package()  {
    curr=`pwd`;
    cd ${BUILD_DIR};
    git_dir=${1}
    repo=${2};
    if test -d ${repo}; then
        cd ${repo};
        git pull;
        cd -;
    else
        git clone ${git_dir}/${repo}.git;
    fi;
    cd ${curr};
}
#
#
# ==================================================================================================
do-checkout()  {
    do_checkout_package ssh://git@gitlab.cern.ch:7999/DPHEP/cernlib  cernlib;
    do_checkout_package ssh://git@gitlab.cern.ch:7999/aleph/software inc;
    do_checkout_package ssh://git@gitlab.cern.ch:7999/aleph/software bos77;
    do_checkout_package ssh://git@gitlab.cern.ch:7999/aleph/software alephlib
    do_checkout_package ssh://git@gitlab.cern.ch:7999/aleph/software dbase
    do_checkout_package ssh://git@gitlab.cern.ch:7999/aleph/software alephio
    do_checkout_package ssh://git@gitlab.cern.ch:7999/aleph/software alpha
    do_checkout_package ssh://git@gitlab.cern.ch:7999/aleph/software julia
    do_checkout_package ssh://git@gitlab.cern.ch:7999/aleph/software galeph
    do_checkout_package ssh://git@gitlab.cern.ch:7999/aleph/software look
    do_checkout_package ssh://git@gitlab.cern.ch:7999/aleph/software mini
    do_checkout_package ssh://git@gitlab.cern.ch:7999/aleph/software tpcsim
    do_checkout_package ssh://git@gitlab.cern.ch:7999/aleph/software uphy
}
#
#
# ==================================================================================================
patch-checkout()   {
    curr=`pwd`;
    cd ${BUILD_DIR};
    if test -f ${BUILD_DIR}/alephlib/ldes/lcalmv.F; then
        mv ${BUILD_DIR}/alephlib/ldes/lcalmv.F ${BUILD_DIR}/alephlib/ldes/lcalmv.F.exclude;
    fi;
    if test -f ${BUILD_DIR}/alephlib/gam/intif4.F; then
        mv ${BUILD_DIR}/alephlib/gam/intif4.F ${BUILD_DIR}/alephlib/ldes/intif4.F.exclude;
    fi;
    if test -f ${BUILD_DIR}/julia/l_/lbdlba.F; then
        mv ${BUILD_DIR}/julia/l_/lbdlba.F ${BUILD_DIR}/julia/l_/lbdlba.F.exclude;
    fi;
    if test -f ${BUILD_DIR}/tpcsim/F77/tpmain.F; then
        mv ${BUILD_DIR}/tpcsim/F77/tpmain.F ${BUILD_DIR}/tpcsim/F77/tpmain.F.exclude;
    fi;
    if test -f ${BUILD_DIR}/galeph/draw/guinti.F; then
        mv ${BUILD_DIR}/galeph/draw/guinti.F ${BUILD_DIR}/galeph/draw/guinti.F.exclude;
    fi;
    if test -f ${BUILD_DIR}/galeph/vdet/agvdet.F; then
        mv ${BUILD_DIR}/galeph/vdet/agvdet.F ${BUILD_DIR}/galeph/vdet/agvdet.F.exclude;
    fi;
    if test -f ${BUILD_DIR}/alpha/pack/qfget_bp.F; then
        mv ${BUILD_DIR}/alpha/pack/qfget_bp.F ${BUILD_DIR}/alpha/pack/qfget_bp.F.exclude;
    fi;
    if test -f ${BUILD_DIR}/alpha/qfn/fit_dmin.F; then
        mv ${BUILD_DIR}/alpha/qfn/fit_dmin.F ${BUILD_DIR}/alpha/qfn/fit_dmin.F.exclude;
    fi;
    if test -f ${BUILD_DIR}/alpha/qfn/btag_fit.F; then
        mv ${BUILD_DIR}/alpha/qfn/btag_fit.F ${BUILD_DIR}/alpha/qfn/btag_fit.F.exclude;
    fi;
    exec_echo cp -r ${ALEPH}/aleph-offline/patches/* ${BUILD_DIR}/;
    cd ${curr};
}
#
#
# ==================================================================================================
verify-checkout()  {
    curr=`pwd`;
    cd ${BUILD_DIR};
    if test -d ./cernlib; then
	echo "" > /dev/null;
    elif test -d ../cernlib; then
	cd ..;
    fi;
    echo "+++ Check software base at `pwd`";
    for i in alephio alephlib alpha bos77 dbase galeph inc julia look mini tpcsim uphy;
    do
        cd $i;
        printf "================================= %s \n" "`pwd`";
        git status;
	git commit -a -m "Use CERNLIB from git@gitlab.cern.ch:7999/DPHEP/cernlib";
	git push origin cmake-build;
        cd ..;
    done;
    cd ${curr};
}
# ==================================================================================================
#
install-cernlib()  {
    curr=`pwd`;
    cernlib_dir=${BUILD_DIR}/cernlib;
    mkdir -p ${cernlib_dir}/build64;
    cd ${cernlib_dir}/build64;
    #
    echo "+++ may need: sudo apt-get install libxaw7-dev ";
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
    mkdir -p ${BUILD_DIR}/build64;
    cd ${BUILD_DIR}/build64;
    python ../dbase/scripts/gen_header.py -o ../aleph_headers/alpha -a;
    cd ${curr};
}
#
install-aleph()  {
    curr=`pwd`;
    mkdir -p ${BUILD_DIR}/build64;
    cd ${BUILD_DIR}/build64;
    #
    exec_echo \
    cmake  -DCMAKE_MODULE_PATH=/cvmfs/sft.cern.ch/lcg/views/LCG_110/x86_64-ubuntu2404-gcc13-opt\
	   -DCERNLIB_DIR=${BUILD_DIR}/cernlib/install64/share/cernlib/cmake \
	   -DCMAKE_INSTALL_PREFIX=`pwd`/../install64 \
	   ..;
    make -j 33 install;
    cd ${curr};
}
# ==================================================================================================
#
export BUILD_DIR=${ALEPH}/gitlab;
build-aleph()  {
    mkdir -p ${BUILD_DIR};
    #
    do-checkout;
    #
    patch-checkout;
    #
    . /cvmfs/sft.cern.ch/lcg/views/LCG_110/x86_64-ubuntu2404-gcc13-opt/setup.sh;
    #
    #
    install-cernlib;
    #
    gen-headers-aleph;
    #
    install-aleph;
    #
    #
    cd ${BUILD_DIR}/build64;
    ./alpha/alpha.124;
}

# ==================================================================================================
echo "+++ type build-aleph to build and checkout in the current working directory.";
# ==================================================================================================
