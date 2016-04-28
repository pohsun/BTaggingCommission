#!/bin/tcsh
if ( $3 == "" ) then
    echo ">> Please input path"
    echo ">> ./eosHadd.csh [inputDir] [rootname] [outputDir]"
    exit
endif

set limit=25
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

#set roots=`ls -l $inputDir | grep '.root' | grep $rootname | awk '{print $9}'`
ls -l $inputDir | grep '.root' | grep $rootname | awk '{print $9}' >! .tmp_roots
if ( `cat .tmp_roots | grep root` == '' ) then
    ls -l $inputDir | grep '.root' | grep $rootname | awk '{print $8}' >! .tmp_roots
endif

if ( `cat .tmp_roots | grep root` == '' ) then
    echo ">> No $rootname root files in $inputDir"
    exit
endif

set roots=`cat .tmp_roots`
set size=`echo $roots | wc  -w`

cmsenv
mkdir -p $outputDir

if ( $size < $limit ) then
    cd $inputDir
        hadd $outputDir'/'$rootname'.root' $roots
    cd -
else
    set i=0
    set s=`echo $size/$limit'+1' | bc`
    echo ">> Out of $limit with $size, split to $s files"

    while ( $i < $s )
        set lines=`echo $i'*'$limit'+1' | bc`
        if ( $i == 0 ) then
            set roots=`cat .tmp_roots | head -n $limit`
        else
            set roots=`cat .tmp_roots | tail -n +$lines | head -n $limit`
        endif
        @ i++
        set sizenew=`echo $roots | wc -w`
        echo ">> File $i/$s with $sizenew"
        cd $inputDir
            hadd $outputDir'/'$rootname'__'$i'.root' $roots
        cd -
    end   
 
endif

rm -f .tmp_roots
