#!/bin/env python
import sys
import argparse 
import os 

def main(args):
    parser = argparse.ArgumentParser(
            "Submit jobs for commands"
            )

    parser.add_argument(
            '-l', '--lumimask',
            type=str,
            default="/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions18/13TeV/ReReco/Cert_314472-325175_13TeV_17SeptEarlyReReco2018ABC_PromptEraD_Collisions18_JSON.txt"
            )
    parser.add_argument(
            '-t', '--target',
            type=str,
            required=True
            )
    parser.add_argument(
            '-g', '--globaltag',
            type=str,
            required=True 
            )
    parser.add_argument(
            '-p', '--pset',
            type=str,
            required=True
            ),
    parser.add_argument(
            '--lfnpath',
            type=str,
            default="/store/group/phys_btag/Commissioning/TTbar/Run2018/crab/"
            )
    try:
        opt = parser.parse_args(args[1:])
    except:
        parser.print_help()
        raise

    import datetime
    today = datetime.date.today()
    CMSSW_BASE = os.getenv("CMSSW_BASE")
    opt.target = str(today) + "_" + opt.target
    opt.pset = CMSSW_BASE + "/src/RecoBTag/PerformanceMeasurements/test/BTaggingCommission/ntupler/data/" + opt.pset
    opt.lfnpath += opt.target

    os.chdir( CMSSW_BASE + "/src/RecoBTag/PerformanceMeasurements/test/ttbar" )
    cmd = "python submitToGrid.py -j {} -c {}/src/RecoBTag/PerformanceMeasurements/test/runBTagAnalyzer_cfg.py --lfn {} -l {} -w {} -s".format(
            opt.pset,
            CMSSW_BASE,
            opt.lfnpath,
            opt.lumimask,
            opt.target
            )
    os.system( cmd )

if __name__ == '__main__':
    main(sys.argv)
