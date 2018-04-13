#!/usr/bin/env python
# vim:set sts=4 sw=4 fdm=syntax et:

import os
from copy import copy
from ROOT import kAzure, kRed, kMagenta, kGreen, kOrange, kBlue, kYellow

mergedInputFile = os.path.expandvars("${CMSSW_BASE}/src/RecoBTag/PerformanceMeasurements/test/BTagAnalyzerMacros/ttbar/output_all.root")
outputDir       = os.path.expandvars("${CMSSW_BASE}/src/RecoBTag/PerformanceMeasurements/test/BTagAnalyzerMacros/ttbar/Commissioning_plots")
outputFormats   = ["pdf","png"]

ttbarConfig = {
    'postfix'   :["_data","_ttbar","_dy","_st","_ww","_wz","_zz"],
    'isData'    :[1]+[0]*6,
    'legend'    :["Data","t#bar{t}","DY","tW","WW","WZ","ZZ"],
    'fillColor' :[20, kAzure-2, kRed+1, kMagenta, kGreen+4, kOrange+2, kBlue+2], # MarkerStyle for data
    'xTitle'    :"",
    'yTitle'    :"",
    'drawOpt'   :[""]+["HIST"]*6,
    'stackOpt'  :"HIST",
    'stLegPos'  :[0.35,0.15,0.70,0.42],
    'ratioOpts' :["divsym"],
    'rpLegPos'  :[0.35,0.15,0.70,0.42],
}

btagConfig = {
    'postfix'   :["_data","_b","_pu","_c","_l"],
    'isData'    :[1]+[0]*4,
    'legend'    :["Data","b","PU","c","udsg"],
    'fillColor' :[20,2,kYellow,8,4],# MarkerStyle for data
    'xTitle'    :"",
    'yTitle'    :"Events",
    'drawOpt'   :[""]+["HIST"]*4,
    'stackOpts' :"",
    'stLegPos'  :[0.35,0.15,0.70,0.42],
    'ratioOpts' :["divsym"],
    'rpLegPos'  :[0.35,0.15,0.70,0.42],
}

ttbarPlots=[
    "nPV_nominal",
    # "met",
    # "mll",
    # "njet",
    # "njet_pt30",
    # "lep0_pt",
    # "lep1_pt",
    # "jet0_pt"
]
btagPlots =[
]

plots={}
for p in ttbarPlots:
    plots[p]=copy(ttbarConfig)
    plots[p]['name'] = p
    plots[p]['xTitle'] = p
for p in btagPlots:
    plots[p]=copy(btagConfig)
    plots[p]['name'] = p

