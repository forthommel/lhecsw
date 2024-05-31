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
    import os
    import DDG4
    from DDG4 import OutputLevel as Output

    kernel = DDG4.Kernel()
    kernel.loadGeometry(str("file:" + os.environ.get('THIS', '..') + "/../Geometry/data/compact/compact.xml"))
    DDG4.Core.setPrintFormat(str("%-32s %6s %s"))
    geant4 = DDG4.Geant4(kernel)
    # Configure UI
    #geant4.setupCshUI(ui='Qt')
    geant4.setupUI(ui=None)

    prt = DDG4.EventAction(kernel, 'Geant4ParticlePrint/ParticlePrint')
    prt.OutputLevel = Output.WARNING
    prt.OutputType = 3  # Print both: table and tree
    kernel.eventAction().adopt(prt)

    geant4.setupROOTOutput('RootOutput', 'output.root')

    gen = DDG4.GeneratorAction(kernel, "Geant4InputAction/Input")
    gen.Input = 'BDSIMParticleGun|~lforthom/public/forPeter/BDSIM_10evts_lstar23.root'
    gen.OutputLevel = Output.DEBUG
    geant4.buildInputStage([gen], output_level=Output.DEBUG)

    # Now build the physics list:
    geant4.setupPhysics('QGSP_BERT')

    scan = DDG4.SteppingAction(kernel, 'Geant4MaterialScanner/MaterialScan')
    kernel.steppingAction().adopt(scan)

    kernel.configure()
    kernel.initialize()
    kernel.NumEvents = 10

    kernel.run()

    kernel.terminate()
    logger.info('End of run. Terminating .......')
    logger.info('TEST_PASSED')


if __name__ == "__main__":
  run()
