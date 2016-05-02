bsub -q cmscaf1nd -o eosHaddBsub.log source eosHaddBsub.sh
python runPileupEstimation.py --json data/lumiSummary_MuonEG_CD.json
