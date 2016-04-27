#!/bin/tcsh
if ( $3 == "" ) then
    echo ">> Please input path"
    echo ">> ./eosHadd.csh [inputDir] [rootname] [outputDir]"
    exit
endif

set inputDir=$1
set outputDir=$3
set rootname=$2

#if ( ! ( -e '~/eos/cms' ) ) then
#    eosmount ~/eos
#endif
if ( ! ( -e $inputDir ) ) then
    echo ">> Error input dir not exist : $inputDir"
    exit
endif

set roots=`ls -l $inputDir | grep '.root' | grep $rootname | awk '{print $9}'`
if ( `echo $roots | grep root` == '' ) then
    set roots=`ls -l $inputDir | grep '.root' | grep $rootname | awk '{print $8}'`
endif

if ( `echo $roots | grep root` == '' ) then
    echo ">> No $rootname root files in $inputDir"
    exit
endif

cmsenv
mkdir -p $outputDir
cd $inputDir
    hadd $outputDir'/'$rootname'.root' $roots
cd -
