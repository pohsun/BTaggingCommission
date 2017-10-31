// vim: set sw=4 sts=4 fdm=marker et:

//Description     : Get the rough b-tagging efficiency.
//Author          : Po-Hsun Chen (pohsun.chen.hep@gmail.com)
//Last Modified   : 31 Oct 2017 10:40 

#include <iostream>

#include "TROOT.h"
#include "TStyle.h"

#include "TFile.h"
#include "TH1D.h"
#include "TAxis.h"
#include "TGraphAsymmErrors.h"
#include "TEfficiency.h"

#include "TCanvas.h"
#include "TLegend.h"

void checkRoughBTagEff(){
    gStyle->SetOptStat(0);
    gStyle->SetPadLeftMargin(0.05);
    gStyle->SetPadRightMargin(0.02);
    TH1D::SetDefaultSumw2(true);

    TFile *fin = new TFile("./output_all.root");
    TH1D *nEvt_run_data = (TH1D*)fin->Get("nEvt_run_data");
    TH1D *nEvt_run_CSVv2L_data = (TH1D*)fin->Get("nEvt_run_CSVv2L_data");
    TH1D *nEvt_run_CSVv2M_data = (TH1D*)fin->Get("nEvt_run_CSVv2M_data");
    TH1D *nEvt_run_CSVv2T_data = (TH1D*)fin->Get("nEvt_run_CSVv2T_data");
    TAxis *axis_data = nEvt_run_data->GetXaxis();
    TAxis *axis_CSVv2L_data = nEvt_run_CSVv2L_data->GetXaxis();
    TAxis *axis_CSVv2M_data = nEvt_run_CSVv2M_data->GetXaxis();
    TAxis *axis_CSVv2T_data = nEvt_run_CSVv2T_data->GetXaxis();
    nEvt_run_data->LabelsDeflate("X");
    nEvt_run_CSVv2L_data->LabelsDeflate("X");
    nEvt_run_CSVv2M_data->LabelsDeflate("X");
    nEvt_run_CSVv2T_data->LabelsDeflate("X");

    TH1D *BTagEff_run_NoCut  = new TH1D("BTagEff_run_NoCut" ,"",1,0,1);
    TH1D *BTagEff_run_CSVv2L = new TH1D("BTagEff_run_CSVv2L","",1,0,1);
    TH1D *BTagEff_run_CSVv2M = new TH1D("BTagEff_run_CSVv2M","",1,0,1);
    TH1D *BTagEff_run_CSVv2T = new TH1D("BTagEff_run_CSVv2T","",1,0,1);

    //nEvt_run_CSVv2L_data->Divide(nEvt_run_data);// Not checking bin label
    for( int iBin=1; iBin<=nEvt_run_data->GetNbinsX(); iBin++){
        const char *binLabel = axis_data->GetBinLabel(iBin);
        double nEvt = nEvt_run_data->GetBinContent(iBin);
        double nEvt_CSVv2L(0), nEvt_CSVv2M(0), nEvt_CSVv2T(0);
        for( int jBin=1; jBin<=nEvt_run_CSVv2L_data->GetNbinsX(); jBin++){
            if (!strcmp(binLabel,axis_CSVv2L_data->GetBinLabel(jBin))){
                nEvt_CSVv2L = nEvt_run_CSVv2L_data->GetBinContent(jBin);
                BTagEff_run_CSVv2L->Fill(binLabel, nEvt_CSVv2L/nEvt);
                BTagEff_run_NoCut ->Fill(binLabel, nEvt);
                BTagEff_run_CSVv2M->Fill(binLabel, 0.);
                BTagEff_run_CSVv2T->Fill(binLabel, 0.);
            }
        }
        for( int jBin=1; jBin<=nEvt_run_CSVv2M_data->GetNbinsX(); jBin++){
            if (!strcmp(binLabel,axis_CSVv2M_data->GetBinLabel(jBin))){
                nEvt_CSVv2M = nEvt_run_CSVv2M_data->GetBinContent(jBin);
                BTagEff_run_CSVv2M->Fill(binLabel, nEvt_CSVv2M/nEvt);
            }
        }
        for( int jBin=1; jBin<=nEvt_run_CSVv2T_data->GetNbinsX(); jBin++){
            if (!strcmp(binLabel,axis_CSVv2T_data->GetBinLabel(jBin))){
                nEvt_CSVv2T = nEvt_run_CSVv2T_data->GetBinContent(jBin);
                BTagEff_run_CSVv2T->Fill(binLabel, nEvt_CSVv2T/nEvt );
            }
        }
        printf("RunNo(%s): %f(L), %f(M), %f(T) / %f(nEvts)\n",binLabel, nEvt_CSVv2L, nEvt_CSVv2M, nEvt_CSVv2T, nEvt);
    }
    BTagEff_run_NoCut ->LabelsDeflate("X");
    BTagEff_run_CSVv2L->LabelsDeflate("X");
    BTagEff_run_CSVv2M->LabelsDeflate("X");
    BTagEff_run_CSVv2T->LabelsDeflate("X");
    for( int iBin=1; iBin<=BTagEff_run_NoCut->GetNbinsX(); iBin++){
        double nEvt = BTagEff_run_NoCut->GetBinContent(iBin);
        BTagEff_run_CSVv2L->SetBinError(iBin, sqrt(BTagEff_run_CSVv2L->GetBinContent(iBin)*(1.-(BTagEff_run_CSVv2L->GetBinContent(iBin))) / nEvt));
        BTagEff_run_CSVv2M->SetBinError(iBin, sqrt(BTagEff_run_CSVv2M->GetBinContent(iBin)*(1.-(BTagEff_run_CSVv2M->GetBinContent(iBin))) / nEvt));
        BTagEff_run_CSVv2T->SetBinError(iBin, sqrt(BTagEff_run_CSVv2T->GetBinContent(iBin)*(1.-(BTagEff_run_CSVv2T->GetBinContent(iBin))) / nEvt));
    }

    TCanvas *canvas = new TCanvas("canvas","",1200,600);
    BTagEff_run_CSVv2L->SetYTitle("Tagging Efficiency");
    BTagEff_run_CSVv2L->SetLineColor(kGreen);
    BTagEff_run_CSVv2M->SetLineColor(kBlue);
    BTagEff_run_CSVv2T->SetLineColor(kRed);
    BTagEff_run_CSVv2L->Draw();
    BTagEff_run_CSVv2M->Draw("SAME");
    BTagEff_run_CSVv2T->Draw("SAME");
    TLegend *leg = new TLegend(0.05,0.7,0.3,0.9);
    leg->SetHeader("= 2 jets, e#mu","C"); // option "C" allows to center the header
    leg->AddEntry(BTagEff_run_CSVv2L,"#geq 2 CSVv2L","le");
    leg->AddEntry(BTagEff_run_CSVv2M,"#geq 2 CSVv2M","le");
    leg->AddEntry(BTagEff_run_CSVv2T,"#geq 2 CSVv2T","le");
    leg->Draw();
    canvas->Update();
    canvas->Print("BTagEff_run_CSVv2_data.pdf");

}
