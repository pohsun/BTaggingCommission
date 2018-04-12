//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Tue Nov 13 15:05:58 2012 by ROOT version 5.32/00
// from TTree ttree/ttree
// found on file: JetTreeTP_Pt-80to120.root
//////////////////////////////////////////////////////////

#ifndef CommPlotProducer4ttbar_h
#define CommPlotProducer4ttbar_h

#include <TROOT.h>
#include <TSystem.h>
#include <TString.h>
#include <TChain.h>
#include <TEventList.h>
#include <TFile.h>
#include <TTreeReader.h>
#include <TTreeReaderValue.h>
#include <TTreeReaderArray.h>
#include <TGraph.h>
#include "TH1D.h"
#include "TH2D.h"

#include <string.h>
#include <iostream>
#include <fstream>
#include <utility>
#include <map>
#include <bitset>

// Header file for the classes stored in the TTree if any.


class CommPlotProducer4ttbar
{
    public:
        // Member functions
        CommPlotProducer4ttbar(TChain *superTree=0, TString puWgtUrlName="${CMSSW_BASE}/src/RecoBTag/PerformanceMeasurements/test/ttbar/data/pileupWgts.root", TString ofname="output.root");
        CommPlotProducer4ttbar(TChain   &superTree, TString puWgtUrlName="${CMSSW_BASE}/src/RecoBTag/PerformanceMeasurements/test/ttbar/data/pileupWgts.root", TString ofname="output.root");
        virtual ~CommPlotProducer4ttbar();

        void    RegBranch(TString name);
        void    AddTrigChannel(int chan);
        void    AddTagWP        (TString tagName, TString varName, float wp);
        bool    SetDefaultTagWP (TString name);
        void    AddSubEventList (TString name, TString cut);

        void    Loop(bool isdata, TH1F* wgtcounter);
            // Selectors. Add needed branched to SetBranchAddress!

        void    Clear();
    private:
        // Member object
        TChain      *ch;  //!pointer to the analyzed TTree or TChain
        TGraph      *puWgtGr,*puWgtDoGr,*puWgtUpGr;
        TFile       *fout;
        std::vector<int> trigChannels;

        // Flags
        int isData;

        // tagger WPs
        int defaultTagWPIndex;
        std::vector<float> v_tagWP;
        std::vector<std::string> v_tagWPVar;
        std::map<std::string, int> m_tagWP;

        // Leaves
        TTreeReader *reader;
        std::vector<TTreeReaderArray<Int_t>* >       v_readerAI;
        std::vector<TTreeReaderArray<Float_t>* >     v_readerAF;
        std::vector<TTreeReaderArray<Double_t>* >    v_readerAD;
        std::map<std::string, int> m_leaf;
        float    GetBranch(TString name, int index=0);
        void    SetBranchAddress();

        // EventList
        std::vector<TEventList*> v_evtList;
        std::vector<std::string> cut_evtList;
        std::map<std::string, int> m_evtList;

        // histograms
        TH1D *totalGenEvts;
        TH1D *puWgtNorms;
        std::vector<TH1D*>                  v_histoBtag;
        std::map<std::string, int>          m_histoBtag;
        std::vector<TH1D*>                  v_histoTTbar;
        std::map<std::string, int>          m_histoTTbar;
        void    AddHistoBtag (TString name, TString title,  int nbins, float min, float max);
        void    AddHistoTTbar(TString name, TString title,  int nbins, float min, float max);
        void    AddHistos(const char*);
        void    AddAllHistos();
        template<typename varType>
        void    FillHistoTTbar(TString name, varType value, double weight=1);
        template<typename varType>
        void    FillHistoBtag (TString name, int flavour, bool isPU, varType value, double weight=1);
        template<typename varType1, typename varType2>
        void    FillHisto2D(TString name, int flavour, bool isPU, varType1 value1, varType2 value2, double weight=1);

        // Filters
        bool                passPresel  (int chan);
        bool                passTrigSel (int chan);
        std::vector<int>    passJetSel  (int chan);
        bool                passTTbarSel(int chan, std::vector<int>&);
        std::bitset<8>      passTrackSel(int chan, int trkIdx);

        // Weights
        void    calcPUWgt       (float *wgt);
        void    calcQCDWgt      (float *wgt, TH1F *wgtcounter);
        void    calcTrigWgt     (float *wgt, int chan);
        void    calcLepEffWgt   (float *wgt);
};
#endif

#ifdef CommPlotProducer4ttbar_cxx

CommPlotProducer4ttbar::CommPlotProducer4ttbar(TChain *superTree, TString puWgtUrl, TString ofname)
{//{{{
    Clear();

    // Set branch addresses and branch pointers
    ch = superTree;
    reader = new TTreeReader(ch);
    //ch->SetBranchStatus("*",0); // On demand loading in RegBranch

    gSystem->ExpandPathName(puWgtUrl);
    TFile *fIn=TFile::Open(puWgtUrl);
    if (fIn){
        puWgtGr     = (TGraph *)fIn->Get("puwgts_nom");
        puWgtDoGr   = (TGraph *)fIn->Get("puwgts_down");
        puWgtUpGr   = (TGraph *)fIn->Get("puwgts_up");
        fIn->Close();
    }else{
        printf("ERROR\t: Unable to find pileupWgts.root, no PU reweighting will be applied.\n");
    }

    // Create output file
    fout = new TFile(ofname.Data(),"RECREATE");
    m_evtList["nominal"]=0;
    v_evtList   .push_back(0);
    cut_evtList .push_back("");
}//}}}

CommPlotProducer4ttbar::CommPlotProducer4ttbar(TChain &superTree, TString puWgtUrl, TString ofname)
{//{{{
    CommPlotProducer4ttbar(&superTree, puWgtUrl, ofname);
}//}}}

CommPlotProducer4ttbar::~CommPlotProducer4ttbar()
{//{{{
    if (!ch) return;
    delete ch->GetCurrentFile();
}//}}}

void CommPlotProducer4ttbar::Clear()
{//{{{
    // Member object
    ch          = 0;  //!pointer to the analyzed TTree or TChain
    puWgtGr     = 0;
    puWgtDoGr   = 0;
    puWgtUpGr   = 0;
    fout        = 0;
    trigChannels.clear();

    // Flags
    isData=-1;

    // tagger WPs
    defaultTagWPIndex=-1;
    v_tagWP        . clear();
    v_tagWPVar     . clear();
    m_tagWP        . clear();

    // Leaves
    v_readerAI . clear();
    v_readerAF . clear();
    v_readerAD . clear();
    m_leaf     . clear();//Indexing

    // histograms
    v_histoBtag  . clear();
    v_histoTTbar . clear();
    m_histoBtag  . clear();
    m_histoTTbar . clear();
    
}//}}}

#endif // #ifdef CommPlotProducer4ttbar_cxx
