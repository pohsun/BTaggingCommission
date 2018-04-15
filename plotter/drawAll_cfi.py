#!/usr/bin/env python
# vim:set sts=4 sw=4 fdm=syntax et:

import os
from copy import copy
from ROOT import kAzure, kRed, kMagenta, kGreen, kOrange, kBlue, kYellow

mergedInputFile = os.path.expandvars("${CMSSW_BASE}/src/RecoBTag/PerformanceMeasurements/test/BTagAnalyzerMacros/ttbar/output_all.root")
outputDir       = os.path.expandvars("${CMSSW_BASE}/src/RecoBTag/PerformanceMeasurements/test/BTagAnalyzerMacros/ttbar/Commissioning_plots")
outputFormats   = ["pdf","png"]
lumi            = 41.86
isPrelim        = True

ttbarConfig = {
    'postfix'   :["_data","_ttbar","_dy","_st","_ww","_wz","_zz"],
    'isData'    :[1]+[0]*6,
    'legend'    :["Data","t#bar{t}","DY","tW","WW","WZ","ZZ"],
    'fillColor' :[20, kRed+1, kAzure-2, kMagenta, kGreen+4, kOrange+2, kBlue+2], # MarkerStyle for data
    'xTitle'    :"",
    'yTitle'    :"Events",
    'drawOpt'   :[""]+["HIST"]*6,
    'stackOpt'  :"HIST",
    'stLegPos'  :[0.75,0.58,0.89,0.84],
    'ratioOpts' :["divsym"],
    'rpLegPos'  :[0.74,0.63,0.89,0.87],
}

btagConfig = {
    'postfix'   :["_data","_b_mc","_pu_mc","_c_mc","_l_mc"],
    'isData'    :[1]+[0]*4,
    'legend'    :["Data","b","PU","c","udsg"],
    'fillColor' :[20,2,kYellow,8,4],# MarkerStyle for data
    'xTitle'    :"",
    'yTitle'    :"Events",
    'drawOpt'   :[""]+["HIST"]*4,
    'stackOpt'  :"HIST",
    'stLegPos'  :[0.75,0.75,0.89,0.84],
    'ratioOpts' :["divsym"],
    'rpLegPos'  :[0.75,0.75,0.89,0.84],
}

ttbarPlots=[
    "nPV_nominal",
    "met_nominal",
    "mll_nominal",
    "njet_nominal",
    "njet_pt30_nominal",
    "lep0_pt_nominal",
    "lep1_pt_nominal",
    "jet0_pt_nominal",
    "nBtag_CSVv2L_nominal",
    "nBtag_CSVv2M_nominal",
    "nBtag_CSVv2T_nominal",
    "nBtag_cMVAv2L_nominal",
    "nBtag_cMVAv2M_nominal",
    "nBtag_cMVAv2T_nominal",
    "nBtag_DeepCSVBDiscL_nominal",
    "nBtag_DeepCSVBDiscM_nominal",
    "nBtag_DeepCSVBDiscT_nominal",
    "nBtag_DeepFlavourBDiscL_nominal",
    "nBtag_DeepFlavourBDiscM_nominal",
    "nBtag_DeepFlavourBDiscT_nominal",
    "nBtag_afterJetSel_CSVv2L_nominal",
    "nBtag_afterJetSel_CSVv2M_nominal",
    "nBtag_afterJetSel_CSVv2T_nominal",
    "nBtag_afterJetSel_cMVAv2L_nominal",
    "nBtag_afterJetSel_cMVAv2M_nominal",
    "nBtag_afterJetSel_cMVAv2T_nominal",
    "nBtag_afterJetSel_DeepCSVBDiscL_nominal",
    "nBtag_afterJetSel_DeepCSVBDiscM_nominal",
    "nBtag_afterJetSel_DeepCSVBDiscT_nominal",
    "nBtag_afterJetSel_DeepFlavourBDiscL_nominal",
    "nBtag_afterJetSel_DeepFlavourBDiscM_nominal",
    "nBtag_afterJetSel_DeepFlavourBDiscT_nominal",
]
btagPlots =[
    "jet_pt_nominal",
    "jet_eta_nominal",
    "sv_multi_0_nominal",
    "sv_flight3DSig_nominal",
    "sv_deltaR_jet_nominal",
    "sv_eta_nominal",
    "sv_phi_nominal",
    "track_multi_sel_nominal",
    "track_pt_nominal",
    "track_nHit_nominal",
    "track_HPix_nominal",
    "track_chi2_nominal",
    "track_dist_nominal",
    "track_len_nominal",
    "track_IP_nominal",
    "track_IPs_nominal",
    "pfmuon_multi_nominal",
    "pfmuon_pt_nominal",
    "pfmuon_eta_nominal",
    "pfmuon_phi_nominal",
    "pfmuon_ptrel_nominal",
    "pfelectron_multi_nominal",
    "pfelectron_pt_nominal",
    "pfelectron_eta_nominal",
    "pfelectron_phi_nominal",
    "pfelectron_ptrel_nominal",
    "tag_JP_nominal",
    "tag_JBP_nominal",
    "tag_CSV_nominal",
    "tag_CSVv2_nominal",
    "tag_cMVAv2_nominal",
    "tag_DeepCSVBDisc_nominal",
    "tag_DeepFlavourCvsB_nominal",
    "tag_DeepFlavourCvsL_nominal",
    "tag_CvsB_nominal",
    "tag_CvsL_nominal",
    "tag_TCHP_nominal",
]

# Insert default setings
plots={}
for p in ttbarPlots:
    plots[p]=copy(ttbarConfig)
    plots[p]['name'] = p
    plots[p]['xTitle'] = " ".join(p.split("_")[:-1])
for p in btagPlots:
    plots[p]=copy(btagConfig)
    plots[p]['name'] = p
    plots[p]['xTitle'] = " ".join(p.split("_")[:-1])

# Plot specified settings
plots["nPV_"                                       ]['xTitle'] = "Number of PVs"
plots["met_nominal"                                ]['xTitle'] = "E^{missing}_{T} [GeV]"
plots["mll_nominal"                                ]['xTitle'] = "M_{ll} [GeV]"
plots["njet_nominal"                               ]['xTitle'] = "Number of jets"
plots["njet_pt30_nominal"                          ]['xTitle'] = "Number of jets"
plots["lep0_pt_nominal"                            ]['xTitle'] = "Leading lepton p_{T} [GeV]"
plots["lep1_pt_nominal"                            ]['xTitle'] = "Sub-leading lepton p_{T} [GeV]"
plots["jet0_pt_nominal"                            ]['xTitle'] = "Leading jet p_{T} [GeV]"
plots["nBtag_CSVv2L_nominal"                       ]['xTitle'] = "Number of loose b-tagged jets (CSVv2)"
plots["nBtag_CSVv2M_nominal"                       ]['xTitle'] = "Number of medium b-tagged jets (CSVv2)"
plots["nBtag_CSVv2T_nominal"                       ]['xTitle'] = "Number of tight b-tagged jets (CSVv2)"
plots["nBtag_cMVAv2L_nominal"                      ]['xTitle'] = "Number of loose b-tagged jets (cMVAv2)"
plots["nBtag_cMVAv2M_nominal"                      ]['xTitle'] = "Number of medium b-tagged jets (cMVAv2)"
plots["nBtag_cMVAv2T_nominal"                      ]['xTitle'] = "Number of tight b-tagged jets (cMVAv2)"
plots["nBtag_DeepCSVBDiscL_nominal"                ]['xTitle'] = "Number of loose b-tagged jets (DeepCSVB)"
plots["nBtag_DeepCSVBDiscM_nominal"                ]['xTitle'] = "Number of medium b-tagged jets (DeepCSVB)"
plots["nBtag_DeepCSVBDiscT_nominal"                ]['xTitle'] = "Number of tight b-tagged jets (DeepCSVB)"
plots["nBtag_DeepFlavourBDiscL_nominal"            ]['xTitle'] = "Number of loose b-tagged jets (DeepFlavourB)"
plots["nBtag_DeepFlavourBDiscM_nominal"            ]['xTitle'] = "Number of medium b-tagged jets (DeepFlavourB)"
plots["nBtag_DeepFlavourBDiscT_nominal"            ]['xTitle'] = "Number of tight b-tagged jets (DeepFlavourB)"
plots["nBtag_afterJetSel_CSVv2L_nominal"           ]['xTitle'] = "Number of loose b-tagged jets (CSVv2)"
plots["nBtag_afterJetSel_CSVv2M_nominal"           ]['xTitle'] = "Number of medium b-tagged jets (CSVv2)"
plots["nBtag_afterJetSel_CSVv2T_nominal"           ]['xTitle'] = "Number of tight b-tagged jets (CSVv2)"
plots["nBtag_afterJetSel_cMVAv2L_nominal"          ]['xTitle'] = "Number of loose b-tagged jets (cMVAv2)"
plots["nBtag_afterJetSel_cMVAv2M_nominal"          ]['xTitle'] = "Number of medium b-tagged jets (cMVAv2)"
plots["nBtag_afterJetSel_cMVAv2T_nominal"          ]['xTitle'] = "Number of tight b-tagged jets (cMVAv2)"
plots["nBtag_afterJetSel_DeepCSVBDiscL_nominal"    ]['xTitle'] = "Number of loose b-tagged jets (DeepCSVB)"
plots["nBtag_afterJetSel_DeepCSVBDiscM_nominal"    ]['xTitle'] = "Number of medium b-tagged jets (DeepCSVB)"
plots["nBtag_afterJetSel_DeepCSVBDiscT_nominal"    ]['xTitle'] = "Number of tight b-tagged jets (DeepCSVB)"
plots["nBtag_afterJetSel_DeepFlavourBDiscL_nominal"]['xTitle'] = "Number of loose b-tagged jets (DeepFlavourB)"
plots["nBtag_afterJetSel_DeepFlavourBDiscM_nominal"]['xTitle'] = "Number of medium b-tagged jets (DeepFlavourB)"
plots["nBtag_afterJetSel_DeepFlavourBDiscT_nominal"]['xTitle'] = "Number of tight b-tagged jets (DeepFlavourB)"

plots["jet_pt_nominal"             ]['xTitle']= "Jet p_{T} [GeV]"
plots["jet_eta_nominal"            ]['xTitle']= "Jet #eta"
plots["sv_multi_0_nominal"         ]['xTitle']= "Number of SV"
plots["sv_flight3DSig_nominal"     ]['xTitle']= "SV 3D flight distance significance"
plots["sv_deltaR_jet_nominal"      ]['xTitle']= "#Delta{}R between the jet and the SV direction"
plots["sv_eta_nominal"             ]['xTitle']= "SV #eta"
plots["sv_phi_nominal"             ]['xTitle']= "SV #phi"
plots["track_multi_sel_nominal"    ]['xTitle']= "Number of selected tracks in the jets"
plots["track_pt_nominal"           ]['xTitle']= "Track p_{T} [GeV]"
plots["track_nHit_nominal"         ]['xTitle']= "Number of hits"
plots["track_HPix_nominal"         ]['xTitle']= "Number of pixel hits"
plots["track_chi2_nominal"         ]['xTitle']= "Normalized #chi^{2} of tracks"
plots["track_dist_nominal"         ]['xTitle']= "Track distance to the jet axis [cm]"
plots["track_len_nominal"          ]['xTitle']= "Track decay length [cm]"
plots["track_IP_nominal"           ]['xTitle']= "3D IP of tracks"
plots["track_IPs_nominal"          ]['xTitle']= "3D IP significance of tracks"
plots["pfmuon_multi_nominal"       ]['xTitle']= "Number of PF muons"
plots["pfmuon_pt_nominal"          ]['xTitle']= "p_{T} of PF muons [GeV]"
plots["pfmuon_eta_nominal"         ]['xTitle']= "#eta of PF muons"
plots["pfmuon_phi_nominal"         ]['xTitle']= "#phi of PF muons"
plots["pfmuon_ptrel_nominal"       ]['xTitle']= "p_{T} rel. of PF muons [GeV]"
plots["pfelectron_multi_nominal"   ]['xTitle']= "Number of PF electrons"
plots["pfelectron_pt_nominal"      ]['xTitle']= "p_{T} of PF electrons [GeV]"
plots["pfelectron_eta_nominal"     ]['xTitle']= "#eta of PF electrons"
plots["pfelectron_phi_nominal"     ]['xTitle']= "#phi of PF electrons"
plots["pfelectron_ptrel_nominal"   ]['xTitle']= "p_{T} rel. of PF electrons [GeV]"
plots["tag_JP_nominal"             ]['xTitle']= "JP discriminator"
plots["tag_JBP_nominal"            ]['xTitle']= "JBP discriminator"
plots["tag_CSV_nominal"            ]['xTitle']= "CSVv2(AVR) discriminator"
plots["tag_CSVv2_nominal"          ]['xTitle']= "CSVv2 discriminator"
plots["tag_cMVAv2_nominal"         ]['xTitle']= "cMVAv2 discriminator"
plots["tag_DeepCSVBDisc_nominal"   ]['xTitle']= "DeepCSV b discriminator"
plots["tag_DeepFlavourCvsB_nominal"]['xTitle']= "C-tag DeepFlavourCvsB discriminator"
plots["tag_DeepFlavourCvsL_nominal"]['xTitle']= "C-tag DeepFlavourCvsL discriminator"
plots["tag_CvsB_nominal"           ]['xTitle']= "C-tag CvsB discriminator"
plots["tag_CvsL_nominal"           ]['xTitle']= "C-tag CvsL discriminator"
plots["tag_TCHP_nominal"           ]['xTitle']= "TCHP discriminator"

