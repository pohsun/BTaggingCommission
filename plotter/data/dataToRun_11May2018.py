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
# defaultLumi=41856. # In /pb, Full2017
defaultLumi=1034. # In /pb, Early Run2018A
defalutPUWgtUrl="/afs/cern.ch/work/p/pchen/public/btaggingCommRun2/prod/20180303_newJEC/CMSSW_9_4_1/src/RecoBTag/PerformanceMeasurements/test/ttbar/03Mar2018_DATA/merged_puWgtXsec69000.root"

#Run2017_17Nov2017ReReco
dataDir="/eos/cms/store/group/phys_btag/Commissioning/TTbar/Run2018/crab/11May2018_RunA-PromptReco-v1/df4374e/"
dataset['runA-v1'] = {
    'xsec'              : -1,
    'ipath'             : [dataDir+'/MuonEG/crab_Data13TeV_MuonEG_2018A-v1'],
    'puWgtUrl'          : defalutPUWgtUrl,
}

# If 'xsec' is -1, take the 'xsec' setting from datasets.
groupdata = {}
groupdata['data'] = {
    'lumi'      : -1,
    'dataset'   : ['runA-v1'],
    'xsec'      : [-1]*1,
}
