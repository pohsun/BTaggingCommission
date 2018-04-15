#!/usr/bin/env python
# vim:set sts=4 sw=4 fdm=syntax fdl=1 et:

import os,imp
from ROOT import TFile
from ROOT import TCanvas, TLegend, TLatex
from ROOT import TStyle, kWhite
from ROOT import TAxis
from ROOT import TH1D, THStack, TRatioPlot

cfi = imp.load_source("cfi","drawAll_cfi.py")
TH1D.SetDefaultSumw2(True)
canvas  = TCanvas("c1", "")
latex   = TLatex()

def SetTdrStyle():
    tdrStyle = TStyle("tdrStyle","Style for P-TDR")

    # For the canvas:
    tdrStyle.SetCanvasBorderMode(0);
    tdrStyle.SetCanvasColor(kWhite);
    tdrStyle.SetCanvasDefH(600); #Height of canvas
    tdrStyle.SetCanvasDefW(600); #Width of canvas
    tdrStyle.SetCanvasDefX(0);   #POsition on screen
    tdrStyle.SetCanvasDefY(0);

    # For the Pad:
    tdrStyle.SetPadBorderMode(0);
    tdrStyle.SetPadColor(kWhite);
    tdrStyle.SetPadGridX(False);
    tdrStyle.SetPadGridY(False);
    tdrStyle.SetGridColor(0);
    tdrStyle.SetGridStyle(3);
    tdrStyle.SetGridWidth(1);

    # For the frame:
    tdrStyle.SetFrameBorderMode(0);
    tdrStyle.SetFrameBorderSize(1);
    tdrStyle.SetFrameFillColor(0);
    tdrStyle.SetFrameFillStyle(0);
    tdrStyle.SetFrameLineColor(1);
    tdrStyle.SetFrameLineStyle(1);
    tdrStyle.SetFrameLineWidth(1);

    # For the histo:
    # tdrStyle.SetHistFillColor(1);
    # tdrStyle.SetHistFillStyle(0);
    tdrStyle.SetHistLineColor(1);
    tdrStyle.SetHistLineStyle(0);
    tdrStyle.SetHistLineWidth(1);
    # tdrStyle.SetLegoInnerR(Float_t rad = 0.5);
    # tdrStyle.SetNumberContours(Int_t number = 20);

    tdrStyle.SetEndErrorSize(2);
    # tdrStyle.SetErrorMarker(20);
    tdrStyle.SetErrorX(0.);

    tdrStyle.SetMarkerStyle(20);

    #For the fit/function:
    tdrStyle.SetOptFit(1);
    tdrStyle.SetFitFormat("5.4g");
    tdrStyle.SetFuncColor(2);
    tdrStyle.SetFuncStyle(1);
    tdrStyle.SetFuncWidth(1);

    #For the date:
    tdrStyle.SetOptDate(0);
    # tdrStyle.SetDateX(Float_t x = 0.01);
    # tdrStyle.SetDateY(Float_t y = 0.01);

    # For the statistics box:
    tdrStyle.SetOptFile(0);
    tdrStyle.SetOptStat(0); # To display the mean and RMS:   SetOptStat("mr");
    tdrStyle.SetStatColor(kWhite);
    tdrStyle.SetStatFont(42);
    tdrStyle.SetStatFontSize(0.025);
    tdrStyle.SetStatTextColor(1);
    tdrStyle.SetStatFormat("6.4g");
    tdrStyle.SetStatBorderSize(1);
    tdrStyle.SetStatH(0.1);
    tdrStyle.SetStatW(0.15);
    # tdrStyle.SetStatStyle(Style_t style = 1001);
    # tdrStyle.SetStatX(Float_t x = 0);
    # tdrStyle.SetStatY(Float_t y = 0);

    # Margins:
    tdrStyle.SetPadTopMargin(0.05);
    tdrStyle.SetPadBottomMargin(0.13);
    tdrStyle.SetPadLeftMargin(0.16);
    tdrStyle.SetPadRightMargin(0.02);

    # For the Global title:
    tdrStyle.SetOptTitle(0);
    tdrStyle.SetTitleFont(42);
    tdrStyle.SetTitleColor(1);
    tdrStyle.SetTitleTextColor(1);
    tdrStyle.SetTitleFillColor(10);
    tdrStyle.SetTitleFontSize(0.05);
    # tdrStyle.SetTitleH(0); # Set the height of the title box
    # tdrStyle.SetTitleW(0); # Set the width of the title box
    # tdrStyle.SetTitleX(0); # Set the position of the title box
    # tdrStyle.SetTitleY(0.985); # Set the position of the title box
    # tdrStyle.SetTitleStyle(Style_t style = 1001);
    # tdrStyle.SetTitleBorderSize(2);

    # For the axis titles:
    tdrStyle.SetTitleColor(1, "XYZ");
    tdrStyle.SetTitleFont(42, "XYZ");
    tdrStyle.SetTitleSize(0.05, "XYZ");
    # tdrStyle.SetTitleXSize(0.05); # Another way to set the size?
    # tdrStyle.SetTitleYSize(0.05);
    tdrStyle.SetTitleXOffset(0.9);
    tdrStyle.SetTitleYOffset(1.25);
    # tdrStyle.SetTitleOffset(1.1, "Y"); # Another way to set the Offset

    # For the axis labels:
    tdrStyle.SetLabelColor(1, "XYZ");
    tdrStyle.SetLabelFont(42, "XYZ");
    tdrStyle.SetLabelOffset(0.007, "XYZ");
    tdrStyle.SetLabelSize(0.04, "XYZ");

    # For the axis:
    tdrStyle.SetAxisColor(1, "XYZ");
    tdrStyle.SetStripDecimals(True);
    tdrStyle.SetTickLength(0.03, "XYZ");
    tdrStyle.SetNdivisions(510, "XYZ");
    tdrStyle.SetPadTickX(1);  # To get tick marks on the opposite side of the frame
    tdrStyle.SetPadTickY(1);

    # Change for log plots:
    tdrStyle.SetOptLogx(0);
    tdrStyle.SetOptLogy(0);
    tdrStyle.SetOptLogz(0);

    # Postscript options:
    tdrStyle.SetPaperSize(20.,20.);
    # tdrStyle.SetLineScalePS(Float_t scale = 3);
    # tdrStyle.SetLineStyleString(Int_t i, const char* text);
    # tdrStyle.SetHeaderPS(const char* header);
    # tdrStyle.SetTitlePS(const char* pstitle);

    # tdrStyle.SetBarOffset(Float_t baroff = 0.5);
    # tdrStyle.SetBarWidth(Float_t barwidth = 0.5);
    # tdrStyle.SetPaintTextFormat(const char* format = "g");
    # tdrStyle.SetPalette(Int_t ncolors = 0, Int_t* colors = 0);
    # tdrStyle.SetTimeOffset(Double_t toffset);
    # tdrStyle.SetHistMinimumZero(kTRUE);

    tdrStyle.SetHatchesLineWidth(5);
    tdrStyle.SetHatchesSpacing(0.05);

    tdrStyle.cd();
    pass

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

def drawLatex(x, y, word):
    latex.SetTextSize(0.04);
    latex.SetTextFont (42);
    latex.SetTextAlign(11);#LB
    latex.SetLineWidth(2);
    latex.DrawLatexNDC(x, y, word);

def draw(cfg, isLog=False):
    SetTdrStyle()
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
