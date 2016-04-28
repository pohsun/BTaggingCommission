#!/bin/bash

WORKSPACE="/afs/cern.ch/work/j/jtsai/BTagging/commissioning/CMSSW_7_6_3/src/RecoBTag/PerformanceMeasurements/test/BTagAnalyzerMacros/BTaggingCommission"

ROOTNAME="MergedJetTree"
INPATH="/eos/cms/store/caf/user/jtsai/BTaggingCommission/Run2015_25ns"
OUTPATH="/eos/cms/store/caf/user/jtsai/BTaggingCommission/Run2015_25ns"
SAMPLE="MC13TeV_TTJets_powheg_pythia8"

BATCHDIR=${PWD}
export SCRAM_ARCH=slc6_amd64_gcc481

cd $WORKSPACE
    eval `scram runtime -sh`

#if [ ! -e $INPATH  ]
#then 
#    echo ">> Error input dir not exist : $INPATH"
#    exit
#fi

roots=`/afs/cern.ch/project/eos/installation/cms/bin/eos.select ls -l $INPATH'/'$SAMPLE | grep '.root' | grep $ROOTNAME | awk '{print $9}'`
size=`echo $roots | grep root | wc -w`
if [ $size == 0 ] 
then
    roots=`/afs/cern.ch/project/eos/installation/cms/bin/eos.select ls -l $INPATH'/'$SAMPLE | grep '.root' | grep $ROOTNAME | awk '{print $8}'`
    size=`echo $roots | grep root | wc -w`
fi
if [ $size == 0 ] 
then
    echo ">> No $ROOTNAME root files in $INPATH'/'$SAMPLE"
    exit
fi
/afs/cern.ch/project/eos/installation/cms/bin/eos.select mkdir -p $OUTPATH'/'$SAMPLE

cd $BATCHDIR
rm -f input.lists
touch input.lists
for fileroot in $roots
do
   echo 'root://eoscms.cern.ch/'$INPATH'/'$SAMPLE'/'$fileroot >> input.lists
done

cat input.lists
hadd $ROOTNAME'.root' @input.lists
echo 'Copying file '$ROOTNAME'.root to '$OUTPATH'/'$SAMPLE'/'$ROOTNAME'.root' 
/afs/cern.ch/project/eos/installation/0.3.15/bin/eos.select cp $ROOTNAME'.root' $OUTPATH'/'$SAMPLE'/'$ROOTNAME'.root'

exitcode=$?
exit $exitcode

