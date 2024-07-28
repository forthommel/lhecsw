from Gaudi.Configuration import *
from Configurables import GeoSvc


geoservice = GeoSvc("GeoSvc",  # DD4hep geometry service
    detectors = [
        'file:${lhecsw}/Geometry/DD4hep/data/compact/LHeD.xml',
    ],
    OutputLevel = INFO,
)
