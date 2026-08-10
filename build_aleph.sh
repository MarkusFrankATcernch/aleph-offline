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
#
# ==================================================================================================
do_checkout_package()  {
    repo=${1};
    if test -d ${repo}; then
        cd ${repo};
        git pull;
    else
        git clone ssh://git@gitlab.cern.ch:7999/aleph/software/${repo}.git;
    fi;
}
#
#
# ==================================================================================================
do_checkout()  {
    do_checkout_package inc;
    do_checkout_package bos77;
    do_checkout_package alephlib
    do_checkout_package dbase
    do_checkout_package alephio
    do_checkout_package alpha
    do_checkout_package julia
    do_checkout_package galeph
    do_checkout_package look
    do_checkout_package mini
    do_checkout_package tpcsim
    do_checkout_package uphy
}
#
#
# ==================================================================================================
patch_checkout()   {
    if test -f ../alephlib/ldes/lcalmv.F; then
        mv ../alephlib/ldes/lcalmv.F alephlib/ldes/lcalmv.F.exclude;
    fi;
    if test -f alephlib/ldes/intof4.F; then
        mv ../alephlib/ldes/intof4.F alephlib/ldes/intof4.F.exclude;
    fi;
    cp -r ${ALEPH}/aleph-offline/patches/* ../;
}
#
#
# ==================================================================================================
verify_checkout()  {
    curr=`pwd`;
    cd ${ALEPH};
    for i in alephio alephlib alpha bos77 dbase galeph inc julia look mini tpcsim uphy;
    do
        cd $i;
        printf "================================= %-16s =================================\n" ${i};
        git status;
        cd ..;
    done;
    cd ${curr};
}
# ==================================================================================================
#
install_cernlib()  {
    curr=`pwd`;
    cd ${ALEPH};

    echo "+++ may need: sudo apt-get install libxaw7-dev ";
    echo "+++ may need: sudo apt-get install libnsl2";

    wget https://cernlib.web.cern.ch/download/2026_source/tar/cernlib-cernlib-2026.01.08.0-free.tar.gz;
    tar -xf cernlib-cernlib-2026.01.08.0-free.tar.gz;
    mkdir -p cernlib-cernlib-2026.01.08.0-free/build;
    cd cernlib-cernlib-2026.01.08.0-free/build;
    cmake -DCMAKE_INSTALL_PREFIX=`pwd`/../install ..;
    make -j 33 install;
    cd ${curr};
}
#
cd ${ALEPH};
mkdir -p gitlab;
cd    gitlab;
#
do_checkout;
#
cp -r ../cernlib ./;
#
patch_checkout;
#
mkdir -p build64;
cd build64;
#

#
#
. /cvmfs/sft.cern.ch/lcg/views/LCG_110/x86_64-ubuntu2404-gcc13-opt/setup.sh;
#
python ../dbase/scripts/gen_header.py -o ../aleph_headers/alpha -a;
#
cmake  -DCMAKE_MODULE_PATH=/cvmfs/sft.cern.ch/lcg/views/LCG_110/x86_64-ubuntu2404-gcc13-opt\
       -DCERNLIB_DIR=${ALEPH}/CERN/cernlib-cernlib-2026.01.08.0-free/install/share/cernlib/cmake \
       -DCMAKE_INSTALL_PREFIX=`pwd`/../install64 \
       ..;
#
#
make -j 33 install;
./alpha/alpha.124;

# ==================================================================================================
