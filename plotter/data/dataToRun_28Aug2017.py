dataset = {}

# An example config
#dataset['name'] = {
#    'xsec'              : -1, # -1 for data, others for MC
#    'ipath'             : ['localpath','lfnpath1','lfnpath2'], # Will check ALL '*.root' under the dir, localpath and lfn are supported.
#    'puWgtUrl'          : "pathname_of_puWgt.root" # Created with 'runPileupEstimation.py'
#}

# defaultLumi=3042 # In /pb, runB
defaultLumi=701 # In /pb, runB_23JunRereco
defalutPUWgtUrl="${CMSSW_BASE}/src/RecoBTag/PerformanceMeasurements/test/ttbar/04Jul2017_RunB/crab_Data13TeV_MuonEG_2017B/results/processedLumis_puWgtXsec69000.root"

dataset['runB'] = {
    'xsec'              : -1,
    'ipath'             : ['/eos/cms/store/group/phys_btag/Commissioning/TTbar/Run2017/crab/04Jul2017_RunB/5719bae/MuonEG/crab_Data13TeV_MuonEG_2017B'],
    'puWgtUrl'          : "${CMSSW_BASE}/src/RecoBTag/PerformanceMeasurements/test/ttbar/04Jul2017_Data/crab_Data13TeV_MuonEG_2017B/results/processedLumis_puWgtXsec69000.root",
}

dataset['runC'] = {
    'xsec'              : -1,
    'ipath'             : ['/eos/cms/store/group/phys_btag/Commissioning/TTbar/Run2017/crab/04Jul2017_RunB/5719bae/MuonEG/crab_Data13TeV_MuonEG_2017B_23JunReReco'],
    'puWgtUrl'          : "${CMSSW_BASE}/src/RecoBTag/PerformanceMeasurements/test/ttbar/04Jul2017_Data/crab_Data13TeV_MuonEG_2017B_23JunReReco/results/processedLumis_puWgtXsec69000.root",
}

dataset['ttbar'] = {
    'xsec'              : 831.77,
    'ipath'             : ['/eos/cms/store/group/phys_btag/Commissioning/TTbar/Run2016/crab/miniAODv2/mc/05Dec2016_Summer16-PUMoriond17/06d3c70/TT_TuneCUETP8M2T4_13TeV-powheg-pythia8'],
    'puWgtUrl'          : defalutPUWgtUrl,
}

dataset['dy1'] = {
    'xsec'              : 18610,
    'ipath'             : ['/eos/cms/store/group/phys_btag/Commissioning/TTbar/Run2016/crab/miniAODv2/mc/05Dec2016_Summer16-PUMoriond17/06d3c70/DYJetsToLL_M-10to50_TuneCUETP8M1_13TeV-madgraphMLM-pythia8'],
    'puWgtUrl'          : defalutPUWgtUrl,
}

dataset['dy2'] = {
    'xsec'              : 6025,
    'ipath'             : ['/eos/cms/store/group/phys_btag/Commissioning/TTbar/Run2016/crab/miniAODv2/mc/05Dec2016_Summer16-PUMoriond17/06d3c70/DYJetsToLL_M-50_TuneCUETP8M1_13TeV-madgraphMLM-pythia8'],
    'puWgtUrl'          : defalutPUWgtUrl,
}

dataset['st1'] = {
    'xsec'              : 35.85,
    'ipath'             : ['/eos/cms/store/group/phys_btag/Commissioning/TTbar/Run2016/crab/miniAODv2/mc/05Dec2016_Summer16-PUMoriond17/06d3c70/ST_tW_top_5f_inclusiveDecays_13TeV-powheg-pythia8_TuneCUETP8M2T4'],
    'puWgtUrl'          : defalutPUWgtUrl,
}

dataset['st2'] = {
    'xsec'              : 35.85,
    'ipath'             : ['/eos/cms/store/group/phys_btag/Commissioning/TTbar/Run2016/crab/miniAODv2/mc/05Dec2016_Summer16-PUMoriond17/06d3c70/ST_tW_antitop_5f_inclusiveDecays_13TeV-powheg-pythia8_TuneCUETP8M1'],
    'puWgtUrl'          : defalutPUWgtUrl,
}

dataset['ww'] = {
    'xsec'              : 118.7,
    'ipath'             : ['/eos/cms/store/group/phys_btag/Commissioning/TTbar/Run2016/crab/miniAODv2/mc/05Dec2016_Summer16-PUMoriond17/06d3c70/WW_TuneCUETP8M1_13TeV-pythia8'],
    # 'ipath'             : ['/eos/cms/store/group/phys_btag/Commissioning/TTbar/Run2016/crab/miniAODv2/mc/05Dec2016_Summer16-PUMoriond17/06d3c70/WWTo2L2Nu_13TeV-powheg'],
    'puWgtUrl'          : defalutPUWgtUrl,
}

dataset['wz'] = {
    'xsec'              : 47.13,
    'ipath'             : ['/eos/cms/store/group/phys_btag/Commissioning/TTbar/Run2016/crab/miniAODv2/mc/05Dec2016_Summer16-PUMoriond17/06d3c70/WZ_TuneCUETP8M1_13TeV-pythia8'],
    # 'ipath'             : ['/eos/cms/store/group/phys_btag/Commissioning/TTbar/Run2016/crab/miniAODv2/mc/05Dec2016_Summer16-PUMoriond17/06d3c70/WZTo3LNu_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8'],
    'puWgtUrl'          : defalutPUWgtUrl,
}

dataset['zz'] = {
    'xsec'              : 16.5,
    'ipath'             : ['/eos/cms/store/group/phys_btag/Commissioning/TTbar/Run2016/crab/miniAODv2/mc/05Dec2016_Summer16-PUMoriond17/06d3c70/ZZ_TuneCUETP8M1_13TeV-pythia8'],
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
    'dataset'   : ['runB_23JunRereco'],
    'xsec'      : [-1],
}
