#!/usr/bin/env bash

lumimask="/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions17/13TeV/Final/Cert_294927-306462_13TeV_PromptReco_Collisions17_JSON.txt"

# Data
workdir="24Apr2018_DATA"
psetname="/afs/cern.ch/work/p/pchen/public/btaggingCommRun2/prod/myBTaggingCommission/ntupler/data/samples_Run2017_24Apr_miniAOD_Data.json"
lfnpath="/store/group/phys_btag/Commissioning/TTbar/Run2017/crab/24Apr2018_deepFlavour"
globalTag="94X_dataRun2_ReReco_EOY17_v2"

# MC
#workdir="24Apr2018_MC"
#psetname="/afs/cern.ch/work/p/pchen/public/btaggingCommRun2/prod/myBTaggingCommission/ntupler/data/samples_Run2017_24Apr_miniAOD.json"
#lfnpath="/store/group/phys_btag/Commissioning/TTbar/Run2017/crab/24Apr2018_deepFlavour"
#globalTag="94X_mc2017_realistic_v12"

cd ${CMSSW_BASE}/src/RecoBTag/PerformanceMeasurements/test/ttbar
python submitToGrid.py -j ${psetname} -c ${CMSSW_BASE}/src/RecoBTag/PerformanceMeasurements/test/runBTagAnalyzer_cfg.py --lfn ${lfnpath} -l ${lumimask} -w ${workdir} -s

