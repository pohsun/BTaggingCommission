bsub -q cmscaf1nd -o eosHaddBsub.log source eosHaddBsub.sh
./submitRunCodeBsub.csh 1 /afs/cern.ch/work/j/jtsai/BTagging/commissioning/CMSSW_7_6_3/src/RecoBTag/PerformanceMeasurements/test/BTagAnalyzerMacros/02May_test
python runPileupEstimation.py --json data/lumiSummary_MuonEG_CD.json
