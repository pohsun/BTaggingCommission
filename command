bsub -q cmscaf1nd -o eosHaddBsub.log source eosHaddBsub.sh
./submitRunCodeBsub.csh 1 /afs/cern.ch/work/j/jtsai/BTagging/commissioning/CMSSW_7_6_3/src/RecoBTag/PerformanceMeasurements/test/BTagAnalyzerMacros/02May_test
python runPileupEstimation.py --json data/lumiSummary_MuonEG_CD.json

hadd output_all.root output_ttbar.root output_dy2.root output_data_mueg.root output_st2.root output_dy1.root output_zz.root output_st1.root output_wz.root output_ww.root
