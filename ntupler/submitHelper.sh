#!/usr/bin/env bash

lumimask="/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions17/13TeV/Final/Cert_294927-306462_13TeV_PromptReco_Collisions17_JSON.txt"

# Data
#workdir="04Jan2018_DATA"
#psetname="/afs/cern.ch/work/p/pchen/public/btaggingCommRun2/prod/myBTaggingCommission/ntupler/data/samples_Run2017_04Jan_miniAOD_Data.json"
#lfnpath="/store/group/phys_btag/Commissioning/TTbar/Run2017/crab/04Jan2018_Ru2017Full-17Nov2017ReReco"
#globalTag="94X_dataRun2_ReReco_EOY17_v2"

# MC
workdir="04Jan2018_MC"
psetname="/afs/cern.ch/work/p/pchen/public/btaggingCommRun2/prod/myBTaggingCommission/ntupler/data/samples_Run2017_04Jan_miniAOD.json"
lfnpath="/store/group/phys_btag/Commissioning/TTbar/Run2017/crab/04Jan2018_Moriond18-94X"
globalTag="94X_mc2017_realistic_v10"

cd ${CMSSW_BASE}/src/RecoBTag/PerformanceMeasurements/test/ttbar
python submitToGrid.py -j ${psetname} -c ${CMSSW_BASE}/src/RecoBTag/PerformanceMeasurements/test/runBTagAnalyzer_cfg.py --lfn ${lfnpath} -l ${lumimask} -w ${workdir} -s

