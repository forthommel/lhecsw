"""

   Perform a material scan using Geant4 shotting geantinos

   @author  M.Frank
   @version 1.0

"""
from __future__ import absolute_import, unicode_literals
import logging

logging.basicConfig(format='%(levelname)s: %(message)s', level=logging.INFO)
logger = logging.getLogger(__name__)


def run():
    import os
    import DDG4
    import g4units
    import LHeD

    DDG4.Core.setPrintFormat(str("%-32s %6s %s"))

    lhed = LHeD.LHeD()
    geant4 = lhed.geant4
    kernel = lhed.kernel

    lhed.loadGeometry()
    # Configure UI
    geant4.setupCshUI(ui=None)
    gun = geant4.setupGun("Gun",
                          Standalone=True,
                          particle='geantino',
                          energy=20 * g4units.GeV,
                          position=(0, 0, 0),
                          multiplicity=1,
                          isotrop=False)
    scan = DDG4.SteppingAction(kernel, 'Geant4MaterialScanner/MaterialScan')
    kernel.steppingAction().adopt(scan)

    # Now build the physics list:
    lhed.setupPhysics('QGSP_BERT')
    lhed.test_config()

    # 3 shots in different directions:
    gun.direction = (0, 1, 0)
    kernel.run()
    gun.direction = (1, 0, 0)
    kernel.run()
    gun.direction = (1, 1, 1)
    kernel.run()

    kernel.terminate()
    logger.info('End of run. Terminating .......')
    logger.info('TEST_PASSED')


if __name__ == "__main__":
    run()
