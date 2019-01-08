#!/usr/bin/env python

from __future__ import print_function

import sys, os, shutil, string, re, copy, imp
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
rootDir         = os.path.expandvars("${CMSSW_BASE}/src/RecoBTag/PerformanceMeasurements/test/BTagAnalyzerMacros")
scriptDir       = "/".join(os.path.realpath(__file__).split('/')[:-1])
eosCmd          = '/usr/bin/eos'
defaultQueue    = 'workday'
batchSize       = 50
CWD             = os.environ['PWD']

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
        shutil.copy(os.path.join(scriptDir,"CommPlotProducer4ttbar.h"), os.path.join(rootDir))
        shutil.copy(os.path.join(scriptDir,"CommPlotProducer4ttbar.C"), os.path.join(rootDir))
        gROOT.ProcessLine(".L CommPlotProducer4ttbar.C++g")
    os.chdir(CWD)
    pass

def puEstimation(args):
    isPUMixerUpdated = raw_input("Is Mixing module in runPileupEstimation.py updated? [y/n] ")
    if isPUMixerUpdated != 'y':
        print("Please update the MixingModule first. Abort.")
        return
    else:
        json = os.path.normpath(args.json)
        puJson = "/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions17/13TeV/PileUp/pileup_latest.txt"
        xsecs = [63000,65000,67000,69000,71000,73000,75000]

        os.chdir(rootDir+'/../ttbar')
        for xs in xsecs:
            cmd = "python runPileupEstimation.py --mbXsec={1} --json={2} --puJson={3}".format(rootDir,str(xs),json,puJson)
            call(cmd, shell=True)
            cmd = "mv {0} {1}".format("{0}/../ttbar/data/pileupWgts.root".format(rootDir), os.path.splitext(json)[0]+'_puWgtXsec'+str(xs)+'.root')
            call(cmd, shell=True)
    pass

def createBatch(args):
    defaultQueue    = "8nh"
    compileMacros(args)

    # Prepare submission script
    subTemplate = """
getenv      = True
output      = DATASET.$(ProcId).out
error       = DATASET.$(ProcId).err
log         = DATASET.log
+JobFlavour = {1}

executable  = {0}/runCode4ttbar.py
arguments   = run $(cfg) --pu $(puwgt)
transfer_input_files = {0}/runJob_cfi.py
initialdir  = WORKDIR

queue cfg,puwgt from JOBS.data
""".format(scriptDir, args.queue)

    # Load dataset, groupdata
    with open(os.path.join(CWD,args.datacard)) as handle:
        exec(handle)

    # create list of input files
    batchDir = os.path.join(rootDir, "batch")
    if not os.path.isdir(batchDir):
        os.makedirs(batchDir)
    for datasetName, data in dataset.iteritems():
        # if datasetName not in ['runE_v1','runF_v1']:
        #     continue
        print("Creating batch jobs for {0}.".format(datasetName))
        # Get full list of input files
        iFileList = []
        for iPath in data['ipath']:
            iFileList += fileLister(iPath)

        data['puWgtUrl'] = os.path.expandvars(data['puWgtUrl'])
        puWgtUrl = data['puWgtUrl'] if data['puWgtUrl'].startswith('/') else os.path.join(CWD,data['puWgtUrl'])

        # Split jobs
        jobsScriptName = os.path.join(batchDir,"jobs_{0}.data".format(datasetName))
        with open(jobsScriptName,'w+') as jobsScript:
            for iPart in range(0,(len(iFileList)-1)/batchSize+1):

                # Config for a single job
                oFileListName = "flist_{0}_part{1:02d}.py".format(datasetName,iPart);
                with open(os.path.join(batchDir,oFileListName),'w+') as oFileList:
                    oFileList.write("dataset = '{0}_part{1:02d}'\n".format(datasetName,iPart))
                    oFileList.write("isData  = {0}\n".format("True" if data['xsec'] == -1 else "False"))
                    oFileList.write("inputFiles = [\n")
                    for iFile in iFileList[iPart*batchSize:batchSize+iPart*batchSize]:
                        oFileList.write("    '"+iFile+"',\n")
                    oFileList.write("]\n")
                os.chmod(os.path.join(batchDir,oFileListName),0644)

                # Config for HTCondor
                jobsScript.write("{0}, {1}\n".format(
                    os.path.join(batchDir,oFileListName),
                    puWgtUrl))

        subScriptName = os.path.join(batchDir,"jobs_{0}.sub".format(datasetName))
        with open(subScriptName, 'w+') as subScript:
            sc = re.sub('WORKDIR'      , rootDir       , subTemplate)
            sc = re.sub('JOBS.data'    , jobsScriptName, sc)
            sc = re.sub('DATASET'      , datasetName   , sc)
            subScript.write(sc)

        # Submit jobs
        if args.submit and not os.path.exists(os.path.join(rootDir,"output_{0}_part{1:02d}.root".format(datasetName,iPart))):
            call("condor_submit {1}".format(args.queue, subScriptName),shell=True)
    pass

def runJob(args):
    # Handle configurations
    with open(os.path.join(CWD,args.cfg)) as handle:
        exec(handle)

    # Start processing
    os.chdir(rootDir)
    if args.compile or not os.path.exists("CommPlotProducer4ttbar_C.so"):
        compileMacros(args)
    else:
        gROOT.ProcessLine(".L CommPlotProducer4ttbar_C.so")
    from ROOT import CommPlotProducer4ttbar

    os.chdir(CWD)
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
    run = CommPlotProducer4ttbar(tree,url,"output_{0}.root".format(dataset))

    # Load trigger, cut, tagger WPs
    runJobConfig = imp.load_source("runJobConfig",os.path.join(scriptDir,"runJob_cfi.py")
    if len(inputFiles) > 0:
        f = TFile.Open(inputFiles[0])
        triggerpaths = f.Get("ttbarselectionproducer/triggerpaths")
        for iTrg in range(1, triggerpaths.GetNbinsX()+1):
            if triggerpaths.GetXaxis().GetBinLabel(iTrg) not in runJobConfig.skipTriggers: # Skip prescaled and non-emu trigger bits
                run.AddTrigChannel(triggerpaths.GetBinContent(iTrg))
            else:
                run.AddTrigChannel(0)
    for eraName, era in runJobConfig.runEras.iteritems():
        run.AddRunRange(era[0])
        run.AddRunRange(era[1])
    for wpName, wp in runJobConfig.tagWPs.iteritems():
        run.AddTagWP(wpName, wp['varName'], wp['wp'])
        run.RegBranch(wp['varName']) # maxJets=2000
    for cutName, cut, in runJobConfig.subCuts.iteritems():
        run.AddSubEventList(cutName, cut['cutStr'])
        for var in cut['cutVar']:
            run.RegBranch(var)

    # Loop over
    run.Loop(isData, inputWeight)
    pass

def hadd(iDir, oDir, datasetName):
    if not os.path.exists(oDir):
        os.mkdir(oDir)
    cmd = "hadd -f {0}/output_{2}.root {1}/output_{2}_part*.root".format(oDir, iDir, datasetName)
    print(cmd)
    call(cmd, shell=True)
    pass

def merge(args):
    # make sure all necessary input parameters are provided
    handle = open(args.cfg)
    exec handle in globals()
    handle.close()

    for datasetName in dataset.keys():
        if not os.path.exists("{0}/output_{1}.root".format(rootDir,datasetName)):
            hadd(rootDir,os.path.join(rootDir,"ttbar"),datasetName)

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

    # put all histograms together and insert event-independent weight
    for groupKey, groupVal in groupdata.iteritems():
        groupVal['xsec'] = [ (lambda x, idx: x if x > 0 else dataset[groupVal['dataset'][idx]]['xsec'])(x,idx) for idx, x in enumerate(groupVal['xsec']) ]
        print(groupKey, groupVal)

        final_histos = {}
        for datasetIdx, datasetKey in enumerate(groupVal['dataset']):
            input_root_file  = os.path.join(main_workdir,"output_{0}.root".format(datasetKey))
            if not os.path.isfile(input_root_file):
                print('ERROR: File ' + input_root_file + ' not found')
                print('Aborting')
                sys.exit(1)

            # open input ROOT file
            root_file = TFile(input_root_file)
            hTotalGenEvts = root_file.Get("totalGenEvts")
            nEventsAll = hTotalGenEvts.GetBinContent(1)
            lumiScale = float(groupVal['xsec'][datasetIdx]*groupVal['lumi'])/nEventsAll if groupVal['xsec'][datasetIdx] > 0 else 1.
            hPuWgtNorms = root_file.Get("puWgtNorms")
            puWgtNormScale={}
            for iBin in range(1, hPuWgtNorms.GetNbinsX()+1, 2):
                sf   = (hPuWgtNorms.GetBinContent(iBin)/hPuWgtNorms.GetBinContent(iBin+1)) if groupVal['xsec'][datasetIdx] > 0 else 1.
                name = "_{0}".format(hPuWgtNorms.GetXaxis().GetBinLabel(iBin).split("_")[-1])
                puWgtNormScale[name]=sf
            print("{0:10} -- Events: {1:.3f} (all); xsec: {2:.3E}; scale: {3:.3E}".format(datasetKey, nEventsAll, dataset[datasetKey]['xsec'], lumiScale*puWgtNormScale['_nominal']))

            # get the number of histograms
            nHistos = root_file.GetListOfKeys().GetEntries()

            # loop over histograms in the input ROOT file
            for h in range(0, nHistos):
                histoName = root_file.GetListOfKeys()[h].GetName()
                htemp = root_file.Get(histoName)
                if htemp.InheritsFrom('TH1') or htemp.InheritsFrom('TH2'):
                    scale = lumiScale
                    for k,v in puWgtNormScale.iteritems():
                        if k in htemp.GetName():
                            scale *= v
                    if histoName not in final_histos.keys():
                        final_histos[histoName] = copy.deepcopy(htemp)
                        final_histos[histoName].SetName(histoName + '_' + groupKey)
                        final_histos[histoName].Scale(scale)
                    else:
                        final_histos[histoName].Add(htemp, scale) 

        output_root_file.cd()
        histos = final_histos.keys()
        histos.sort()
        print("Writing histograms...")
        for histo in histos:
            final_histos[histo].Write()
        print('Done')

    output_root_file.Close()
    pass

def drawAll(args):
    drawer = imp.load_source("drawer", os.path.join(scriptDir, "drawAll.py"))
    os.chdir(rootDir)
    if not os.path.exists("ttbar/Commissioning_plots"):
        os.makedirs("ttbar/Commissioning_plots")

    for pName, cfg in drawer.cfi.plots.iteritems():
        drawer.draw(copy.deepcopy(cfg), isLog=False)
        drawer.draw(copy.deepcopy(cfg), isLog=True)

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
        help="JobFlavour of HTCondor")
    subparserCreate.add_argument("-s", dest="submit", default=False, action="store_true",
        help="Submit the jobs")

    subparserRun = subparsers.add_parser('run')
    subparserRun.set_defaults(func=runJob)
    subparserRun.add_argument('cfg',
        help="Auto-generated datacard to be processed")
    subparserRun.add_argument("--pu", dest="puWgtUrl",
        default="${CMSSW_BASE}/src/RecoBTag/PerformanceMeasurements/test/ttbar/data/pileupWgts.root",
        help="Pathname to the output of runPUEstimation.py")
    subparserRun.add_argument("-o", "--output", dest="outputName",
        help="Output filename",
        default='output.root')

    subparserMerge = subparsers.add_parser('merge')
    subparserMerge.set_defaults(func=merge)
    subparserMerge.add_argument('cfg',
        help="Datacard of datasets to be merged")
    subparserMerge.add_argument("-w", "--workDir", dest="workDir",
        help="Main working directory",
        default=os.path.join(rootDir,"ttbar"))
    subparserMerge.add_argument("-o", "--output", dest="outputName",
        help="Output filename",
        default='output_all')

    subparserDraw = subparsers.add_parser('draw')
    subparserDraw.set_defaults(func=drawAll)

    args = parser.parse_args()
    args.func(args)

    sys.exit()
