#!/usr/bin/env python
# vim:set sts=4 sw=4 fdm=indent et:

import os, re
from copy import deepcopy
from ROOT import kAzure, kRed, kMagenta, kGreen, kOrange, kBlue, kYellow, kTeal

mergedInputFile = [
    os.path.expandvars("${CMSSW_BASE}/src/RecoBTag/PerformanceMeasurements/test/BTagAnalyzerMacros/ttbar/output_all.root"),
    os.path.expandvars("/afs/cern.ch/work/p/pchen/public/btaggingCommRun2/prod/20180511_Run2018A/CMSSW_10_1_2_patch2/src/RecoBTag/PerformanceMeasurements/test/BTagAnalyzerMacros/ttbar/output_all_11May2018.root"),
    os.path.expandvars("/afs/cern.ch/work/p/pchen/public/btaggingCommRun2/prod/20180424_DeepFlavour/CMSSW_9_4_4/src/RecoBTag/PerformanceMeasurements/test/BTagAnalyzerMacros/ttbar/output_all.root"),
]
outputDir       = os.path.expandvars("${CMSSW_BASE}/src/RecoBTag/PerformanceMeasurements/test/BTagAnalyzerMacros/ttbar/Commissioning_plots")
outputFormats   = ["pdf","png"]
# lumi            = 41.86 # Full2017
# lumi            = 1.03  # Early Run2018A
lumi            = 7.63  # Run2018A+Early Run2018B
isPrelim        = True

# Define histogram groups
ttbarMC = {
    'postfix'   :["_ttbar","_dy","_st","_ww","_wz","_zz"],
    'fileIndex' :2,
    'legend'    :["t#bar{t}","DY","tW","WW","WZ","ZZ"],
    'fillColor' :[kRed+1, kTeal, kOrange+1, kGreen-8, kMagenta+2, kBlue+2, 1],
    'drawOpt'   :"HIST",
    'isData'    :False,
    # 'forceNorm' :"1.034/41.856", # Scale lumi to Run2018
    'forceNorm' :"forceNormS = cfg['data'][1]['hTotal'].GetSumOfWeights()/convHStackToHist(data['hTotal']).GetSumOfWeights()", # Scale to data norm
}

btagMC = {
    'postfix'   :["_b_mc","_pu_mc","_c_mc","_l_mc"],
    'fileIndex' :2,
    'legend'    :["b","PU","c","udsg"],
    'fillColor' :[2, 8, kOrange+1, 4, 1],
    'drawOpt'   :"HIST",
    'isData'    :False,
    # 'forceNorm' :"1.034/41.856", # Scale lumi to Run2018
    'forceNorm' :"forceNormS = cfg['data'][1]['hTotal'].GetSumOfWeights()/convHStackToHist(data['hTotal']).GetSumOfWeights()", # Scale to data norm
}

defaultDATA = {
    'postfix'   :["_data"], 
    'fileIndex' :0,
    'legend'    :["Run2018A"],
    'fillColor' :[[1,20],[1,20]],
    'drawOpt'   :"E1",
    'isData'    :True,
    'forceNorm' : "forceNormS = 1" ,
}

compareDATA1 = {
    'postfix'   :["_data"],
    'fileIndex' :1,
    'legend'    :["2017 (Re-miniAOD)"],
    'fillColor' :[[kMagenta-3,43],[kMagenta-3,43]],
    'drawOpt'   :"E1",
    'isData'    :True,
    'forceNorm' : "forceNormS = 1.034/41.856",
}

compareDATA2 = {
    'postfix'   :["_data"], 
    'fileIndex' :2,
    'legend'    :["2017 (17Nov2017)"],
    'fillColor' :[[kGreen-5,21],[kGreen-5,21]],
    'drawOpt'   :"E1",
    'isData'    :True,
    'forceNorm' : "forceNormS = 1.034/41.856",
}

# Define config for plotters
ttbarConfig = {
    'data'      :[ttbarMC, defaultDATA, compareDATA1],
    'xTitle'    :"",
    'yTitle'    :"Events",
    'stLegPos'  :[0.75,0.58,0.89,0.89],
    'ratioOpts' :["divsym"],
    'rpLegPos'  :[0.60,0.65,0.89,0.89],
}

btagConfig = {
    'data'      :[btagMC, defaultDATA, compareDATA1],
    'xTitle'    :"",
    'yTitle'    :"Events",
    'stLegPos'  :[0.75,0.58,0.89,0.89],
    'ratioOpts' :["divsym"],
    'rpLegPos'  :[0.60,0.65,0.89,0.89],
}

profConfig = {# No MC
    'data'      :[defaultDATA],
    'xTitle'    :"",
    'yTitle'    :"Events",
    'stLegPos'  :[0.75,0.58,0.89,0.89],
    'ratioOpts' :["divsym"],
    'rpLegPos'  :[0.75,0.63,0.89,0.89],
}

ttbarPlots=[
    "ttbar_nPV_nominal",
    "ttbar_met_nominal",
    "ttbar_mll_nominal",
    "ttbar_njet_nominal",
    "ttbar_njet_pt30_nominal",
    "ttbar_lep0_pt_nominal",
    "ttbar_lep1_pt_nominal",
    "ttbar_jet0_pt_nominal",
    "ttbar_nBtag_CSVv2L_nominal",
    "ttbar_nBtag_CSVv2M_nominal",
    "ttbar_nBtag_CSVv2T_nominal",
    "ttbar_nBtag_cMVAv2L_nominal",
    "ttbar_nBtag_cMVAv2M_nominal",
    "ttbar_nBtag_cMVAv2T_nominal",
    "ttbar_nBtag_DeepCSVBDiscL_nominal",
    "ttbar_nBtag_DeepCSVBDiscM_nominal",
    "ttbar_nBtag_DeepCSVBDiscT_nominal",
    "ttbar_nBtag_DeepFlavourBDiscL_nominal",
    "ttbar_nBtag_DeepFlavourBDiscM_nominal",
    "ttbar_nBtag_DeepFlavourBDiscT_nominal",
    "ttbar_nBtag_afterJetSel_CSVv2L_nominal",
    "ttbar_nBtag_afterJetSel_CSVv2M_nominal",
    "ttbar_nBtag_afterJetSel_CSVv2T_nominal",
    "ttbar_nBtag_afterJetSel_cMVAv2L_nominal",
    "ttbar_nBtag_afterJetSel_cMVAv2M_nominal",
    "ttbar_nBtag_afterJetSel_cMVAv2T_nominal",
    "ttbar_nBtag_afterJetSel_DeepCSVBDiscL_nominal",
    "ttbar_nBtag_afterJetSel_DeepCSVBDiscM_nominal",
    "ttbar_nBtag_afterJetSel_DeepCSVBDiscT_nominal",
    "ttbar_nBtag_afterJetSel_DeepFlavourBDiscL_nominal",
    "ttbar_nBtag_afterJetSel_DeepFlavourBDiscM_nominal",
    "ttbar_nBtag_afterJetSel_DeepFlavourBDiscT_nominal",
]
btagPlots =[
    "btag_jet_pt_nominal",
    "btag_jet_eta_nominal",
    "btag_sv_multi_0_nominal",
    "btag_sv_flight3DSig_nominal",
    "btag_sv_deltaR_jet_nominal",
    "btag_sv_eta_nominal",
    "btag_sv_phi_nominal",
    "btag_track_multi_sel_nominal",
    "btag_track_pt_nominal",
    "btag_track_nHit_nominal",
    "btag_track_HPix_nominal",
    "btag_track_chi2_nominal",
    "btag_track_dist_nominal",
    "btag_track_len_nominal",
    "btag_track_IP_nominal",
    "btag_track_IPs_nominal",
    "btag_pfmuon_multi_nominal",
    "btag_pfmuon_pt_nominal",
    "btag_pfmuon_eta_nominal",
    "btag_pfmuon_phi_nominal",
    "btag_pfmuon_ptrel_nominal",
    "btag_pfelectron_multi_nominal",
    "btag_pfelectron_pt_nominal",
    "btag_pfelectron_eta_nominal",
    "btag_pfelectron_phi_nominal",
    "btag_pfelectron_ptrel_nominal",
    # "btag_tag_JP_nominal",
    # "btag_tag_JBP_nominal",
    # "btag_tag_TCHP_nominal",
    # "btag_tag_CSV_nominal",
    "btag_tag_CSVv2_nominal",
    "btag_tag_cMVAv2_nominal",
    "btag_tag_DeepCSVBDisc_nominal",
    "btag_tag_DeepFlavourBDisc_nominal",
    "btag_tag_DeepFlavourCvsB_nominal",
    "btag_tag_DeepFlavourCvsL_nominal",
    "btag_tag_CvsB_nominal",
    "btag_tag_CvsL_nominal",
]

profPlots = [
    "prof_nPV_nominal",
]

# Insert default setings
plots={}
for p in ttbarPlots:
    plots[p]=deepcopy(ttbarConfig)
    plots[p]['name'] = p
    plots[p]['xTitle'] = " ".join(p.split("_")[:-1])
for p in btagPlots:
    plots[p]=deepcopy(btagConfig)
    plots[p]['name'] = p
    plots[p]['xTitle'] = " ".join(p.split("_")[:-1])
for p in profPlots:
    plots[p]=deepcopy(profConfig)
    plots[p]['name'] = p
    plots[p]['xTitle'] = " ".join(p.split("_")[:-1])

# Plot specified settings
plots['ttbar_nPV_nominal'                                ]['xTitle'] = "Number of PVs"
plots['ttbar_met_nominal'                                ]['xTitle'] = "E^{missing}_{T} [GeV]"
plots['ttbar_mll_nominal'                                ]['xTitle'] = "M_{ll} [GeV]"
plots['ttbar_njet_nominal'                               ]['xTitle'] = "Number of jets"
plots['ttbar_njet_pt30_nominal'                          ]['xTitle'] = "Number of jets"
plots['ttbar_lep0_pt_nominal'                            ]['xTitle'] = "Leading lepton p_{T} [GeV]"
plots['ttbar_lep1_pt_nominal'                            ]['xTitle'] = "Sub-leading lepton p_{T} [GeV]"
plots['ttbar_jet0_pt_nominal'                            ]['xTitle'] = "Leading jet p_{T} [GeV]"
plots['ttbar_nBtag_CSVv2L_nominal'                       ]['xTitle'] = "Number of loose b-tagged jets (CSVv2)"
plots['ttbar_nBtag_CSVv2M_nominal'                       ]['xTitle'] = "Number of medium b-tagged jets (CSVv2)"
plots['ttbar_nBtag_CSVv2T_nominal'                       ]['xTitle'] = "Number of tight b-tagged jets (CSVv2)"
plots['ttbar_nBtag_cMVAv2L_nominal'                      ]['xTitle'] = "Number of loose b-tagged jets (cMVAv2)"
plots['ttbar_nBtag_cMVAv2M_nominal'                      ]['xTitle'] = "Number of medium b-tagged jets (cMVAv2)"
plots['ttbar_nBtag_cMVAv2T_nominal'                      ]['xTitle'] = "Number of tight b-tagged jets (cMVAv2)"
plots['ttbar_nBtag_DeepCSVBDiscL_nominal'                ]['xTitle'] = "Number of loose b-tagged jets (DeepCSVB)"
plots['ttbar_nBtag_DeepCSVBDiscM_nominal'                ]['xTitle'] = "Number of medium b-tagged jets (DeepCSVB)"
plots['ttbar_nBtag_DeepCSVBDiscT_nominal'                ]['xTitle'] = "Number of tight b-tagged jets (DeepCSVB)"
plots['ttbar_nBtag_DeepFlavourBDiscL_nominal'            ]['xTitle'] = "Number of loose b-tagged jets (DeepFlavourB)"
plots['ttbar_nBtag_DeepFlavourBDiscM_nominal'            ]['xTitle'] = "Number of medium b-tagged jets (DeepFlavourB)"
plots['ttbar_nBtag_DeepFlavourBDiscT_nominal'            ]['xTitle'] = "Number of tight b-tagged jets (DeepFlavourB)"
plots['ttbar_nBtag_afterJetSel_CSVv2L_nominal'           ]['xTitle'] = "Number of loose b-tagged jets (CSVv2)"
plots['ttbar_nBtag_afterJetSel_CSVv2M_nominal'           ]['xTitle'] = "Number of medium b-tagged jets (CSVv2)"
plots['ttbar_nBtag_afterJetSel_CSVv2T_nominal'           ]['xTitle'] = "Number of tight b-tagged jets (CSVv2)"
plots['ttbar_nBtag_afterJetSel_cMVAv2L_nominal'          ]['xTitle'] = "Number of loose b-tagged jets (cMVAv2)"
plots['ttbar_nBtag_afterJetSel_cMVAv2M_nominal'          ]['xTitle'] = "Number of medium b-tagged jets (cMVAv2)"
plots['ttbar_nBtag_afterJetSel_cMVAv2T_nominal'          ]['xTitle'] = "Number of tight b-tagged jets (cMVAv2)"
plots['ttbar_nBtag_afterJetSel_DeepCSVBDiscL_nominal'    ]['xTitle'] = "Number of loose b-tagged jets (DeepCSVB)"
plots['ttbar_nBtag_afterJetSel_DeepCSVBDiscM_nominal'    ]['xTitle'] = "Number of medium b-tagged jets (DeepCSVB)"
plots['ttbar_nBtag_afterJetSel_DeepCSVBDiscT_nominal'    ]['xTitle'] = "Number of tight b-tagged jets (DeepCSVB)"
plots['ttbar_nBtag_afterJetSel_DeepFlavourBDiscL_nominal']['xTitle'] = "Number of loose b-tagged jets (DeepFlavourB)"
plots['ttbar_nBtag_afterJetSel_DeepFlavourBDiscM_nominal']['xTitle'] = "Number of medium b-tagged jets (DeepFlavourB)"
plots['ttbar_nBtag_afterJetSel_DeepFlavourBDiscT_nominal']['xTitle'] = "Number of tight b-tagged jets (DeepFlavourB)"

plots['btag_jet_pt_nominal'             ]['xTitle']= "Jet p_{T} [GeV]"
plots['btag_jet_eta_nominal'            ]['xTitle']= "Jet #eta"
plots['btag_sv_multi_0_nominal'         ]['xTitle']= "Number of SV"
plots['btag_sv_flight3DSig_nominal'     ]['xTitle']= "SV 3D flight distance significance"
plots['btag_sv_deltaR_jet_nominal'      ]['xTitle']= "#Delta{}R between the jet and the SV direction"
plots['btag_sv_eta_nominal'             ]['xTitle']= "SV #eta"
plots['btag_sv_phi_nominal'             ]['xTitle']= "SV #phi"
plots['btag_track_multi_sel_nominal'    ]['xTitle']= "Number of selected tracks in the jets"
plots['btag_track_pt_nominal'           ]['xTitle']= "Track p_{T} [GeV]"
plots['btag_track_nHit_nominal'         ]['xTitle']= "Number of hits"
plots['btag_track_HPix_nominal'         ]['xTitle']= "Number of pixel hits"
plots['btag_track_chi2_nominal'         ]['xTitle']= "Normalized #chi^{2} of tracks"
plots['btag_track_dist_nominal'         ]['xTitle']= "Track distance to the jet axis [cm]"
plots['btag_track_len_nominal'          ]['xTitle']= "Track decay length [cm]"
plots['btag_track_IP_nominal'           ]['xTitle']= "3D IP of tracks"
plots['btag_track_IPs_nominal'          ]['xTitle']= "3D IP significance of tracks"
plots['btag_pfmuon_multi_nominal'       ]['xTitle']= "Number of PF muons"
plots['btag_pfmuon_pt_nominal'          ]['xTitle']= "p_{T} of PF muons [GeV]"
plots['btag_pfmuon_eta_nominal'         ]['xTitle']= "#eta of PF muons"
plots['btag_pfmuon_phi_nominal'         ]['xTitle']= "#phi of PF muons"
plots['btag_pfmuon_ptrel_nominal'       ]['xTitle']= "p_{T} rel. of PF muons [GeV]"
plots['btag_pfelectron_multi_nominal'   ]['xTitle']= "Number of PF electrons"
plots['btag_pfelectron_pt_nominal'      ]['xTitle']= "p_{T} of PF electrons [GeV]"
plots['btag_pfelectron_eta_nominal'     ]['xTitle']= "#eta of PF electrons"
plots['btag_pfelectron_phi_nominal'     ]['xTitle']= "#phi of PF electrons"
plots['btag_pfelectron_ptrel_nominal'   ]['xTitle']= "p_{T} rel. of PF electrons [GeV]"
# plots['btag_tag_JP_nominal'             ]['xTitle']= "JP discriminator"
# plots['btag_tag_JBP_nominal'            ]['xTitle']= "JBP discriminator"
# plots['btag_tag_TCHP_nominal'           ]['xTitle']= "TCHP discriminator"
# plots['btag_tag_CSV_nominal'            ]['xTitle']= "CSVv2(AVR) discriminator"
plots['btag_tag_CSVv2_nominal'          ]['xTitle']= "CSVv2 discriminator"
plots['btag_tag_cMVAv2_nominal'         ]['xTitle']= "cMVAv2 discriminator"
plots['btag_tag_DeepCSVBDisc_nominal'   ]['xTitle']= "DeepCSV b discriminator"
plots['btag_tag_DeepFlavourBDisc_nominal']['xTitle']="B-tag DeepFlavourBDisc discriminator"
plots['btag_tag_DeepFlavourCvsB_nominal']['xTitle']= "C-tag DeepFlavourCvsB discriminator"
plots['btag_tag_DeepFlavourCvsL_nominal']['xTitle']= "C-tag DeepFlavourCvsL discriminator"
plots['btag_tag_CvsB_nominal'           ]['xTitle']= "C-tag CvsB discriminator"
plots['btag_tag_CvsL_nominal'           ]['xTitle']= "C-tag CvsL discriminator"

plots['ttbar_nPV_nominal']['stExtra']="""
#hStack.GetYaxis().SetMinimum(1)
"""
plots['ttbar_nPV_nominal']['rpExtra']="""
#rp.GetYaxis().SetMinimum(1)
"""

# Final de-select plots
regexDeselPatterns = [
    "prof_.*",
]
regexDesel = [ re.compile(e) for e in regexDeselPatterns ]
for p in plots.keys():
    for r in regexDesel:
        if r.match(p):
            plots.pop(p)

