dataset = {}

# An example config
#dataset['name'] = {
#    'xsec'              : -1, # -1 for data, others for MC
#    'ipath'             : ['localpath','lfnpath1','lfnpath2'], # Will check ALL '*.root' under the dir, localpath and lfn are supported.
#}

defaultLumi= 35922 # In /pb, Full2016
defalutPUWgtUrl="/afs/cern.ch/work/p/pchen/public/btaggingCommRun2/prod/20190101_Moriond19/CMSSW_9_4_12/src/RecoBTag/PerformanceMeasurements/test/ttbar/data/pileupWgts_puWgtXsec69000.root"

dataset['Run2016_Jul18ReReco'] = {
    'xsec'              : -1,
    'ipath'             : [
        '/eos/cms/store/group/phys_btag/Commissioning/TTbar/Run2016/crab/20190101_DATA_17Jul2018ReReco/68a10d1/MuonEG/crab_Data13TeV_MuonEG_2016B_Jul18_ver2',
        '/eos/cms/store/group/phys_btag/Commissioning/TTbar/Run2016/crab/20190101_DATA_17Jul2018ReReco/68a10d1/MuonEG/crab_Data13TeV_MuonEG_2016C_Jul18',
        '/eos/cms/store/group/phys_btag/Commissioning/TTbar/Run2016/crab/20190101_DATA_17Jul2018ReReco/68a10d1/MuonEG/crab_Data13TeV_MuonEG_2016D_Jul18',
        '/eos/cms/store/group/phys_btag/Commissioning/TTbar/Run2016/crab/20190101_DATA_17Jul2018ReReco/68a10d1/MuonEG/crab_Data13TeV_MuonEG_2016E_Jul18',
        '/eos/cms/store/group/phys_btag/Commissioning/TTbar/Run2016/crab/20190101_DATA_17Jul2018ReReco/68a10d1/MuonEG/crab_Data13TeV_MuonEG_2016F_Jul18',
        '/eos/cms/store/group/phys_btag/Commissioning/TTbar/Run2016/crab/20190101_DATA_17Jul2018ReReco/68a10d1/MuonEG/crab_Data13TeV_MuonEG_2016G_Jul18',
        '/eos/cms/store/group/phys_btag/Commissioning/TTbar/Run2016/crab/20190101_DATA_17Jul2018ReReco/68a10d1/MuonEG/crab_Data13TeV_MuonEG_2016H_Jul18',
        ],
    'puWgtUrl'          : defalutPUWgtUrl,
}

dataset['ttbar'] = {
    'xsec'              : 831.76,
    'ipath'             : ['/eos/cms/store/group/phys_btag/Commissioning/TTbar/Run2016/crab/20190101_MC_RunIISummer16MiniAODv3/68a10d1/TT_TuneCUETP8M2T4_13TeV-powheg-pythia8/crab_MC13TeV_TTJets'],
    'puWgtUrl'          : defalutPUWgtUrl,
}

dataset['dy1'] = {
    'xsec'              : 18610,
    'ipath'             : ['/eos/cms/store/group/phys_btag/Commissioning/TTbar/Run2016/crab/20190101_MC_RunIISummer16MiniAODv3/68a10d1/DYJetsToLL_M-10to50_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8/crab_MC13TeV_DYJetsToLL_M-10to50_FXFX'],
    'puWgtUrl'          : defalutPUWgtUrl,
}

dataset['dy2'] = {
    'xsec'              : 2075.14*3,
    'ipath'             : ['/eos/cms/store/group/phys_btag/Commissioning/TTbar/Run2016/crab/20190101_MC_RunIISummer16MiniAODv3/68a10d1/DYJetsToLL_M-50_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8/crab_MC13TeV_DYJetsToLL_M-50_FXFX'],
    'puWgtUrl'          : defalutPUWgtUrl,
}

dataset['st1'] = {
    'xsec'              : 35.85,
    'ipath'             : ['/eos/cms/store/group/phys_btag/Commissioning/TTbar/Run2016/crab/20190101_MC_RunIISummer16MiniAODv3/68a10d1/ST_tW_top_5f_inclusiveDecays_13TeV-powheg-pythia8_TuneCUETP8M2T4/crab_MC13TeV_tW'],
    'puWgtUrl'          : defalutPUWgtUrl,
}

dataset['st2'] = {
    'xsec'              : 35.85,
    'ipath'             : ['/eos/cms/store/group/phys_btag/Commissioning/TTbar/Run2016/crab/20190101_MC_RunIISummer16MiniAODv3/68a10d1/ST_tW_antitop_5f_inclusiveDecays_13TeV-powheg-pythia8_TuneCUETP8M2T4/crab_MC13TeV_atW'],
    'puWgtUrl'          : defalutPUWgtUrl,
}

dataset['ww'] = {
    'xsec'              : 118.7,
    'ipath'             : [
        '/eos/cms/store/group/phys_btag/Commissioning/TTbar/Run2016/crab/20190101_MC_RunIISummer16MiniAODv3/68a10d1/WW_TuneCUETP8M1_13TeV-pythia8/crab_MC13TeV_WW',
        '/eos/cms/store/group/phys_btag/Commissioning/TTbar/Run2016/crab/20190101_MC_RunIISummer16MiniAODv3/68a10d1/WW_TuneCUETP8M1_13TeV-pythia8/crab_MC13TeV_WW_ext'
    ],
    'puWgtUrl'          : defalutPUWgtUrl,
}

dataset['wz'] = {
    'xsec'              : 47.13,
    'ipath'             : [
        '/eos/cms/store/group/phys_btag/Commissioning/TTbar/Run2016/crab/20190101_MC_RunIISummer16MiniAODv3/68a10d1/WZ_TuneCUETP8M1_13TeV-pythia8/crab_MC13TeV_WZ',
        '/eos/cms/store/group/phys_btag/Commissioning/TTbar/Run2016/crab/20190101_MC_RunIISummer16MiniAODv3/68a10d1/WZ_TuneCUETP8M1_13TeV-pythia8/crab_MC13TeV_WZ_ext'
    ],
    'puWgtUrl'          : defalutPUWgtUrl,
}

dataset['zz'] = {
    'xsec'              : 16.523,
    'ipath'             : ['/eos/cms/store/group/phys_btag/Commissioning/TTbar/Run2016/crab/20190101_MC_RunIISummer16MiniAODv3/68a10d1/ZZ_TuneCUETP8M1_13TeV-pythia8/crab_MC13TeV_ZZ'],
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
    'dataset'   : ['Run2016_Jul18ReReco',],
    'xsec'      : [-1,],
}
