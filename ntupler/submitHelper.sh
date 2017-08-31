#!/usr/bin/env bash

lumimask="/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions17/13TeV/PromptReco/Cert_294927-301567_13TeV_PromptReco_Collisions17_JSON.txt"



## Data
workdir="28Aug2017_DATA"
psetname="/afs/cern.ch/work/p/pchen/public/btaggingCommRun2/prod/myBTaggingCommission/ntupler/data/samples_Run2017_28Aug_miniAOD_Data.json"
lfnpath="/store/group/phys_btag/Commissioning/TTbar/Run2017/crab/28Aug2017_RunBC-PromptReco"

## MC
#workdir="28Aug2017_MC"
#psetname="/afs/cern.ch/work/p/pchen/public/btaggingCommRun2/prod/myBTaggingCommission/ntupler/data/samples_Run2017_28Aug_miniAOD.json"
#lfnpath="/store/group/phys_btag/Commissioning/TTbar/Run2017/crab/28Aug2017_Summer17-92X"

cd ${CMSSW_BASE}/src/RecoBTag/PerformanceMeasurements/test/ttbar
python submitToGrid.py -j ${psetname} -c ${CMSSW_BASE}/src/RecoBTag/PerformanceMeasurements/test/runBTagAnalyzer_cfg.py --lfn ${lfnpath} -l ${lumimask} -w ${workdir} -s

