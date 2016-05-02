#!/bin/tcsh
if ( $2 == "" ) then
    echo ">> Please give options"
    echo "   ./submitRunCodeBsub.csh [number or all] [outpath]"
    exit
endif

if ( ! ( -e $2 ) ) then
    mkdir $2
endif

set here=`echo $PWD | sed 's/\//\\\//g'`
set out=`echo $2 | sed 's/\//\\\//g'`
set totalN=9
if ( $1 == 'all' ) then
    i=0
    while ( i<9 )
        cat runCodeBsub.sh | sed "s/RUNN_tmp/$i/g" | sed "s/WORKSPACE_tmp/$here/g" | sed "s/OUTPATH_tmp/$out/g" >! $2/runCodeBsub_$i.sh 
        bsub -q cmscaf1nd -o $2/runCodeBsub_$i.log source $2/runCodeBsub_$i.sh
        @ i++
    end
else
    cat runCodeBsub.sh | sed 's/RUNN_tmp/'$1'/g' | sed 's/WORKSPACE_tmp/'$here'/g' | sed 's/OUTPATH_tmp/'$out'/g' >! $2/runCodeBsub_$1.sh 
    bsub -q cmscaf1nd -o $2/runCodeBsub_$1.log source $2/runCodeBsub_$1.sh
endif

