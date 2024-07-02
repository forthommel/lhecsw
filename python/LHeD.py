from __future__ import absolute_import, unicode_literals
import sys
import logging
import DDG4

logging.basicConfig(format='%(levelname)s: %(message)s', level=logging.INFO)
logger = logging.getLogger(__name__)


class LHeD:
    def __init__(self, tracker='Geant4TrackerCombineAction', no_physics=True):
        self.kernel = DDG4.Kernel()
        self.description = self.kernel.detectorDescription()
        self.geant4 = DDG4.Geant4(self.kernel, tracker=tracker)
        self.kernel.UI = ""
        if no_physics:
            self.noPhysics()

    def loadGeometry(self, path='LHeD.xml', file=None):
        from os import environ
        install_dir = environ.get('lhecsw', '..')
        if file is None:
            self.kernel.loadGeometry(f"file:{install_dir}/Geometry/data/compact/{path}")
        else:
            ui = DDG4.DD4hepUI(self.description)
            ui.importROOT(file)
        return self

    def setupField(self, quiet=True):
        '''Configure G4 magnetic field tracking'''
        return self.geant4.setupTrackingField(prt=True)

    def setupRandom(self, name, type=None, seed=None, quiet=True):
        '''Set up the random number generator'''
        rndm = DDG4.Action(self.kernel, 'Geant4Random/' + name)
        if seed:
            rndm.Seed = seed
        if type:
            rndm.Type = type
        rndm.initialize()
        if not quiet:
            rndm.showStatus()
        return rndm

    def setupPhysics(self, model):
        '''Configure the Geant4 physics list'''
        phys = self.geant4.setupPhysics(model)
        ph = DDG4.PhysicsList(self.kernel, 'Geant4PhysicsList/Myphysics')
        # Add bosons to the model (redundant if already implemented by the model)
        ph.addParticleGroup(str('G4BosonConstructor'))
        # Add leptons to the model (redundant if already implemented by the model)
        ph.addParticleGroup(str('G4LeptonConstructor'))
        # Add multiple scattering in the material
        ph.addParticleProcess(str('e[+-]'), str('G4eMultipleScattering'), -1, 1, 1)
        # Add optical physics (RICH dets etc)
        ph.addPhysicsConstructor(str('G4OpticalPhysics'))
        # Interactivity
        ph.enableUI()
        phys.adopt(ph)
        phys.dump()
        return phys

    def noPhysics(self):
        '''No physics list wanted for tests?'''
        self.geant4.setupPhysics('')
        return self

    def setupDetectors(self):
        logger.info("#  First the tracking detectors")
        seq, act = self.geant4.setupTracker('SiVertexBarrel')
        seq, act = self.geant4.setupTracker('SiVertexBarrel2')
        seq, act = self.geant4.setupTracker('SiTrackerBarrel')
        seq, act = self.geant4.setupTracker('SiTrackerOBarrel')
        seq, act = self.geant4.setupTracker('SiTrackerForward')
        seq, act = self.geant4.setupTracker('SiTrackerBackward')
        logger.info("#  Now setup the calorimeters")
        seq, act = self.geant4.setupCalorimeter('EcalBarreli')
        seq, act = self.geant4.setupCalorimeter('EcalBarrelm')
        seq, act = self.geant4.setupCalorimeter('EcalBarrelo')
        seq, act = self.geant4.setupCalorimeter('EcalPlug_fwd')
        seq, act = self.geant4.setupCalorimeter('EcalPlug_bwd')
        seq, act = self.geant4.setupCalorimeter('HcalBarrel')
        seq, act = self.geant4.setupCalorimeter('HcalEndcap_fwd')
        seq, act = self.geant4.setupCalorimeter('HcalEndcap_bwd')
        seq, act = self.geant4.setupCalorimeter('HcalPlug_fwd')
        seq, act = self.geant4.setupCalorimeter('HcalPlug_bwd')
        logger.info("#  Now setup the muon chambers")
        seq, act = self.geant4.setupCalorimeter('MuonBarrel')
        seq, act = self.geant4.setupCalorimeter('MuonEndcap_fwd1')
        seq, act = self.geant4.setupCalorimeter('MuonEndcap_fwd2')
        seq, act = self.geant4.setupCalorimeter('MuonEndcap_bwd1')
        seq, act = self.geant4.setupCalorimeter('MuonEndcap_bwd2')
        return self

    def test_config(self, have_geo=True):
        '''Test the configuration'''
        self.kernel.configure()
        if have_geo:
            self.kernel.initialize()
        return self

    def test_run(self, have_geo=True, have_physics=False, num_events=2):
        '''Test runner'''
        self.test_config(have_geo)
        if have_geo:
            self.kernel.NumEvents = num_events
            self.kernel.run()
        self.kernel.terminate()
        logger.info('+++++ All Done....\n\nTEST_PASSED')
        sys.exit(0)
