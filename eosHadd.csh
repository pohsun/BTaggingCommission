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

mkdir -p $outputDir
set roots=`ls -l $inputDir | grep '.root' | grep $rootname | awk '{print $9}'`

cmsenv
cd $inputDir
    hadd $outputDir'/'$rootname'.root' $roots
cd -
