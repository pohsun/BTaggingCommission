#!/usr/bin/env bash

#lumimask="/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions18/13TeV/PromptReco/Cert_314472-325175_13TeV_PromptReco_Collisions18_JSON.txt"
lumimask="/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions18/13TeV/ReReco/Cert_314472-325175_13TeV_17SeptEarlyReReco2018ABC_PromptEraD_Collisions18_JSON.txt"

# Data
workdir="16May2019_MuonEG_ultimateSF"
psetname="/afs/cern.ch/work/p/pusheng/CMSSW_10_2_11/src/RecoBTag/PerformanceMeasurements/test/BTaggingCommission/ntupler/data/samples_Run2018_20190101_miniAOD_Data.json"
lfnpath="/store/group/phys_btag/Commissioning/TTbar/Run2018/crab/16May2019_MuonEG_ultimateSF"
globalTag="102X_dataRun2_Sep2018Rereco_v1"

# MC
#workdir="03Dec2018_MC_Fall17v6JEC"
#psetname="/afs/cern.ch/work/p/pchen/public/btaggingCommRun2/prod/myBTaggingCommission/ntupler/data/samples_Run2017_20181203_miniAOD.json"
#lfnpath="/store/group/phys_btag/Commissioning/TTbar/Run2017/crab/03Dec2018_Fall17MCv2_Fall17v6JEC"
#globalTag="94X_mc2017_realistic_v12"

cd ${CMSSW_BASE}/src/RecoBTag/PerformanceMeasurements/test/ttbar
python submitToGrid.py -j ${psetname} -c ${CMSSW_BASE}/src/RecoBTag/PerformanceMeasurements/test/runBTagAnalyzer_cfg.py --lfn ${lfnpath} -l ${lumimask} -w ${workdir} -s

