**Patches to the ALEPH offline software**

Found necessary changes to the Aleph offline software to 
build and exceute the full processing chain:
- **Kingal** step to generate MC events
- **Galeph** step to simulate the detector response
- **Julia** step to reconstruct the simulated events
- **Aleph** step to read the reconstructed data


**Aleph** repositories are:
- ssh://git@gitlab.cern.ch:7999/aleph/software/inc;
- ssh://git@gitlab.cern.ch:7999/aleph/software/bos77;
- ssh://git@gitlab.cern.ch:7999/aleph/software/alephio
- ssh://git@gitlab.cern.ch:7999/aleph/software/alephlib
- ssh://git@gitlab.cern.ch:7999/aleph/software/dbase
- ssh://git@gitlab.cern.ch:7999/aleph/software/kin
- ssh://git@gitlab.cern.ch:7999/aleph/software/galeph
- ssh://git@gitlab.cern.ch:7999/aleph/software/julia
- ssh://git@gitlab.cern.ch:7999/aleph/software/alpha
- ssh://git@gitlab.cern.ch:7999/aleph/software/phy
- ssh://git@gitlab.cern.ch:7999/aleph/software/uphy
- ssh://git@gitlab.cern.ch:7999/aleph/software/look
- ssh://git@gitlab.cern.ch:7999/aleph/software/mini
- ssh://git@gitlab.cern.ch:7999/aleph/software/tpcsim

The changes are incorporated in the braches cmake-build.

Steps to build the Aleph offline software
```
$> mkdir -p Aleph/offline
$> cd Aleph/offline
$> git clone  gitlab ssh://git@gitlab.cern.ch:7999/frankm/aleph-offline.git
$>
$> # Edit this file:  
$> # Set ALEPH_SOFT=<current directory>
#> #
$> . aleph-offline/patches/build_aleph.ssh
$> # build all software (cross fingers)
$> aleph-build

$> # To test the full chain:
$> cd $ALEPH_BUILD_DIR/build64;
$> aleph-tests
```
If all works well, you should see the following output (current directory=/afs/cern.ch/work/f/frankb/frankm/Aleph/offline):

```
[frankm@lxplus9111 offline]$ pwd
/afs/cern.ch/work/f/frankb/frankm/Aleph/offline

[frankm@lxplus9111 offline]$ aleph-tests
+++ ALPHACARDS:        /afs/cern.ch/work/f/frankb/frankm/Aleph/offline/gitlab/cards/analysis.cards
+++ KINGALCARDS:       /afs/cern.ch/work/f/frankb/frankm/Aleph/offline/gitlab/cards/pyth05.cards
+++ GALEPHCARDS:       /afs/cern.ch/work/f/frankb/frankm/Aleph/offline/gitlab/cards/galeph.cards
+++ JULIACARDS:        /afs/cern.ch/work/f/frankb/frankm/Aleph/offline/gitlab/cards/julia.cards
rm: cannot remove 'pyth05-test.epio': No such file or directory
+++ KINGAL step with pythia 5 SUCCEEDED. Log file: pyth05.log
rm: cannot remove 'galeph-test.epio': No such file or directory
+++ GALEPH simulation step SUCCEEDED. Log file: galeph.log
rm: cannot remove 'julia-test.epio': No such file or directory
+++ JULIA reconstruction step SUCCEEDED. Log file: julia.log
+++ ALPHA analysis step SUCCEEDED. Log file: alpha.log

```

For detail consult the above mentioned log files.

To see the setup:
```
[frankm@lxplus9111 offline]$ aleph-info
+++ LCG_VIEW:                   /cvmfs/sft.cern.ch/lcg/views/LCG_110/x86_64-el9-gcc15-opt
+++ CERNLIB_DIR:                /cvmfs/dphep.cern.ch/cernlib/releases/almalinux-9-x86_64/cm/std/gcc/new
+++ ALEPH_BUILD_DIR:            /afs/cern.ch/work/f/frankb/frankm/Aleph/offline/gitlab
+++ ALEPH_SOFT:                 /afs/cern.ch/work/f/frankb/frankm/Aleph/offline
+++ ALEPH_DBASE:                /eos/experiment/aleph/sw/Linux
+++ 
+++ Shortcut commands:
+++      aleph-build            to build and checkout in the current working directory.
+++      aleph-install          to build aleph stuff only.
+++      aleph-pathes           to set PATH and LD_LIBRARY_PATH
+++      aleph-patch            to patch checkouts
+++      aleph-gen-headers      to generate header files corresponding to BOS banks
+++      aleph-verify-checkout  check sttaus all subdirectories known.
+++      cernlib-install        to build cernlib standalone with cmake build.
[frankm@lxplus9111 offline]$ 
```

Markus Frank