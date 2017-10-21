#!/usr/bin/env bash

lumimask="/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions17/13TeV/PromptReco/Cert_294927-302343_13TeV_PromptReco_Collisions17_JSON.txt"

## Data
#workdir="25Sep2017_DATA"
#psetname="/afs/cern.ch/work/p/pchen/public/btaggingCommRun2/prod/myBTaggingCommission/ntupler/data/samples_Run2017_25Sep_miniAOD_Data.json"
#lfnpath="/store/group/phys_btag/Commissioning/TTbar/Run2017/crab/25Sep2017_RunBCD-PromptReco"
#globalTag="92X_dataRun2_Prompt_v8"

workdir="21Oct2017_DATA"
psetname="/afs/cern.ch/work/p/pchen/public/btaggingCommRun2/prod/myBTaggingCommission/ntupler/data/samples_Run2017_21Oct_miniAOD_Data.json"
lfnpath="/store/group/phys_btag/Commissioning/TTbar/Run2017/crab/21Oct2017_RunEF-PromptReco"
globalTag="92X_dataRun2_Prompt_v9"


## MC
#workdir="25Sep2017_MC"
#psetname="/afs/cern.ch/work/p/pchen/public/btaggingCommRun2/prod/myBTaggingCommission/ntupler/data/samples_Run2017_25Sep_miniAOD.json"
#lfnpath="/store/group/phys_btag/Commissioning/TTbar/Run2017/crab/25Sep2017_Summer17-92X"
#globalTag="92X_upgrade2017_realistic_v10"

cd ${CMSSW_BASE}/src/RecoBTag/PerformanceMeasurements/test/ttbar
python submitToGrid.py -j ${psetname} -c ${CMSSW_BASE}/src/RecoBTag/PerformanceMeasurements/test/runBTagAnalyzer_cfg.py --lfn ${lfnpath} -l ${lumimask} -w ${workdir} -s

