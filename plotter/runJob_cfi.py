#!/usr/bin/env python
# vim: set sts=4 sw=4 fdm=syntax et:

runEras={
    'Run2017B':[297020,299329],
    'Run2017C':[299337,302029],
    'Run2017D':[302030,303434],
    'Run2017E':[303435,304826],
    'Run2017F':[304911,306462]
}

tagWPs = {}
tagWPs['CSVv2T'] = {
    'varName':"Jet_CombIVF",
    'wp': 0.9535
}
tagWPs['CSVv2M'] = {
    'varName':"Jet_CombIVF",
    'wp': 0.8484
}
tagWPs['CSVv2L'] = {
    'varName':"Jet_CombIVF",
    'wp': 0.5426
}
tagWPs['cMVAv2T'] = {
    'varName':"Jet_cMVAv2",
    'wp': 0.9432
}
tagWPs['cMVAv2M'] = {
    'varName':"Jet_cMVAv2",
    'wp': 0.4432
}
tagWPs['cMVAv2L'] = {
    'varName':"Jet_cMVAv2",
    'wp': -0.5884
}
tagWPs['DeepCSVBDiscT'] = {
    'varName':"Jet_DeepCSVBDisc",
    'wp': 0.8001
}
tagWPs['DeepCSVBDiscM'] = {
        'varName':"Jet_DeepCSVBDisc",
    'wp': 0.4941
}
tagWPs['DeepCSVBDiscL'] = {
    'varName':"Jet_DeepCSVBDisc",
    'wp': 0.1522
}
tagWPs['DeepFlavourBDiscT'] = {
    'varName':"Jet_DeepFlavourBDisc",
    'wp': 0.9068
}
tagWPs['DeepFlavourBDiscM'] = {
    'varName':"Jet_DeepFlavourBDisc",
    'wp': 0.4318
}
tagWPs['DeepFlavourBDiscL'] = {
    'varName':"Jet_DeepFlavourBDisc",
    'wp': 0.0574
}

subCuts = {}
# subCuts['RunB']={
    # 'cutStr': "Run> && Run<",
    # 'cutVar':["Run"]
# }
