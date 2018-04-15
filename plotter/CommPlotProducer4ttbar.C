// vim: set sts=4 sw=4 fdm=marker et:
#ifndef CommPlotProducer4ttbar_cxx
#define CommPlotProducer4ttbar_cxx

#include "CommPlotProducer4ttbar.h"
#include "TTreeReaderValue.h"
#include "TTreeReaderArray.h"

#include <iostream>
#include <fstream>
#include <string.h>
#include <typeinfo>
#include <bitset>

#include "TLeaf.h"
#include "TString.h"

#include "TMath.h"
#include "TLorentzVector.h"
#include "TRandom.h"
#include "TRandom3.h"

#include "TH1D.h"
#include "TGraph.h"

#define BIGNUMBER 9999
#define PI 3.14159265359

TRandom3 *theRand = new TRandom3(12345);  // used to apply btag efficiency (btag SF closure test)

void CommPlotProducer4ttbar::Loop(bool isdata, TH1F* wgtcounter)
{//{{{

    // Check if ready to run.
    if (ch == 0){
        return;
    }

    // Initialize counters
    isData = isdata;
    int chan        = -11*13; // ttbar in emu channel
    int puWgtBit    = 0;// Nominal, Lower, Upper
    int qcdWgtBit   = 0;
    int trigWgtBit  = 0;
    int lepEffWgtBit= 0;
    SetBranchAddress();// Load branches;

    // Build base EntryList
    printf("INFO\t: Filter nominal event list from %lld entries.\n",ch->GetEntries());
    TEventList *elist_nominal = new TEventList("elist_nominal");
    while(reader->Next()){//{{{
        // apply basic selections
        if (!passTrigSel(chan)) continue;
        if (!passPresel (chan)) continue;
        std::vector<int> selJets = passJetSel(chan);
        if (!passTTbarSel(chan, selJets)) continue;

        elist_nominal->Enter(reader->GetCurrentEntry());
    }//}}}
    printf("INFO\t: Nominal event list with %d entries.\n",elist_nominal->GetN());

    // Create sub-EventLists, interestingly, TTree::Draw has nothing to do with BranchStatus.
    ch->SetEventList(elist_nominal);
    for(std::map<std::string, int>::iterator iterEvL=m_evtList.begin(); iterEvL != m_evtList.end(); iterEvL++){
        if (iterEvL->first=="nominal"){
            v_evtList[0] = elist_nominal;
        }else{
            ch->Draw(TString::Format(">>elist_%s",iterEvL->first.c_str()).Data(), cut_evtList[iterEvL->second].c_str());
        }
        printf("INFO\t: EventList %s created.\n",iterEvL->first.c_str());
    }

    // Define histograms for loop.
    AddAllHistos();

    // Record total generated events for mc
    if( !isData ){
        totalGenEvts->Fill(0.,wgtcounter->GetBinContent(1));
    }

    // Loop over nominal EntryList and fill histograms
    std::vector<float> sumWeight                (m_evtList.size(), 0);
    std::vector<float> sumWeightNoPU            (m_evtList.size(), 0);
    reader->Restart();
    while(reader->Next()){//{{{
        int jentry = reader->GetCurrentEntry();
        if (!v_evtList[0]->Contains(jentry)) continue;

        //--------------------------------------------//
        //------------------MC reweighting------------//
        //--------------------------------------------//
        float puWgt[3]={1.,1.,1.};//Nominal, Lo, Up
        calcPUWgt(puWgt);

        float qcdWgt[3]={1.,1.,1.};
        calcQCDWgt(qcdWgt,wgtcounter);

        float trigWgt[3]={1.,1.,1.};
        calcTrigWgt(trigWgt,chan);

        float lepEffWgt[3]={1.,1.,1.};
        calcLepEffWgt(lepEffWgt);

        // Switch bit
        float evtWgt = puWgt[puWgtBit]*qcdWgt[qcdWgtBit]*trigWgt[trigWgtBit]*lepEffWgt[lepEffWgtBit];

        //-------------------------------------------------//
        //------------------Most used variables------------//
        //-------------------------------------------------//
        TLorentzVector lep0, lep1;
        lep0.SetPtEtaPhiM(GetBranch("ttbar_lpt",0),GetBranch("ttbar_leta",0),GetBranch("ttbar_lphi",0),GetBranch("ttbar_lm",0));
        lep1.SetPtEtaPhiM(GetBranch("ttbar_lpt",1),GetBranch("ttbar_leta",1),GetBranch("ttbar_lphi",1),GetBranch("ttbar_lm",1));

        //-----------------------------------
        // Fill control plot
        //-----------------------------------
        // Define jet counters
        std::vector<int> v_tagWPCounter             (m_tagWP  .size(), 0);
        std::vector<int> v_tagWPCounter_selJet30    (m_tagWP  .size(), 0);
        int     njet_pt30   = 0;
        float   jet0_pt     = 0;

        //Loop over selected jets
        std::vector<int> selJets = passJetSel(chan);
        for (unsigned int ijet = 0; ijet < selJets.size(); ijet++){//{{{ Loop over Jets

            int selJetIdx = selJets[ijet];

            float jet_pt     = GetBranch("Jet_pt"      ,selJetIdx);
            float jet_eta    = GetBranch("Jet_eta"     ,selJetIdx);
            float jet_phi    = GetBranch("Jet_phi"     ,selJetIdx);
            TLorentzVector jetVec;
            jetVec.SetPtEtaPhiM(jet_pt, jet_eta, jet_phi, 0);

            int   jet_flav      = round(GetBranch("Jet_flavour" ,selJetIdx));
            int   jet_nTrk      = round(GetBranch("Jet_ntracks" ,selJetIdx));
            int   jet_SV_multi  = round(GetBranch("Jet_SV_multi",selJetIdx));

                // Taggers
            float jet_TCHE              = GetBranch("Jet_Ip2P"                , selJetIdx);
            float jet_TCHP              = GetBranch("Jet_Ip3P"                , selJetIdx);
            float jet_JP                = GetBranch("Jet_ProbaP"              , selJetIdx);
            float jet_JBP               = GetBranch("Jet_BprobP"              , selJetIdx);
            float jet_SSV               = GetBranch("Jet_Svx"                 , selJetIdx);
            float jet_SSVHP             = GetBranch("Jet_SvxHP"               , selJetIdx);
            float jet_CSV               = GetBranch("Jet_CombSvx"             , selJetIdx);
            float jet_CSVv2             = GetBranch("Jet_CombIVF"             , selJetIdx);
            float jet_cMVAv2            = GetBranch("Jet_cMVAv2"              , selJetIdx);
            float jet_DeepCSVBDisc      = GetBranch("Jet_DeepCSVBDisc"        , selJetIdx);
            float jet_DeepFlavourBDisc  = GetBranch("Jet_DeepFlavourBDisc"    , selJetIdx);
            float jet_DeepFlavourCVSB   = GetBranch("Jet_DeepFlavourCvsBDisc" , selJetIdx);
            float jet_DeepFlavourCVSL   = GetBranch("Jet_DeepFlavourCvsLDisc" , selJetIdx);
            float jet_CvsB              = GetBranch("CTag_Jet_CvsB"           , selJetIdx);
            float jet_CvsL              = GetBranch("CTag_Jet_CvsL"           , selJetIdx);

            bool  isPU       = false;
            if (!isData && GetBranch("Jet_genpt",selJetIdx) < 8) isPU=true;
            if (jet_pt > 30) njet_pt30++;
            if (jet_pt > jet0_pt) jet0_pt=jet_pt;

            // Fill info for ttbar 
            for (std::map<std::string, int>::iterator iterEvL=m_evtList.begin(); iterEvL != m_evtList.end(); iterEvL++){
                if (!v_evtList[iterEvL->second]->Contains(jentry)) continue;
                for (std::map<std::string, int>::iterator iterTagWP=m_tagWP.begin(); iterTagWP != m_tagWP.end(); iterTagWP++){
                    int tagIdx = iterTagWP->second;
                    if (GetBranch(v_tagWPVar[tagIdx].c_str(),selJetIdx)>v_tagWP[tagIdx]){
                        v_tagWPCounter[tagIdx]++;
                        if( GetBranch("Jet_pt",selJets[0]) > 30 && GetBranch("Jet_pt",selJets[1]) >= 30 ){
                            v_tagWPCounter_selJet30[tagIdx]++;
                        }
                    }
                }
            }

            for (std::map<std::string, int>::iterator iterEvL=m_evtList.begin(); iterEvL != m_evtList.end(); iterEvL++){//{{{// Fill Jet info
                if (!v_evtList[iterEvL->second]->Contains(jentry)) continue;
                FillHistoBtag(TString::Format("jet_pt_%s"               , iterEvL->first.c_str()) , jet_flav , isPU , jet_pt               , evtWgt);
                FillHistoBtag(TString::Format("jet_eta_%s"              , iterEvL->first.c_str()) , jet_flav , isPU , jet_eta              , evtWgt);
                FillHistoBtag(TString::Format("sv_multi_0_%s"           , iterEvL->first.c_str()) , jet_flav , isPU , jet_SV_multi         , evtWgt);

                FillHistoBtag(TString::Format("tag_TCHE_%s"             , iterEvL->first.c_str()) , jet_flav , isPU , jet_TCHE             , evtWgt);
                FillHistoBtag(TString::Format("tag_TCHP_%s"             , iterEvL->first.c_str()) , jet_flav , isPU , jet_TCHP             , evtWgt);
                FillHistoBtag(TString::Format("tag_JP_%s"               , iterEvL->first.c_str()) , jet_flav , isPU , jet_JP               , evtWgt);
                FillHistoBtag(TString::Format("tag_JBP_%s"              , iterEvL->first.c_str()) , jet_flav , isPU , jet_JBP              , evtWgt);
                FillHistoBtag(TString::Format("tag_SSV_%s"              , iterEvL->first.c_str()) , jet_flav , isPU , jet_SSV              , evtWgt);
                FillHistoBtag(TString::Format("tag_SSVHP_%s"            , iterEvL->first.c_str()) , jet_flav , isPU , jet_SSVHP            , evtWgt);
                FillHistoBtag(TString::Format("tag_CSV_%s"              , iterEvL->first.c_str()) , jet_flav , isPU , jet_CSV              , evtWgt);
                FillHistoBtag(TString::Format("tag_CSVv2_%s"            , iterEvL->first.c_str()) , jet_flav , isPU , jet_CSVv2            , evtWgt);
                FillHistoBtag(TString::Format("tag_cMVAv2_%s"           , iterEvL->first.c_str()) , jet_flav , isPU , jet_cMVAv2           , evtWgt);
                FillHistoBtag(TString::Format("tag_DeepCSVBDisc_%s"     , iterEvL->first.c_str()) , jet_flav , isPU , jet_DeepCSVBDisc     , evtWgt);
                FillHistoBtag(TString::Format("tag_DeepFlavourBDisc_%s" , iterEvL->first.c_str()) , jet_flav , isPU , jet_DeepFlavourBDisc , evtWgt);
                FillHistoBtag(TString::Format("tag_DeepFlavourCvsB_%s"  , iterEvL->first.c_str()) , jet_flav , isPU , jet_DeepFlavourCVSB  , evtWgt);
                FillHistoBtag(TString::Format("tag_DeepFlavourCvsL_%s"  , iterEvL->first.c_str()) , jet_flav , isPU , jet_DeepFlavourCVSL  , evtWgt);
                FillHistoBtag(TString::Format("tag_CvsB_%s"             , iterEvL->first.c_str()) , jet_flav , isPU , jet_CvsB             , evtWgt);
                FillHistoBtag(TString::Format("tag_CvsL_%s"             , iterEvL->first.c_str()) , jet_flav , isPU , jet_CvsL             , evtWgt);
            }//}}}

            // Track information
            std::vector<int> nTrackSel(m_evtList.size(), 0);
            for (int iTrk=GetBranch("Jet_nFirstTrack",selJetIdx); iTrk<GetBranch("Jet_nLastTrack",selJetIdx); iTrk++){//{{{
                float track_pt        = GetBranch("Track_pt"       ,iTrk);
                float track_nHitAll   = GetBranch("Track_nHitAll"  ,iTrk);
                float track_nHitPixel = GetBranch("Track_nHitPixel",iTrk);
                float track_chi2      = GetBranch("Track_chi2"     ,iTrk);
                float track_dist      = fabs(GetBranch("Track_dist",iTrk));
                float track_length    = GetBranch("Track_length"   ,iTrk);
                float track_IP        = GetBranch("Track_IP"       ,iTrk);
                float track_IPsig     = GetBranch("Track_IPsig"    ,iTrk);
                auto  track_bits      = passTrackSel(chan, iTrk);
                bool  isGoodTrack     = track_bits.all();
                if ( track_bits.count()+1 >= track_bits.size()){// n-1 plots
                    for (std::map<std::string, int>::iterator iterEvL=m_evtList.begin(); iterEvL != m_evtList.end(); iterEvL++){
                        if (!v_evtList[iterEvL->second]->Contains(jentry)) continue;
                        if (isGoodTrack) nTrackSel[iterEvL->second]++;
                        if (isGoodTrack || track_bits[0] == 0) FillHistoBtag(TString::Format("track_pt_%s",  iterEvL->first.c_str()),    jet_flav, isPU ,track_pt        ,evtWgt);
                        if (isGoodTrack || track_bits[1] == 0) FillHistoBtag(TString::Format("track_nHit_%s",iterEvL->first.c_str()),    jet_flav, isPU ,track_nHitAll   ,evtWgt);
                        if (isGoodTrack || track_bits[2] == 0) FillHistoBtag(TString::Format("track_HPix_%s",iterEvL->first.c_str()),    jet_flav, isPU ,track_nHitPixel ,evtWgt);
                        if (isGoodTrack || track_bits[3] == 0) FillHistoBtag(TString::Format("track_chi2_%s",iterEvL->first.c_str()),    jet_flav, isPU ,track_chi2      ,evtWgt);
                        if (isGoodTrack || track_bits[4] == 0) FillHistoBtag(TString::Format("track_dist_%s",iterEvL->first.c_str()),    jet_flav, isPU ,track_dist      ,evtWgt);
                        if (isGoodTrack || track_bits[5] == 0) FillHistoBtag(TString::Format("track_len_%s", iterEvL->first.c_str()),    jet_flav, isPU ,track_length    ,evtWgt);
                        if (isGoodTrack || track_bits[6] == 0) FillHistoBtag(TString::Format("track_IP_%s",  iterEvL->first.c_str()),    jet_flav, isPU ,track_IP        ,evtWgt);
                        if (isGoodTrack || track_bits[7] == 0) FillHistoBtag(TString::Format("track_IPs_%s", iterEvL->first.c_str()),    jet_flav, isPU ,track_IPsig     ,evtWgt);
                    }
                }
            }//}}}end tracks loop
            for (std::map<std::string, int>::iterator iterEvL=m_evtList.begin(); iterEvL != m_evtList.end(); iterEvL++){
                FillHistoBtag(TString::Format("track_multi_sel_%s",iterEvL->first.c_str()), jet_flav, isPU ,nTrackSel[iterEvL->second], evtWgt);
            }

            // SV information
            if (jet_SV_multi>0){//{{{ Fill SV info
                int     jet_nFirstSV = round(GetBranch("Jet_nFirstSV",selJetIdx));
                float   sv_flight3DSig = GetBranch("SV_flight",     jet_nFirstSV)/GetBranch("SV_flightErr", jet_nFirstSV);
                float   sv_dR_jet      = GetBranch("SV_deltaR_jet", jet_nFirstSV);
                float   sv_eta         = GetBranch("SV_vtx_eta",    jet_nFirstSV);
                float   sv_phi         = GetBranch("SV_vtx_phi",    jet_nFirstSV);
                for (std::map<std::string, int>::iterator iterEvL=m_evtList.begin(); iterEvL != m_evtList.end(); iterEvL++){
                    if (!v_evtList[iterEvL->second]->Contains(jentry)) continue;
                    FillHistoBtag(TString::Format("sv_deltaR_jet_%s", iterEvL->first.c_str()),  jet_flav, isPU ,sv_dR_jet          , evtWgt);
                    FillHistoBtag(TString::Format("sv_eta_%s",        iterEvL->first.c_str()),  jet_flav, isPU ,sv_eta             , evtWgt);
                    FillHistoBtag(TString::Format("sv_phi_%s",        iterEvL->first.c_str()),  jet_flav, isPU ,sv_phi             , evtWgt);
                    FillHistoBtag(TString::Format("sv_flight3DSig_%s",iterEvL->first.c_str()),  jet_flav, isPU ,sv_flight3DSig     , evtWgt);
                }
            }//}}}

            // PFMuon
            std::vector<int> indPFmu(m_evtList.size(),-1);
            std::vector<int> nPFmu  (m_evtList.size(), 0);
            std::vector<float> maxPFmuPt(m_evtList.size(),0.);
            for( int im=0; im<round(GetBranch("nPFMuon",0)); im++){//{{{ Count PFMuon
                TLorentzVector thePFmu;
                thePFmu.SetPtEtaPhiM(GetBranch("PFMuon_pt",im),GetBranch("PFMuon_eta",im),GetBranch("PFMuon_phi",im),0);
                float drPFj     = thePFmu.DeltaR(jetVec);
                float diffdr    = fabs(drPFj-GetBranch("PFMuon_deltaR",im));
                if (drPFj < 0.5 && diffdr < 0.05 && GetBranch("PFMuon_GoodQuality",im) > 0){
                    for (std::map<std::string, int>::iterator iterEvL=m_evtList.begin(); iterEvL != m_evtList.end(); iterEvL++){
                        if (!v_evtList[iterEvL->second]->Contains(jentry)) continue;
                        if (thePFmu.Pt() > maxPFmuPt[iterEvL->second] ){
                            indPFmu[iterEvL->second]=im;
                            maxPFmuPt[iterEvL->second] = thePFmu.Pt();
                        }
                        nPFmu[iterEvL->second]++;
                    }
                }
            }//}}}

            for (std::map<std::string, int>::iterator iterEvL=m_evtList.begin(); iterEvL != m_evtList.end(); iterEvL++){//{{{ Fill PFMuon
                FillHistoBtag(TString::Format("pfmuon_multi_%s",iterEvL->first.c_str()),  jet_flav, isPU , nPFmu[iterEvL->second]   ,evtWgt);
                if (indPFmu[iterEvL->second]>-1){
                    FillHistoBtag(TString::Format("pfmuon_pt_%s"          , iterEvL->first.c_str()) , jet_flav , isPU , GetBranch("PFMuon_pt"          , indPFmu[iterEvL->second]) , evtWgt);
                    FillHistoBtag(TString::Format("pfmuon_eta_%s"         , iterEvL->first.c_str()) , jet_flav , isPU , GetBranch("PFMuon_eta"         , indPFmu[iterEvL->second]) , evtWgt);
                    FillHistoBtag(TString::Format("pfmuon_phi_%s"         , iterEvL->first.c_str()) , jet_flav , isPU , GetBranch("PFMuon_phi"         , indPFmu[iterEvL->second]) , evtWgt);
                    FillHistoBtag(TString::Format("pfmuon_ptrel_%s"       , iterEvL->first.c_str()) , jet_flav , isPU , GetBranch("PFMuon_ptrel"       , indPFmu[iterEvL->second]) , evtWgt);
                }
            }//}}}

            //PFElectron
            std::vector<int> indPFel(m_evtList.size(),-1);
            std::vector<int> nPFel  (m_evtList.size(), 0);
            std::vector<float> maxPFelPt(m_evtList.size(),0);
            for (int ie=0; ie<round(GetBranch("nPFElectron",0)); ie++){//{{{ Count PFElectron
                TLorentzVector thePFel;
                thePFel.SetPtEtaPhiM(GetBranch("PFElectron_pt",ie),GetBranch("PFElectron_eta",ie),GetBranch("PFElectron_phi",ie),0);
                float drPFj     = thePFel.DeltaR(jetVec);
                float diffdr    = fabs(drPFj-GetBranch("PFElectron_deltaR",ie));
                if (drPFj< 0.5 && diffdr<0.05 && thePFel.Pt() > 2.){
                    for (std::map<std::string, int>::iterator iterEvL=m_evtList.begin(); iterEvL != m_evtList.end(); iterEvL++){
                        if (!v_evtList[iterEvL->second]->Contains(jentry)) continue;
                        if (thePFel.Pt() > maxPFelPt[iterEvL->second] ){
                            indPFel[iterEvL->second]=ie;
                            maxPFelPt[iterEvL->second] = thePFel.Pt();
                        }
                        nPFel[iterEvL->second]++;
                    }
                }
            }//}}}

            for (std::map<std::string, int>::iterator iterEvL=m_evtList.begin(); iterEvL != m_evtList.end(); iterEvL++){//{{{ Fill PFElectron
                FillHistoBtag(TString::Format("pfelectron_multi_%s", iterEvL->first.c_str()),  jet_flav, isPU , nPFel[iterEvL->second]   ,evtWgt);
                if (indPFel[iterEvL->second]>-1){
                    FillHistoBtag(TString::Format("pfelectron_pt_%s"       , iterEvL->first.c_str()) , jet_flav , isPU , GetBranch("PFElectron_pt"       , indPFel[iterEvL->second]) , evtWgt);
                    FillHistoBtag(TString::Format("pfelectron_eta_%s"      , iterEvL->first.c_str()) , jet_flav , isPU , GetBranch("PFElectron_eta"      , indPFel[iterEvL->second]) , evtWgt);
                    FillHistoBtag(TString::Format("pfelectron_phi_%s"      , iterEvL->first.c_str()) , jet_flav , isPU , GetBranch("PFElectron_phi"      , indPFel[iterEvL->second]) , evtWgt);
                    FillHistoBtag(TString::Format("pfelectron_ptrel_%s"    , iterEvL->first.c_str()) , jet_flav , isPU , GetBranch("PFElectron_ptrel"    , indPFel[iterEvL->second]) , evtWgt);
                }
            }//}}}

        }//}}} End Loop on Jets

        // Fill Commissioning plots
        int     nPV = round(GetBranch("nPV",0));
        float   met = GetBranch("ttbar_metpt",0);
        for (std::map<std::string, int>::iterator iterEvL=m_evtList.begin(); iterEvL != m_evtList.end(); iterEvL++){//{{{
            if (!v_evtList[iterEvL->second]->Contains(jentry)) continue;
            sumWeight       [iterEvL->second]+=puWgt[puWgtBit]*qcdWgt[qcdWgtBit]*trigWgt[trigWgtBit]*lepEffWgt[lepEffWgtBit];
            sumWeightNoPU   [iterEvL->second]+=                qcdWgt[qcdWgtBit]*trigWgt[trigWgtBit]*lepEffWgt[lepEffWgtBit];
            FillHistoTTbar(TString::Format("nPV_%s"            , iterEvL->first.c_str()) , nPV                  , evtWgt);
            FillHistoTTbar(TString::Format("met_%s"            , iterEvL->first.c_str()) , met                  , evtWgt);
            FillHistoTTbar(TString::Format("mll_%s"            , iterEvL->first.c_str()) , (lep0+lep1).Mag()    , evtWgt);
            FillHistoTTbar(TString::Format("njet_%s"           , iterEvL->first.c_str()) , selJets.size()       , evtWgt);
            FillHistoTTbar(TString::Format("njet_pt30_%s"      , iterEvL->first.c_str()) , njet_pt30            , evtWgt);
            FillHistoTTbar(TString::Format("lep0_pt_%s"        , iterEvL->first.c_str()) , lep0.Pt()            , evtWgt);//??
            FillHistoTTbar(TString::Format("lep1_pt_%s"        , iterEvL->first.c_str()) , lep1.Pt()            , evtWgt);//??
            FillHistoTTbar(TString::Format("jet0_pt_%s"        , iterEvL->first.c_str()) , jet0_pt              , evtWgt);
            // Count of tagged jets
            for (std::map<std::string, int>::iterator iterTagWP=m_tagWP.begin(); iterTagWP != m_tagWP.end(); iterTagWP++){
                FillHistoTTbar(TString::Format("nBtag_%s_%s"            ,iterTagWP->first.c_str(),iterEvL->first.c_str()), v_tagWPCounter           [iterTagWP->second] , evtWgt);
                FillHistoTTbar(TString::Format("nBtag_afterJetSel_%s_%s",iterTagWP->first.c_str(),iterEvL->first.c_str()), v_tagWPCounter_selJet30  [iterTagWP->second] , evtWgt);
            }
        }//}}}
    }//}}} End event loop.

    // Store PU reweighting with ratio of sum of weights
    for (std::map<std::string, int>::iterator iterEvL=m_evtList.begin(); iterEvL != m_evtList.end(); iterEvL++){//{{{
        puWgtNorms->Fill(TString::Format("sumWeightNoPU_%s" , iterEvL->first.c_str()).Data() , sumWeightNoPU[iterEvL->second]);
        puWgtNorms->Fill(TString::Format("sumWeight_%s"     , iterEvL->first.c_str()).Data() , sumWeight    [iterEvL->second]);
    }//}}}

    // Decorate and save
    fout->cd();
    totalGenEvts->Write();
    puWgtNorms  ->Write();
    for (unsigned int i=0; i<v_histoTTbar.size(); i++){
        v_histoTTbar[i]->Write();
    }
    for (unsigned int i=0; i<v_histoBtag .size(); i++){
        v_histoBtag[i]->Write();
    }

    fout->Close();
} //}}}

void CommPlotProducer4ttbar::RegBranch(TString name)
{//{{{
    if (m_leaf.find(name.Data()) == m_leaf.end()){
        std::string leafType = ch->GetLeaf(name.Data())->GetTypeName();
        if ( leafType == "Int_t") {
            m_leaf[name.Data()]=v_readerAI.size()*100+0;// Add index
            v_readerAI.push_back(new TTreeReaderArray<Int_t>(*reader,name.Data()));
            //printf("DEBUG\t: Register %s into v_readerAI(%d).\n",name.Data(), v_readerAI.size());
        }else if ( leafType == "Float_t"){
            m_leaf[name.Data()]=v_readerAF.size()*100+1;// Add index
            v_readerAF.push_back(new TTreeReaderArray<Float_t>(*reader,name.Data()));
            //printf("DEBUG\t: Register %s into v_readerAF(%d).\n",name.Data(), v_readerAF.size());
        }else if ( leafType == "Double_t"){
            m_leaf[name.Data()]=v_readerAD.size()*100+2;// Add index
            v_readerAD.push_back(new TTreeReaderArray<Double_t>(*reader,name.Data()));
            //printf("DEBUG\t: Register %s into v_readerAD(%d).\n",name.Data(), v_readerAD.size());
        }else{
            printf("ERROR\t: Please define type( %s ) in RegBranch.\n",leafType.c_str());
        }
    }else{
        printf("WARNING\t: %s is a registered branch.\n",name.Data());
    }
}//}}}

void CommPlotProducer4ttbar::SetBranchAddress()
{//{{{

    // Register branches for selectors
    RegBranch("Run"                ) ;
    RegBranch("Evt"                ) ;
    RegBranch("nPV"                ) ;
    RegBranch("nPUtrue"            ) ;

    RegBranch("ttbar_nl"           ) ;
    RegBranch("ttbar_lpt"          ) ;
    RegBranch("ttbar_leta"         ) ;
    RegBranch("ttbar_lphi"         ) ;
    RegBranch("ttbar_lm"           ) ;
    RegBranch("ttbar_lch"          ) ;
    RegBranch("ttbar_lid"          ) ;
    RegBranch("ttbar_trigWord"     ) ;
    RegBranch("ttbar_metpt"        ) ;
    RegBranch("ttbar_nw"           ) ;
    RegBranch("ttbar_w"            ) ;

    RegBranch("nJet"               ) ;
    RegBranch("Jet_pt"             ) ;
    RegBranch("Jet_eta"            ) ;
    RegBranch("Jet_phi"            ) ;
    RegBranch("Jet_mass"           ) ;
    RegBranch("Jet_flavour"        ) ;
    RegBranch("Jet_ntracks"        ) ;
    RegBranch("Jet_SV_multi"       ) ;
    RegBranch("Jet_genpt"          ) ;

    RegBranch("Jet_nFirstTrack"    ) ;
    RegBranch("Jet_nLastTrack"     ) ;
    RegBranch("Track_pt"           ) ;
    RegBranch("Track_nHitAll"      ) ;
    RegBranch("Track_nHitPixel"    ) ;
    RegBranch("Track_chi2"         ) ;
    RegBranch("Track_dz"           ) ;
    RegBranch("Track_dist"         ) ;
    RegBranch("Track_length"       ) ;
    RegBranch("Track_IP"           ) ;
    RegBranch("Track_IPsig"        ) ;
    RegBranch("Track_IP2D"         ) ;

    RegBranch("Jet_nFirstSV"       ) ;
    RegBranch("SV_flight"          ) ;
    RegBranch("SV_flightErr"       ) ;
    RegBranch("SV_deltaR_jet"      ) ;
    RegBranch("SV_vtx_eta"         ) ;
    RegBranch("SV_vtx_phi"         ) ;

    RegBranch("nPFMuon"            ) ;
    RegBranch("PFMuon_pt"          ) ;
    RegBranch("PFMuon_eta"         ) ;
    RegBranch("PFMuon_phi"         ) ;
    RegBranch("PFMuon_ptrel"       ) ;
    RegBranch("PFMuon_deltaR"      ) ;
    RegBranch("PFMuon_GoodQuality" ) ;

    RegBranch("nPFElectron"        ) ;
    RegBranch("PFElectron_pt"      ) ;
    RegBranch("PFElectron_eta"     ) ;
    RegBranch("PFElectron_phi"     ) ;
    RegBranch("PFElectron_ptrel"   ) ;
    RegBranch("PFElectron_deltaR"  ) ;

    // Taggers
    RegBranch("Jet_Ip2P"                ) ;
    RegBranch("Jet_Ip3P"                ) ;
    RegBranch("Jet_ProbaP"              ) ;
    RegBranch("Jet_BprobP"              ) ;
    RegBranch("Jet_Svx"                 ) ;
    RegBranch("Jet_SvxHP"               ) ;
    RegBranch("Jet_CombSvx"             ) ;
    RegBranch("Jet_CombIVF"             ) ;
    RegBranch("Jet_cMVAv2"              ) ;
    RegBranch("Jet_DeepCSVBDisc"        ) ;
    RegBranch("Jet_DeepFlavourBDisc"    ) ;
    RegBranch("Jet_DeepFlavourCvsBDisc" ) ;
    RegBranch("Jet_DeepFlavourCvsLDisc" ) ;
    RegBranch("CTag_Jet_CvsB"           ) ;
    RegBranch("CTag_Jet_CvsL"           ) ;

    printf("INFO\t: All branches settled.\n");
    return;
}//}}}

float CommPlotProducer4ttbar::GetBranch(TString name, int index)
{//{{{
    float out = -1*BIGNUMBER;
    if ( m_leaf.find(name.Data()) != m_leaf.end() ){
        int vIndex = m_leaf.find(name.Data())->second;
        switch (vIndex%100) {
            case 0:
                out = (*v_readerAI[vIndex/100])[index];
                break;
            case 1:
                out = (*v_readerAF[vIndex/100])[index];
                break;
            case 2:
                out = (*v_readerAD[vIndex/100])[index];
                break;
            default:
                printf("ERROR\t: Parse vIndex %d failed.\n",vIndex);
        }
    }else{
        printf("ERROR\t: Branch %s not found!\n", name.Data());
    }
    //printf("DEBUG\t: GetBranch(\"%s\",%d)=%f\n",name.Data(), index, out);
    return out;
}//}}}

void CommPlotProducer4ttbar::AddTagWP(TString tagName, TString varName, float wp)
{//{{{
    if (m_tagWP.find(tagName.Data()) == m_tagWP.end()){
        if (defaultTagWPIndex==-1){
            defaultTagWPIndex = 0;
        }
        m_tagWP[tagName.Data()] = v_tagWP.size();
        v_tagWP.push_back(wp);
        v_tagWPVar.push_back(varName.Data());
        printf("INFO\t: Add tag %s(%.3f) to working point list.\n",tagName.Data(),wp);
    }else{
        printf("WARNING\t: Tag %s already exists. Skip\n",tagName.Data());
    }
}//}}}

bool CommPlotProducer4ttbar::SetDefaultTagWP(TString name)
{//{{{
    if ( m_tagWP.find(name.Data()) == m_tagWP.end()){
        return false;
    };
    defaultTagWPIndex = m_tagWP.find(name.Data())->second;
    return true;
}//}}}

void CommPlotProducer4ttbar::AddSubEventList(TString name, TString cut)
{//{{{
    if (m_evtList.find(name.Data()) == m_evtList.end()){
        m_evtList[name.Data()]=v_evtList.size();// Add index
        TEventList *list = new TEventList(TString::Format("elist_%s",name.Data()).Data());
        v_evtList      .push_back(list);
        cut_evtList    .push_back(cut .Data());
    }
}//}}}

void CommPlotProducer4ttbar::AddHistoBtag(TString name, TString title, int nbins, float min, float max)
{//{{{

    TH1D* h_data   = new TH1D(name       , title+"_data" , nbins , min , max);
    TH1D* h_b      = new TH1D(name+"_b"  , title+"_b"    , nbins , min , max);
    TH1D* h_pu     = new TH1D(name+"_pu" , title+"_pu"   , nbins , min , max);
    TH1D* h_c      = new TH1D(name+"_c"  , title+"_c"    , nbins , min , max);
    TH1D* h_l      = new TH1D(name+"_l"  , title+"_l"    , nbins , min , max);

    h_data     ->Sumw2();
    h_b        ->Sumw2();
    h_pu       ->Sumw2();
    h_c        ->Sumw2();
    h_l        ->Sumw2();

    m_histoBtag[name.Data()] = v_histoBtag.size();
    v_histoBtag.push_back(h_data);
    v_histoBtag.push_back(h_b);
    v_histoBtag.push_back(h_pu);
    v_histoBtag.push_back(h_c);
    v_histoBtag.push_back(h_l);
}//}}}

void CommPlotProducer4ttbar::AddHistoTTbar(TString name, TString title, int nbins, float min, float max)
{//{{{
    TH1D* h = new TH1D(name, title, nbins, min, max);
    h->Sumw2();

    m_histoTTbar[name.Data()] = v_histoTTbar.size();
    v_histoTTbar.push_back(h);
}//}}}

void CommPlotProducer4ttbar::AddHistos(const char* tag)
{//{{{
    // Event Info
    AddHistoTTbar(TString::Format("nPV_%s" ,tag)           , "Number of PV"             , 100 , -0.5 , 99.5);
    AddHistoTTbar(TString::Format("met_%s",tag)            , "MET"                      , 30  , 0    , 300);
    AddHistoTTbar(TString::Format("mll_%s",tag)            , "M_{ll}"                   , 60  , 0    , 300);
    AddHistoTTbar(TString::Format("njet_%s",tag)           , "Number of jets"           , 10  , -0.5 , 9.5);
    AddHistoTTbar(TString::Format("njet_pt30_%s",tag)      , "Number of jets pt30"      , 10  , -0.5 , 9.5);
    AddHistoTTbar(TString::Format("lep0_pt_%s",tag)        , "p_{T}^{leading lep}"      , 50  , 0    , 200);
    AddHistoTTbar(TString::Format("lep1_pt_%s",tag)        , "p_{T}^{sub-leading lep}"  , 50  , 0    , 200);
    AddHistoTTbar(TString::Format("jet0_pt_%s",tag)        , "p_{T}^{leading jet}"      , 50  , 0    , 400);
    for(std::map<std::string, int>::iterator iterTagWP=m_tagWP.begin(); iterTagWP != m_tagWP.end(); iterTagWP++){
        //AddHistoTTbar(TString::Format("nEvt_run_BTag-%s_%s"     ,iterTagWP->first.c_str(),tag) , "Number of evt VS run"             , 40 , 297020 , 306462);//Fill run
        AddHistoTTbar(TString::Format("nBtag_%s_%s"             ,iterTagWP->first.c_str(),tag) , "Number of btag jets"              , 6  , -0.5   , 5.5);
        AddHistoTTbar(TString::Format("nBtag_afterJetSel_%s_%s" ,iterTagWP->first.c_str(),tag) , "Number of btag jets"              , 6  , -0.5   , 5.5);
    }

    // Jet info
    AddHistoBtag(TString::Format("jet_pt_%s"               , tag ) , "pT of all jets"                        , 50  , 0      , 400  ) ;
    AddHistoBtag(TString::Format("jet_eta_%s"              , tag ) , "#eta of all jets"                      , 50  , -2.5   , 2.5  ) ;

    AddHistoBtag(TString::Format("sv_multi_0_%s"           , tag ) , "Number of secondary vertex"            , 6   , -0.5   , 5.5  ) ;
    AddHistoBtag(TString::Format("sv_flight3DSig_%s"       , tag ) , "Flight distance significance 3D"       , 50  , 0.     , 80.  ) ;
    AddHistoBtag(TString::Format("sv_deltaR_jet_%s"        , tag ) , "SV_#Delta{}R_jet"                      , 50  , 0.     , 0.5  ) ;
    AddHistoBtag(TString::Format("sv_eta_%s"               , tag ) , "SV #eta"                               , 50  , -2.5   , 2.5  ) ;
    AddHistoBtag(TString::Format("sv_phi_%s"               , tag ) , "SV #phi"                               , 40  , -1.*PI , PI   ) ;

    AddHistoBtag(TString::Format("track_multi_sel_%s"      , tag ) , "Number of selected tracks in the jets" , 40  , -0.5   , 39.5 ) ;
    AddHistoBtag(TString::Format("track_pt_%s"             , tag ) , "p_{T} of the tracks"                   , 100 , 0.     , 30.  ) ;
    AddHistoBtag(TString::Format("track_nHit_%s"           , tag ) , "Number of hits "                       , 35  , -0.5   , 34.5 ) ;
    AddHistoBtag(TString::Format("track_HPix_%s"           , tag ) , "Number of hits in the Pixel"           , 10  , -0.5   , 9.5  ) ;
    AddHistoBtag(TString::Format("track_chi2_%s"           , tag ) , "Normalized #chi^{2} of the tracks"     , 100 , 0.     , 30.  ) ;
    AddHistoBtag(TString::Format("track_dist_%s"           , tag ) , "Track distance to jet axis length"     , 100 , 0.     , 0.3  ) ;
    AddHistoBtag(TString::Format("track_len_%s"            , tag ) , "Track decay length"                    , 100 , 0.     , 25.  ) ;
    AddHistoBtag(TString::Format("track_IP_%s"             , tag ) , "3D IP of all tracks"                   , 100 , -0.1   , 0.1  ) ;
    AddHistoBtag(TString::Format("track_IPs_%s"            , tag ) , "3D IP significance of all tracks"      , 70 , -35.   , 35.  ) ;

    AddHistoBtag(TString::Format("pfmuon_multi_%s"         , tag ) , "Number of pfmuons"                     , 7   , -0.5   , 6.5  ) ;
    AddHistoBtag(TString::Format("pfmuon_pt_%s"            , tag ) , "PFMuon p_{T}"                          , 50  , 0      , 100  ) ;
    AddHistoBtag(TString::Format("pfmuon_eta_%s"           , tag ) , "PFMuon #eta"                           , 50  , -2.5      , 2.5  ) ;
    AddHistoBtag(TString::Format("pfmuon_phi_%s"           , tag ) , "PFMuon #phi"                           , 40  , -1*PI      , PI  ) ;
    AddHistoBtag(TString::Format("pfmuon_ptrel_%s"         , tag ) , "pT rel. of the muon"                   , 50  , 0      , 5    ) ;

    AddHistoBtag(TString::Format("pfelectron_multi_%s"     , tag ) , "Number of pfelectron"                  , 7   , -0.5   , 6.5  ) ;
    AddHistoBtag(TString::Format("pfelectron_pt_%s"        , tag ) , "PFElectron p_{T}"                      , 50  , 0      , 100  ) ;
    AddHistoBtag(TString::Format("pfelectron_eta_%s"       , tag ) , "PFElectron #eta"                       , 50  , -2.5      , 2.5  ) ;
    AddHistoBtag(TString::Format("pfelectron_phi_%s"       , tag ) , "PFElectron #phi"                       , 40  , -1*PI      , PI  ) ;
    AddHistoBtag(TString::Format("pfelectron_ptrel_%s"     , tag ) , "pT rel. of the pfelectron"             , 50  , 0      , 5    ) ;

    AddHistoBtag(TString::Format("tag_JP_%s"               , tag ) , "JP"                                    , 30  , 0.     , 1.5  ) ;
    AddHistoBtag(TString::Format("tag_JBP_%s"              , tag ) , "JBP"                                   , 25  , 0.     , 4.   ) ;
    AddHistoBtag(TString::Format("tag_SSV_%s"              , tag ) , "CSV"                                   , 70  , 0.     , 7.   ) ;
    AddHistoBtag(TString::Format("tag_SSVHP_%s"            , tag ) , "CSVv2"                                 , 70  , 0.     , 7.   ) ;
    AddHistoBtag(TString::Format("tag_CSV_%s"              , tag ) , "CSV"                                   , 50  , 0.     , 1.   ) ;
    AddHistoBtag(TString::Format("tag_CSVv2_%s"            , tag ) , "CSVv2"                                 , 50  , 0.     , 1.   ) ;
    AddHistoBtag(TString::Format("tag_DeepCSVBDisc_%s"     , tag ) , "DeepCSVBDisc"                          , 50  , 0.     , 1.   ) ;
    AddHistoBtag(TString::Format("tag_DeepFlavourBDisc_%s" , tag ) , "DeepFlavourBDisc"                      , 50  , 0.     , 1.   ) ;
    AddHistoBtag(TString::Format("tag_cMVAv2_%s"           , tag ) , "cMVAv2"                                , 50  , -1.    , 1.   ) ;
    AddHistoBtag(TString::Format("tag_CvsB_%s"             , tag ) , "CvsB"                                  , 50  , -1.    , 1.   ) ;
    AddHistoBtag(TString::Format("tag_CvsL_%s"             , tag ) , "CvsL"                                  , 50  , -1.    , 1.   ) ;
    AddHistoBtag(TString::Format("tag_TCHP_%s"             , tag ) , "TCHP"                                  , 50  , 0.     , 30.  ) ;
    AddHistoBtag(TString::Format("tag_TCHE_%s"             , tag ) , "TCHP"                                  , 50  , 0.     , 30.  ) ;
    AddHistoBtag(TString::Format("tag_DeepFlavourCvsB_%s"  , tag ) , "DeepFlavour CvsB"                      , 50  , -1.    , 1.   ) ;
    AddHistoBtag(TString::Format("tag_DeepFlavourCvsL_%s"  , tag ) , "DeepFlavour CvsL"                      , 50  , -1.    , 1.   ) ;
    return;
}//}}}

void CommPlotProducer4ttbar::AddAllHistos()
{//{{{
    printf("INFO\t: Create histograms.\n");
    fout->cd();
    totalGenEvts            = new TH1D("totalGenEvts"    , "" , 1 , 0 , 1);// For overall scaling , e.g. lumi , xsec , etc..
    puWgtNorms              = new TH1D("puWgtNorms"      , "" , 1 , 0 , 1);// Sum of weight (Alphabetical)

    // For all sub-EventList
    for(std::map<std::string, int>::iterator iterEvL=m_evtList.begin(); iterEvL != m_evtList.end(); iterEvL++){
        AddHistos(iterEvL->first.c_str());
    }
    printf("INFO\t: All histograms created.\n");
}//}}}

template<typename varType>
void CommPlotProducer4ttbar::FillHistoBtag(TString name, int flavour, bool isPU, varType value, double weight)
{//{{{
    if (m_histoBtag.find(name.Data()) == m_histoBtag.end()){
        printf("ERROR\t: HistoBtag %s does not exist.\n",name.Data());
        return;
    }else{
        //printf("DEBUG\t: Fill HistoBtag %s.\n",name.Data());
    }

    int vIndex = m_histoBtag[name.Data()] ;
    if (!isData){
        if (isPU){//pu
            v_histoBtag[vIndex +2]->Fill(value,weight);
        }else{
            switch(abs(flavour)){
                case 5://b
                    v_histoBtag[vIndex +1]->Fill(value,weight);
                    break;
                case 4://c
                    v_histoBtag[vIndex +3]->Fill(value,weight);
                    break;
                case 3://l
                case 2:
                case 1:
                case 0:
                case 21:
                    v_histoBtag[vIndex +4]->Fill(value,weight);
                    break;
                default:
                    printf("ERROR\t: Unknown flavour %d.\n",flavour);
            }
        }
    }else{
        v_histoBtag[vIndex +0]->Fill(value);
    }
}//}}}

template<typename varType>
void CommPlotProducer4ttbar::FillHistoTTbar(TString name, varType value, double weight)
{//{{{
    if (m_histoTTbar.find(name.Data()) == m_histoTTbar.end()){
        printf("ERROR\t: HistoTTbar %s does not exist.\n",name.Data());
        return;
    }else{
        //printf("DEBUG\t: Fill HistoTTbar %s.\n",name.Data());
    }
    int vIndex = m_histoTTbar[name.Data()] ;
    v_histoTTbar[vIndex]->Fill(value,weight);
}//}}}

bool CommPlotProducer4ttbar::passPresel(int chan)
{//{{{
    if (fabs(GetBranch("ttbar_nl")-2) > 0.1 )                               return false;
    if (GetBranch("nJet",0) < 2. )                                          return false;

    // Lepton
    if (fabs(GetBranch("ttbar_lid",0)*GetBranch("ttbar_lid",1)*GetBranch("ttbar_lch",0)*GetBranch("ttbar_lch",1)-chan) > 0.1) return false;

    TLorentzVector lep0, lep1;
    lep0.SetPtEtaPhiM(GetBranch("ttbar_lpt",0),GetBranch("ttbar_leta",0),GetBranch("ttbar_lphi",0),GetBranch("ttbar_lm",0));
    lep1.SetPtEtaPhiM(GetBranch("ttbar_lpt",1),GetBranch("ttbar_leta",1),GetBranch("ttbar_lphi",1),GetBranch("ttbar_lm",1));
    if (lep0.Pt() < 25. || lep1.Pt() < 25.) return false;
    if ((lep0+lep1).M() < 12) return false;

    return true;
}//}}}

void CommPlotProducer4ttbar::AddTrigChannel(int chan)
{//{{{
    // To be called in python to add trigger paths. MIND the order.
    trigChannels.push_back(chan);
}//}}}

bool CommPlotProducer4ttbar::passTrigSel(int chan)
{//{{{
    // Pass the corresponding 
    if (!isData) return true;
    if (trigChannels.size() == 0) return true;

    bool isTriggered = false;
    int triggerWord = round(GetBranch("ttbar_trigWord",0));
    for(unsigned int iCh=0; iCh<trigChannels.size(); iCh++){
        if (abs(trigChannels[iCh]) != abs(chan)) continue;
        isTriggered |= ((triggerWord>>iCh) & 1);
    }
    return isTriggered;
}//}}}

std::vector<int> CommPlotProducer4ttbar::passJetSel(int chan)
{//{{{
    TLorentzVector lep0, lep1;
    lep0.SetPtEtaPhiM(GetBranch("ttbar_lpt",0),GetBranch("ttbar_leta",0),GetBranch("ttbar_lphi",0),GetBranch("ttbar_lm",0));
    lep1.SetPtEtaPhiM(GetBranch("ttbar_lpt",1),GetBranch("ttbar_leta",1),GetBranch("ttbar_lphi",1),GetBranch("ttbar_lm",1));

    std::vector<int> selJets;
    unsigned int nJ = round(GetBranch("nJet",0));
    for(unsigned int iJ=0; iJ<nJ; iJ++){
        TLorentzVector vJ;
        float Jet_pt    = GetBranch("Jet_pt",iJ);
        float Jet_eta   = GetBranch("Jet_eta",iJ);
        float Jet_phi   = GetBranch("Jet_phi",iJ);
        float Jet_mass  = GetBranch("Jet_mass",iJ);
        vJ.SetPtEtaPhiM(Jet_pt,Jet_eta,Jet_phi,Jet_mass);

        bool isSelJet = true;
        if ( vJ.Pt() < 25 )             isSelJet = false;
        if ( fabs(vJ.Eta()) > 2.4 )     isSelJet = false;
        if ( vJ.DeltaR(lep0) < 0.4 )    isSelJet = false;
        if ( vJ.DeltaR(lep1) < 0.4 )    isSelJet = false;
        if (isSelJet) selJets.push_back(iJ);
    }
    return selJets;
}//}}}

bool CommPlotProducer4ttbar::passTTbarSel(int chan, std::vector<int> &selJets)
{//{{{
    if ( selJets.size() < 2 ) return false;
    return true;
}//}}}

std::bitset<8> CommPlotProducer4ttbar::passTrackSel(int chan, int trkIdx)
{//{{{
    std::bitset<8> bits(0);
    if (GetBranch("Track_nHitAll",trkIdx)>=0)           bits[0]=1;// HIP mitigation
    if (GetBranch("Track_nHitPixel",trkIdx)>=1)         bits[1]=1;// HIP mitigation
    if (fabs(GetBranch("Track_dz",trkIdx))<17)          bits[2]=1;
    if (GetBranch("Track_pt",trkIdx)>1)                 bits[3]=1;
    if (GetBranch("Track_chi2",trkIdx)<5)               bits[4]=1;
    if (fabs(GetBranch("Track_dist",trkIdx))<0.07)      bits[5]=1;
    if (GetBranch("Track_length",trkIdx)<5)             bits[6]=1;
    if (fabs(GetBranch("Track_IP2D",trkIdx))<0.2)       bits[7]=1;

    return bits;
}//}}}

void CommPlotProducer4ttbar::calcPUWgt(float *wgt)
{//{{{
    if (!isData && puWgtGr != 0){
        int nPUtrue = round(GetBranch("nPUtrue",0));
        wgt[0] = nPUtrue < 95 && nPUtrue >= 0 ? puWgtGr  ->Eval(nPUtrue) : 0;
        wgt[1] = nPUtrue < 95 && nPUtrue >= 0 ? puWgtDoGr->Eval(nPUtrue) : 0;
        wgt[2] = nPUtrue < 95 && nPUtrue >= 0 ? puWgtUpGr->Eval(nPUtrue) : 0;
    }
}//}}}

void CommPlotProducer4ttbar::calcQCDWgt(float *wgt, TH1F *wgtcounter)
{//{{{
    if (!isData && GetBranch("ttbar_nw",0) > 17){
        wgt[1] = GetBranch("ttbar_w",9)*(wgtcounter->GetBinContent(10)/wgtcounter->GetBinContent(1))/GetBranch("ttbar_w",0);
        wgt[2] = GetBranch("ttbar_w",5)*(wgtcounter->GetBinContent( 6)/wgtcounter->GetBinContent(1))/GetBranch("ttbar_w",0);
    }
}//}}} 

void CommPlotProducer4ttbar::calcTrigWgt(float *wgt, int chan)
{//{{{
    // https://indico.cern.ch/event/434078/#preview:1614815
    if (chan == -11*13){
        double mean(0.901), err(0.015);
        wgt[1] = (mean-err)/mean;
        wgt[2] = (mean+err)/mean;
    }
}//}}}

void CommPlotProducer4ttbar::calcLepEffWgt(float *wgt)
{//{{{
    // CMS AN 022/2015 v15
    auto eff = [=](int flav, double pt, double eta) -> std::pair<float, float>{
        std::pair<float, float> res(1.,0.);
        if(abs(flav)==11){
            if (fabs(eta)<0.8){
                if (pt<30)      { res.first=0.927; res.second=0.073; }
                else if (pt<40) { res.first=0.975; res.second=0.018; }
                else if (pt<50) { res.first=0.962; res.second=0.036; }
                else            { res.first=0.955; res.second=0.022; }
            }else if (fabs(eta)<1.5){
                if (pt<30)      { res.first=0.891; res.second=0.074; }
                else if (pt<40) { res.first=0.965; res.second=0.020; }
                else if (pt<50) { res.first=0.968; res.second=0.018; }
                else            { res.first=0.955; res.second=0.018; }
            }else{
                if (pt<30)      { res.first=0.956; res.second=0.059; }
                else if (pt<40) { res.first=0.995; res.second=0.018; }
                else if (pt<50) { res.first=0.993; res.second=0.019; }
                else            { res.first=0.985; res.second=0.023; }
            }
        }else if (abs(flav)==13){
            if (fabs(eta)<0.9){
                if (pt<30)      { res.first=1.003; res.second=0.019; }
                else if (pt<40) { res.first=1.014; res.second=0.015; }
                else if (pt<50) { res.first=1.001; res.second=0.014; }
                else            { res.first=0.983; res.second=0.014; }
            }else if(fabs(eta)<1.2){
                if (pt<30)      { res.first=0.993; res.second=0.019; }
                else if (pt<40) { res.first=0.994; res.second=0.015; }
                else if (pt<50) { res.first=0.980; res.second=0.014; }
                else            { res.first=0.987; res.second=0.015; }
            }else{
                if (pt<30)      { res.first=1.023; res.second=0.028; }
                else if (pt<40) { res.first=0.994; res.second=0.014; }
                else if (pt<50) { res.first=0.996; res.second=0.014; }
                else            { res.first=0.979; res.second=0.014; }
            }
        }
        return res;
    };
    std::pair<float, float> lepEff0 = eff(round(GetBranch("ttbar_lid",0)),
                                          GetBranch("ttbar_lpt",0),
                                          GetBranch("ttbar_leta",0));
    std::pair<float, float> lepEff1 = eff(round(GetBranch("ttbar_lid",1)),
                                          GetBranch("ttbar_lpt",1),
                                          GetBranch("ttbar_leta",1));
    wgt[0] = lepEff0.first*lepEff1.first;
    wgt[1] = (lepEff0.first-lepEff0.second)*(lepEff1.first-lepEff1.second)/wgt[0];
    wgt[2] = (lepEff0.first+lepEff0.second)*(lepEff1.first+lepEff1.second)/wgt[0];
}//}}}

#endif
