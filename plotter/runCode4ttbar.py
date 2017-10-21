#!/usr/bin/env python

import sys, os, string, re, copy
import argparse
from subprocess import Popen, PIPE, call
import fnmatch

from ROOT import gROOT, TChain, TFile, TH1F
gROOT.SetBatch()

usage = """Usage: ./runAnalyzer4ttbar.py subcommand [options]\n
Example: ./runAnalyzer4ttbar.py submit\n
For more info: ./runAnalyzer4ttbar.py --help
"""

# Settings
rootDir = os.path.expandvars("${CMSSW_BASE}/src/RecoBTag/PerformanceMeasurements/test/BTagAnalyzerMacros")
# eosCmd          = '/afs/cern.ch/project/eos/installation/0.3.84-aquamarine/bin/eos.select'
eosCmd          = '/usr/bin/eos'
defaultQueue    = '8nh'
batchSize       = 100
CWD             = os.getcwd()

# File listers
def plainFileLister(ipath='.'):
    return [ "file://{0}".format(f) for f in os.listdir(ipath) if fnmatch.fnmatch(f, '*.root') ]

def eosFileLister(ipath,crabDirTag="crab_.*"):
    """This function returns a list of outputs of a crab task"""
    output = []
    crabDirPattern = re.compile('/eos/cms/store/group/phys_btag/Commissioning/TTbar/.*/'+crabDirTag+'/[0-9]{6}_[0-9]{6}/[0-9]{4}$')
    for root, dirs, files in os.walk(ipath):
        if crabDirPattern.match(root):
            output += [ "root://eoscms.cern.ch//{0}/{1}".format(os.path.join(*root.split('/')[3:]),f) for f in files ]
    return output

def fileLister(ipath):
    if ipath.startswith('/eos/cms/'):
        return eosFileLister(ipath)
    else:
        return plainFileLister(ipath)

def writeJobScript(ifname,ofname,ifPathType='eos'):
    pass

def compileMacros(args):
    os.chdir(rootDir)
    if args.compile or not os.path.exists("CommPlotProducer4ttbar_C.so"):
        gROOT.ProcessLine(".L ../TTbarSelector.C++")
        gROOT.ProcessLine(".L CommPlotProducer4ttbar.C++g")
    os.chdir(CWD)
    pass

def puEstimation(args):
    isPUMixerUpdated = raw_input("Is Mixing module in runPileupEstimation.py updated? [y/n] ")
    if isPUMixerUpdated != 'y':
        print "Please update the MixingModule first. Abort."
        return
    else:
        json = os.path.expandvars(args.json)
        xsecs = [63000,65000,67000,69000,71000,73000,75000]

        for xs in xsecs:
            cmd = "python {0}/../ttbar/runPileupEstimation.py --mbXsec={1} --json={2}".format(rootDir,str(xs),json)
            call(cmd, shell=True)
            cmd = "mv {0} {1}".format("{0}/../ttbar/data/pileupWgts.root".format(rootDir), os.path.splitext(json)[0]+'_puWgtXsec'+str(xs)+'.root')
            call(cmd, shell=True)
    pass


def createBatch(args):
    defaultQueue    = "8nh"
    compileMacros(args)

    # Prepare runtime script
    batchTemplate = """#!/usr/bin/env bash

#BSUB-q QUEUE
#BSUB-J JOBNAME
#BSUB-c 60

cd WORKDIR
eval `scramv1 runtime -sh`

python {0} run CFG --pu "PUWGT"
""".format(os.path.realpath(__file__))

    # Load dataset, groupdata
    handle = open(os.path.join(CWD,args.datacard))
    exec(handle)
    handle.close()

    # create list of input files
    scriptDir = os.path.join(rootDir, "batch")
    if not os.path.isdir(scriptDir):
        os.makedirs(scriptDir)
    for datasetName, data in dataset.iteritems():
        print "Creating batch jobs for {0}.".format(datasetName)
        # Get full list of input files
        iFileList = []
        for iPath in data['ipath']:
            iFileList += fileLister(iPath)

        data['puWgtUrl'] = os.path.expandvars(data['puWgtUrl'])
        puWgtUrl = data['puWgtUrl'] if data['puWgtUrl'].startswith('/') else os.path.join(CWD,data['puWgtUrl'])

        # Split jobs
        for iPart in range(0,(len(iFileList)-1)/batchSize+1):
            oFileListName = "flist_{0}_part{1:02d}.py".format(datasetName,iPart);
            with open(os.path.join(scriptDir,oFileListName),'w+') as oFileList:
                oFileList.write("dataset = '{0}_part{1:02d}'\n".format(datasetName,iPart))
                oFileList.write("isData  = {0}\n".format("True" if data['xsec'] == -1 else "False"))
                oFileList.write("inputFiles = [\n")
                for iFile in iFileList[iPart*batchSize:batchSize+iPart*batchSize]:
                    oFileList.write("    '"+iFile+"',\n")
                oFileList.write("]\n")
            os.chmod(os.path.join(scriptDir,oFileListName),0644)

            batchScript = re.sub('QUEUE'        , args.queue                                    , batchTemplate)
            batchScript = re.sub('WORKDIR'      , rootDir                                       , batchScript)
            batchScript = re.sub('JOBNAME'      , "{0}_p{1:02d}".format(datasetName,iPart)      , batchScript)
            batchScript = re.sub("CFG"          , os.path.join(scriptDir,oFileListName)         , batchScript)
            batchScript = re.sub("PUWGT"        , puWgtUrl                                      , batchScript)
            oBatchScriptName=os.path.join(scriptDir,"batchJob_{0}_part{1:02d}.sh".format(datasetName,iPart))
            with open(oBatchScriptName,'w+') as batchScriptFile:
                batchScriptFile.write(batchScript)
            os.chmod(oBatchScriptName,0755)
            if args.submit and not os.path.exists(os.path.join(rootDir,"output_{0}_part{1:02d}.root".format(datasetName,iPart))):
                call("bsub -q {0} {1}".format(args.queue, oBatchScriptName),shell=True)
    pass

def runJob(args):
    # Handle configurations
    with open(os.path.join(CWD,args.cfg)) as handle:
        exec(handle)

    # Start processing
    os.chdir(rootDir)
    gROOT.ProcessLine(".L ../TTbarSelector.C+")
    gROOT.ProcessLine(".L CommPlotProducer4ttbar.C+")
    os.chdir(CWD)
    from ROOT import CommPlotProducer4ttbar

    PtMin_Cut = 30  # Not really used in CommPlotProducer4ttbar
    PtMax_Cut = 500
    syst = ""

    tree = TChain("btagana/ttree")
    inputWeight = 0 if isData else TH1F("wgtcounter","",1,0,1)
    for iFile in inputFiles:
        if not isData:
            f = TFile.Open(iFile)
            w = f.Get("ttbarselectionproducer/wgtcounter")
            inputWeight.Fill(0,w.GetBinContent(1))
            f.Close()
        tree.Add(iFile)
    url = args.puWgtUrl if os.path.expandvars(args.puWgtUrl).startswith('/') else os.path.join(CWD,args.puWgtUrl)
    run = CommPlotProducer4ttbar(tree,1,1,0,url)
    run.Loop(isData, "output_{0}".format(dataset), inputWeight, syst)
    pass

def hadd():
    pass

def merge(args):
    isJobsMerged = raw_input("Have you run hadd for output from CommPlotProducer4ttbar? [y/n] ")
    if isJobsMerged != 'y':
        print "Please hadd the output first. Abort."
        return

    # make sure all necessary input parameters are provided
    handle = open(args.cfg)
    exec handle in globals()
    handle.close()

    main_workdir = args.workDir
    # redefine main_workdir as an absolute path (if not defined in such form already)
    if not re.search('^/', main_workdir):
        main_workdir = os.path.join(os.getcwd(),main_workdir)
    output_dir = main_workdir

    # open and read the dataset_list_for_merging
    filename=args.outputName
    if not filename.endswith('.root'):
        filename = filename+'.root'

    output_root_file = TFile( os.path.join(output_dir,filename), 'RECREATE' )

    # write histograms
    for groupKey, groupVal in groupdata.iteritems():
        groupVal['xsec'] = [ (lambda x, idx: x if x > 0 else dataset[groupVal['dataset'][idx]]['xsec'])(x,idx) for idx, x in enumerate(groupVal['xsec']) ]
        print groupKey, groupVal

        final_histos = {}
        for datasetIdx, datasetKey in enumerate(groupVal['dataset']):
            input_root_file  = os.path.join(main_workdir,"output_{0}.root".format(datasetKey))
            if not os.path.isfile(input_root_file):
                print 'ERROR: File ' + input_root_file + ' not found'
                print 'Aborting'
                sys.exit(1)

            # open input ROOT file
            root_file = TFile(input_root_file)
            htemp = root_file.Get("TotalGenEvts")
            htemp.Print()
            nEventsAll = htemp.GetBinContent(1)
            scale = float(groupVal['xsec'][datasetIdx]*groupVal['lumi'])/nEventsAll if groupVal['xsec'][datasetIdx] > 0 else 1.
            print "{0:10} -- Events: {1:.3f} (all); xsec: {2:.3E}; scale: {3:.3E}".format(datasetKey, nEventsAll, dataset[datasetKey]['xsec'], scale)

            # get the number of histograms
            nHistos = root_file.GetListOfKeys().GetEntries()

            # loop over histograms in the input ROOT file
            for h in range(0, nHistos):
                histoName = root_file.GetListOfKeys()[h].GetName()
                htemp = root_file.Get(histoName)
                if htemp.InheritsFrom('TH1') or htemp.InheritsFrom('TH2'):
                    if histoName not in final_histos.keys():
                        final_histos[histoName] = copy.deepcopy(htemp)
                        final_histos[histoName].SetName(histoName + '_' + groupKey)
                        final_histos[histoName].Scale(scale)
                    else:
                        final_histos[histoName].Add(htemp, scale) 

        output_root_file.cd()
        histos = final_histos.keys()
        histos.sort()
        print "Writing histograms..."
        for histo in histos:
            final_histos[histo].Write()
        print 'Done'

    output_root_file.Close()
    pass

def drawAll(args):
    os.chdir(rootDir)
    gROOT.ProcessLine(".L DrawCommPlot4ttbar.C+")
    from ROOT import Draw, DrawTTbar
    os.chdir(CWD)
    pass

if __name__ == "__main__":
    # read options
    parser = argparse.ArgumentParser()
    parser.add_argument("-c", "--compile", dest="compile", default=False, action="store_true",
        help="Compile TTbarSelector and CommPlotProducer4ttbar")

    subparsers = parser.add_subparsers(help='Functions')

    subparserPU = subparsers.add_parser('pu')
    subparserPU.set_defaults(func=puEstimation)
    subparserPU.add_argument('json',
        help="LumiMask to be estimated.")

    subparserCreate = subparsers.add_parser('create')
    subparserCreate.set_defaults(func=createBatch)
    subparserCreate.add_argument('datacard',
        help="Datacard to be processed")
    subparserCreate.add_argument("-q", "--queue", dest="queue", default=defaultQueue,
        help="Queue name of LXBatch")
    subparserCreate.add_argument("-s", dest="submit", default=False, action="store_true",
        help="Submit the jobs")

    subparserRun = subparsers.add_parser('run')
    subparserRun.set_defaults(func=runJob)
    subparserRun.add_argument('cfg',
        help="Auto-generated datacard to be processed")
    subparserRun.add_argument("--pu", dest="puWgtUrl",
        default="${CMSSW_BASE}/src/RecoBTag/PerformanceMeasurements/test/ttbar/data/pileupWgts.root",
        help="Pathname to the output of runPUEstimation.py")

    subparserMerge = subparsers.add_parser('merge')
    subparserMerge.set_defaults(func=merge)
    subparserMerge.add_argument('cfg',
        help="Datacard of datasets to be merged")
    subparserMerge.add_argument("-w", "--workDir", dest="workDir",
        help="Main working directory",
        default=rootDir)
    subparserMerge.add_argument("-o", "--output", dest="outputName",
        help="Output filename",
        default='output_all')

    subparserDraw = subparsers.add_parser('draw')
    subparserDraw.set_defaults(func=drawAll)

    args = parser.parse_args()
    compileMacros(args)
    args.func(args)

    sys.exit()
