#!/usr/bin/env python
# vim:set sts=4 sw=4 fdm=syntax et:

import os,imp
from ROOT import TFile
from ROOT import TCanvas, TLegend, TLatex
from ROOT import TAxis
from ROOT import TH1D, THStack, TRatioPlot

cfi = imp.load_source("cfi","drawAll_cfi.py")
TH1D.SetDefaultSumw2(True)
canvas  = TCanvas("c1", "", 800, 600)
latex   = TLatex()

def setDefaultLegend(leg):
    leg.SetBorderSize(1);
    leg.SetTextFont(62);
    leg.SetLineColor(1);
    leg.SetLineStyle(1);
    leg.SetLineWidth(1);
    # leg.SetFillStyle(1001);
    leg.SetFillStyle(0);# Transparent
    leg.SetBorderSize(0);
    leg.SetFillColor(0);
    return leg

def SetDefaultHist(hist):
    return hist

def drawLatexCMS(x=0.10, y=0.94):
    latex.SetTextSize(0.065);
    latex.SetTextFont(62);
    latex.SetTextAlign(11);#RB
    latex.SetLineWidth(2);
    latex.DrawLatexNDC(x, y, "CMS")

    if (cfi.isPrelim):
        latex.SetTextSize(0.055);
        latex.SetTextFont (52);
        latex.SetTextAlign(11);#LB
        latex.SetLineWidth(2);
        latex.DrawLatexNDC(x+0.1, y, "Preliminary");
    pass

def drawLatexLumi(x=0.89,y=0.89):
    latex.SetTextSize(0.05);
    latex.SetTextFont (42);
    latex.SetTextAlign(33);#RU
    latex.SetLineWidth(2);
    latex.DrawLatexNDC(x, y, "{0} fb^{{-1}}".format(cfi.lumi));
    pass

def drawLatexSel(x=0.50, y=0.89, sel="e#mu channel #geq 2 jets"):
    latex.SetTextSize(0.04);
    latex.SetTextFont (42);
    latex.SetTextAlign(21);#CB
    latex.SetLineWidth(2);
    latex.DrawLatexNDC(x, y, sel);
    pass

def drawLatexJetType(x=0.50, y=0.85):
    latex.SetTextSize(0.04);
    latex.SetTextFont (42);
    latex.SetTextAlign(21);#CB
    latex.SetLineWidth(2);
    latex.DrawLatexNDC(x, y, "p_{T} #geq 20 GeV");
    pass

def draw(cfg, isLog=False):
    canvas.SetLogy(isLog)

    fIn = TFile.Open(cfi.mergedInputFile)
    hists = [ fIn.Get(cfg['name']+postfix) for postfix in cfg['postfix'] ]
    histTotal = [0,0]

    # Stack and Total
    hStack = THStack("hstack","")
    for hIdx, hist in enumerate(hists):
        isData = cfg['isData'][hIdx]

        hist.GetXaxis().SetTitle(cfg['xTitle'])
        hist.GetYaxis().SetTitle(cfg['yTitle'])
        if isData:
            hist.SetMarkerStyle(cfg['fillColor'][hIdx])
            hist.SetMarkerSize(0.75)
        else:
            hist.SetFillColor  (cfg['fillColor'][hIdx])
            hStack.Add(hist)

        # Fill to total
        if histTotal[isData] == 0:
            histTotal[isData] = hist.Clone("{0}_total_{1}".format(cfg['name'], "data" if isData else "mc"));
        else:
            histTotal[isData].Add(hist);

    # Legend
    stLeg  = TLegend(*cfg['stLegPos'])
    setDefaultLegend(stLeg)
    stLeg.AddEntry(histTotal[1], cfg['legend'][0], "ep")
    for hIdx, hist in enumerate(hists):
        if not cfg['isData'][hIdx]:
            stLeg.AddEntry(hist,cfg['legend'][hIdx],"f")

    # Latex labels
    hStack.Draw(cfg['stackOpt'])
    hStack.GetXaxis().SetTitle(cfg['xTitle'])
    hStack.GetYaxis().SetTitle(cfg['yTitle'])
    histTotal[1].Draw("SAME "+cfg['drawOpt'][0])
    stLeg.Draw()
    drawLatexCMS    ()
    drawLatexLumi   (x=0.89,y=0.89)
    drawLatexJetType(x=0.50,y=0.86)
    drawLatexSel    (x=0.50,y=0.81)

    # Print files
    for oFormat in cfi.outputFormats:
        canvas.Update()
        canvas.Print(os.path.join(cfi.outputDir,"hstack_{0}{1}.{2}".format(cfg['name'],"_log" if isLog else "",oFormat)))

    # Ratio
    hRatio = [ TRatioPlot(hStack, histTotal[1],opt) for opt in cfg['ratioOpts'] ]
    rpLeg  = TLegend(stLeg)
    rpLeg.SetX1NDC(cfg['rpLegPos'][0]);
    rpLeg.SetY1NDC(cfg['rpLegPos'][1]);
    rpLeg.SetX2NDC(cfg['rpLegPos'][2]);
    rpLeg.SetY2NDC(cfg['rpLegPos'][3]);

    for rpIdx, rp in enumerate(hRatio):
        rp.Draw("")
        rp.GetXaxis().SetTitle(cfg['xTitle'])
        rp.GetUpperRefYaxis().SetTitle(cfg['yTitle'])
        rp.GetLowerRefYaxis().SetTitle("Ratio")
        histTotal[1].Draw("SAME "+cfg['drawOpt'][0])
        rpLeg.Draw()
        drawLatexCMS    ()
        drawLatexLumi   (x=0.88,y=0.92)
        drawLatexJetType(x=0.50,y=0.89)
        drawLatexSel    (x=0.50,y=0.84)
        for oFormat in cfi.outputFormats:
            canvas.Update()
            canvas.Print(os.path.join(cfi.outputDir,"hratio_{0}_{1}{2}.{3}".format(cfg['ratioOpts'][rpIdx],cfg['name'],"_log" if isLog else "",oFormat)))
    pass
