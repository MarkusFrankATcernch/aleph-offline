# ==========================================================================
#  AIDA Detector description implementation
# --------------------------------------------------------------------------
# Copyright (C) Organisation europeenne pour la Recherche nucleaire (CERN)
# All rights reserved.
#
# For the licensing terms see $DD4hepINSTALL/LICENSE.
# For the list of contributors see $DD4hepINSTALL/doc/CREDITS.
#
# ==========================================================================
#
import sys
import DDG4
import logging

logging.basicConfig(format='%(levelname)s: %(message)s', level=logging.INFO)
logger = logging.getLogger(__name__)


class Aleph:
  def __init__( self, tracker='Geant4TrackerCombineAction' ):
    self.kernel = DDG4.Kernel( )
    self.description = self.kernel.detectorDescription( )
    self.geant4 = DDG4.Geant4( self.kernel, tracker=tracker )
    DDG4.setPrintFormat( '%-24s %7s %s' )
    self.kernel.UI = ''

  def loadGeometry( self, file=None ):
    import os
    if file is None:
      install_dir = os.environ['DD4hepINSTALL']
      level = DDG4.OutputLevel.INFO   ## DDG4.printLevel( )
      DDG4.setPrintLevel( level )
      print( '+++ Loading geometry setup: '+sys.argv[1] )
      self.kernel.loadGeometry( str("file:" + sys.argv[1]) )
      DDG4.setPrintLevel( level )
    else:
      ui = DDG4.DD4hepUI( self.description )
      ui.importROOT( file )
    return self

  # Example to show how to configure G4 magnetic field tracking
  def setupField(self, quiet=True):
    return self.geant4.setupTrackingField(prt=True)

  # Example to show how to setup random generator
  def setupRandom( self, name, type=None, seed=None, quiet=True ):  # noqa: A002
    rndm = DDG4.Action( self.kernel, 'Geant4Random/' + name )
    if seed:
      rndm.Seed = seed
    if type:
      rndm.Type = type
    rndm.initialize( )
    if not quiet:
      rndm.showStatus( )
    return rndm

  # Example to show how to configure the Geant4 physics list
  def setupPhysics( self, model ):
    phys = self.geant4.setupPhysics( model )
    phys.dump( )
    return phys

  # No physics list wanted for tests ? See how:
  def setupPhysics( self):
    self.geant4.setupPhysics('QGSP_BERT')
    return self

  def setupDetectors(self, debug_volid=False):
    import pdb
    pdb.set_trace()
    logger.info( "#  First the tracking detectors" )
    seq, act = self.geant4.setupTracker( 'VDET', debug_volid=debug_volid )
    logger.info( "#  Now setup the calorimeters" )
    return self

  # Test the configuration
  def test_config( self, have_geo=True ):
    self.kernel.configure( )
    if have_geo:
      self.kernel.initialize( )
    return self

  # Test runner
  def test_run(self, have_geo=True, have_physics=False, num_events=0):
    aleph.setupDetectors( True )
    self.test_config(have_geo)
    self.setupRandom( )
    self.setupPhysics( )
    if have_geo:
      self.kernel.NumEvents = num_events
      self.kernel.run( )
    self.kernel.terminate( )
    logger.info( '+++++ All Done....\n\nTEST_PASSED' )
    sys.exit( 0 )


if __name__ == "__main__":
  aleph = Aleph( )
  aleph.loadGeometry( )
  aleph.test_run( True )
