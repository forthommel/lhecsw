# tracker components
tracker_vertex_components = [
    ("vertexBarrel", "SiVertexBarrelHits"),
    ("vertexOuterBarrel", "SiVertexBarrel2Hits"),
]
tracker_outer_barrel_components = [
    ("trackerBarrel", "SiTrackerBarrelHits"),
    ("trackerOuterBarrel", "SiTrackerOBarrelHits"),
]
tracker_outer_endcaps_components = [
    ("trackerForward", "SiTrackerForwardHits"),
    ("trackerBackward", "SiTrackerBackwardHits"),
]
tracker_outer_components = tracker_outer_barrel_components + tracker_outer_endcaps_components
tracker_components = tracker_vertex_components + tracker_outer_components

# calorimeter components
calo_ecal_barrel_components = [
    ("ecalBarrel1", "EcalBarrelHits1"),
    ("ecalBarrel2", "EcalBarrelHits2"),
    ("ecalBarrel3", "EcalBarrelHits3"),
]
calo_ecal_plug_components = [
    ("ecalPlug0", "EcalPlugHits0"),
    ("ecalPlug1", "EcalPlugHits1"),
]
calo_ecal_components = calo_ecal_barrel_components + calo_ecal_plug_components

calo_hcal_barrel_components = [
    ("hcalBarrel", "HcalBarrelHits"),
]
calo_hcal_endcaps_components = [
    ("hcalEndcap0", "HcalEndcapHits0"),
    ("hcalEndcap1", "HcalEndcapHits1"),
]
calo_hcal_plugs_components = [
    ("hcalPlug01", "HcalPlugHits01"),
    ("hcalPlug11", "HcalPlugHits11"),
]
calo_hcal_components = calo_hcal_barrel_components + calo_hcal_endcaps_components + calo_hcal_plugs_components

calo_components = calo_ecal_components + calo_hcal_components

# muon chambers components
muon_components = [
    ("muonBarrel", "MuonBarrelHits"),
    ("muonEndcap01", "MuonEndcapHits01"),
    ("muonEndcap02", "MuonEndcapHits02"),
    ("muonEndcap11", "MuonEndcapHits11"),
    ("muonEndcap12", "MuonEndcapHits12"),
]
