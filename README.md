**Patches to the ALEPH offline software**

Found necessary changes to the aleph offline software to 
build and exceute the full processing chain:
- Kingal step to generate MC events
- Galeph step to simulate the detector response
- Julia step to reconstruct the simulated events
- Aleph step to read the reconstructed data

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

Markus Frank