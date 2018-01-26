#define CommPlotProducer4ttbar_cxx
#include "CommPlotProducer4ttbar.h"

#include <TH2.h>
#include <TStyle.h>
#include "TH1D.h"
#include "TH2D.h"
#include "TGraph.h"
#include "TCanvas.h"
#include <iostream>
#include <fstream>
#include "TMath.h"
#include <iomanip>
#include "TLorentzVector.h"
#include <string.h>
#include "THStack.h"
#include "TRandom.h"
#include "TRandom3.h"
#include <TLegend.h>
#include <TProfile.h>
#include <TLatex.h>
#include <TAxis.h>

#include "../common.h"

using namespace std;


//------------------------------------------------------------------------------//
//--------------------   Set cross sections  ---------------------------------//
//--------------------------------------------------------------------------//
void CommPlotProducer4ttbar::Loop(bool isdata, TString output_name, TH1F* wgtcounter, TString syst, bool debug)
{

    if (syst != "") cout << "Running over " << syst << "..." << endl;
    else            cout << "Running over nominal sample" << endl;

    isData = isdata;

    //---------------Configuration-----------------------------------------//
    float EtaCut = 2.4;
    double pi=acos(-1);

    // Cross check variables--------------------------------------------------//
    int   Nevent = 0;
    float N_event_mc_before_sel   =0;
    float N_event_data_before_sel =0;
    float N_event_mc_after_sel    =0;
    float N_event_data_after_sel  =0;


    int njet_c     =0;
    int njet_b     =0;
    int njet_bfromg=0;
    int njet_l     =0;
    int njet_pu    =0;
    int njet_mc    =0;
    int njet_data  =0;
    int nSVbins    = 50;
    int nTrackbins = 100;


    bool passNhit;
    bool passPix    ;
    bool passIPz    ;
    bool passPt     ;
    bool passnormchi2;
    bool passtrkdist ;
    bool passtrklen  ;
    bool passTrackIP2D;

    TString tmpsyst = "";
    if(syst != "") tmpsyst = "_"+syst;

    //---------------------------------------------------------------------//
    TFile *myfile=new TFile(output_name+tmpsyst+".root",      "recreate");

    // --------------------------------------Histograms declaration------------------------------------------------//
    TH1D* nPU_mc                  = new TH1D("nPU_mc",                "nPU_mc",                50,-0.5,49.5 );
    TH1D* nPU_data                = new TH1D("nPU_data",              "nPU_data",              50,-0.5,49.5 );
    TH1D* nPV_mc                  = new TH1D("nPV_mc",                "nPV_mc",                50,-0.5,49.5 );
    TH1D* pt_hat                  = new TH1D("pt_hat",                "pt_hat",                80,   0,800  );
    TH1D* jet_pt_mc               = new TH1D("jet_pt_mc",         "jet_pt_mc",         80,   0,500  );
    TH1D* totalGenEvts            = new TH1D("TotalGenEvts",          "",                      1,    0, 1);  // For normaliztion to lumi

    // --------------------------------------Histograms declaration -----------------------------------------//
    if(!produceCTagTree){//{{{
        AddHistoTTbar("nPV",            "number of PV",               50,-0.5,49.5);
        AddHistoTTbar("nPV_unweighted", "unweighted number of PV",    50,-0.5,49.5);
        AddHistoTTbar("met",            "MET",                        30,  0.,300.);
        AddHistoTTbar("mll",            "M_{ll}",                     60,  0.,300.);
        AddHistoTTbar("njet",           "number of jets",             10,-0.5, 9.5);
        AddHistoTTbar("njet_pt30",      "number of jets pt30",        10,-0.5, 9.5);
        AddHistoTTbar("pt_e",           "P_{T}^{e}",                  50,  0.,200.);
        AddHistoTTbar("pt_mu",          "P_{T}^{#mu}",                50,  0.,200.);
        AddHistoTTbar("pt_jet",         "P_{T}^{leading jet}",        50,  0.,400.);

        // HIP check (as function of run range for Run2016B)
        AddHistoTTbar("nEvt_run",          "number of evt VS run",                  20,  0,  20);
        AddHistoTTbar("nEvt_run_CSVv2L",   "number of evt VS run(b-jet csvl)",      20,  0,  20);
        AddHistoTTbar("nEvt_run_CSVv2M",   "number of evt VS run(b-jet csvm)",      20,  0,  20);
        AddHistoTTbar("nEvt_run_CSVv2T",   "number of evt VS run(b-jet csvt)",      20,  0,  20);
        AddHistoTTbar("nEvt_run_2Jet",     "number of evt VS run(2 Jets)",          20,  0,  20);
        AddHistoTTbar("nEvt_run_3Jet",     "number of evt VS run(3 Jets)",          20,  0,  20);
        AddHistoTTbar("nEvt_run_4Jet",     "number of evt VS run(4 Jets)",          20,  0,  20);
        AddHistoTTbar("nEvt_run_5Jet",     "number of evt VS run(5 Jets)",          20,  0,  20);
        AddHistoTTbar("nEvt_run_6Jet",     "number of evt VS run(6 Jets)",          20,  0,  20);
        AddHistoTTbar("nEvt_run_7Jet",     "number of evt VS run(7 Jets)",          20,  0,  20);
        AddHistoTTbar("nEvt_run_8Jet",     "number of evt VS run(8 Jets)",          20,  0,  20);
        AddHistoTTbar("nEvt_run_9Jet",     "number of evt VS run(9 Jets)",          20,  0,  20);
        AddHistoTTbar("nEvt_run_MoreJet",  "number of evt VS run(>9 Jets)",         20,  0,  20);

        AddHistoTTbar("nbtag_CSVv2T","number of btag jets (tight WP)",             6,-0.5,5.5);
        AddHistoTTbar("nbtag_CSVv2M","number of btag jets (medium WP)",            6,-0.5,5.5);
        AddHistoTTbar("nbtag_CSVv2L","number of btag jets (loose WP)",             6,-0.5,5.5);
        AddHistoTTbar("nbtag_cMVAv2T","number of btag jets (tight WP)",            6,-0.5,5.5);
        AddHistoTTbar("nbtag_cMVAv2M","number of btag jets (medium WP)",           6,-0.5,5.5);
        AddHistoTTbar("nbtag_cMVAv2L","number of btag jets (loose WP)",            6,-0.5,5.5);

        AddHistoTTbar("nbtag_all_afterJetSel_cMVAv2T","number of btag jets (tight WP)",            6,-0.5,5.5);
        AddHistoTTbar("nbtag_all_afterJetSel_cMVAv2M","number of btag jets (medium WP)",           6,-0.5,5.5);
        AddHistoTTbar("nbtag_all_afterJetSel_cMVAv2L","number of btag jets (loose WP)",            6,-0.5,5.5);

        AddHistoTTbar("nbtag_all_Inf60_afterJetSel_cMVAv2T","number of btag jets (tight WP)",          6,-0.5,5.5);
        AddHistoTTbar("nbtag_all_Inf60_afterJetSel_cMVAv2M","number of btag jets (medium WP)",         6,-0.5,5.5);
        AddHistoTTbar("nbtag_all_Inf60_afterJetSel_cMVAv2L","number of btag jets (loose WP)",          6,-0.5,5.5);

        AddHistoTTbar("nbtag_all_60-120_afterJetSel_cMVAv2T","number of btag jets (tight WP)",         6,-0.5,5.5);
        AddHistoTTbar("nbtag_all_60-120_afterJetSel_cMVAv2M","number of btag jets (medium WP)",        6,-0.5,5.5);
        AddHistoTTbar("nbtag_all_60-120_afterJetSel_cMVAv2L","number of btag jets (loose WP)",         6,-0.5,5.5);

        AddHistoTTbar("nbtag_all_120-320_afterJetSel_cMVAv2T","number of btag jets (tight WP)",        6,-0.5,5.5);
        AddHistoTTbar("nbtag_all_120-320_afterJetSel_cMVAv2M","number of btag jets (medium WP)",       6,-0.5,5.5);
        AddHistoTTbar("nbtag_all_120-320_afterJetSel_cMVAv2L","number of btag jets (loose WP)",        6,-0.5,5.5);


        AddHistoTTbar("nbtag_2b_afterJetSel_cMVAv2T","number of btag jets (tight WP)",             6,-0.5,5.5);
        AddHistoTTbar("nbtag_2b_afterJetSel_cMVAv2M","number of btag jets (medium WP)",            6,-0.5,5.5);
        AddHistoTTbar("nbtag_2b_afterJetSel_cMVAv2L","number of btag jets (loose WP)",             6,-0.5,5.5);

        AddHistoTTbar("nbtag_2b_Inf60_afterJetSel_cMVAv2T","number of btag jets (tight WP)",           6,-0.5,5.5);
        AddHistoTTbar("nbtag_2b_Inf60_afterJetSel_cMVAv2M","number of btag jets (medium WP)",          6,-0.5,5.5);
        AddHistoTTbar("nbtag_2b_Inf60_afterJetSel_cMVAv2L","number of btag jets (loose WP)",           6,-0.5,5.5);

        AddHistoTTbar("nbtag_2b_60-120_afterJetSel_cMVAv2T","number of btag jets (tight WP)",          6,-0.5,5.5);
        AddHistoTTbar("nbtag_2b_60-120_afterJetSel_cMVAv2M","number of btag jets (medium WP)",         6,-0.5,5.5);
        AddHistoTTbar("nbtag_2b_60-120_afterJetSel_cMVAv2L","number of btag jets (loose WP)",          6,-0.5,5.5);

        AddHistoTTbar("nbtag_2b_120-320_afterJetSel_cMVAv2T","number of btag jets (tight WP)",         6,-0.5,5.5);
        AddHistoTTbar("nbtag_2b_120-320_afterJetSel_cMVAv2M","number of btag jets (medium WP)",        6,-0.5,5.5);
        AddHistoTTbar("nbtag_2b_120-320_afterJetSel_cMVAv2L","number of btag jets (loose WP)",         6,-0.5,5.5);


        AddHistoTTbar("nbtag_1b1c_afterJetSel_cMVAv2T","number of btag jets (tight WP)",           6,-0.5,5.5);
        AddHistoTTbar("nbtag_1b1c_afterJetSel_cMVAv2M","number of btag jets (medium WP)",          6,-0.5,5.5);
        AddHistoTTbar("nbtag_1b1c_afterJetSel_cMVAv2L","number of btag jets (loose WP)",           6,-0.5,5.5);

        AddHistoTTbar("nbtag_1b1c_Inf60_afterJetSel_cMVAv2T","number of btag jets (tight WP)",         6,-0.5,5.5);
        AddHistoTTbar("nbtag_1b1c_Inf60_afterJetSel_cMVAv2M","number of btag jets (medium WP)",        6,-0.5,5.5);
        AddHistoTTbar("nbtag_1b1c_Inf60_afterJetSel_cMVAv2L","number of btag jets (loose WP)",         6,-0.5,5.5);

        AddHistoTTbar("nbtag_1b1c_60-120_afterJetSel_cMVAv2T","number of btag jets (tight WP)",        6,-0.5,5.5);
        AddHistoTTbar("nbtag_1b1c_60-120_afterJetSel_cMVAv2M","number of btag jets (medium WP)",       6,-0.5,5.5);
        AddHistoTTbar("nbtag_1b1c_60-120_afterJetSel_cMVAv2L","number of btag jets (loose WP)",        6,-0.5,5.5);

        AddHistoTTbar("nbtag_1b1c_120-320_afterJetSel_cMVAv2T","number of btag jets (tight WP)",       6,-0.5,5.5);
        AddHistoTTbar("nbtag_1b1c_120-320_afterJetSel_cMVAv2M","number of btag jets (medium WP)",      6,-0.5,5.5);
        AddHistoTTbar("nbtag_1b1c_120-320_afterJetSel_cMVAv2L","number of btag jets (loose WP)",       6,-0.5,5.5);


        AddHistoTTbar("nbtag_1b1l_afterJetSel_cMVAv2T","number of btag jets (tight WP)",           6,-0.5,5.5);
        AddHistoTTbar("nbtag_1b1l_afterJetSel_cMVAv2M","number of btag jets (medium WP)",          6,-0.5,5.5);
        AddHistoTTbar("nbtag_1b1l_afterJetSel_cMVAv2L","number of btag jets (loose WP)",           6,-0.5,5.5);

        AddHistoTTbar("nbtag_1b1l_Inf60_afterJetSel_cMVAv2T","number of btag jets (tight WP)",         6,-0.5,5.5);
        AddHistoTTbar("nbtag_1b1l_Inf60_afterJetSel_cMVAv2M","number of btag jets (medium WP)",        6,-0.5,5.5);
        AddHistoTTbar("nbtag_1b1l_Inf60_afterJetSel_cMVAv2L","number of btag jets (loose WP)",         6,-0.5,5.5);

        AddHistoTTbar("nbtag_1b1l_60-120_afterJetSel_cMVAv2T","number of btag jets (tight WP)",        6,-0.5,5.5);
        AddHistoTTbar("nbtag_1b1l_60-120_afterJetSel_cMVAv2M","number of btag jets (medium WP)",       6,-0.5,5.5);
        AddHistoTTbar("nbtag_1b1l_60-120_afterJetSel_cMVAv2L","number of btag jets (loose WP)",        6,-0.5,5.5);

        AddHistoTTbar("nbtag_1b1l_120-320_afterJetSel_cMVAv2T","number of btag jets (tight WP)",       6,-0.5,5.5);
        AddHistoTTbar("nbtag_1b1l_120-320_afterJetSel_cMVAv2M","number of btag jets (medium WP)",      6,-0.5,5.5);
        AddHistoTTbar("nbtag_1b1l_120-320_afterJetSel_cMVAv2L","number of btag jets (loose WP)",       6,-0.5,5.5);


        AddHistoTTbar("nbtag_2c_afterJetSel_cMVAv2T","number of btag jets (tight WP)",             6,-0.5,5.5);
        AddHistoTTbar("nbtag_2c_afterJetSel_cMVAv2M","number of btag jets (medium WP)",            6,-0.5,5.5);
        AddHistoTTbar("nbtag_2c_afterJetSel_cMVAv2L","number of btag jets (loose WP)",             6,-0.5,5.5);

        AddHistoTTbar("nbtag_2c_Inf60_afterJetSel_cMVAv2T","number of btag jets (tight WP)",           6,-0.5,5.5);
        AddHistoTTbar("nbtag_2c_Inf60_afterJetSel_cMVAv2M","number of btag jets (medium WP)",          6,-0.5,5.5);
        AddHistoTTbar("nbtag_2c_Inf60_afterJetSel_cMVAv2L","number of btag jets (loose WP)",           6,-0.5,5.5);

        AddHistoTTbar("nbtag_2c_60-120_afterJetSel_cMVAv2T","number of btag jets (tight WP)",          6,-0.5,5.5);
        AddHistoTTbar("nbtag_2c_60-120_afterJetSel_cMVAv2M","number of btag jets (medium WP)",         6,-0.5,5.5);
        AddHistoTTbar("nbtag_2c_60-120_afterJetSel_cMVAv2L","number of btag jets (loose WP)",          6,-0.5,5.5);

        AddHistoTTbar("nbtag_2c_120-320_afterJetSel_cMVAv2T","number of btag jets (tight WP)",         6,-0.5,5.5);
        AddHistoTTbar("nbtag_2c_120-320_afterJetSel_cMVAv2M","number of btag jets (medium WP)",        6,-0.5,5.5);
        AddHistoTTbar("nbtag_2c_120-320_afterJetSel_cMVAv2L","number of btag jets (loose WP)",         6,-0.5,5.5);


        AddHistoTTbar("nbtag_1c1l_afterJetSel_cMVAv2T","number of btag jets (tight WP)",           6,-0.5,5.5);
        AddHistoTTbar("nbtag_1c1l_afterJetSel_cMVAv2M","number of btag jets (medium WP)",          6,-0.5,5.5);
        AddHistoTTbar("nbtag_1c1l_afterJetSel_cMVAv2L","number of btag jets (loose WP)",           6,-0.5,5.5);

        AddHistoTTbar("nbtag_1c1l_Inf60_afterJetSel_cMVAv2T","number of btag jets (tight WP)",         6,-0.5,5.5);
        AddHistoTTbar("nbtag_1c1l_Inf60_afterJetSel_cMVAv2M","number of btag jets (medium WP)",        6,-0.5,5.5);
        AddHistoTTbar("nbtag_1c1l_Inf60_afterJetSel_cMVAv2L","number of btag jets (loose WP)",         6,-0.5,5.5);

        AddHistoTTbar("nbtag_1c1l_60-120_afterJetSel_cMVAv2T","number of btag jets (tight WP)",        6,-0.5,5.5);
        AddHistoTTbar("nbtag_1c1l_60-120_afterJetSel_cMVAv2M","number of btag jets (medium WP)",       6,-0.5,5.5);
        AddHistoTTbar("nbtag_1c1l_60-120_afterJetSel_cMVAv2L","number of btag jets (loose WP)",        6,-0.5,5.5);

        AddHistoTTbar("nbtag_1c1l_120-320_afterJetSel_cMVAv2T","number of btag jets (tight WP)",       6,-0.5,5.5);
        AddHistoTTbar("nbtag_1c1l_120-320_afterJetSel_cMVAv2M","number of btag jets (medium WP)",      6,-0.5,5.5);
        AddHistoTTbar("nbtag_1c1l_120-320_afterJetSel_cMVAv2L","number of btag jets (loose WP)",       6,-0.5,5.5);


        AddHistoTTbar("nbtag_2l_afterJetSel_cMVAv2T","number of btag jets (tight WP)",             6,-0.5,5.5);
        AddHistoTTbar("nbtag_2l_afterJetSel_cMVAv2M","number of btag jets (medium WP)",            6,-0.5,5.5);
        AddHistoTTbar("nbtag_2l_afterJetSel_cMVAv2L","number of btag jets (loose WP)",             6,-0.5,5.5);

        AddHistoTTbar("nbtag_2l_Inf60_afterJetSel_cMVAv2T","number of btag jets (tight WP)",           6,-0.5,5.5);
        AddHistoTTbar("nbtag_2l_Inf60_afterJetSel_cMVAv2M","number of btag jets (medium WP)",          6,-0.5,5.5);
        AddHistoTTbar("nbtag_2l_Inf60_afterJetSel_cMVAv2L","number of btag jets (loose WP)",           6,-0.5,5.5);

        AddHistoTTbar("nbtag_2l_60-120_afterJetSel_cMVAv2T","number of btag jets (tight WP)",          6,-0.5,5.5);
        AddHistoTTbar("nbtag_2l_60-120_afterJetSel_cMVAv2M","number of btag jets (medium WP)",         6,-0.5,5.5);
        AddHistoTTbar("nbtag_2l_60-120_afterJetSel_cMVAv2L","number of btag jets (loose WP)",          6,-0.5,5.5);

        AddHistoTTbar("nbtag_2l_120-320_afterJetSel_cMVAv2T","number of btag jets (tight WP)",         6,-0.5,5.5);
        AddHistoTTbar("nbtag_2l_120-320_afterJetSel_cMVAv2M","number of btag jets (medium WP)",        6,-0.5,5.5);
        AddHistoTTbar("nbtag_2l_120-320_afterJetSel_cMVAv2L","number of btag jets (loose WP)",         6,-0.5,5.5);




        AddHistoTTbar("nbtag_all_afterJetSel_CSVv2T","number of btag jets (tight WP)",             6,-0.5,5.5);
        AddHistoTTbar("nbtag_all_afterJetSel_CSVv2M","number of btag jets (medium WP)",            6,-0.5,5.5);
        AddHistoTTbar("nbtag_all_afterJetSel_CSVv2M_SFapplied","number of btag jets (medium WP)",          6,-0.5,5.5);
        AddHistoTTbar("nbtag_all_afterJetSel_CSVv2L","number of btag jets (loose WP)",             6,-0.5,5.5);

        AddHistoTTbar("nbtag_all_Inf60_afterJetSel_CSVv2T","number of btag jets (tight WP)",           6,-0.5,5.5);
        AddHistoTTbar("nbtag_all_Inf60_afterJetSel_CSVv2M","number of btag jets (medium WP)",          6,-0.5,5.5);
        AddHistoTTbar("nbtag_all_Inf60_afterJetSel_CSVv2L","number of btag jets (loose WP)",           6,-0.5,5.5);

        AddHistoTTbar("nbtag_all_60-120_afterJetSel_CSVv2T","number of btag jets (tight WP)",          6,-0.5,5.5);
        AddHistoTTbar("nbtag_all_60-120_afterJetSel_CSVv2M","number of btag jets (medium WP)",         6,-0.5,5.5);
        AddHistoTTbar("nbtag_all_60-120_afterJetSel_CSVv2L","number of btag jets (loose WP)",          6,-0.5,5.5);

        AddHistoTTbar("nbtag_all_120-320_afterJetSel_CSVv2T","number of btag jets (tight WP)",         6,-0.5,5.5);
        AddHistoTTbar("nbtag_all_120-320_afterJetSel_CSVv2M","number of btag jets (medium WP)",        6,-0.5,5.5);
        AddHistoTTbar("nbtag_all_120-320_afterJetSel_CSVv2L","number of btag jets (loose WP)",         6,-0.5,5.5);


        AddHistoTTbar("nbtag_2b_afterJetSel_CSVv2T","number of btag jets (tight WP)",          6,-0.5,5.5);
        AddHistoTTbar("nbtag_2b_afterJetSel_CSVv2M","number of btag jets (medium WP)",         6,-0.5,5.5);
        AddHistoTTbar("nbtag_2b_afterJetSel_CSVv2L","number of btag jets (loose WP)",          6,-0.5,5.5);

        AddHistoTTbar("nbtag_2b_Inf60_afterJetSel_CSVv2T","number of btag jets (tight WP)",        6,-0.5,5.5);
        AddHistoTTbar("nbtag_2b_Inf60_afterJetSel_CSVv2M","number of btag jets (medium WP)",       6,-0.5,5.5);
        AddHistoTTbar("nbtag_2b_Inf60_afterJetSel_CSVv2L","number of btag jets (loose WP)",        6,-0.5,5.5);

        AddHistoTTbar("nbtag_2b_60-120_afterJetSel_CSVv2T","number of btag jets (tight WP)",       6,-0.5,5.5);
        AddHistoTTbar("nbtag_2b_60-120_afterJetSel_CSVv2M","number of btag jets (medium WP)",      6,-0.5,5.5);
        AddHistoTTbar("nbtag_2b_60-120_afterJetSel_CSVv2L","number of btag jets (loose WP)",       6,-0.5,5.5);

        AddHistoTTbar("nbtag_2b_120-320_afterJetSel_CSVv2T","number of btag jets (tight WP)",      6,-0.5,5.5);
        AddHistoTTbar("nbtag_2b_120-320_afterJetSel_CSVv2M","number of btag jets (medium WP)",     6,-0.5,5.5);
        AddHistoTTbar("nbtag_2b_120-320_afterJetSel_CSVv2L","number of btag jets (loose WP)",      6,-0.5,5.5);


        AddHistoTTbar("nbtag_1b1c_afterJetSel_CSVv2T","number of btag jets (tight WP)",        6,-0.5,5.5);
        AddHistoTTbar("nbtag_1b1c_afterJetSel_CSVv2M","number of btag jets (medium WP)",       6,-0.5,5.5);
        AddHistoTTbar("nbtag_1b1c_afterJetSel_CSVv2L","number of btag jets (loose WP)",        6,-0.5,5.5);

        AddHistoTTbar("nbtag_1b1c_Inf60_afterJetSel_CSVv2T","number of btag jets (tight WP)",      6,-0.5,5.5);
        AddHistoTTbar("nbtag_1b1c_Inf60_afterJetSel_CSVv2M","number of btag jets (medium WP)",     6,-0.5,5.5);
        AddHistoTTbar("nbtag_1b1c_Inf60_afterJetSel_CSVv2L","number of btag jets (loose WP)",      6,-0.5,5.5);

        AddHistoTTbar("nbtag_1b1c_60-120_afterJetSel_CSVv2T","number of btag jets (tight WP)",     6,-0.5,5.5);
        AddHistoTTbar("nbtag_1b1c_60-120_afterJetSel_CSVv2M","number of btag jets (medium WP)",    6,-0.5,5.5);
        AddHistoTTbar("nbtag_1b1c_60-120_afterJetSel_CSVv2L","number of btag jets (loose WP)",     6,-0.5,5.5);

        AddHistoTTbar("nbtag_1b1c_120-320_afterJetSel_CSVv2T","number of btag jets (tight WP)",     6,-0.5,5. );
        AddHistoTTbar("nbtag_1b1c_120-320_afterJetSel_CSVv2M","number of btag jets (medium WP)",    6,-0.5,5. );
        AddHistoTTbar("nbtag_1b1c_120-320_afterJetSel_CSVv2L","number of btag jets (loose WP)",     6,-0.5,5. );


        AddHistoTTbar("nbtag_1b1l_afterJetSel_CSVv2T","number of btag jets (tight WP)",        6,-0.5,5.5);
        AddHistoTTbar("nbtag_1b1l_afterJetSel_CSVv2M","number of btag jets (medium WP)",       6,-0.5,5.5);
        AddHistoTTbar("nbtag_1b1l_afterJetSel_CSVv2L","number of btag jets (loose WP)",        6,-0.5,5.5);

        AddHistoTTbar("nbtag_1b1l_Inf60_afterJetSel_CSVv2T","number of btag jets (tight WP)",      6,-0.5,5.5);
        AddHistoTTbar("nbtag_1b1l_Inf60_afterJetSel_CSVv2M","number of btag jets (medium WP)",     6,-0.5,5.5);
        AddHistoTTbar("nbtag_1b1l_Inf60_afterJetSel_CSVv2L","number of btag jets (loose WP)",      6,-0.5,5.5);

        AddHistoTTbar("nbtag_1b1l_60-120_afterJetSel_CSVv2T","number of btag jets (tight WP)",     6,-0.5,5.5);
        AddHistoTTbar("nbtag_1b1l_60-120_afterJetSel_CSVv2M","number of btag jets (medium WP)",    6,-0.5,5.5);
        AddHistoTTbar("nbtag_1b1l_60-120_afterJetSel_CSVv2L","number of btag jets (loose WP)",     6,-0.5,5.5);

        AddHistoTTbar("nbtag_1b1l_120-320_afterJetSel_CSVv2T","number of btag jets (tight WP)",     6,-0.5,5. );
        AddHistoTTbar("nbtag_1b1l_120-320_afterJetSel_CSVv2M","number of btag jets (medium WP)",    6,-0.5,5. );
        AddHistoTTbar("nbtag_1b1l_120-320_afterJetSel_CSVv2L","number of btag jets (loose WP)",     6,-0.5,5. );


        AddHistoTTbar("nbtag_2c_afterJetSel_CSVv2T","number of btag jets (tight WP)",          6,-0.5,5.5);
        AddHistoTTbar("nbtag_2c_afterJetSel_CSVv2M","number of btag jets (medium WP)",         6,-0.5,5.5);
        AddHistoTTbar("nbtag_2c_afterJetSel_CSVv2L","number of btag jets (loose WP)",          6,-0.5,5.5);

        AddHistoTTbar("nbtag_2c_Inf60_afterJetSel_CSVv2T","number of btag jets (tight WP)",        6,-0.5,5.5);
        AddHistoTTbar("nbtag_2c_Inf60_afterJetSel_CSVv2M","number of btag jets (medium WP)",       6,-0.5,5.5);
        AddHistoTTbar("nbtag_2c_Inf60_afterJetSel_CSVv2L","number of btag jets (loose WP)",        6,-0.5,5.5);

        AddHistoTTbar("nbtag_2c_60-120_afterJetSel_CSVv2T","number of btag jets (tight WP)",       6,-0.5,5.5);
        AddHistoTTbar("nbtag_2c_60-120_afterJetSel_CSVv2M","number of btag jets (medium WP)",      6,-0.5,5.5);
        AddHistoTTbar("nbtag_2c_60-120_afterJetSel_CSVv2L","number of btag jets (loose WP)",       6,-0.5,5.5);

        AddHistoTTbar("nbtag_2c_120-320_afterJetSel_CSVv2T","number of btag jets (tight WP)",      6,-0.5,5.5);
        AddHistoTTbar("nbtag_2c_120-320_afterJetSel_CSVv2M","number of btag jets (medium WP)",     6,-0.5,5.5);
        AddHistoTTbar("nbtag_2c_120-320_afterJetSel_CSVv2L","number of btag jets (loose WP)",      6,-0.5,5.5);


        AddHistoTTbar("nbtag_1c1l_afterJetSel_CSVv2T","number of btag jets (tight WP)",        6,-0.5,5.5);
        AddHistoTTbar("nbtag_1c1l_afterJetSel_CSVv2M","number of btag jets (medium WP)",       6,-0.5,5.5);
        AddHistoTTbar("nbtag_1c1l_afterJetSel_CSVv2L","number of btag jets (loose WP)",        6,-0.5,5.5);

        AddHistoTTbar("nbtag_1c1l_Inf60_afterJetSel_CSVv2T","number of btag jets (tight WP)",      6,-0.5,5.5);
        AddHistoTTbar("nbtag_1c1l_Inf60_afterJetSel_CSVv2M","number of btag jets (medium WP)",     6,-0.5,5.5);
        AddHistoTTbar("nbtag_1c1l_Inf60_afterJetSel_CSVv2L","number of btag jets (loose WP)",      6,-0.5,5.5);

        AddHistoTTbar("nbtag_1c1l_60-120_afterJetSel_CSVv2T","number of btag jets (tight WP)",     6,-0.5,5.5);
        AddHistoTTbar("nbtag_1c1l_60-120_afterJetSel_CSVv2M","number of btag jets (medium WP)",    6,-0.5,5.5);
        AddHistoTTbar("nbtag_1c1l_60-120_afterJetSel_CSVv2L","number of btag jets (loose WP)",     6,-0.5,5.5);

        AddHistoTTbar("nbtag_1c1l_120-320_afterJetSel_CSVv2T","number of btag jets (tight WP)",     6,-0.5,5. );
        AddHistoTTbar("nbtag_1c1l_120-320_afterJetSel_CSVv2M","number of btag jets (medium WP)",    6,-0.5,5. );
        AddHistoTTbar("nbtag_1c1l_120-320_afterJetSel_CSVv2L","number of btag jets (loose WP)",     6,-0.5,5. );


        AddHistoTTbar("nbtag_2l_afterJetSel_CSVv2T","number of btag jets (tight WP)",          6,-0.5,5.5);
        AddHistoTTbar("nbtag_2l_afterJetSel_CSVv2M","number of btag jets (medium WP)",         6,-0.5,5.5);
        AddHistoTTbar("nbtag_2l_afterJetSel_CSVv2L","number of btag jets (loose WP)",          6,-0.5,5.5);

        AddHistoTTbar("nbtag_2l_Inf60_afterJetSel_CSVv2T","number of btag jets (tight WP)",        6,-0.5,5.5);
        AddHistoTTbar("nbtag_2l_Inf60_afterJetSel_CSVv2M","number of btag jets (medium WP)",       6,-0.5,5.5);
        AddHistoTTbar("nbtag_2l_Inf60_afterJetSel_CSVv2L","number of btag jets (loose WP)",        6,-0.5,5.5);

        AddHistoTTbar("nbtag_2l_60-120_afterJetSel_CSVv2T","number of btag jets (tight WP)",       6,-0.5,5.5);
        AddHistoTTbar("nbtag_2l_60-120_afterJetSel_CSVv2M","number of btag jets (medium WP)",      6,-0.5,5.5);
        AddHistoTTbar("nbtag_2l_60-120_afterJetSel_CSVv2L","number of btag jets (loose WP)",       6,-0.5,5.5);

        AddHistoTTbar("nbtag_2l_120-320_afterJetSel_CSVv2T","number of btag jets (tight WP)",      6,-0.5,5.5);
        AddHistoTTbar("nbtag_2l_120-320_afterJetSel_CSVv2M","number of btag jets (medium WP)",     6,-0.5,5.5);
        AddHistoTTbar("nbtag_2l_120-320_afterJetSel_CSVv2L","number of btag jets (loose WP)",      6,-0.5,5.5);

        AddHistoBtag("sv_deltaR_jet",      "sv_deltaR_jet",                                       50,0.,0.5  );
        AddHistoBtag("sv_deltaR_sumJet",   "SVvtxSumJetDeltaR",                                   50,0.,0.5  );
        AddHistoBtag("sv_deltaR_sumDir",   "SVvtxSumVtxDirDeltaR",                                50,0.,0.5  );
        AddHistoBtag("sv_en_ratio",        "Fractional energy",                                   50,0.,1.   );
        AddHistoBtag("sv_pt",              "Vtx p_{T}",                                           50,0.,100. );
        AddHistoBtag("sv_eta",             "Vtx #eta",                                            50, -2.5, 2.5);
        AddHistoBtag("sv_phi",             "Vtx #phi",                                            40, -1.*pi,pi);
        AddHistoBtag("sv_flightSig2D",     "Flight distance significance 2D",                     50,0.,80.   );
        AddHistoBtag("sv_flight2D",        "Flight distance 2D",                                  50,0.,2.5   );
        AddHistoBtag("sv_flight3D",        "Flight distance 3D",                                  50,0.,15.   );
        AddHistoBtag("sv_flight3DSig" ,    "flight distance significance 3D",                     50,0.,80.   );
        AddHistoBtag("sv_multi_0"   ,      "number of secondary vertex",                          6,-0.5,5.5 );
        AddHistoBtag("sv_multi"     ,      "number of secondary vertex",                          6,-0.5,5.5 );
        AddHistoBtag("sv_mass"      ,      "invariant mass of the secondary vertex",              50,0.,8.   );
        AddHistoBtag("sv_chi2norm"  ,      "normalized chi2 of the secondary vertex",                50,0.,10.   );
        AddHistoBtag("sv_tot_charge",      "Total charge",                                                        21,-10.5,10.5 );
        AddHistoBtag("svnTrk",             "Track multiplicity : SVnVertexTracks (centered)",                     13, -0.5,12.5);
        AddHistoBtag("sv_flight3Derr",     "Flight distance error 3D",                                            50,   0.,0.2 );
        AddHistoBtag("sv_flight2Derr",     "Flight distance error 2D",                                            50,   0.,0.05);
        AddHistoBtag("sv_mass_3trk"   ,"invariant mass of the secondary vertex with at least 3 SV tracks",    50,   0.,8. );

        AddHistoBtag("track_multi"  ,      "number of tracks in the jets",                40,-0.5,39.5);
        AddHistoBtag("trk_multi_sel"  ,    "number of selected tracks in the jets",       40,-0.5,39.5);
        AddHistoBtag("track_chi2"   ,      "normalized chi2 of the tracks",               100,0.,30.  );
        AddHistoBtag("track_nHit" ,        "number of hits ",               35,-0.5, 34.5 );
        AddHistoBtag("track_HPix"   ,      "number of hits in the Pixel",                 10,-0.5, 9.5 );

        AddHistoBtag("track_IPs"    ,      "3D IP significance of all tracks",            100,-35.,35.);
        AddHistoBtag("track_IPs1tr" ,      "3D IP significance of the first track",       100,-35.,35.);
        AddHistoBtag("track_IPs2tr" ,      "3D IP significance of the second track",      100,-35.,35.);
        AddHistoBtag("track_IP"     ,      "3D IP of all tracks",                         100,-0.1,0.1);
        AddHistoBtag("track_IP1tr"  ,      "3D IP of the first track",                    100,-0.1,0.1);
        AddHistoBtag("track_IP2tr"  ,      "3D IP of the second track",                   100,-0.1,0.1);
        AddHistoBtag("track_IP2Ds"    ,    "2D IP significance of all tracks",            100,-35.,35.);
        AddHistoBtag("track_IP2Ds1tr" ,    "2D IP significance of the first track",       100,-35.,35.);
        AddHistoBtag("track_IP2Ds2tr" ,    "2D IP significance of the second track",      100,-35.,35.);
        AddHistoBtag("track_IP2D"    ,     "2D IP of all tracks",                         100,-0.1,0.1);
        AddHistoBtag("track_IP2D1tr" ,     "2D IP of the first track",                    100,-0.1,0.1);
        AddHistoBtag("track_IP2D2tr" ,     "2D IP of the second track",                   100,-0.1,0.1);
        AddHistoBtag("track_IP2Derr1tr" ,  "2D IP error of the first track",              100,0,0.1   );
        AddHistoBtag("track_IPerr1tr"   ,  "3D IP error of the first track",              100,0,0.1   );
        AddHistoBtag("track_IP2Derr2tr" ,  "2D IP error of the second track",             100,0,0.1   );
        AddHistoBtag("track_IPerr2tr"   ,  "3D IP error of the second track",             100,0,0.1   );
        AddHistoBtag("track_IP2Derr" ,     "2D IP error",                                 100,0,0.1   );
        AddHistoBtag("track_IPerr"   ,     "3D IP error",                                 100,0,0.1   );
        AddHistoBtag("track_IPs3tr" ,      "3D IP significance of the third track",       100,-35.,35.);
        AddHistoBtag("track_IP3tr"  ,      "3D IP of the third track",                    100,-0.1,0.1);
        AddHistoBtag("track_IPerr3tr"   ,  "3D IP error of the third track",              100,0,0.1   );
        AddHistoBtag("track_IP2Ds3tr" ,    "2D IP significance of the second track",      100,-35.,35.);
        AddHistoBtag("track_IP2D3tr" ,     "2D IP of the third track",                    100,-0.1,0.1);
        AddHistoBtag("track_IP2Derr3tr" ,  "2D IP error of the third track",              100,0,0.1   );

        AddHistoBtag("track_len"     ,     "decay length",                        100,0,25.    );
        AddHistoBtag("track_dist"    ,     "distance to the jet axis",                    100,0.,0.3  );
        AddHistoBtag("track_dz"     ,      "transverse IP",                               100,-20.,20.);
        AddHistoBtag("track_isfromSV",     "Track is from SV",                            2,-0.5, 1.5 );
        AddHistoBtag("track_pt"     ,      "pT of all the tracks",                    80,0.,200. );
        AddHistoBtag("track_chi2_cut"     ,"normalized chi2 ",                            100,0.,30. );
        AddHistoBtag("track_nHit_cut"     ,"number of hits  ",                            35,-0.5, 34.5);
        AddHistoBtag("track_HPix_cut"     ,"number of hits in the Pixel ",                10,-0.5, 9.5 );
        AddHistoBtag("track_len_cut"      ,"decay length ",                       100,0,25.   );
        AddHistoBtag("track_dist_cut"     ,"distance to the jet axis ",                   100,0.,0.3);
        AddHistoBtag("track_dz_cut"       ,"transverse IP ",                      100,-20., 20.);
        AddHistoBtag("track_pt_cut"       ,"pT ",                                         80,0.,200.);
        AddHistoBtag("track_IP2D_cut"     ,"IP2D ",                                   100,-0.1,0.1);

        AddHistoBtag("TCHE_extended1"   ,"TCHE_extended1",                70, -30.,30.);
        AddHistoBtag("TCHP_extended1"   ,"TCHP_extended1",                70, -30.,30.);
        AddHistoBtag("TCHE_extended2"   ,"TCHE_extended2",                100,-30.,30.);
        AddHistoBtag("TCHP_extended2"   ,"TCHP_extended2",                100,-30.,30.);
        AddHistoBtag("discri_ssche0",    "SSVHE Discriminator",                       80, -1., 7. );
        AddHistoBtag("discri_sschp0",    "SSVHP Discriminator",                       80, -1., 7. );

        AddHistoBtag("TCHE"     ,"TCHE",                   50,0.,30. );
        AddHistoBtag("TCHP"     ,"TCHP",                   50,0.,30. );
        AddHistoBtag("JP"       ,"JP",                     30,0.,1.5 );
        AddHistoBtag("JBP"      ,"JBP",                    25,0.,4.  );
        AddHistoBtag("SSV"      ,"SSVHE",                  70,0.,7.  );
        AddHistoBtag("SSVHP"    ,"SSVHP",                  70,0.,7.  );
        AddHistoBtag("CSV"      ,"CSV",                    50,0.,1.  );
        AddHistoBtag("CSVv2"    ,"CSVv2",                  50,0.,1.  );
        AddHistoBtag("CSVv2pu"      ,"CSVv2pu",            50,0.,1.  );
        AddHistoBtag("DeepCSVb"     ,"DeepCSVb",           50,0.,1.  );
        AddHistoBtag("DeepCSVbb"    ,"DeepCSVbb",          50,0.,1.  );
        AddHistoBtag("DeepCSVBDisc"     ,"DeepCSVBDisc",           50,0.,1.  );
        AddHistoBtag("DeepCSVbpu"   ,"DeepCSVbpu",             50,0.,1.  );
        AddHistoBtag("DeepFlavourBDisc"     ,"DeepFlavourBDisc",           50,0.,1.  );
        AddHistoBtag("DeepFlavourBDiscpu"   ,"DeepFlavourBDiscpu",             50,0.,1.  );
        AddHistoBtag("cMVAv2"   ,"cMVAv2",                     50,-1.,1. );
        AddHistoBtag("CvsB"     ,"CvsB",                                       50,-1.,1. );
        AddHistoBtag("CvsL"     ,"CvsL",                                       50,-1.,1. );

        AddHistoBtag("SoftMu"       ,"SoftMu",                                 50,0.,1.  );
        AddHistoBtag("SoftEl"       ,"SoftEl",                                 50,0.,1.  );

        AddHistoBtag("pfmuon_multi",      "number of pfmuons",            7,    -0.5, 6.5);
        AddHistoBtag("pfmuon_goodquality","quality of pfmuons",           3,    -0.5, 2.5 );
        AddHistoBtag("pfmuon_pt",     "pfmuon p_{T}",             50,      0, 100 );
        AddHistoBtag("pfmuon_eta",    "pfmuon #eta",              50,   -2.5, 2.5 );
        AddHistoBtag("pfmuon_phi",        "pfmuon #phi",                  40, -1.*pi,  pi );
        AddHistoBtag("pfmuon_Sip",    "3D IP significance of pfmuon", 50,    -35,  35 );
        AddHistoBtag("pfmuon_ptrel",      "pT rel. of the muon",          50,      0,   5 );
        AddHistoBtag("pfmuon_ratio",      "ratio of pfmuon",              50,      0,   2);
        AddHistoBtag("pfmuon_ratiorel",   "ratioRel of pfmuon",           50,      0,0.05 );
        AddHistoBtag("pfmuon_deltar", "#DeltaR(pfmuon,jet)",          50,      0, 0.5 );

        AddHistoBtag("pfelectron_multi",  "number of pfelectron",         7,    -0.5, 6.5 );
        AddHistoBtag("pfelectron_pt", "pfelectron p_{T}",             50,      0, 100 );
        AddHistoBtag("pfelectron_eta",    "pfelectron #eta",              50,   -2.5, 2.5 );
        AddHistoBtag("pfelectron_phi",    "pfelectron #phi",              40, -1.*pi,  pi );
        AddHistoBtag("pfelectron_ptrel",  "pT rel. of the pfelectron",    50,      0,   5 );
        AddHistoBtag("pfelectron_ratio",  "ratio of pfelectron",          50,      0,   2 );
        AddHistoBtag("pfelectron_ratiorel","ratioRel of pfelectron",      50,      0,0.05 );
        AddHistoBtag("pfelectron_deltar", "#DeltaR(pfelectron,jet)",      50,      0, 0.5 );


        AddHisto2D("seltrack_vs_jetpt", "sel track multiplicity vs jet pt",         30,60,1000, 100,-0.5,99.5 );
        AddHisto2D("sv_mass_vs_flightDist3D", " SVMass vs SV 3D flight distance ",  100,0, 10,100,0,6        );
        AddHisto2D("avg_sv_mass_vs_jetpt","Avg SVMass vs jet pt",                   30,60,1000, 100,0,6      );
        AddHisto2D("sv_deltar_jet_vs_jetpt","SVJetDeltaR vs jet pt",                25,60,300, 50,0.,0.5     );
        AddHisto2D("sv_deltar_sum_jet_vs_jetpt","SVvtxSumJetDeltaR vs jet pt",      25,60,300, 50,0.,0.5     );
        AddHisto2D("sv_deltar_sum_dir_vs_jetpt","SVvtxSumVtxDirDeltaR vs jet pt",   25,60,300, 50,0.,0.5     );

        AddHistoBtag("tagvarCSV_vertexCategory",          "vertex category",                      3, -0.5, 2.5 );
        AddHistoBtag("tagvarCSV_Sig2dAboveCharm",         "IP significance 2D charm",       nSVbins, -35.,35.  );
        AddHistoBtag("tagvarCSV_trackEtaRel",             "Track etaRel",                        40,   0.,8.   );
        AddHistoBtag("tagvarCSV_trackSumJetEtRatio",      "Track  SumJet ET ratio",              40,   0.,1.5  );
        AddHistoBtag("tagvarCSV_trackSumJetDeltaR",       "Track  SumJet Delta r",               40,   0.,0.5  );

        AddHistoBtag("tagvarCSV_vertexmass_cat0",         "SV mass",                        nSVbins,   0.,8.  );
        AddHistoBtag("tagvarCSV_vertexmass3trk_cat0",     "SV mass (at least 3 SV tracks)", nSVbins,   0.,8.  );
        AddHistoBtag("tagvarCSV_vertexNTracks_cat0",      "# SV tracks",                         13, -0.5,12.5);
        AddHistoBtag("tagvarCSV_energyratio",             "Fractional energy",              nSVbins,   0.,1.  );
        AddHistoBtag("tagvarCSV_trackSip3dSig",           "3D IP significance",          nTrackbins, -35.,35. );
        AddHistoBtag("tagvarCSV_2DsigFlightDist_cat0",    "Flight distance significance 2D",nSVbins,   0.,80. );
        AddHistoBtag("tagvarCSV_vertexJetDeltaR_cat0",    "DeltaR(SV,jet) ",                nSVbins,   0.,0.4 );
    } //}}}end !produceCTagTree

    AddHistoBtag("jet_multi"    ,"number of jets",                 20,        0,      20);
    AddHistoBtag("jet_pt_all"   ,"pT of all jets",                 50,  0,    500);
    AddHistoBtag("genjet_pt_all"        ,"genpT of all jets",         50     ,  -0.5,    49.5);
    AddHistoBtag("jet_pt_sv"    ,"pT of jets containing a SV",     50,  0,    500);
    AddHistoBtag("jet_eta"      ,"eta of all jets",                50,        -2.5,   2.5);
    AddHistoBtag("jet_phi"      ,"phi of all jets",                40,        -1.*pi, pi);

    //CTag Comm//
    if(produceCTagTree){//{{{
        AddHistoBtag("CTag_tagvarCSV_vertexCategory",          "vertex category CSV",                      3, -0.5, 2.5 );
        AddHistoBtag("CTag_tagvarCSV_Sig2dAboveCharm",         "IP significance 2D charm CSV",       nSVbins, -35.,35.  );
        AddHistoBtag("CTag_tagvarCSV_trackEtaRel",             "Track etaRel CSV",                        40,   0.,8.   );
        AddHistoBtag("CTag_tagvarCSV_trackSumJetEtRatio",      "Track  SumJet ET ratio CSV",              40,   0.,1.5  );
        AddHistoBtag("CTag_tagvarCSV_trackSumJetDeltaR",       "Track  SumJet Delta r CSV",               40,   0.,0.5  );

        AddHistoBtag("CTag_tagvarCSV_vertexmass_cat0",         "SV mass CSV",                        nSVbins,   0.,8.   );
        AddHistoBtag("CTag_tagvarCSV_vertexmass3trk_cat0",     "SV mass (at least 3 SV tracks) CSV", nSVbins,   0.,8.   );
        AddHistoBtag("CTag_tagvarCSV_vertexNTracks_cat0",      "# SV tracks CSV",                         13, -0.5,12.5 );
        AddHistoBtag("CTag_tagvarCSV_energyratio",             "Fractional energy CSV",              nSVbins,   0.,1.   );
        AddHistoBtag("CTag_tagvarCSV_trackSip3dSig",           "3D IP significance CSV",          nTrackbins, -35.,35.  );
        AddHistoBtag("CTag_tagvarCSV_2DsigFlightDist_cat0",    "Flight distance significance 2D CSV",nSVbins,   0.,80.  );
        AddHistoBtag("CTag_tagvarCSV_vertexJetDeltaR_cat0",    "DeltaR(SV,jet) CSV",                nSVbins,   0.,0.4 );

        AddHistoBtag("JP"           ,"JP",                                     30,0.,1.5 );
        AddHistoBtag("CSVv2"        ,"CSVv2",                                  50,0.,1.  );
        AddHistoBtag("CSVv2pu"     ,"CSVv2pu",                               50,0.,1.  );
        AddHistoBtag("cMVAv2"       ,"cMVAv2",                                 50,-1.,1. );

        AddHistoBtag("CvsB"         ,"CvsB",                                   50,-1.,1. );
        AddHistoBtag("CvsBN"        ,"CvsBN",                                  50,-1.,1. );
        AddHistoBtag("CvsBP"        ,"CvsBP",                                  50,-1.,1. );
        AddHistoBtag("CvsL"         ,"CvsL",                                   50,-1.,1. );
        AddHistoBtag("CvsLN"        ,"CvsLN",                                  50,-1.,1. );
        AddHistoBtag("CvsLP"        ,"CvsLP",                                  50,-1.,1. );
        AddHistoBtag("CTag_jetNTracks"            ,"CTag_jetNTracks",     40,-0.5,39.5 );
        AddHistoBtag("CTag_jetNTracksEtaRel"      ,"CTag_jetNTracksEtaRel",   40,-0.5,39.5  );
        AddHistoBtag("CTag_jetNLeptons"           ,"CTag_jetNLeptons",        7,-0.5, 6.5 );
        AddHistoBtag("CTag_trackSumJetEtRatio"    ,"CTag_trackSumJetEtRatio",     40,   0.,1.5 );
        AddHistoBtag("CTag_trackSumJetDeltaR"     ,"CTag_trackSumJetDeltaR",  40,   0.,0.3  );
        AddHistoBtag("CTag_trackSip2dSigAboveCharm","CTag_trackSip2dSigAboveCharm",50, -35,35 );
        AddHistoBtag("CTag_trackSip3dSigAboveCharm","CTag_trackSip3dSigAboveCharm",50, -35,35 );
        AddHistoBtag("CTag_vertexCategory"    ,"CTag_vertexCategory",     3, -0.5, 2.5 );
        AddHistoBtag("CTag_jetNSecondaryVertices" ,"CTag_jetNSecondaryVertices",  6,-0.5,5.5 );
        AddHistoBtag("CTag_vertexMass"        ,"CTag_vertexMass",     nSVbins,   0.,8. );
        AddHistoBtag("CTag_vertexNTracks"     ,"CTag_vertexNTracks",      13, -0.5,12.5 );
        AddHistoBtag("CTag_vertexEnergyRatio" ,"CTag_vertexEnergyRatio",  nSVbins,   0.,1.  );
        AddHistoBtag("CTag_vertexJetDeltaR"   ,"CTag_vertexJetDeltaR",    nSVbins,   0.,0.4 );
        AddHistoBtag("CTag_flightDistance2dSig"   ,"CTag_flightDistance2dSig",    nSVbins,   0.,80. );
        AddHistoBtag("CTag_flightDistance3dSig"   ,"CTag_flightDistance3dSig",    nSVbins,   0.,80.  );
        AddHistoBtag("CTag_massVertexEnergyFraction","CTag_massVertexEnergyFraction", 50, 0, 1 );
        AddHistoBtag("CTag_vertexBoostOverSqrtJetPt","CTag_vertexBoostOverSqrtJetPt", 50, 0, 1 );
        AddHistoBtag("CTag_vertexLeptonCategory"  ,"CTag_vertexLeptonCategory",   7,-0.5, 6.5 );
        AddHistoBtag("CTag_trackPtRel"  ,"CTag_trackPtRel", 40, 0 ,10   );
        AddHistoBtag("CTag_trackPPar"  ,"CTag_trackPPar", 40, 0, 200   );
        AddHistoBtag("CTag_trackDeltaR"  ,"CTag_trackDeltaR", 40, 0, 0.4   );
        AddHistoBtag("CTag_trackPtRatio"  ,"CTag_trackPtRatio", 30, 0, 0.3   );
        AddHistoBtag("CTag_trackPParRatio"  ,"CTag_trackPParRatio", 40, 0.95, 1   );
        AddHistoBtag("CTag_trackSip2dSig"  ,"CTag_trackSip2dSig", 40, -30, 30   );
        AddHistoBtag("CTag_trackSip3dSig"  ,"CTag_trackSip3dSig", 40, -30, 30   );
        AddHistoBtag("CTag_trackDecayLenVal"  ,"CTag_trackDecayLenVal", 40, 0, 5   );
        AddHistoBtag("CTag_trackJetDistVal"  ,"CTag_trackJetDistVal", 40, -0.07, 0   );
        AddHistoBtag("CTag_trackEtaRel"  ,"CTag_trackEtaRel", 40, 0, 8   );
        AddHistoBtag("CTag_leptonPtRel"  ,"CTag_leptonPtRel", 40, 0 ,6   );
        AddHistoBtag("CTag_leptonSip3d"  ,"CTag_leptonSip3d", 50, -1, 1   );
        AddHistoBtag("CTag_leptonDeltaR"  ,"CTag_leptonDeltaR", 40 , 0, 0.4   );
        AddHistoBtag("CTag_leptonRatioRel"  ,"CTag_leptonRatioRel", 40, 0 , 0.02   );
        AddHistoBtag("CTag_leptonEtaRel"  ,"CTag_leptonEtaRel", 40, 0 , 0.1   );
        AddHistoBtag("CTag_leptonRatio"  ,"CTag_leptonRatio", 40 , 0 ,1   );
        //With CTag_vertexCategory==0 //
        AddHistoBtag("CTag_jetNSecondaryVertices_Vcat0" ,"CTag_jetNSecondaryVertices_Vcat0",  6,-0.5,5.5   );
        AddHistoBtag("CTag_vertexMass_Vcat0"            ,"CTag_vertexMass_Vcat0",             nSVbins,   0.,8.   );
        AddHistoBtag("CTag_vertexNTracks_Vcat0"         ,"CTag_vertexNTracks_Vcat0",          13, -0.5,12.5 );
        AddHistoBtag("CTag_vertexEnergyRatio_Vcat0"     ,"CTag_vertexEnergyRatio_Vcat0",      nSVbins,   0.,1.   );
        AddHistoBtag("CTag_vertexJetDeltaR_Vcat0"       ,"CTag_vertexJetDeltaR_Vcat0",        nSVbins,   0.,0.4  );
        AddHistoBtag("CTag_flightDistance2dSig_Vcat0"   ,"CTag_flightDistance2dSig_Vcat0",    nSVbins,   0.,80.  );
        AddHistoBtag("CTag_flightDistance3dSig_Vcat0"   ,"CTag_flightDistance3dSig_Vcat0",    nSVbins,   0.,80.  );
        AddHistoBtag("CTag_massVertexEnergyFraction_Vcat0","CTag_massVertexEnergyFraction_Vcat0", 50, 0, 1 );
        AddHistoBtag("CTag_vertexBoostOverSqrtJetPt_Vcat0","CTag_vertexBoostOverSqrtJetPt_Vcat0", 50, 0, 1 );
    }//}}}


    Nevent = 0;
    if (fChain == 0) return;

    double sumWeightWoPUreweighting       = 0;
    double sumWeightWithPUreweighting     = 0;
    double sumWeightWithPUreweightingUp   = 0;
    double sumWeightWithPUreweightingDown = 0;

    TRandom3 *theRand_ = new TRandom3(12345);  // used to apply btag efficiency (btag SF closure test)
    Long64_t nentries = fChain->GetEntries();
    Long64_t nbytes = 0, nb = 0;

    // Record total generated events for mc
    if( !isData ) totalGenEvts->Fill(0.,wgtcounter->GetBinContent(1));

    //------------------------------------------------------------------------------------------------------------------//
    //----------------------------------------EVENT LOOP ---------------------------------------------------------------//
    //------------------------------------------------------------------------------------------------------------------//

    std::cout<<"Running "<<nentries<<" events....";
    for (Long64_t jentry=0; jentry<nentries;jentry++)
    {

        Long64_t ientry = LoadTree(jentry);
        if (ientry < 0) break;
        if (debug && ientry % 10 == 0) printProgressBar(ientry, nentries);
        nb = fChain->GetEntry(jentry);   nbytes += nb;

        //if (ientry > 100000) break;

        if (isData) N_event_data_before_sel++;
        else        N_event_mc_before_sel++;

        //-----------------------------------
        //initialaze the weight at 1
        //-----------------------------------
        double ww=1;

        vector<TLorentzVector>    theLeptColl;
        vector<Int_t>             theLeptIds;

        for( unsigned short int ilep = 0; ilep < ttbar_nl; ilep++)
        {
            TLorentzVector    theTmpLept;
            theTmpLept.SetPtEtaPhiM(ttbar_lpt[ilep], ttbar_leta[ilep], ttbar_lphi[ilep], ttbar_lm[ilep]);
            theLeptColl.push_back(theTmpLept);
            theLeptIds.push_back(ttbar_lid[ilep]*ttbar_lch[ilep]);
        }

        if(ttbar_nl >= 2)
        {
            thettbarselector_.lept1_.SetPtEtaPhiM(ttbar_lpt[0], ttbar_leta[0], ttbar_lphi[0], ttbar_lm[0]);
            thettbarselector_.lept2_.SetPtEtaPhiM(ttbar_lpt[1], ttbar_leta[1], ttbar_lphi[1], ttbar_lm[1]);
        }

        vector< pair< TLorentzVector, Float_t> > theJetColl;
        for (unsigned short int ijet = 0; ijet < nJet; ijet++)
        {
            TLorentzVector tmpJet_;
            tmpJet_.SetPtEtaPhiM(Jet_pt[ijet], Jet_eta[ijet], Jet_phi[ijet], Jet_mass[ijet]);
            theJetColl.push_back(make_pair( tmpJet_, Jet_genpt[ijet]) );
        }

        thettbarselector_.met_ = ttbar_metpt;

        //--------------------------------------------//
        //-------------pile-up reweighting------------//
        //--------------------------------------------//
        Float_t puWgtLo(1.0), puWgtNom(1.0), puWgtHi(1.0);
        if(!isData)
        {
            // Hot patch to be fixed for future high PU situation!
            if(puWgtGr_)     puWgtNom = nPUtrue < 95 && nPUtrue >= 0 ? puWgtGr_->Eval(nPUtrue) : 0;
            if(puWgtDownGr_) puWgtLo  = nPUtrue < 95 && nPUtrue >= 0 ? puWgtDownGr_->Eval(nPUtrue) : 0;
            if(puWgtUpGr_)   puWgtHi  = nPUtrue < 95 && nPUtrue >= 0 ? puWgtUpGr_->Eval(nPUtrue) : 0;
        }


        // to compute the event weight but no the event selection (for PU reweighting)
        bool computeEvtWgtOnly = true;
        bool computeWeights =  thettbarselector_.passTTbarSelection( isData, theLeptColl, theLeptIds, theJetColl, ttbar_trigWord, ttbar_w, ttbar_nw, wgtcounter, syst, computeEvtWgtOnly);

        // if something goes wrong in the event weight calculation
        if(!computeWeights)  continue;

        // compute the evt weight
        if(!isData) ww = thettbarselector_.evWgt*puWgtNom;

        if(     syst == "PU__minus") ww *= puWgtLo/puWgtNom;
        else if(syst == "PU__plus" ) ww *= puWgtHi/puWgtNom;

        //if(output_name == "output_ttbar") cout << "evtwgt = " << ww << endl;

        // To rescale PU reweighting with ratio of sum of weights (at the end)
        if(!isData)
        {
            sumWeightWoPUreweighting        += thettbarselector_.evWgt;
            sumWeightWithPUreweighting      += thettbarselector_.evWgt*puWgtNom;
            sumWeightWithPUreweightingUp    += thettbarselector_.evWgt*puWgtLo;
            sumWeightWithPUreweightingDown  += thettbarselector_.evWgt*puWgtHi;
        }

        computeEvtWgtOnly = false;

        // do the event selection
        bool passsel = false;
        if(produceCTagTree){
            passsel =  thettbarselector_.passSemiLepTTbarSelection( isData, theLeptColl, theLeptIds, theJetColl, ttbar_trigWord, ttbar_w, ttbar_nw, wgtcounter, syst, computeEvtWgtOnly);
        } else{
            passsel = thettbarselector_.passTTbarSelection( isData, theLeptColl, theLeptIds, theJetColl, ttbar_trigWord, ttbar_w, ttbar_nw, wgtcounter, syst, computeEvtWgtOnly);
        }
        if(!passsel)  continue;

        bool fillCommissioningHistograms = false;
        // fill commissioning histograms only if running over nominal samples
        if(syst == "" && thettbarselector_.theSelJetColl.size() >= 2) fillCommissioningHistograms = true;


        //-----------------------------------
        // Counter of events
        //-----------------------------------
        Nevent++;


        //-----------------------------------
        // Fill control plot
        //-----------------------------------
        if(fillCommissioningHistograms)
        {//{{{
            if(isData)
            {
                N_event_data_after_sel++;
                nPU_data       ->Fill(nPV);
            }
            else
            {
                N_event_mc_after_sel++;
                nPU_mc         ->Fill(nPUtrue,ww);
                pt_hat         ->Fill(pthat,ww);
                nPV_mc         ->Fill(nPV,ww);
            }
            if(!produceCTagTree){
                FillHistoTTbar("nPV",                nPV                                ,ww);
                FillHistoTTbar("nPV_unweighted",     nPV                                 ,1);
                FillHistoTTbar("met",              thettbarselector_.met_             ,ww);
                FillHistoTTbar("mll",              thettbarselector_.mll_             ,ww);
                FillHistoTTbar("pt_e",             thettbarselector_.lept1_.Pt()      ,ww);
                FillHistoTTbar("pt_mu",            thettbarselector_.lept2_.Pt()      ,ww);
            }
        }//}}}


        int nbjet_ttbar_CSVv2_TWP =0;
        int nbjet_ttbar_CSVv2_MWP =0;
        int nbjet_ttbar_CSVv2_LWP =0;
        int nbjet_ttbar_cMVAv2_TWP =0;
        int nbjet_ttbar_cMVAv2_MWP =0;
        int nbjet_ttbar_cMVAv2_LWP =0;
        int nbjet_ttbar_DeepCSV_TWP=0;
        int nbjet_ttbar_DeepCSV_MWP=0;
        int nbjet_ttbar_DeepCSV_LWP=0;
        int nbjet_ttbar_DeepFlavourBDisc_TWP=0;
        int nbjet_ttbar_DeepFlavourBDisc_MWP=0;
        int nbjet_ttbar_DeepFlavourBDisc_LWP=0;
        float ptjet_ttbar=0;

        int nbjet_afterJetSel_ttbar_CSVv2_TWP =0;
        int nbjet_afterJetSel_ttbar_CSVv2_MWP =0;
        int nbjet_afterJetSel_ttbar_CSVv2_LWP =0;
        int nbjet_afterJetSel_ttbar_cMVAv2_TWP =0;
        int nbjet_afterJetSel_ttbar_cMVAv2_MWP =0;
        int nbjet_afterJetSel_ttbar_cMVAv2_LWP =0;
        int nbjet_afterJetSel_ttbar_DeepCSVb_TWP=0;
        int nbjet_afterJetSel_ttbar_DeepCSVb_MWP=0;
        int nbjet_afterJetSel_ttbar_DeepCSVb_LWP=0;
        int nbjet_afterJetSel_ttbar_DeepFlavourBDisc_TWP=0;
        int nbjet_afterJetSel_ttbar_DeepFlavourBDisc_MWP=0;
        int nbjet_afterJetSel_ttbar_DeepFlavourBDisc_LWP=0;

        int n_ttbar_bgenjet =0;
        int n_ttbar_cgenjet =0;
        int n_ttbar_lgenjet =0;
        int n_ttbar_applySF =0;

        bool isTTbarSelForSF    = false;
        bool isLowerThan60      = false;
        bool isBetween60and120  = false;
        bool isBetween120and320 = false;

        //-----------------------------------
        //Loop on jets
        //-----------------------------------
        int nJets_pt30=0;
        for (unsigned int ijet = 0; ijet < thettbarselector_.theSelJetColl.size(); ijet++)
        {//{{{

            int newJetIndex = thettbarselector_.theSelJetColl[ijet].second;

            float ptjet   = Jet_pt[newJetIndex];
            float etajet  = Jet_eta[newJetIndex];
            float phijet  = Jet_phi[newJetIndex];
            float ntrkjet  = Jet_ntracks[newJetIndex];
            int   flav     = Jet_flavour[newJetIndex];

            if( ptjet >= 30 ) nJets_pt30++;

            //fill info for ttbar SF
            if(thettbarselector_.theSelJetColl.size() == 2)
            {
                if( thettbarselector_.theSelJetColl[0].first.Pt() >= 30 && thettbarselector_.theSelJetColl[1].first.Pt() >= 30 )
                {
                    isTTbarSelForSF = true;

                    // pT splitting
                    if( thettbarselector_.theSelJetColl[0].first.Pt() < 60 && thettbarselector_.theSelJetColl[1].first.Pt() < 60)        isLowerThan60=true;
                    else if( thettbarselector_.theSelJetColl[0].first.Pt() < 120 && thettbarselector_.theSelJetColl[1].first.Pt() < 120) isBetween60and120=true;
                    else if( thettbarselector_.theSelJetColl[0].first.Pt() < 320 && thettbarselector_.theSelJetColl[1].first.Pt() < 320) isBetween120and320=true;

                    if( !isData)
                    {
                        if (fabs(flav) == 5)
                        {
                            n_ttbar_bgenjet++;
                        }
                        else if (fabs(flav) == 4)
                        {
                            n_ttbar_cgenjet++;
                        }
                        else if (fabs(flav) < 4 || fabs(flav) == 21)
                        {
                            n_ttbar_lgenjet++;
                        }
                    }

                    if(!isData && fabs(flav) == 5)
                    {
                        if( theRand_->Uniform(1.) <= 0.8484)  n_ttbar_applySF++; //CSVv2M 80X
                    }
                    else if (Jet_CombIVF[newJetIndex] > 0.8484) n_ttbar_applySF++;  //CSVv2M 80X

                    if (Jet_CombIVF[newJetIndex]  > 0.9535) nbjet_afterJetSel_ttbar_CSVv2_TWP++;    // Tight WP for     80X
                    if (Jet_CombIVF[newJetIndex]  > 0.8484) nbjet_afterJetSel_ttbar_CSVv2_MWP++;    // Medium WP for    80X
                    if (Jet_CombIVF[newJetIndex]  > 0.5426) nbjet_afterJetSel_ttbar_CSVv2_LWP++;    // Loose WP for     80X
                    if (Jet_cMVAv2 [newJetIndex]  > 0.9432) nbjet_afterJetSel_ttbar_cMVAv2_TWP++;   // Tight WP for     80X
                    if (Jet_cMVAv2 [newJetIndex]  > 0.4432) nbjet_afterJetSel_ttbar_cMVAv2_MWP++;   // Medium WP for    80X
                    if (Jet_cMVAv2 [newJetIndex]  >-0.5884) nbjet_afterJetSel_ttbar_cMVAv2_LWP++;   // Loose WP for     80X
                    if (Jet_DeepCSVb[newJetIndex] > 0.1522) nbjet_afterJetSel_ttbar_DeepCSVb_TWP++; // Tight WP for     94X
                    if (Jet_DeepCSVb[newJetIndex] > 0.4941) nbjet_afterJetSel_ttbar_DeepCSVb_MWP++; // Medium WP for    94X
                    if (Jet_DeepCSVb[newJetIndex] > 0.8001) nbjet_afterJetSel_ttbar_DeepCSVb_LWP++; // Loose WP for     94X
                    if (Jet_DeepFlavourBDisc[newJetIndex] > 0.0574) nbjet_afterJetSel_ttbar_DeepFlavourBDisc_TWP++; // Tight WP for     94X
                    if (Jet_DeepFlavourBDisc[newJetIndex] > 0.4318) nbjet_afterJetSel_ttbar_DeepFlavourBDisc_MWP++; // Medium WP for    94X
                    if (Jet_DeepFlavourBDisc[newJetIndex] > 0.9068) nbjet_afterJetSel_ttbar_DeepFlavourBDisc_LWP++; // Loose WP for     94X

                }
            }

            if(!produceCTagTree){
                if (nTrack>1000) cout << " data nTrack " << nTrack << endl;
            }

            if (ptjet>ptjet_ttbar) ptjet_ttbar=ptjet;

            int n_sv             = 0.;
            float mass_sv        =-1.;
            float chi2norm_sv    =-1.;
            float flightSig_sv   =-1.;
            float flight2DSig_sv =-1.;
            float sv_dR_jet      =-1.;
            float sv_dR_dir_sum  =-1.;
            float sv_dR_jet_sum  =-1.;
            float sv_en_rat      =-1.;
            float sv_pt      =-1.;
            float sveta         =-1000.;
            float svphi         =-1000.;
            float sv_flight3D    =-1.;
            float sv_flight3Derr =-1.;
            float sv_flight2D    =-1.;
            float sv_flight2Derr =-1.;
            int   sv_totchar     =-1.;
            float sv_nTrk        = 0.;


            float tche     = Jet_Ip2P[newJetIndex];
            float tchp     = Jet_Ip3P[newJetIndex];
            float jetproba = Jet_ProbaP[newJetIndex];
            float jetbproba= Jet_BprobP[newJetIndex];
            float ssvhe    = Jet_Svx[newJetIndex] ;
            float ssvhp    = Jet_SvxHP[newJetIndex];
            float csv      = Jet_CombSvx[newJetIndex];
            float csv_v2   = Jet_CombIVF[newJetIndex];
            float deepcsvB = Jet_DeepCSVb[newJetIndex];
            float deepcsvBB= Jet_DeepCSVbb[newJetIndex];
            float deepflavourBDisc  = Jet_DeepFlavourBDisc[newJetIndex];
            float cmva_v2  = Jet_cMVAv2[newJetIndex];
            float cvsB     = CTag_Jet_CvsB[newJetIndex];
            float cvsL     = CTag_Jet_CvsL[newJetIndex];

            float CvsB     = CTag_Jet_CvsB[newJetIndex];
            float CvsBN    = CTag_Jet_CvsBN[newJetIndex];
            float CvsBP    = CTag_Jet_CvsBP[newJetIndex];
            float CvsL     = CTag_Jet_CvsL[newJetIndex];
            float CvsLN    = CTag_Jet_CvsLN[newJetIndex];
            float CvsLP    = CTag_Jet_CvsLP[newJetIndex];

            bool isPU=false;

            if( fillCommissioningHistograms)
            {
                if(!isData && Jet_genpt[newJetIndex] <= 8) isPU = true;

                //fill jet multiplicity
                if (!isData)
                {
                    if      (fabs(flav)==5)                  njet_b++;
                    else if (fabs(flav)==4)                  njet_c++;
                    else if (fabs(flav)<4 || fabs(flav)==21) njet_l++;
                    njet_mc++;
                    jet_pt_mc   ->Fill(ptjet,ww);
                }
                else njet_data++;

                if (csv_v2>0.9535  ) nbjet_ttbar_CSVv2_TWP++;   // Tight WP for  80X
                if (csv_v2>0.8484  ) nbjet_ttbar_CSVv2_MWP++;   // Medium WP for 80X
                if (csv_v2>0.5426  ) nbjet_ttbar_CSVv2_LWP++;   // Loose WP for  80X
                if (deepcsvB>0.8001) nbjet_ttbar_DeepCSV_TWP++; // Tight WP for  94X
                if (deepcsvB>0.4941) nbjet_ttbar_DeepCSV_MWP++; // Medium WP for 94X
                if (deepcsvB>0.1522) nbjet_ttbar_DeepCSV_LWP++; // Loose WP for  94X
                if (deepflavourBDisc >0.9068) nbjet_ttbar_DeepFlavourBDisc_TWP++; // Tight WP for  94X
                if (deepflavourBDisc >0.4318) nbjet_ttbar_DeepFlavourBDisc_MWP++; // Medium WP for 94X
                if (deepflavourBDisc >0.0574) nbjet_ttbar_DeepFlavourBDisc_LWP++; // Loose WP for  94X
                if (cmva_v2> 0.9432) nbjet_ttbar_cMVAv2_TWP++;  // Tight WP for  80X
                if (cmva_v2> 0.4432) nbjet_ttbar_cMVAv2_MWP++;  // Medium WP for 80X
                if (cmva_v2>-0.5884) nbjet_ttbar_cMVAv2_LWP++;  // Loose WP for  80X

                //if(output_name == "output_dy1") cout << "csvv2 = " << csv_v2 << "ptjet= " << ptjet << endl;

                FillHistoBtag("jet_multi",                  flav, isPU ,thettbarselector_.theSelJetColl.size() , ww);
                FillHistoBtag("jet_pt_all",                 flav, isPU ,ptjet                                  , ww);
                FillHistoBtag("genjet_pt_all",              flav, isPU ,Jet_genpt[newJetIndex]                 , ww);

                if(isPU) FillHistoBtag("CSVv2pu",          flav, isPU ,csv_v2                                 , ww);
                if(isPU) FillHistoBtag("DeepCSVbpu",       flav, isPU ,deepcsvB                               , ww);
                if(isPU) FillHistoBtag("DeepFlavourBDiscpu",       flav, isPU ,deepflavourBDisc               , ww);
                if(!produceCTagTree){
                    if (nSV > 0)FillHistoBtag("jet_pt_sv",      flav, isPU ,ptjet                                  , ww);
                }
                FillHistoBtag("jet_eta",     flav, isPU ,etajet   , ww);
                FillHistoBtag("jet_phi",     flav, isPU ,phijet   , ww);
                FillHistoBtag(  "track_multi", flav, isPU ,ntrkjet  , ww);

            }

            int n1_ip=-1;
            int n2_ip=-1;
            int n3_ip=-1;
            float sig1_ip=-9999;
            float sig2_ip=-9999;
            float sig3_ip=-9999;
            float sig12D_ip=-9999;
            float sig22D_ip=-9999;
            float sig32D_ip=-9999;

            int ntracksel  =0;


            if ( fillCommissioningHistograms && produceJetProbaTree )
            {

                for (int itrk=Jet_nFirstTrack[newJetIndex]; itrk<Jet_nLastTrack[newJetIndex] ; itrk++)
                {

                    //-------------------------//
                    //-----Track selection-----//
                    //-------------------------//
                    passNhit=false;
                    passPix= false;
                    passIPz=false;
                    passPt=false;
                    passnormchi2=false;
                    passtrkdist=false;
                    passtrklen=false;
                    passTrackIP2D=false;

                    //if (Track_nHitAll[itrk]>=8)           passNhit=true;
                    //if (Track_nHitPixel[itrk]>=2)         passPix= true;
                    if (Track_nHitAll[itrk]>=0)           passNhit=true;     // HIP mitigation
                    if (Track_nHitPixel[itrk]>=1)         passPix= true;     // HIP mitigation
                    if (fabs(Track_dz[itrk])<17)          passIPz=true;
                    if (Track_pt[itrk]>1)                 passPt=true;
                    if (Track_chi2[itrk]<5)               passnormchi2=true;
                    if (fabs(Track_dist[itrk])<0.07)      passtrkdist=true;
                    if (Track_length[itrk]<5)             passtrklen=true;
                    if (fabs(Track_IP2D[itrk])<0.2)       passTrackIP2D=true;

                    if (!use_selected_tracks)
                    {

                        if (passNhit && passPix && passIPz && passPt && passnormchi2 && passtrkdist && passTrackIP2D)
                        {
                            FillHistoBtag("track_len_cut",   flav, isPU ,Track_length[itrk]          , ww);
                        }
                        if (passNhit && passPix && passIPz && passPt && passnormchi2 && passtrklen && passTrackIP2D)
                        {
                            FillHistoBtag("track_dist_cut",  flav, isPU ,fabs(Track_dist[itrk])      , ww);
                        }
                        if (passNhit && passPix && passIPz && passPt && passtrkdist && passtrklen && passTrackIP2D)
                        {
                            FillHistoBtag("track_chi2_cut",  flav, isPU ,Track_chi2[itrk]         , ww);
                        }
                        if (passNhit && passPix && passIPz && passnormchi2 && passtrkdist && passtrklen && passTrackIP2D)
                        {
                            FillHistoBtag("track_pt_cut",    flav, isPU ,Track_pt[itrk]              , ww);
                        }
                        if (passNhit && passPix && passPt && passnormchi2 && passtrkdist && passtrklen){

                            FillHistoBtag("track_dz_cut",    flav, isPU ,Track_dz[itrk]              , ww);
                        }
                        if (passNhit && passIPz && passPt && passnormchi2 && passtrkdist && passtrklen && passTrackIP2D)
                        {
                            FillHistoBtag(  "track_HPix_cut",  flav, isPU ,Track_nHitPixel[itrk]       , ww);
                        }
                        if (passPix && passIPz && passPt && passnormchi2 && passtrkdist && passtrklen && passTrackIP2D)
                        {
                            FillHistoBtag(  "track_nHit_cut",  flav, isPU ,Track_nHitAll[itrk]         , ww);
                        }
                        if (passNhit && passPix && passIPz && passPt && passnormchi2 && passtrkdist && passtrklen )
                        {
                            FillHistoBtag(  "track_IP2D_cut",  flav, isPU ,Track_IP2D[itrk]            , ww);
                        }
                    }

                    if (passNhit && passPix && passIPz && passPt && passnormchi2 && passtrkdist && passtrklen && passTrackIP2D)
                    {
                        ntracksel++;
                        FillHistoBtag("track_chi2",    flav, isPU ,Track_chi2[itrk] ,ww);
                        FillHistoBtag(  "track_nHit",    flav, isPU ,Track_nHitAll[itrk]     ,ww);
                        FillHistoBtag(  "track_HPix",    flav, isPU ,Track_nHitPixel[itrk]   ,ww);
                        FillHistoBtag("track_IPs",     flav, isPU ,Track_IPsig[itrk]       ,ww);
                        FillHistoBtag("track_IP",      flav, isPU ,Track_IP[itrk]          ,ww);
                        FillHistoBtag("track_IP2Ds",   flav, isPU ,Track_IP2Dsig[itrk]     ,ww);
                        FillHistoBtag("track_IP2D",    flav, isPU ,Track_IP2D[itrk]        ,ww);
                        FillHistoBtag("track_IP2Derr", flav, isPU ,Track_IP2Derr[itrk]     ,ww);
                        FillHistoBtag("track_IPerr",   flav, isPU ,Track_IPerr[itrk]       ,ww);
                        FillHistoBtag("track_dz",      flav, isPU ,Track_dz[itrk]          ,ww);
                        FillHistoBtag(  "track_isfromSV",flav, isPU ,Track_isfromSV[itrk]    ,ww);
                        FillHistoBtag("track_len",     flav, isPU ,Track_length[itrk]      ,ww);
                        FillHistoBtag("track_dist",    flav, isPU ,fabs(Track_dist[itrk])  ,ww);
                        FillHistoBtag("track_pt",      flav, isPU ,Track_pt[itrk]          ,ww);


                        //Tracks sorted by IP
                        Float_t sig  =Track_IP[itrk]/Track_IPerr[itrk];
                        Float_t sig2D=Track_IP2D[itrk]/Track_IP2Derr[itrk];
                        if (sig>sig1_ip)
                        {
                            sig3_ip=sig2_ip;
                            sig2_ip=sig1_ip;
                            sig1_ip=sig;
                            sig32D_ip=sig22D_ip;
                            sig22D_ip=sig12D_ip;
                            sig12D_ip=sig2D;
                            n3_ip=n2_ip;
                            n2_ip=n1_ip;
                            n1_ip=itrk;
                        }
                        else if (sig>sig2_ip)
                        {
                            sig3_ip=sig2_ip;
                            sig2_ip=sig;
                            sig32D_ip=sig22D_ip;
                            sig22D_ip=sig2D;
                            n3_ip=n2_ip;
                            n2_ip=itrk;
                        }
                        else if (sig>sig3_ip)
                        {
                            sig3_ip=sig;
                            sig32D_ip=sig2D;
                            n3_ip=itrk;
                        }

                    }//end selected tracks

                }//end tracks loop


                if (n1_ip>-1)
                {
                    FillHistoBtag("track_IPs1tr",    flav, isPU ,sig1_ip               , ww);
                    FillHistoBtag("track_IP1tr",     flav, isPU ,Track_IP[n1_ip]       , ww);
                    FillHistoBtag("track_IPerr1tr",  flav, isPU ,Track_IPerr[n1_ip]    , ww);
                    FillHistoBtag("track_IP2Ds1tr",  flav, isPU ,sig12D_ip             , ww);
                    FillHistoBtag("track_IP2D1tr",   flav, isPU ,Track_IP2D[n1_ip]     , ww);
                    FillHistoBtag("track_IP2Derr1tr",flav, isPU ,Track_IP2Derr[n1_ip]  , ww);
                }

                if (n2_ip>-1)
                {
                    FillHistoBtag("track_IPs2tr",    flav, isPU ,sig2_ip               , ww);
                    FillHistoBtag("track_IP2tr",     flav, isPU ,Track_IP[n2_ip]       , ww);
                    FillHistoBtag("track_IPerr2tr",  flav, isPU ,Track_IPerr[n2_ip]    , ww);
                    FillHistoBtag("track_IP2Ds2tr",  flav, isPU ,sig22D_ip             , ww);
                    FillHistoBtag("track_IP2D2tr",   flav, isPU ,Track_IP2D[n2_ip]     , ww);
                    FillHistoBtag("track_IP2Derr2tr",flav, isPU ,Track_IP2Derr[n2_ip]  , ww);
                }

                if (n3_ip>-1)
                {
                    FillHistoBtag("track_IPs3tr",    flav, isPU ,sig3_ip               , ww);
                    FillHistoBtag("track_IP3tr",     flav, isPU ,Track_IP[n3_ip]       , ww);
                    FillHistoBtag("track_IPerr3tr",  flav, isPU ,Track_IPerr[n3_ip]    , ww);
                    FillHistoBtag("track_IP2Ds3tr",  flav, isPU ,sig32D_ip             , ww);
                    FillHistoBtag("track_IP2D3tr",   flav, isPU ,Track_IP2D[n3_ip]     , ww);
                    FillHistoBtag("track_IP2Derr3tr",flav, isPU ,Track_IP2Derr[n3_ip]  , ww);
                }

                FillHistoBtag(        "trk_multi_sel",     flav, isPU ,ntracksel             , ww);
                FillHisto2D("seltrack_vs_jetpt", flav, isPU ,ptjet ,  ntracksel , ww);


                FillHistoBtag("tagvarCSV_vertexCategory",      flav, isPU, TagVarCSV_vertexCategory[newJetIndex],              ww);
                FillHistoBtag("tagvarCSV_Sig2dAboveCharm",     flav, isPU, TagVarCSV_trackSip2dSigAboveCharm[newJetIndex],     ww);
                FillHistoBtag("tagvarCSV_trackSumJetEtRatio",  flav, isPU, TagVarCSV_trackSumJetEtRatio[newJetIndex],          ww);
                FillHistoBtag("tagvarCSV_trackSumJetDeltaR",   flav, isPU, TagVarCSV_trackSumJetDeltaR[newJetIndex],           ww);

                for (int inrel=Jet_nFirstTrkEtaRelTagVarCSV[newJetIndex]; inrel<Jet_nLastTrkEtaRelTagVarCSV[newJetIndex]; inrel++)
                {
                    FillHistoBtag("tagvarCSV_trackEtaRel",      flav, isPU, TagVarCSV_trackEtaRel[inrel],                       ww);
                }

                FillHistoBtag("tagvarCSV_energyratio",         flav, isPU, TagVarCSV_vertexEnergyRatio[newJetIndex],           ww);

                for (int inrel=Jet_nFirstTrkTagVarCSV[newJetIndex]; inrel<Jet_nLastTrkTagVarCSV[newJetIndex]; inrel++)
                {
                    FillHistoBtag("tagvarCSV_trackSip3dSig",    flav, isPU, TagVarCSV_trackSip3dSig[inrel],                     ww);
                }
                if (TagVarCSV_vertexCategory[newJetIndex]==0)
                {
                    FillHistoBtag("tagvarCSV_vertexmass_cat0",   flav, isPU, TagVarCSV_vertexMass[newJetIndex],                  ww);
                    if (TagVarCSV_vertexNTracks[newJetIndex]>=3)
                    {
                        FillHistoBtag("tagvarCSV_vertexmass3trk_cat0",     flav, isPU, TagVarCSV_vertexMass[newJetIndex],      ww);
                    }
                    FillHistoBtag("tagvarCSV_vertexNTracks_cat0",   flav, isPU, TagVarCSV_vertexNTracks[newJetIndex],            ww);
                    FillHistoBtag("tagvarCSV_2DsigFlightDist_cat0", flav, isPU, TagVarCSV_flightDistance2dSig[newJetIndex],      ww);
                    FillHistoBtag("tagvarCSV_vertexJetDeltaR_cat0", flav, isPU, TagVarCSV_vertexJetDeltaR[newJetIndex],          ww);
                }


                //---------------------------------
                //fill information related to SV
                //---------------------------------
                n_sv           = Jet_SV_multi[newJetIndex];
                FillHistoBtag(  "sv_multi_0",      flav, isPU ,n_sv      ,ww);

                if (n_sv>0){
                    chi2norm_sv    = SV_chi2[Jet_nFirstSV[newJetIndex]]/SV_ndf[Jet_nFirstSV[newJetIndex]];
                    flightSig_sv   = SV_flight[Jet_nFirstSV[newJetIndex]]/SV_flightErr[Jet_nFirstSV[newJetIndex]];
                    flight2DSig_sv = SV_flight2D[Jet_nFirstSV[newJetIndex]]/SV_flight2DErr[Jet_nFirstSV[newJetIndex]];
                    mass_sv        = SV_mass[Jet_nFirstSV[newJetIndex]];
                    sv_dR_jet      = SV_deltaR_jet[Jet_nFirstSV[newJetIndex]];
                    sv_dR_dir_sum  = SV_deltaR_sum_dir[Jet_nFirstSV[newJetIndex]];
                    sv_dR_jet_sum  = SV_deltaR_sum_jet[Jet_nFirstSV[newJetIndex]];
                    sv_en_rat      = SV_EnergyRatio[Jet_nFirstSV[newJetIndex]];
                    sv_pt          = SV_vtx_pt[Jet_nFirstSV[newJetIndex]];
                    sveta          = SV_vtx_eta[Jet_nFirstSV[newJetIndex]];
                    svphi          = SV_vtx_phi[Jet_nFirstSV[newJetIndex]];
                    sv_flight3D    = SV_flight[Jet_nFirstSV[newJetIndex]] ;
                    sv_flight3Derr = SV_flightErr[Jet_nFirstSV[newJetIndex]];
                    sv_flight2D    = SV_flight2D[Jet_nFirstSV[newJetIndex]];
                    sv_flight2Derr = SV_flight2DErr[Jet_nFirstSV[newJetIndex]];
                    sv_totchar     = SV_totCharge[Jet_nFirstSV[newJetIndex]] ;
                    sv_nTrk        = SV_nTrk[Jet_nFirstSV[newJetIndex]] ;

                    //-------------------------//
                    //-----SV histograms-------//
                    //-------------------------//
                    FillHistoBtag(  "sv_multi",        flav, isPU ,n_sv               , ww);
                    FillHistoBtag("sv_chi2norm",     flav, isPU ,chi2norm_sv        , ww);
                    FillHistoBtag("sv_mass",         flav, isPU ,mass_sv            , ww);
                    FillHistoBtag("sv_deltaR_jet",   flav, isPU ,sv_dR_jet          , ww);
                    FillHistoBtag("sv_deltaR_sumJet",flav, isPU ,sv_dR_dir_sum      , ww);
                    FillHistoBtag("sv_deltaR_sumDir",flav, isPU ,sv_dR_jet_sum      , ww);
                    FillHistoBtag("sv_en_ratio",     flav, isPU ,sv_en_rat          , ww);
                    FillHistoBtag("sv_pt",           flav, isPU ,sv_pt              , ww);
                    FillHistoBtag("sv_flight2D",     flav, isPU ,sv_flight2D        , ww);
                    FillHistoBtag("sv_flight2Derr",  flav, isPU ,sv_flight2Derr     , ww);
                    FillHistoBtag("sv_flightSig2D",  flav, isPU ,flight2DSig_sv     , ww);
                    FillHistoBtag("sv_tot_charge",     flav, isPU ,sv_totchar         , ww);
                    FillHistoBtag(  "svnTrk",          flav, isPU ,sv_nTrk            , ww);
                    FillHistoBtag("sv_eta",          flav, isPU ,sveta              , ww);
                    FillHistoBtag("sv_phi",          flav, isPU ,svphi              , ww);
                    FillHistoBtag("sv_flight3D",     flav, isPU ,sv_flight3D        , ww);
                    FillHistoBtag("sv_flight3Derr",  flav, isPU ,sv_flight3Derr     , ww);
                    FillHistoBtag("sv_flight3DSig",  flav, isPU ,flightSig_sv       , ww);

                    if (sv_nTrk >2)FillHistoBtag("sv_mass_3trk", flav, isPU ,mass_sv, ww);

                    FillHisto2D("sv_mass_vs_flightDist3D"     ,flav,isPU ,sv_flight3D,mass_sv  , ww);
                    FillHisto2D("avg_sv_mass_vs_jetpt"        ,flav,isPU ,ptjet,mass_sv        , ww);
                    FillHisto2D("sv_deltar_jet_vs_jetpt"      ,flav,isPU ,ptjet,sv_dR_jet      , ww);
                    FillHisto2D("sv_deltar_sum_jet_vs_jetpt"  ,flav,isPU ,ptjet,sv_dR_dir_sum  , ww);
                    FillHisto2D("sv_deltar_sum_dir_vs_jetpt"  ,flav,isPU ,ptjet,sv_dR_jet_sum  , ww);

                }//end n_sv > 0 condition

            }//end produce jetProbaTree and fillCommissioningHistograms


            if( fillCommissioningHistograms)
            {

                //Taggers
                FillHistoBtag("TCHE",  flav, isPU, tche , ww);
                FillHistoBtag("TCHP",  flav, isPU, tchp , ww);
                FillHistoBtag("JP",    flav, isPU, jetproba    , ww);
                FillHistoBtag("JBP",   flav, isPU, jetbproba   , ww);
                FillHistoBtag("SSV",   flav, isPU, ssvhe    , ww);
                FillHistoBtag("SSVHP", flav, isPU, ssvhp    , ww);
                FillHistoBtag("CSV",   flav, isPU, csv          , ww);
                FillHistoBtag("CSVv2", flav, isPU, csv_v2      , ww);
                FillHistoBtag("DeepCSVb", flav, isPU, deepcsvB      , ww);
                FillHistoBtag("DeepCSVbb", flav, isPU, deepcsvBB      , ww);
                FillHistoBtag("DeepCSVBDisc", flav, isPU, deepcsvB+deepcsvBB      , ww);
                FillHistoBtag("DeepFlavourBDisc", flav, isPU, deepflavourBDisc      , ww);
                FillHistoBtag("cMVAv2",flav, isPU, cmva_v2     , ww);
                FillHistoBtag("CvsB",  flav, isPU, cvsB        , ww);
                FillHistoBtag("CvsL",  flav, isPU, cvsL        , ww);
            }

            if (fillCommissioningHistograms && produceNewAlgoTree)
            {
                float softmu            = Jet_SoftMu[newJetIndex]  ;
                float solfel            = Jet_SoftEl[newJetIndex];

                FillHistoBtag("SoftMu",      flav, isPU, softmu        , ww);
                FillHistoBtag("SoftEl",      flav, isPU, solfel        , ww);

            }

            if( fillCommissioningHistograms)
            {
                //Taggers
                FillHistoBtag("TCHE_extended1",  flav, isPU, tche  , ww);
                FillHistoBtag("TCHP_extended1",  flav, isPU, tchp  , ww);
                FillHistoBtag("TCHE_extended2",  flav, isPU, tche  , ww);
                FillHistoBtag("TCHP_extended2",  flav, isPU, tchp  , ww);
                FillHistoBtag("discri_ssche0",   flav, isPU, ssvhe , ww);
                FillHistoBtag("discri_sschp0",   flav, isPU, ssvhp , ww);

            } // end of fillCommissioningHistograms


            if (fillCommissioningHistograms && produceNewAlgoTree)
            {
                // PFMuon
                int npfmu=0;
                int indpfmu=-1;

                TLorentzVector thejet;
                thejet.SetPtEtaPhiM(ptjet, etajet, phijet, 0);

                float minpf=0;
                for (int im=0; im<nPFMuon; im++)
                {
                    TLorentzVector thepfmu;
                    thepfmu.SetPtEtaPhiM(PFMuon_pt[im],PFMuon_eta[im],PFMuon_phi[im],0);
                    float drpfj=thepfmu.DeltaR(thejet);
                    float diffdr = drpfj-PFMuon_deltaR[im];
                    if (diffdr<0.) diffdr*=-1.;
                    if (drpfj< 0.5 && diffdr<0.05 && PFMuon_GoodQuality[im]>0)
                    {
                        if (PFMuon_pt[im]> minpf)
                        {
                            indpfmu=im;
                            minpf=PFMuon_pt[im];
                        }
                        npfmu++;
                    }
                }


                FillHistoBtag(  "pfmuon_multi",  flav, isPU , npfmu   ,ww);

                if (indpfmu>-1)
                {
                    FillHistoBtag(  "pfmuon_goodquality", flav, isPU, PFMuon_GoodQuality[indpfmu]       , ww);
                    FillHistoBtag("pfmuon_pt",          flav, isPU, PFMuon_pt[indpfmu]                , ww);
                    FillHistoBtag("pfmuon_eta",         flav, isPU, PFMuon_eta[indpfmu]               , ww);
                    FillHistoBtag("pfmuon_phi",         flav, isPU, PFMuon_phi[indpfmu]               , ww);
                    FillHistoBtag("pfmuon_Sip",         flav, isPU, PFMuon_IPsig[indpfmu]             , ww);
                    FillHistoBtag("pfmuon_ptrel",       flav, isPU, PFMuon_ptrel[indpfmu]             , ww);
                    FillHistoBtag("pfmuon_ratio",       flav, isPU, PFMuon_ratio[indpfmu]             , ww);
                    FillHistoBtag("pfmuon_ratiorel",    flav, isPU, PFMuon_ratioRel[indpfmu]          , ww);
                    FillHistoBtag("pfmuon_deltar",      flav, isPU, PFMuon_deltaR[indpfmu]            , ww);
                }

                //PFElectron
                int npfel=0;
                int indpfel=-1;

                minpf=0;
                for (int im=0; im<nPFElectron; im++)
                {
                    TLorentzVector thepfel;
                    thepfel.SetPtEtaPhiM(PFElectron_pt[im],PFElectron_eta[im],PFElectron_phi[im],0);
                    float drpfj=thepfel.DeltaR(thejet);
                    float diffdr = drpfj-PFElectron_deltaR[im];
                    if (diffdr<0.) diffdr*=-1.;
                    if (drpfj< 0.5 && diffdr<0.05 && PFElectron_pt[im]>2.)
                    {
                        if (PFElectron_pt[im]> minpf)
                        {
                            indpfel=im;
                            minpf=PFElectron_pt[im];
                        }
                        npfel++;
                    }
                }


                FillHistoBtag(  "pfelectron_multi",  flav, isPU , npfel   ,ww);

                if (indpfel>-1)
                {
                    FillHistoBtag("pfelectron_pt",        flav, isPU, PFElectron_pt[indpfel]           , ww);
                    FillHistoBtag("pfelectron_eta",       flav, isPU, PFElectron_eta[indpfel]          , ww);
                    FillHistoBtag("pfelectron_phi",       flav, isPU, PFElectron_phi[indpfel]          , ww);
                    FillHistoBtag("pfelectron_ptrel",     flav, isPU, PFElectron_ptrel[indpfel]        , ww);
                    FillHistoBtag("pfelectron_ratio",     flav, isPU, PFElectron_ratio[indpfel]        , ww);
                    FillHistoBtag("pfelectron_ratiorel",  flav, isPU, PFElectron_ratioRel[indpfel]     , ww);
                    FillHistoBtag("pfelectron_deltar",    flav, isPU, PFElectron_deltaR[indpfel]       , ww);
                }


            }// end produceNewAlgoTree and fillCommissioningHistograms

            if ( fillCommissioningHistograms && produceCTagTree ){
                FillHistoBtag("CTag_tagvarCSV_vertexCategory",      flav, isPU, TagVarCSV_vertexCategory[newJetIndex],              ww);
                FillHistoBtag("CTag_tagvarCSV_Sig2dAboveCharm",     flav, isPU, TagVarCSV_trackSip2dSigAboveCharm[newJetIndex],     ww);
                FillHistoBtag("CTag_tagvarCSV_trackSumJetEtRatio",  flav, isPU, TagVarCSV_trackSumJetEtRatio[newJetIndex],          ww);
                FillHistoBtag("CTag_tagvarCSV_trackSumJetDeltaR",   flav, isPU, TagVarCSV_trackSumJetDeltaR[newJetIndex],           ww);

                for (int inrel=Jet_nFirstTrkEtaRelTagVarCSV[newJetIndex]; inrel<Jet_nLastTrkEtaRelTagVarCSV[newJetIndex]; inrel++)
                {
                    FillHistoBtag("CTag_tagvarCSV_trackEtaRel",      flav, isPU, TagVarCSV_trackEtaRel[inrel],                       ww);
                }

                FillHistoBtag("CTag_tagvarCSV_energyratio",         flav, isPU, TagVarCSV_vertexEnergyRatio[newJetIndex],           ww);

                for (int inrel=Jet_nFirstTrkTagVarCSV[newJetIndex]; inrel<Jet_nLastTrkTagVarCSV[newJetIndex]; inrel++)
                {
                    FillHistoBtag("CTag_tagvarCSV_trackSip3dSig",    flav, isPU, TagVarCSV_trackSip3dSig[inrel],                     ww);
                }
                if (TagVarCSV_vertexCategory[newJetIndex]==0)
                {
                    FillHistoBtag("CTag_tagvarCSV_vertexmass_cat0",   flav, isPU, TagVarCSV_vertexMass[newJetIndex],                  ww);
                    if (TagVarCSV_vertexNTracks[newJetIndex]>=3)
                    {
                        FillHistoBtag("CTag_tagvarCSV_vertexmass3trk_cat0",     flav, isPU, TagVarCSV_vertexMass[newJetIndex],      ww);
                    }
                    FillHistoBtag("CTag_tagvarCSV_vertexNTracks_cat0",   flav, isPU, TagVarCSV_vertexNTracks[newJetIndex],            ww);
                    FillHistoBtag("CTag_tagvarCSV_2DsigFlightDist_cat0", flav, isPU, TagVarCSV_flightDistance2dSig[newJetIndex],      ww);
                    FillHistoBtag("CTag_tagvarCSV_vertexJetDeltaR_cat0", flav, isPU, TagVarCSV_vertexJetDeltaR[newJetIndex],          ww);
                }

                FillHistoBtag("CTag_jetNTracks",               flav, isPU ,CTag_jetNTracks[newJetIndex],              ww);
                FillHistoBtag("CTag_jetNTracksEtaRel",         flav, isPU ,CTag_jetNTracksEtaRel[newJetIndex],              ww);
                FillHistoBtag("CTag_jetNLeptons",              flav, isPU ,CTag_jetNLeptons[newJetIndex],              ww);
                FillHistoBtag("CTag_trackSumJetEtRatio",       flav, isPU ,CTag_trackSumJetEtRatio[newJetIndex],              ww);
                FillHistoBtag("CTag_trackSumJetDeltaR",        flav, isPU ,CTag_trackSumJetDeltaR[newJetIndex],              ww);
                FillHistoBtag("CTag_trackSip2dSigAboveCharm",  flav, isPU ,CTag_trackSip2dSigAboveCharm[newJetIndex],              ww);
                FillHistoBtag("CTag_trackSip3dSigAboveCharm",  flav, isPU ,CTag_trackSip3dSigAboveCharm[newJetIndex],              ww);
                FillHistoBtag("CTag_vertexCategory",           flav, isPU ,CTag_vertexCategory[newJetIndex],              ww);
                FillHistoBtag("CTag_jetNSecondaryVertices",    flav, isPU ,CTag_jetNSecondaryVertices[newJetIndex],              ww);
                FillHistoBtag("CTag_vertexMass",               flav, isPU ,CTag_vertexMass[newJetIndex],              ww);
                FillHistoBtag("CTag_vertexNTracks",            flav, isPU ,CTag_vertexNTracks[newJetIndex],              ww);
                FillHistoBtag("CTag_vertexEnergyRatio",        flav, isPU ,CTag_vertexEnergyRatio[newJetIndex],              ww);
                FillHistoBtag("CTag_vertexJetDeltaR",          flav, isPU ,CTag_vertexJetDeltaR[newJetIndex],              ww);
                FillHistoBtag("CTag_flightDistance2dSig",      flav, isPU ,CTag_flightDistance2dSig[newJetIndex],              ww);
                FillHistoBtag("CTag_flightDistance3dSig",      flav, isPU ,CTag_flightDistance3dSig[newJetIndex],              ww);
                FillHistoBtag("CTag_massVertexEnergyFraction", flav, isPU ,CTag_massVertexEnergyFraction[newJetIndex],              ww);
                FillHistoBtag("CTag_vertexBoostOverSqrtJetPt", flav, isPU ,CTag_vertexBoostOverSqrtJetPt[newJetIndex],              ww);
                FillHistoBtag("CTag_vertexLeptonCategory",     flav, isPU ,CTag_vertexLeptonCategory[newJetIndex],              ww);

                if (CTag_vertexCategory[newJetIndex]==0){
                    FillHistoBtag("CTag_jetNSecondaryVertices_Vcat0",    flav, isPU ,CTag_jetNSecondaryVertices[newJetIndex],              ww);
                    FillHistoBtag("CTag_vertexMass_Vcat0",               flav, isPU ,CTag_vertexMass[newJetIndex],              ww);
                    FillHistoBtag("CTag_vertexNTracks_Vcat0",            flav, isPU ,CTag_vertexNTracks[newJetIndex],              ww);
                    FillHistoBtag("CTag_vertexEnergyRatio_Vcat0",        flav, isPU ,CTag_vertexEnergyRatio[newJetIndex],              ww);
                    FillHistoBtag("CTag_vertexJetDeltaR_Vcat0",          flav, isPU ,CTag_vertexJetDeltaR[newJetIndex],              ww);
                    FillHistoBtag("CTag_flightDistance2dSig_Vcat0",      flav, isPU ,CTag_flightDistance2dSig[newJetIndex],              ww);
                    FillHistoBtag("CTag_flightDistance3dSig_Vcat0",      flav, isPU ,CTag_flightDistance3dSig[newJetIndex],              ww);
                    FillHistoBtag("CTag_massVertexEnergyFraction_Vcat0", flav, isPU ,CTag_massVertexEnergyFraction[newJetIndex],              ww);
                    FillHistoBtag("CTag_vertexBoostOverSqrtJetPt_Vcat0", flav, isPU ,CTag_vertexBoostOverSqrtJetPt[newJetIndex],              ww);
                }

                for (int inrel=Jet_nFirstTrkCTagVar[newJetIndex]; inrel<Jet_nLastTrkCTagVar[newJetIndex]; inrel++)
                {
                    FillHistoBtag("CTag_trackPtRel",    flav, isPU, CTag_trackPtRel[inrel],                     ww);
                    FillHistoBtag("CTag_trackPPar",    flav, isPU, CTag_trackPPar[inrel],                     ww);
                    FillHistoBtag("CTag_trackDeltaR",    flav, isPU, CTag_trackDeltaR[inrel],                     ww);
                    FillHistoBtag("CTag_trackPtRatio",    flav, isPU, CTag_trackPtRatio[inrel],                     ww);
                    FillHistoBtag("CTag_trackPParRatio",    flav, isPU, CTag_trackPParRatio[inrel],                     ww);
                    FillHistoBtag("CTag_trackSip2dSig",    flav, isPU, CTag_trackSip2dSig[inrel],                     ww);
                    FillHistoBtag("CTag_trackSip3dSig",    flav, isPU, CTag_trackSip3dSig[inrel],                     ww);
                    FillHistoBtag("CTag_trackDecayLenVal",    flav, isPU, CTag_trackDecayLenVal[inrel],                     ww);
                    FillHistoBtag("CTag_trackJetDistVal",    flav, isPU, CTag_trackJetDistVal[inrel],                     ww);
                }
                for (int inrel=Jet_nFirstTrkEtaRelCTagVar[newJetIndex]; inrel<Jet_nLastTrkEtaRelCTagVar[newJetIndex]; inrel++)
                {
                    FillHistoBtag("CTag_trackEtaRel",      flav, isPU, CTag_trackEtaRel[inrel],                       ww);
                }
                for (int inrel=Jet_nFirstLepCTagVar[newJetIndex]; inrel<Jet_nLastLepCTagVar[newJetIndex]; inrel++)
                {
                    FillHistoBtag("CTag_leptonPtRel",      flav, isPU, CTag_leptonPtRel[inrel],                       ww);
                    FillHistoBtag("CTag_leptonSip3d",      flav, isPU, CTag_leptonSip3d[inrel],                       ww);
                    FillHistoBtag("CTag_leptonDeltaR",      flav, isPU, CTag_leptonDeltaR[inrel],                       ww);
                    FillHistoBtag("CTag_leptonRatioRel",      flav, isPU, CTag_leptonRatioRel[inrel],                       ww);
                    FillHistoBtag("CTag_leptonEtaRel",      flav, isPU, CTag_leptonEtaRel[inrel],                       ww);
                    FillHistoBtag("CTag_leptonRatio",      flav, isPU, CTag_leptonRatio[inrel],                       ww);
                }

                FillHistoBtag("JP",    flav, isPU, jetproba    , ww);
                FillHistoBtag("CSVv2", flav, isPU, csv_v2      , ww);
                FillHistoBtag("DeepCSVb", flav, isPU, deepcsvB , ww);
                FillHistoBtag("DeepCSVbb", flav, isPU, deepcsvBB , ww);
                FillHistoBtag("DeepCSVBDisc", flav, isPU, deepcsvB+deepcsvBB      , ww);
                FillHistoBtag("DeepFlavourBDisc", flav, isPU, deepflavourBDisc      , ww);
                FillHistoBtag("cMVAv2",flav, isPU, cmva_v2     , ww);

                FillHistoBtag("CvsB",  flav, isPU, CvsB        ,ww);
                FillHistoBtag("CvsBN", flav, isPU, CvsBN       ,ww);
                FillHistoBtag("CvsBP", flav, isPU, CvsBP       ,ww);
                FillHistoBtag("CvsL",  flav, isPU, CvsL        ,ww);
                FillHistoBtag("CvsLN", flav, isPU, CvsLN       ,ww);
                FillHistoBtag("CvsLP", flav, isPU, CvsLP       ,ww);

            }// End of fillCommissioningHistograms && produceCTagTree

        }//}}} End Loop on Jets

        if(!produceCTagTree){//{{{
            if( nJets_pt30 >= 2 ) FillHistoTTbar("njet_pt30", nJets_pt30  , ww);

            if(fillCommissioningHistograms)
            {
                FillHistoTTbar("njet",           thettbarselector_.theSelJetColl.size()  , ww);
                FillHistoTTbar("nbtag_CSVv2T",   nbjet_ttbar_CSVv2_TWP                   , ww);
                FillHistoTTbar("nbtag_CSVv2M",   nbjet_ttbar_CSVv2_MWP                   , ww);
                FillHistoTTbar("nbtag_CSVv2L",   nbjet_ttbar_CSVv2_LWP                   , ww);
                FillHistoTTbar("nbtag_cMVAv2T",  nbjet_ttbar_cMVAv2_TWP                  , ww);
                FillHistoTTbar("nbtag_cMVAv2M",  nbjet_ttbar_cMVAv2_MWP                  , ww);
                FillHistoTTbar("nbtag_cMVAv2L",  nbjet_ttbar_cMVAv2_LWP                  , ww);
                FillHistoTTbar("pt_jet",       ptjet_ttbar                             , ww);

                // HIP check (as function of run range for Run2016B)
                if( isData )//ALPHA
                {
                    int nJet = thettbarselector_.theSelJetColl.size();
                    switch ( nJet ){
                        case 2:
                            if ( nbjet_ttbar_CSVv2_LWP >= 2 ) FillHistoTTbar("nEvt_run_CSVv2L", TString::Format("%d",Run).Data(), 1);
                            if ( nbjet_ttbar_CSVv2_MWP >= 2 ) FillHistoTTbar("nEvt_run_CSVv2M", TString::Format("%d",Run).Data(), 1);
                            if ( nbjet_ttbar_CSVv2_TWP >= 2 ) FillHistoTTbar("nEvt_run_CSVv2T", TString::Format("%d",Run).Data(), 1);
                            break;
                    }
                    if ( nJet >= 2 ){
                        FillHistoTTbar("nEvt_run", TString::Format("%d",Run).Data(), 1);
                        if (nJet <= 9){
                            FillHistoTTbar(TString::Format("nEvt_run_%dJet",nJet).Data(),   TString::Format("%d",Run).Data(), 1);
                        }else{
                            FillHistoTTbar("nEvt_run_MoreJet",TString::Format("%d",Run).Data(), 1);
                        }
                    }
                }
            }

            TString tmp_ptbin = "";

            if(isTTbarSelForSF == true)
            {

                if(isLowerThan60)           tmp_ptbin = "_Inf60";
                else if(isBetween60and120)  tmp_ptbin = "_60-120";
                else if(isBetween120and320) tmp_ptbin = "_120-320";

                FillHistoTTbar("nbtag_all_afterJetSel_CSVv2T",               nbjet_afterJetSel_ttbar_CSVv2_TWP  ,ww);
                FillHistoTTbar("nbtag_all_afterJetSel_CSVv2M_SFapplied",     n_ttbar_applySF                    ,ww);
                FillHistoTTbar("nbtag_all_afterJetSel_CSVv2M",               nbjet_afterJetSel_ttbar_CSVv2_MWP  ,ww);
                FillHistoTTbar("nbtag_all_afterJetSel_CSVv2L",               nbjet_afterJetSel_ttbar_CSVv2_LWP  ,ww);

                FillHistoTTbar("nbtag_all_afterJetSel_cMVAv2T",              nbjet_afterJetSel_ttbar_cMVAv2_TWP ,ww);
                FillHistoTTbar("nbtag_all_afterJetSel_cMVAv2M",              nbjet_afterJetSel_ttbar_cMVAv2_MWP ,ww);
                FillHistoTTbar("nbtag_all_afterJetSel_cMVAv2L",              nbjet_afterJetSel_ttbar_cMVAv2_LWP ,ww);

                if(tmp_ptbin != "")
                {

                    FillHistoTTbar("nbtag_all"+tmp_ptbin+"_afterJetSel_CSVv2T", nbjet_afterJetSel_ttbar_CSVv2_TWP     ,ww);
                    FillHistoTTbar("nbtag_all"+tmp_ptbin+"_afterJetSel_CSVv2M", nbjet_afterJetSel_ttbar_CSVv2_MWP     ,ww);
                    FillHistoTTbar("nbtag_all"+tmp_ptbin+"_afterJetSel_CSVv2L", nbjet_afterJetSel_ttbar_CSVv2_LWP     ,ww);

                    FillHistoTTbar("nbtag_all"+tmp_ptbin+"_afterJetSel_cMVAv2T", nbjet_afterJetSel_ttbar_cMVAv2_TWP     ,ww);
                    FillHistoTTbar("nbtag_all"+tmp_ptbin+"_afterJetSel_cMVAv2M", nbjet_afterJetSel_ttbar_cMVAv2_MWP     ,ww);
                    FillHistoTTbar("nbtag_all"+tmp_ptbin+"_afterJetSel_cMVAv2L", nbjet_afterJetSel_ttbar_cMVAv2_LWP     ,ww);

                }


                if( n_ttbar_bgenjet == 2)
                {

                    FillHistoTTbar("nbtag_2b_afterJetSel_CSVv2T", nbjet_afterJetSel_ttbar_CSVv2_TWP     ,ww);
                    FillHistoTTbar("nbtag_2b_afterJetSel_CSVv2M", nbjet_afterJetSel_ttbar_CSVv2_MWP     ,ww);
                    FillHistoTTbar("nbtag_2b_afterJetSel_CSVv2L", nbjet_afterJetSel_ttbar_CSVv2_LWP     ,ww);

                    FillHistoTTbar("nbtag_2b_afterJetSel_cMVAv2T", nbjet_afterJetSel_ttbar_cMVAv2_TWP     ,ww);
                    FillHistoTTbar("nbtag_2b_afterJetSel_cMVAv2M", nbjet_afterJetSel_ttbar_cMVAv2_MWP     ,ww);
                    FillHistoTTbar("nbtag_2b_afterJetSel_cMVAv2L", nbjet_afterJetSel_ttbar_cMVAv2_LWP     ,ww);

                    if(tmp_ptbin != "")
                    {

                        FillHistoTTbar("nbtag_2b"+tmp_ptbin+"_afterJetSel_CSVv2T", nbjet_afterJetSel_ttbar_CSVv2_TWP     ,ww);
                        FillHistoTTbar("nbtag_2b"+tmp_ptbin+"_afterJetSel_CSVv2M", nbjet_afterJetSel_ttbar_CSVv2_MWP     ,ww);
                        FillHistoTTbar("nbtag_2b"+tmp_ptbin+"_afterJetSel_CSVv2L", nbjet_afterJetSel_ttbar_CSVv2_LWP     ,ww);

                        FillHistoTTbar("nbtag_2b"+tmp_ptbin+"_afterJetSel_cMVAv2T", nbjet_afterJetSel_ttbar_cMVAv2_TWP     ,ww);
                        FillHistoTTbar("nbtag_2b"+tmp_ptbin+"_afterJetSel_cMVAv2M", nbjet_afterJetSel_ttbar_cMVAv2_MWP     ,ww);
                        FillHistoTTbar("nbtag_2b"+tmp_ptbin+"_afterJetSel_cMVAv2L", nbjet_afterJetSel_ttbar_cMVAv2_LWP     ,ww);

                    }
                }
                else if( n_ttbar_bgenjet == 1 && n_ttbar_cgenjet == 1 )
                {

                    FillHistoTTbar("nbtag_1b1c_afterJetSel_CSVv2T", nbjet_afterJetSel_ttbar_CSVv2_TWP     ,ww);
                    FillHistoTTbar("nbtag_1b1c_afterJetSel_CSVv2M", nbjet_afterJetSel_ttbar_CSVv2_MWP     ,ww);
                    FillHistoTTbar("nbtag_1b1c_afterJetSel_CSVv2L", nbjet_afterJetSel_ttbar_CSVv2_LWP     ,ww);

                    FillHistoTTbar("nbtag_1b1c_afterJetSel_cMVAv2T", nbjet_afterJetSel_ttbar_cMVAv2_TWP     ,ww);
                    FillHistoTTbar("nbtag_1b1c_afterJetSel_cMVAv2M", nbjet_afterJetSel_ttbar_cMVAv2_MWP     ,ww);
                    FillHistoTTbar("nbtag_1b1c_afterJetSel_cMVAv2L", nbjet_afterJetSel_ttbar_cMVAv2_LWP     ,ww);

                    if(tmp_ptbin != "")
                    {

                        FillHistoTTbar("nbtag_1b1c"+tmp_ptbin+"_afterJetSel_CSVv2T", nbjet_afterJetSel_ttbar_CSVv2_TWP     ,ww);
                        FillHistoTTbar("nbtag_1b1c"+tmp_ptbin+"_afterJetSel_CSVv2M", nbjet_afterJetSel_ttbar_CSVv2_MWP     ,ww);
                        FillHistoTTbar("nbtag_1b1c"+tmp_ptbin+"_afterJetSel_CSVv2L", nbjet_afterJetSel_ttbar_CSVv2_LWP     ,ww);

                        FillHistoTTbar("nbtag_1b1c"+tmp_ptbin+"_afterJetSel_cMVAv2T", nbjet_afterJetSel_ttbar_cMVAv2_TWP     ,ww);
                        FillHistoTTbar("nbtag_1b1c"+tmp_ptbin+"_afterJetSel_cMVAv2M", nbjet_afterJetSel_ttbar_cMVAv2_MWP     ,ww);
                        FillHistoTTbar("nbtag_1b1c"+tmp_ptbin+"_afterJetSel_cMVAv2L", nbjet_afterJetSel_ttbar_cMVAv2_LWP     ,ww);

                    }
                }
                else if( n_ttbar_bgenjet == 1 && n_ttbar_lgenjet == 1 )
                {

                    FillHistoTTbar("nbtag_1b1l_afterJetSel_CSVv2T", nbjet_afterJetSel_ttbar_CSVv2_TWP     ,ww);
                    FillHistoTTbar("nbtag_1b1l_afterJetSel_CSVv2M", nbjet_afterJetSel_ttbar_CSVv2_MWP     ,ww);
                    FillHistoTTbar("nbtag_1b1l_afterJetSel_CSVv2L", nbjet_afterJetSel_ttbar_CSVv2_LWP     ,ww);

                    FillHistoTTbar("nbtag_1b1l_afterJetSel_cMVAv2T", nbjet_afterJetSel_ttbar_cMVAv2_TWP     ,ww);
                    FillHistoTTbar("nbtag_1b1l_afterJetSel_cMVAv2M", nbjet_afterJetSel_ttbar_cMVAv2_MWP     ,ww);
                    FillHistoTTbar("nbtag_1b1l_afterJetSel_cMVAv2L", nbjet_afterJetSel_ttbar_cMVAv2_LWP     ,ww);

                    if(tmp_ptbin != "")
                    {

                        FillHistoTTbar("nbtag_1b1l"+tmp_ptbin+"_afterJetSel_CSVv2T", nbjet_afterJetSel_ttbar_CSVv2_TWP     ,ww);
                        FillHistoTTbar("nbtag_1b1l"+tmp_ptbin+"_afterJetSel_CSVv2M", nbjet_afterJetSel_ttbar_CSVv2_MWP     ,ww);
                        FillHistoTTbar("nbtag_1b1l"+tmp_ptbin+"_afterJetSel_CSVv2L", nbjet_afterJetSel_ttbar_CSVv2_LWP     ,ww);

                        FillHistoTTbar("nbtag_1b1l"+tmp_ptbin+"_afterJetSel_cMVAv2T", nbjet_afterJetSel_ttbar_cMVAv2_TWP     ,ww);
                        FillHistoTTbar("nbtag_1b1l"+tmp_ptbin+"_afterJetSel_cMVAv2M", nbjet_afterJetSel_ttbar_cMVAv2_MWP     ,ww);
                        FillHistoTTbar("nbtag_1b1l"+tmp_ptbin+"_afterJetSel_cMVAv2L", nbjet_afterJetSel_ttbar_cMVAv2_LWP     ,ww);

                    }
                }
                else if( n_ttbar_cgenjet == 2)
                {

                    FillHistoTTbar("nbtag_2c_afterJetSel_CSVv2T", nbjet_afterJetSel_ttbar_CSVv2_TWP     ,ww);
                    FillHistoTTbar("nbtag_2c_afterJetSel_CSVv2M", nbjet_afterJetSel_ttbar_CSVv2_MWP     ,ww);
                    FillHistoTTbar("nbtag_2c_afterJetSel_CSVv2L", nbjet_afterJetSel_ttbar_CSVv2_LWP     ,ww);

                    FillHistoTTbar("nbtag_2c_afterJetSel_cMVAv2T", nbjet_afterJetSel_ttbar_cMVAv2_TWP     ,ww);
                    FillHistoTTbar("nbtag_2c_afterJetSel_cMVAv2M", nbjet_afterJetSel_ttbar_cMVAv2_MWP     ,ww);
                    FillHistoTTbar("nbtag_2c_afterJetSel_cMVAv2L", nbjet_afterJetSel_ttbar_cMVAv2_LWP     ,ww);

                    if(tmp_ptbin != "")
                    {

                        FillHistoTTbar("nbtag_2c"+tmp_ptbin+"_afterJetSel_CSVv2T", nbjet_afterJetSel_ttbar_CSVv2_TWP     ,ww);
                        FillHistoTTbar("nbtag_2c"+tmp_ptbin+"_afterJetSel_CSVv2M", nbjet_afterJetSel_ttbar_CSVv2_MWP     ,ww);
                        FillHistoTTbar("nbtag_2c"+tmp_ptbin+"_afterJetSel_CSVv2L", nbjet_afterJetSel_ttbar_CSVv2_LWP     ,ww);

                        FillHistoTTbar("nbtag_2c"+tmp_ptbin+"_afterJetSel_cMVAv2T", nbjet_afterJetSel_ttbar_cMVAv2_TWP     ,ww);
                        FillHistoTTbar("nbtag_2c"+tmp_ptbin+"_afterJetSel_cMVAv2M", nbjet_afterJetSel_ttbar_cMVAv2_MWP     ,ww);
                        FillHistoTTbar("nbtag_2c"+tmp_ptbin+"_afterJetSel_cMVAv2L", nbjet_afterJetSel_ttbar_cMVAv2_LWP     ,ww);

                    }
                }
                else if( n_ttbar_cgenjet == 1 && n_ttbar_lgenjet == 1 )
                {

                    FillHistoTTbar("nbtag_1c1l_afterJetSel_CSVv2T", nbjet_afterJetSel_ttbar_CSVv2_TWP     ,ww);
                    FillHistoTTbar("nbtag_1c1l_afterJetSel_CSVv2M", nbjet_afterJetSel_ttbar_CSVv2_MWP     ,ww);
                    FillHistoTTbar("nbtag_1c1l_afterJetSel_CSVv2L", nbjet_afterJetSel_ttbar_CSVv2_LWP     ,ww);

                    FillHistoTTbar("nbtag_1c1l_afterJetSel_cMVAv2T", nbjet_afterJetSel_ttbar_cMVAv2_TWP     ,ww);
                    FillHistoTTbar("nbtag_1c1l_afterJetSel_cMVAv2M", nbjet_afterJetSel_ttbar_cMVAv2_MWP     ,ww);
                    FillHistoTTbar("nbtag_1c1l_afterJetSel_cMVAv2L", nbjet_afterJetSel_ttbar_cMVAv2_LWP     ,ww);

                    if(tmp_ptbin != "")
                    {

                        FillHistoTTbar("nbtag_1c1l"+tmp_ptbin+"_afterJetSel_CSVv2T", nbjet_afterJetSel_ttbar_CSVv2_TWP     ,ww);
                        FillHistoTTbar("nbtag_1c1l"+tmp_ptbin+"_afterJetSel_CSVv2M", nbjet_afterJetSel_ttbar_CSVv2_MWP     ,ww);
                        FillHistoTTbar("nbtag_1c1l"+tmp_ptbin+"_afterJetSel_CSVv2L", nbjet_afterJetSel_ttbar_CSVv2_LWP     ,ww);

                        FillHistoTTbar("nbtag_1c1l"+tmp_ptbin+"_afterJetSel_cMVAv2T", nbjet_afterJetSel_ttbar_cMVAv2_TWP     ,ww);
                        FillHistoTTbar("nbtag_1c1l"+tmp_ptbin+"_afterJetSel_cMVAv2M", nbjet_afterJetSel_ttbar_cMVAv2_MWP     ,ww);
                        FillHistoTTbar("nbtag_1c1l"+tmp_ptbin+"_afterJetSel_cMVAv2L", nbjet_afterJetSel_ttbar_cMVAv2_LWP     ,ww);

                    }
                }
                else if( n_ttbar_lgenjet == 2 )
                {

                    FillHistoTTbar("nbtag_2l_afterJetSel_CSVv2T", nbjet_afterJetSel_ttbar_CSVv2_TWP     ,ww);
                    FillHistoTTbar("nbtag_2l_afterJetSel_CSVv2M", nbjet_afterJetSel_ttbar_CSVv2_MWP     ,ww);
                    FillHistoTTbar("nbtag_2l_afterJetSel_CSVv2L", nbjet_afterJetSel_ttbar_CSVv2_LWP     ,ww);

                    FillHistoTTbar("nbtag_2l_afterJetSel_cMVAv2T", nbjet_afterJetSel_ttbar_cMVAv2_TWP     ,ww);
                    FillHistoTTbar("nbtag_2l_afterJetSel_cMVAv2M", nbjet_afterJetSel_ttbar_cMVAv2_MWP     ,ww);
                    FillHistoTTbar("nbtag_2l_afterJetSel_cMVAv2L", nbjet_afterJetSel_ttbar_cMVAv2_LWP     ,ww);

                    if(tmp_ptbin != "")
                    {

                        FillHistoTTbar("nbtag_2l"+tmp_ptbin+"_afterJetSel_CSVv2T", nbjet_afterJetSel_ttbar_CSVv2_TWP     ,ww);
                        FillHistoTTbar("nbtag_2l"+tmp_ptbin+"_afterJetSel_CSVv2M", nbjet_afterJetSel_ttbar_CSVv2_MWP     ,ww);
                        FillHistoTTbar("nbtag_2l"+tmp_ptbin+"_afterJetSel_CSVv2L", nbjet_afterJetSel_ttbar_CSVv2_LWP     ,ww);

                        FillHistoTTbar("nbtag_2l"+tmp_ptbin+"_afterJetSel_cMVAv2T",nbjet_afterJetSel_ttbar_cMVAv2_TWP     ,ww);
                        FillHistoTTbar("nbtag_2l"+tmp_ptbin+"_afterJetSel_cMVAv2M",nbjet_afterJetSel_ttbar_cMVAv2_MWP     ,ww);
                        FillHistoTTbar("nbtag_2l"+tmp_ptbin+"_afterJetSel_cMVAv2L",nbjet_afterJetSel_ttbar_cMVAv2_LWP     ,ww);

                    }
                }
                else if( !isData ) cout << "There is an issue with the gen jets: see line: " << __LINE__ << " and n_ttbar[b,c,l]genjet = " << n_ttbar_bgenjet << " | " << n_ttbar_cgenjet << " | " << n_ttbar_lgenjet << endl;

            } // end of isTTbarSelForSF condition

        }//}}}end of !produceCTagTree

        //cout << "---------------------End event---------------------------" << endl;

    }//----------------------------------------End loop events ----------------------------------------------------------------------------------------------//

    cout << endl;
    cout << "----------------Configuration-------------------" <<endl;
    cout << " Run over "           << N_event_data_before_sel << " data events and " << N_event_mc_before_sel << " mc events."    << endl;
    cout << " After selection -> " << N_event_data_after_sel  << " data events and " << N_event_mc_after_sel  << "mc events left" << endl;
    cout << endl;

    myfile->cd();

    // scale PU reweighting with ratio of sum of weights
    Double_t puScale = sumWeightWoPUreweighting/sumWeightWithPUreweighting;
    if(!isData) cout << "sumWeightWoPUreweighting = " << sumWeightWoPUreweighting << " | sumWeightWithPUreweighting = " << sumWeightWithPUreweighting << "  | scale (PUreweighting) = " << puScale << endl;

    for (unsigned int i=0; i<HistoBtag.size(); i++)
    {
        if(!isData && sumWeightWithPUreweighting != 0) HistoBtag[i]->Scale(puScale);
        HistoBtag[i]->Write();
    }
    for (unsigned int i=0; i<HistoTTbar.size(); i++)
    {
        if(!isData && sumWeightWithPUreweighting != 0) HistoTTbar[i]->Scale(puScale);
        HistoTTbar[i]->Write();
    }
    for (unsigned int i=0; i<HistoBtag2D.size(); i++)
    {
        if(!isData && sumWeightWithPUreweighting != 0) HistoBtag2D[i]->Scale(puScale);
        HistoBtag2D[i]->Write();
    }
    nPU_mc       ->Write();
    nPU_data     ->Write();
    nPV_mc       ->Write();
    pt_hat       ->Write();
    jet_pt_mc    ->Write();
    totalGenEvts ->Write();

    myfile->Close();

}

void CommPlotProducer4ttbar::AddHistoBtag(TString name, TString title, int nbins, float min, float max)
{

    TH1D* h_b      = new TH1D(name+"_b",         title+"_b",    nbins,min,max);
    TH1D* h_pu     = new TH1D(name+"_pu",        title+"_pu",   nbins,min,max);
    TH1D* h_c      = new TH1D(name+"_c",         title+"_c",    nbins,min,max);
    TH1D* h_l      = new TH1D(name+"_l",         title+"_l",    nbins,min,max);
    TH1D* h_data   = new TH1D(name,              title+"_data", nbins,min,max);

    h_b        ->Sumw2();
    h_pu       ->Sumw2();
    h_c        ->Sumw2();
    h_l        ->Sumw2();
    h_data     ->Sumw2();

    HistoBtag.push_back(h_b);
    HistoBtag.push_back(h_pu);
    HistoBtag.push_back(h_c);
    HistoBtag.push_back(h_l);
    HistoBtag.push_back(h_data);
    HistoBtag_map[name.Data()] = numb_histo;

    numb_histo++;

}

void CommPlotProducer4ttbar::AddHistoTTbar(TString name, TString title, int nbins, float min, float max)
{

    TH1D* h = new TH1D(name, title, nbins, min, max);

    HistoTTbar.push_back(h);
    HistoTTbar_map[name.Data()] = numb_histo2;

    numb_histo2++;

}

    template<typename varType>
void CommPlotProducer4ttbar::FillHistoBtag(TString name, int flavour, bool isPU, varType value, double weight)
{

    int number = HistoBtag_map[name.Data()] ;
    if (!isData)
    {
        if (isPU)                                       HistoBtag[number*5 +1]->Fill(value,weight);
        else if (fabs(flavour)==5)                      HistoBtag[number*5 +0]->Fill(value,weight);
        else if (fabs(flavour)==4)                      HistoBtag[number*5 +2]->Fill(value,weight);
        else if (fabs(flavour)< 4 || fabs(flavour)==21) HistoBtag[number*5 +3]->Fill(value,weight);
    }
    else                                              HistoBtag[number*5 +4]->Fill(value);

}

    template<typename varType>
void CommPlotProducer4ttbar::FillHistoTTbar(TString name, varType value, double weight)
{
    int number = HistoTTbar_map[name.Data()] ;
    HistoTTbar[number]->Fill(value,weight);
}

//-----------------------------------------------------------------------------------------------------------------//
//----------------------------------------------------------2D PLOTS-----------------------------------------------//
//-----------------------------------------------------------------------------------------------------------------//
void CommPlotProducer4ttbar::AddHisto2D(TString name, TString title, int nbins, float min, float max, int nbins2, float min2, float max2)
{

    TH2D* h_b      = new TH2D(name+"_b",  title+"_b",            nbins,min,max,nbins2,min2,max2);
    TH2D* h_pu     = new TH2D(name+"_pu", title+"_pu",           nbins,min,max,nbins2,min2,max2);
    TH2D* h_c      = new TH2D(name+"_c",  title+"_c",            nbins,min,max,nbins2,min2,max2);
    TH2D* h_l      = new TH2D(name+"_l",  title+"_l",            nbins,min,max,nbins2,min2,max2);
    TH2D* h_data   = new TH2D(name,       title+"_data",         nbins,min,max,nbins2,min2,max2);


    h_b        ->Sumw2();
    h_pu       ->Sumw2();
    h_c        ->Sumw2();
    h_l        ->Sumw2();
    h_data     ->Sumw2();

    HistoBtag2D.push_back(h_b);
    HistoBtag2D.push_back(h_pu);
    HistoBtag2D.push_back(h_c);
    HistoBtag2D.push_back(h_l);
    HistoBtag2D.push_back(h_data);
    HistoBtag2D_map[name.Data()] = numb_histo2D;
    numb_histo2D++;

}

    template<typename varType1, typename varType2>
void CommPlotProducer4ttbar::FillHisto2D(TString name, int flavour, bool isPU, varType1 value1, varType2 value2, double weight)
{

    int number = HistoBtag2D_map[name.Data()] ;
    if (!isData){
        if (isPU)                                       HistoBtag2D[number*5 +1]->Fill(value1,value2,weight);
        else if (fabs(flavour)==5)                      HistoBtag2D[number*5 +0]->Fill(value1,value2,weight);
        else if (fabs(flavour)==4)                      HistoBtag2D[number*5 +2]->Fill(value1,value2,weight);
        else if (fabs(flavour)< 4 || fabs(flavour)==21) HistoBtag2D[number*5 +3]->Fill(value1,value2,weight);

    }
    else                                              HistoBtag2D[number*5 +4]->Fill(value1,value2);

}

