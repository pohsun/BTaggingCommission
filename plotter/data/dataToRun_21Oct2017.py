dataset = {}

# An example config
#dataset['name'] = {
#    'xsec'              : -1, # -1 for data, others for MC
#    'ipath'             : ['localpath','lfnpath1','lfnpath2'], # Will check ALL '*.root' under the dir, localpath and lfn are supported.
#}

# defaultLumi=3042. # In /pb, runB
# defaultLumi=701.__package__ # In /pb, runB_23JunRereco
# defaultLumi=11427. # In /pb, runB_v1, runB_v2, runC_v1~3, runD_v1
defaultLumi= 15349 # In /pb, runE_v1, runF_v1
defalutPUWgtUrl="/afs/cern.ch/work/p/pchen/public/btaggingCommRun2/prod/20171021_PixelStudy/CMSSW_9_2_13/src/RecoBTag/PerformanceMeasurements/test/ttbar/21Oct2017_DATA/pileup.root"

#Run2017_PromptReco
# dataset['runB_v1'] = {
#     'xsec'              : -1,
#     'ipath'             : ['/eos/cms/store/group/phys_btag/Commissioning/TTbar/Run2017/crab/04Jul2017_RunB/5719bae/MuonEG/crab_Data13TeV_MuonEG_2017B'],
#     'puWgtUrl'          : "/afs/cern.ch/work/p/pchen/public/btaggingCommRun2/prod/20170704_Run2017B/CMSSW_9_2_3_patch2/src/RecoBTag/PerformanceMeasurements/test/ttbar/04Jul2017_RunB/crab_Data13TeV_MuonEG_2017B/results/processedLumis_puWgtXsec69000.root",
# }

# dataset['runB_v2'] = {
#     'xsec'              : -1,
#     'ipath'             : ['/eos/cms/store/group/phys_btag/Commissioning/TTbar/Run2017/crab/25Sep2017_RunBCD-PromptReco/f72e542/MuonEG/crab_Data13TeV_MuonEG_2017B-v2'],
#     'puWgtUrl'          : "/afs/cern.ch/work/p/pchen/public/btaggingCommRun2/prod/20170925_Summer17MC/CMSSW_9_2_7_patch1/src/RecoBTag/PerformanceMeasurements/test/ttbar/25Sep2017_DATA/crab_Data13TeV_MuonEG_2017B-v2/results/processedLumis_puWgtXsec69000.root",
# }

# dataset['runC_v1'] = {
#     'xsec'              : -1,
#     'ipath'             : ['/eos/cms/store/group/phys_btag/Commissioning/TTbar/Run2017/crab/25Sep2017_RunBCD-PromptReco/f72e542/MuonEG/crab_Data13TeV_MuonEG_2017C-v1'],
#     'puWgtUrl'          : "/afs/cern.ch/work/p/pchen/public/btaggingCommRun2/prod/20170925_Summer17MC/CMSSW_9_2_7_patch1/src/RecoBTag/PerformanceMeasurements/test/ttbar/25Sep2017_DATA/crab_Data13TeV_MuonEG_2017C-v1/results/processedLumis_puWgtXsec69000.root",
# }
# dataset['runC_v2'] = {
#     'xsec'              : -1,
#     'ipath'             : ['/eos/cms/store/group/phys_btag/Commissioning/TTbar/Run2017/crab/25Sep2017_RunBCD-PromptReco/f72e542/MuonEG/crab_Data13TeV_MuonEG_2017C-v2'],
#     'puWgtUrl'          : "/afs/cern.ch/work/p/pchen/public/btaggingCommRun2/prod/20170925_Summer17MC/CMSSW_9_2_7_patch1/src/RecoBTag/PerformanceMeasurements/test/ttbar/25Sep2017_DATA/crab_Data13TeV_MuonEG_2017C-v2/results/processedLumis_puWgtXsec69000.root",
# }
# dataset['runC_v3'] = {
#     'xsec'              : -1,
#     'ipath'             : ['/eos/cms/store/group/phys_btag/Commissioning/TTbar/Run2017/crab/25Sep2017_RunBCD-PromptReco/f72e542/MuonEG/crab_Data13TeV_MuonEG_2017C-v3'],
#     'puWgtUrl'          : "/afs/cern.ch/work/p/pchen/public/btaggingCommRun2/prod/20170925_Summer17MC/CMSSW_9_2_7_patch1/src/RecoBTag/PerformanceMeasurements/test/ttbar/25Sep2017_DATA/crab_Data13TeV_MuonEG_2017C-v3/results/processedLumis_puWgtXsec69000.root",
# }

# dataset['runD_v1'] = {
#     'xsec'              : -1,
#     'ipath'             : ['/eos/cms/store/group/phys_btag/Commissioning/TTbar/Run2017/crab/25Sep2017_RunBCD-PromptReco/f72e542/MuonEG/crab_Data13TeV_MuonEG_2017D-v1'],
#     'puWgtUrl'          : "/afs/cern.ch/work/p/pchen/public/btaggingCommRun2/prod/20170925_Summer17MC/CMSSW_9_2_7_patch1/src/RecoBTag/PerformanceMeasurements/test/ttbar/25Sep2017_DATA/crab_Data13TeV_MuonEG_2017D-v1/results/processedLumis_puWgtXsec69000.root",
# }

dataset['runE_v1'] = {
    'xsec'              : -1,
    'ipath'             : ['/eos/cms/store/group/phys_btag/Commissioning/TTbar/Run2017/crab/21Oct2017_RunEF-PromptReco/131c7e5/MuonEG/crab_Data13TeV_MuonEG_2017E-v1'],
    'puWgtUrl'          : "/afs/cern.ch/work/p/pchen/public/btaggingCommRun2/prod/20171021_PixelStudy/CMSSW_9_2_13/src/RecoBTag/PerformanceMeasurements/test/ttbar/21Oct2017_DATA/Cert_303435-304507_13TeV_PromptReco_Collisions17_JSON_forRunEFPileupReWgt_puWgtXsec69000.root",
}

dataset['runF_v1'] = {
    'xsec'              : -1,
    'ipath'             : ['/eos/cms/store/group/phys_btag/Commissioning/TTbar/Run2017/crab/21Oct2017_RunEF-PromptReco/131c7e5/MuonEG/crab_Data13TeV_MuonEG_2017F-v1'],
    'puWgtUrl'          : "/afs/cern.ch/work/p/pchen/public/btaggingCommRun2/prod/20171021_PixelStudy/CMSSW_9_2_13/src/RecoBTag/PerformanceMeasurements/test/ttbar/21Oct2017_DATA/Cert_303435-304507_13TeV_PromptReco_Collisions17_JSON_forRunEFPileupReWgt_puWgtXsec69000.root",
}

dataset['ttbar'] = {
    'xsec'              : 831.77,
    'ipath'             : ['/eos/cms/store/group/phys_btag/Commissioning/TTbar/Run2017/crab/25Sep2017_Summer17-92X/f72e542/TT_TuneCUETP8M2T4_13TeV-powheg-pythia8/crab_MC13TeV_TTJets'],
    'puWgtUrl'          : defalutPUWgtUrl,
}

dataset['dy1'] = {
    'xsec'              : 18610,
    'ipath'             : ['/eos/cms/store/group/phys_btag/Commissioning/TTbar/Run2017/crab/25Sep2017_Summer17-92X/f72e542/DYJetsToLL_M-10to50_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/crab_MC13TeV_DY10to50'],
    'puWgtUrl'          : defalutPUWgtUrl,
}

dataset['dy2'] = {
    'xsec'              : 6025,
    'ipath'             : ['/eos/cms/store/group/phys_btag/Commissioning/TTbar/Run2017/crab/25Sep2017_Summer17-92X/f72e542/DYJetsToLL_M-50_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/crab_MC13TeV_DY50toInf'],
    'puWgtUrl'          : defalutPUWgtUrl,
}

dataset['st1'] = {
    'xsec'              : 35.85,
    'ipath'             : ['/eos/cms/store/group/phys_btag/Commissioning/TTbar/Run2016/crab/miniAODv2/mc/05Dec2016_Summer16-PUMoriond17/06d3c70/ST_tW_top_5f_inclusiveDecays_13TeV-powheg-pythia8_TuneCUETP8M2T4/crab_MC13TeV_SingleT_tW'],
    'puWgtUrl'          : defalutPUWgtUrl,
}

dataset['st2'] = {
    'xsec'              : 35.85,
    'ipath'             : ['/eos/cms/store/group/phys_btag/Commissioning/TTbar/Run2016/crab/miniAODv2/mc/05Dec2016_Summer16-PUMoriond17/06d3c70/ST_tW_antitop_5f_inclusiveDecays_13TeV-powheg-pythia8_TuneCUETP8M1/crab_MC13TeV_SingleTbar_tW'],
    'puWgtUrl'          : defalutPUWgtUrl,
}

dataset['ww'] = {
    'xsec'              : 118.7,
    'ipath'             : ['/eos/cms/store/group/phys_btag/Commissioning/TTbar/Run2017/crab/25Sep2017_Summer17-92X/f72e542/WW_TuneCUETP8M1_13TeV-pythia8/crab_MC13TeV_WW'],
    'puWgtUrl'          : defalutPUWgtUrl,
}

dataset['wz'] = {
    'xsec'              : 47.13,
    'ipath'             : ['/eos/cms/store/group/phys_btag/Commissioning/TTbar/Run2017/crab/25Sep2017_Summer17-92X/f72e542/WZ_TuneCUETP8M1_13TeV-pythia8/crab_MC13TeV_WZ'],
    'puWgtUrl'          : defalutPUWgtUrl,
}

dataset['zz'] = {
    'xsec'              : 16.5,
    'ipath'             : ['/eos/cms/store/group/phys_btag/Commissioning/TTbar/Run2017/crab/25Sep2017_Summer17-92X/f72e542/ZZ_TuneCUETP8M1_13TeV-pythia8/crab_MC13TeV_ZZ'],
    'puWgtUrl'          : defalutPUWgtUrl,
}

# If 'xsec' is -1, take the 'xsec' setting from datasets.
groupdata = {}
groupdata['mc'] = {
    'lumi'      : defaultLumi,
    'dataset'   : ['ttbar','st1','st2','dy1','dy2','ww','wz','zz'],
    'xsec'      : [-1,-1,-1,-1,-1,-1,-1,-1],
}
groupdata['ttbar'] = {
    'lumi'      : defaultLumi,
    'dataset'   : ['ttbar'],
    'xsec'      : [-1],
}
groupdata['st'] = {
    'lumi'      : defaultLumi,
    'dataset'   : ['st1','st2'],
    'xsec'      : [-1, -1],
}
groupdata['dy'] = {
    'lumi'      : defaultLumi,
    'dataset'   : ['dy1','dy2'],
    'xsec'      : [-1, -1],
}
groupdata['ww'] = {
    'lumi'      : defaultLumi,
    'dataset'   : ['ww'],
    'xsec'      : [-1],
}
groupdata['wz'] = {
    'lumi'      : defaultLumi,
    'dataset'   : ['wz'],
    'xsec'      : [-1],
}
groupdata['zz'] = {
    'lumi'      : defaultLumi,
    'dataset'   : ['zz'],
    'xsec'      : [-1],
}
groupdata['data'] = {
    'lumi'      : -1,
    'dataset'   : ['runE_v1', 'runF_v1'],
    'xsec'      : [-1,-1],
}
