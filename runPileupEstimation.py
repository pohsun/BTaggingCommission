import optparse
import os,sys
import json
import commands
import ROOT
from SimGeneral.MixingModule.mix_2016_25ns_SpringMC_PUScenarioV1_PoissonOOTPU_cfi import *

"""
steer the script
"""
def main():

    #configuration
    # https://hypernews.cern.ch/HyperNews/CMS/get/luminosity/583/3.html

    usage = 'usage: %prog [options]'
    parser = optparse.OptionParser(usage)
    parser.add_option('-j', '--json',      dest='inJson'  ,      help='json file with processed runs',      default=None,    type='string')
    parser.add_option('-x', '--mbXsec',    dest='mbXsec'  ,      help='minimum bias cross section to use',  default=71300,   type=float)
    parser.add_option('-p', '--puJson',    dest='puJson'  ,      help='pileup json file',      
                      default='/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions16/13TeV/PileUp/pileup_latest.txt',    type='string')
    parser.add_option('-N', '--normtag',   dest='normtagJson',   help='normtag',      
                      default='/afs/cern.ch/user/l/lumipro/public/normtag_file/normtag_DATACERT.json',    type='string')
    parser.add_option('-o', '--output',    dest='outpath' ,      help='output path',                         default='./',    type='string')
    (opt, args) = parser.parse_args()
    
    #simulated pileup
    NPUBINS=len(mix.input.nbPileupEvents.probValue)
    simPuH=ROOT.TH1F('simPuH','',NPUBINS,float(0),float(NPUBINS))
    for xbin in xrange(0,NPUBINS):
        probVal=mix.input.nbPileupEvents.probValue[xbin]
        simPuH.SetBinContent(xbin,probVal)
    simPuH.Scale(1./simPuH.Integral())

    cmd='setenv PATH ${HOME}/.local/bin:/afs/cern.ch/cms/lumi/brilconda-1.0.3/bin:${PATH}'
    commands.getstatusoutput(cmd)
    #compute pileup in data assuming different xsec
    puDist=[]
    puWgts=[]
    MINBIASXSEC={'nom':opt.mbXsec,'up':opt.mbXsec*1.1,'down':opt.mbXsec*0.9}
    for scenario in MINBIASXSEC:
        print scenario, 'xsec=',MINBIASXSEC[scenario]
        #cmd='pileupCalc.py -i %s --inputLumiJSON %s --calcMode true --minBiasXsec %f --maxPileupBin %d --numPileupBins %s Pileup.root'%(opt.inJson,opt.puJson,MINBIASXSEC[scenario],NPUBINS,NPUBINS)
        cmd='brilcalc lumi -b "STABLE BEAMS" -i %s -o Pileup.txt --normtag %s  --byls --minBiasXsec %f'%(opt.inJson,opt.normtagJson,MINBIASXSEC[scenario])
        #cmd='brilcalc lumi -b "STABLE BEAMS" -i %s -o Pileup.txt --byls --minBiasXsec %f'%(opt.inJson,MINBIASXSEC[scenario])
        commands.getstatusoutput(cmd)
        print cmd

        dataPuH=ROOT.TH1F('dataPuH'+scenario,'',NPUBINS,float(0),float(NPUBINS))
        f = open('Pileup.txt', 'r')
        for line in f:
            if not line.startswith('#'):
                dataPuH.Fill(float(line.split(',')[7]));

        dataPuH.Scale(1./dataPuH.Integral())
        puDist.append( ROOT.TGraph(dataPuH) )
        puDist[-1].SetName('pu_'+scenario)

        dataPuH.Divide(simPuH)
        puWgts.append( ROOT.TGraph(dataPuH) )
        puWgts[-1].SetName('puwgts_'+scenario)
        commands.getstatusoutput('rm -f Pileup.txt')
    #save pileup weights to file
    #fOut=ROOT.TFile.Open('$CMSSW_BASE/src/RecoBTag/PerformanceMeasurements/test/ttbar/data/pileupWgts.root','RECREATE')
    fOut=ROOT.TFile.Open(opt.outpath+'/pileupWgts.root','RECREATE')
    for gr in puWgts: gr.Write()
    for gr in puDist: gr.Write()
    fOut.Close()

"""
for execution from another script
"""
if __name__ == "__main__":
    sys.exit(main())
