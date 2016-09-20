#!/bin/tcsh

set rootname='MergedJetTree'
set inpath='~/eos/cms/store/group/phys_btag/Commissioning/TTbar/25ns'
set outpath='~/eos/cms/store/user/jtsai/BTaggingCommission/Run2015_25ns'
set samlpes='Data13TeV_MuonEG_2015D'
#set samlpes='Data13TeV_MuonEG_2015D MC13TeV_TTJets_powheg_pythia8 MC13TeV_DY10to50 MC13TeV_DY50toInf MC13TeV_SingleTbar_tW MC13TeV_SingleT_tW MC13TeV_WWTo2L2Nu MC13TeV_WZ MC13TeV_ZZ'
#set samlpes='Data13TeV_MuonEG_2015C Data13TeV_MuonEG_2015D MC13TeV_TTJets_powheg_pythia8 MC13TeV_DY10to50 MC13TeV_DY50toInf MC13TeV_SingleTbar_tW MC13TeV_SingleT_tW MC13TeV_WWTo2L2Nu MC13TeV_WZ MC13TeV_ZZ'

foreach sample($samlpes)
    set indir=$inpath'/'$sample
    set outdir=$outpath'/'$sample
    echo  $indir
    echo  $outdir
    ./eosHadd.csh $indir $rootname $outdir 
end
