"""

   Run simulation using Geant4/BDSIM input file loader

   @author  L.Forthomme
   @version 1.0

"""
from __future__ import absolute_import, unicode_literals
import logging

logging.basicConfig(format='%(levelname)s: %(message)s', level=logging.INFO)
logger = logging.getLogger(__name__)


def run():
    import DDG4
    from DDG4 import OutputLevel as Output
    import LHeD

    lhed = LHeD.LHeD()
    geant4 = lhed.geant4
    kernel = lhed.kernel

    lhed.loadGeometry()
    geant4.printDetectors()
    # Configure UI
    #geant4.setupCshUI(vis=True)
    #geant4.setupCshUI()
    #geant4.setupUI(typ='qt', vis=True)
    geant4.setupUI(vis=False)
    lhed.setupField(quiet=False)
    DDG4.importConstants(kernel.detectorDescription(), debug=False)

    #prt = DDG4.EventAction(kernel, 'Geant4ParticlePrint/ParticlePrint')
    #prt.OutputLevel = Output.WARNING
    #prt.OutputType = 3  # Print both: table and tree
    #kernel.eventAction().adopt(prt)

    geant4.setupROOTOutput('RootOutput', 'output.root')

    gen = DDG4.GeneratorAction(kernel, "Geant4InputAction/Input")
    gen.Input = 'CepGenEventGenerator'
    gen.OutputLevel = Output.DEBUG
    gen.Parameters = dict(
        Verbosity = 3,
        Process = [
            'name:lpair',
            'kinematics/beam1id:2212',
            'kinematics/beam2id:11',
            'kinematics/beam1pz:7000.0',
            'kinematics/beam2pz:50.0',
            'kinematics/ptmin:5.0',
        ],
    )
    geant4.buildInputStage([gen], output_level=Output.DEBUG)

    part = DDG4.GeneratorAction(kernel, 'Geant4ParticleHandler/ParticleHandler')
    kernel.generatorAction().adopt(part)
    part.enableUI()

    lhed.setupDetectors()
    # Now build the physics list:
    lhed.setupPhysics('QGSP_BERT')
    lhed.test_config()

    #scan = DDG4.SteppingAction(kernel, 'Geant4MaterialScanner/MaterialScan')
    #kernel.steppingAction().adopt(scan)

    #geant4.run()

    kernel.NumEvents = 10

    kernel.run()
    kernel.terminate()
    logger.info('End of run. Terminating .......')
    logger.info('TEST_PASSED')


if __name__ == "__main__":
    run()
