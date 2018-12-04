#!/usr/bin/env bash

#lumimask="/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions18/13TeV/PromptReco/Cert_314472-325175_13TeV_PromptReco_Collisions18_JSON.txt"
lumimask="/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions17/13TeV/Final/Cert_294927-306462_13TeV_PromptReco_Collisions17_JSON.txt"

# Data
#workdir="03Dec2018_DATA_Fall17v6JEC"
#psetname="/afs/cern.ch/work/p/pchen/public/btaggingCommRun2/prod/myBTaggingCommission/ntupler/data/samples_Run2017_20181203_miniAOD_Data.json"
#lfnpath="/store/group/phys_btag/Commissioning/TTbar/Run2017/crab/03Dec2018_31Mar2018-v1_Fall17v6JEC"
#globalTag="94X_dataRun2_ReReco_EOY17_v6"

# MC
workdir="03Dec2018_MC_Fall17v6JEC"
psetname="/afs/cern.ch/work/p/pchen/public/btaggingCommRun2/prod/myBTaggingCommission/ntupler/data/samples_Run2017_20181203_miniAOD.json"
lfnpath="/store/group/phys_btag/Commissioning/TTbar/Run2017/crab/03Dec2018_Fall17MCv2_Fall17v6JEC"
globalTag="94X_mc2017_realistic_v12"

cd ${CMSSW_BASE}/src/RecoBTag/PerformanceMeasurements/test/ttbar
python submitToGrid.py -j ${psetname} -c ${CMSSW_BASE}/src/RecoBTag/PerformanceMeasurements/test/runBTagAnalyzer_cfg.py --lfn ${lfnpath} -l ${lumimask} -w ${workdir} -s

