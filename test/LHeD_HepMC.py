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
    import argparse
    import DDG4
    from DDG4 import OutputLevel as Output
    import LHeD

    DDG4.Core.setPrintFormat(str("%-32s %6s %s"))

    parser = argparse.ArgumentParser()
    parser.add_argument('--input-file', help="HepMC input file for event generation", default='/home/laurent/work/dev/cepgen/build/output.hepmc')
    parser.add_argument('--parser', help="Event sample parser (e.g. Geant4EventReaderHepMC, or HEPMC3FileReader)", default='Geant4EventReaderHepMC')
    parser.add_argument('--macro', help="UI macro to run")
    parser.add_argument('--vis', help="Enable GUI?", default=False)
    parser.add_argument('--output', default='output.root')
    args = parser.parse_args()

    lhed = LHeD.LHeD()
    geant4 = lhed.geant4
    kernel = lhed.kernel

    lhed.loadGeometry()
    #lhed.loadGeometry('compact_Lhe_dip_sol_ell.xml')
    geant4.printDetectors()
    # Configure UI
    if args.macro:
        geant4.setupCshUI(vis=args.vis, macro=args.macro)
    else:
        geant4.setupCshUI(vis=args.vis)
    #geant4.setupUI(vis=False)
    lhed.setupField(quiet=False)
    DDG4.importConstants(kernel.detectorDescription(), debug=False)

    prt = DDG4.EventAction(kernel, 'Geant4ParticlePrint/ParticlePrint')
    prt.OutputLevel = Output.WARNING
    prt.OutputType = 3  # Print both: table and tree
    kernel.eventAction().adopt(prt)

    if args.output:
        geant4.setupROOTOutput('RootOutput', args.output)

    gen = DDG4.GeneratorAction(kernel, 'Geant4InputAction/Input')
    gen.Input = f'{args.parser}|{args.input_file}'
    gen.OutputLevel = Output.DEBUG
    geant4.buildInputStage([gen], output_level=Output.DEBUG)

    # Merge all existing interaction records
    merger = DDG4.GeneratorAction(kernel, "Geant4InteractionMerger/InteractionMerger")
    merger.enableUI()
    kernel.generatorAction().adopt(merger)

    part = DDG4.GeneratorAction(kernel, 'Geant4ParticleHandler/ParticleHandler')
    kernel.generatorAction().adopt(part)
    part.enableUI()
    user = DDG4.Action(kernel, "Geant4TCUserParticleHandler/UserParticleHandler")
    user.TrackingVolume_Zmax = max(DDG4.EcalPlug_zmin_fwd, DDG4.EcalPlug_zmin_bwd)
    user.TrackingVolume_Rmax = DDG4.EcalBarrel_rmin1
    user.enableUI()
    part.adopt(user)

    lhed.setupDetectors()
    # Now build the physics list:
    lhed.setupPhysics('QGSP_BERT')
    lhed.test_config()

    #gen.generator()

    #scan = DDG4.SteppingAction(kernel, 'Geant4MaterialScanner/MaterialScan')
    #kernel.steppingAction().adopt(scan)

    #kernel.configure()
    #kernel.initialize()
    kernel.NumEvents = 10

    kernel.run()
    kernel.terminate()
    logger.info('End of run. Terminating .......')
    logger.info('TEST_PASSED')


if __name__ == "__main__":
    run()
