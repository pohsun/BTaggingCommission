dataset = {}

# An example config
#dataset['name'] = {
#    'xsec'              : -1, # -1 for data, others for MC
#    'ipath'             : ['localpath','lfnpath1','lfnpath2'], # Will check ALL '*.root' under the dir, localpath and lfn are supported.
#}

# defaultLumi=3042. # In /pb, runB
# defaultLumi=701. # In /pb, runB_23JunRereco
# defaultLumi=11427. # In /pb, runB_v1, runB_v2, runC_v1~3, runD_v1
# defaultLumi=15349. # In /pb, runE_v1, runF_v1
defaultLumi=41860. # In /pb, Full2017
defalutPUWgtUrl="/afs/cern.ch/work/p/pchen/public/btaggingCommRun2/prod/20180104_Moriond18/CMSSW_9_4_1/src/RecoBTag/PerformanceMeasurements/test/ttbar/04Jan2018_DATA/Cert_294927-306462_13TeV_PromptReco_Collisions17_JSON_puWgtXsec69000.root"

#Run2017_17Nov2017ReReco
dataset['runB_v1'] = {
    'xsec'              : -1,
    'ipath'             : ['/eos/cms/store/group/phys_btag/Commissioning/TTbar/Run2017/crab/04Jan2018_Ru2017Full-17Nov2017ReReco/b55aad4/MuonEG/crab_Data13TeV_MuonEG_2017B-v1'],
    'puWgtUrl'          : defalutPUWgtUrl,
}
dataset['runC_v1'] = {
    'xsec'              : -1,
    'ipath'             : ['/eos/cms/store/group/phys_btag/Commissioning/TTbar/Run2017/crab/04Jan2018_Ru2017Full-17Nov2017ReReco/b55aad4/MuonEG/crab_Data13TeV_MuonEG_2017C-v1'],
    'puWgtUrl'          : defalutPUWgtUrl,
}
dataset['runD_v1'] = {
    'xsec'              : -1,
    'ipath'             : ['/eos/cms/store/group/phys_btag/Commissioning/TTbar/Run2017/crab/04Jan2018_Ru2017Full-17Nov2017ReReco/b55aad4/MuonEG/crab_Data13TeV_MuonEG_2017D-v1'],
    'puWgtUrl'          : defalutPUWgtUrl,
}
dataset['runE_v1'] = {
    'xsec'              : -1,
    'ipath'             : ['/eos/cms/store/group/phys_btag/Commissioning/TTbar/Run2017/crab/04Jan2018_Ru2017Full-17Nov2017ReReco/b55aad4/MuonEG/crab_Data13TeV_MuonEG_2017E-v1'],
    'puWgtUrl'          : defalutPUWgtUrl,
}
dataset['runF_v1'] = {
    'xsec'              : -1,
    'ipath'             : ['/eos/cms/store/group/phys_btag/Commissioning/TTbar/Run2017/crab/04Jan2018_Ru2017Full-17Nov2017ReReco/b55aad4/MuonEG/crab_Data13TeV_MuonEG_2017F-v1'],
    'puWgtUrl'          : defalutPUWgtUrl,
}

dataset['ttbar'] = {
    'xsec'              : 831.77,
    'ipath'             : ['/eos/cms/store/group/phys_btag/Commissioning/TTbar/Run2017/crab/04Jan2018_Moriond18-94X/b55aad4/TTTo2L2Nu_TuneCP5_PSweights_13TeV-powheg-pythia8/crab_MC13TeV_TTJets'],
    'puWgtUrl'          : defalutPUWgtUrl,
}

# dataset['dy1'] = {
#     'xsec'              : 18610,
#     'ipath'             : ['/eos/cms/store/group/phys_btag/Commissioning/TTbar/Run2017/crab/25Sep2017_Summer17-92X/f72e542/DYJetsToLL_M-10to50_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/crab_MC13TeV_DY10to50'],
#     'puWgtUrl'          : defalutPUWgtUrl,
# }

dataset['dy2'] = {
    'xsec'              : 6025,
    'ipath'             : ['/eos/cms/store/group/phys_btag/Commissioning/TTbar/Run2017/crab/04Jan2018_Moriond18-94X/b55aad4/DYJetsToLL_M-50_TuneCP5_13TeV-amcatnloFXFX-pythia8/crab_MC13TeV_DY50toInf'],
    'puWgtUrl'          : defalutPUWgtUrl,
}

dataset['st1'] = {
    'xsec'              : 35.85,
    'ipath'             : ['/eos/cms/store/group/phys_btag/Commissioning/TTbar/Run2017/crab/04Jan2018_Moriond18-94X/b55aad4/ST_tW_top_5f_inclusiveDecays_TuneCP5_PSweights_13TeV-powheg-pythia8/crab_MC13TeV_SingleT_tW'],
    'puWgtUrl'          : defalutPUWgtUrl,
}

dataset['st2'] = {
    'xsec'              : 35.85,
    'ipath'             : ['/eos/cms/store/group/phys_btag/Commissioning/TTbar/Run2017/crab/04Jan2018_Moriond18-94X/b55aad4/ST_tW_antitop_5f_inclusiveDecays_TuneCP5_PSweights_13TeV-powheg-pythia8/crab_MC13TeV_SingleTbar_tW'],
    'puWgtUrl'          : defalutPUWgtUrl,
}

dataset['ww'] = {
    'xsec'              : 118.7,
    'ipath'             : ['/eos/cms/store/group/phys_btag/Commissioning/TTbar/Run2017/crab/04Jan2018_Moriond18-94X/b55aad4/WW_TuneCP5_13TeV-pythia8/crab_MC13TeV_WW'],
    'puWgtUrl'          : defalutPUWgtUrl,
}

dataset['wz'] = {
    'xsec'              : 47.13,
    'ipath'             : ['/eos/cms/store/group/phys_btag/Commissioning/TTbar/Run2017/crab/04Jan2018_Moriond18-94X/b55aad4/WZ_TuneCP5_13TeV-pythia8/crab_MC13TeV_WZ'],
    'puWgtUrl'          : defalutPUWgtUrl,
}

dataset['zz'] = {
    'xsec'              : 16.5,
    'ipath'             : ['/eos/cms/store/group/phys_btag/Commissioning/TTbar/Run2017/crab/04Jan2018_Moriond18-94X/b55aad4/ZZ_TuneCP5_13TeV-pythia8/crab_MC13TeV_ZZ'],
    'puWgtUrl'          : defalutPUWgtUrl,
}

# If 'xsec' is -1, take the 'xsec' setting from datasets.
groupdata = {}
groupdata['mc'] = {
    'lumi'      : defaultLumi,
    # 'dataset'   : ['ttbar','st1','st2','dy1','dy2','ww','wz','zz'],
    # 'xsec'      : [-1,-1,-1,-1,-1,-1,-1,-1],
    'dataset'   : ['ttbar','st1','st2','dy2','ww','wz','zz'],
    'xsec'      : [-1]*7,
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
    'dataset'   : ['dy2'],
    'xsec'      : [-1],
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
    'dataset'   : ['runB_v1','runC_v1','runD_v1','runE_v1','runF_v1'],
    'xsec'      : [-1,-1,-1,-1,-1],
}
