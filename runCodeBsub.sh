#!/bin/bash

#RUNN=0
#WORKSPACE="/afs/cern.ch/work/j/jtsai/BTagging/commissioning/CMSSW_7_6_3/src/RecoBTag/PerformanceMeasurements/test/BTagAnalyzerMacros/BTaggingCommission"
RUNN=RUNN_tmp
WORKSPACE=WORKSPACE_tmp
OUTPATH=OUTPATH_tmp

BATCHDIR=${PWD}
mkdir subdir 
export SCRAM_ARCH=slc6_amd64_gcc530

cd $WORKSPACE
    eval `scram runtime -sh`
    cp runCode4ttbar.C             $BATCHDIR/subdir/runCode4ttbar.C
    cp run.C                       $BATCHDIR/subdir/run.C 
    cp ../CommPlotProducer4ttbar.h $BATCHDIR/subdir/CommPlotProducer4ttbar.h
    cp ../CommPlotProducer4ttbar.C $BATCHDIR/subdir/CommPlotProducer4ttbar.C
    cp ../../common.h              $BATCHDIR/common.h
    cp ../../TTbarSelector.C       $BATCHDIR/TTbarSelector.C
    cp ../../TTbarSelector.h       $BATCHDIR/TTbarSelector.h
    sed -i "s/runN/${RUNN}/g" $BATCHDIR/subdir/runCode4ttbar.C 

cd $BATCHDIR/subdir
    root run.C -b -q
    cp *.root $OUTPATH

exitcode=$?
exit $exitcode

