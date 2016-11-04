{

    float luminosity = 2488; // /pb  see mbuttign@lxplus0058:~/BTagCommissioningPlots/CMSSW_7_6_3/src/RecoBTag/PerformanceMeasurements/test/ttbar and do brilcalc lumi -i lumiSummary_MuonEG_CD.json

    //std::string samplepath="root://eoscms.cern.ch//eos/cms/store/caf/user/jtsai/BTaggingCommission/Run2015_25ns";
    std::string samplepath="root://eoscms//eos/cms/store/caf/user/jtsai/BTaggingCommission/Run2015_25ns";
    std::string sampleroot="MergedJetTree.root";

    // initialisation
    //TFile* file_data = TFile::Open((samplepath+"/Data13TeV_MuonEG_2015CD/"+sampleroot).c_str());
    //TChain* tree_data = (TChain*) file_data->Get("btagana/ttree");

    double n_ttbar=0, wgtcounter_ttbar=0;
    double n_dy1=0, wgtcounter_dy1=0;
    double n_dy2=0, wgtcounter_dy2=0;
    double n_st1=0, wgtcounter_st1=0;
    double n_st2=0, wgtcounter_st2=0;
    double n_ww=0,  wgtcounter_ww=0;
    double n_wz=0,  wgtcounter_wz=0;
    double n_zz=0,  wgtcounter_zz=0;

    //TFile* file_ttbar = TFile::Open((samplepath+"/MC13TeV_TTJets_powheg_pythia8/"+sampleroot).c_str());
    //TH1F* inputWeight = (TH1F*)file_ttbar->Get("ttbarselectionproducer/wgtcounter");
    //wgtcounter_ttbar = inputWeight->GetBinContent(1);
    //TChain* tree_ttbar = (TChain*) file_ttbar->Get("btagana/ttree");

    TFile* file_dy1 = TFile::Open((samplepath+"/MC13TeV_DY10to50/"+sampleroot).c_str());
    TH1F* inputWeight2 = (TH1F*)file_dy1->Get("ttbarselectionproducer/wgtcounter");
    wgtcounter_dy1 = inputWeight2->GetBinContent(1);
    TChain* tree_dy1 = (TChain*) file_dy1->Get("btagana/ttree");

    TFile* file_dy2 = TFile::Open((samplepath+"/MC13TeV_DY50toInf/"+sampleroot).c_str());
    TH1F* inputWeight3 = (TH1F*)file_dy2->Get("ttbarselectionproducer/wgtcounter");
    wgtcounter_dy2 = inputWeight3->GetBinContent(1);
    TChain* tree_dy2 = (TChain*) file_dy2->Get("btagana/ttree");

    TFile* file_st1 = TFile::Open((samplepath+"/MC13TeV_SingleT_tW/"+sampleroot).c_str());
    TH1F* inputWeight4 = (TH1F*)file_st1->Get("ttbarselectionproducer/wgtcounter");
    wgtcounter_st1 = inputWeight4->GetBinContent(1);
    TChain* tree_st1 = (TChain*) file_st1->Get("btagana/ttree");

    TFile* file_st2 = TFile::Open((samplepath+"/MC13TeV_SingleTbar_tW/"+sampleroot).c_str());
    TH1F* inputWeight5 = (TH1F*)file_st2->Get("ttbarselectionproducer/wgtcounter");
    wgtcounter_st2 = inputWeight5->GetBinContent(1);
    TChain* tree_st2 = (TChain*) file_st2->Get("btagana/ttree");

    TFile* file_ww = TFile::Open((samplepath+"/MC13TeV_WWTo2L2Nu/"+sampleroot).c_str());
    TH1F* inputWeight6 = (TH1F*)file_ww->Get("ttbarselectionproducer/wgtcounter");
    wgtcounter_ww = inputWeight6->GetBinContent(1);
    TChain* tree_ww = (TChain*) file_ww->Get("btagana/ttree");

    TFile* file_wz = TFile::Open((samplepath+"/MC13TeV_WZ/"+sampleroot).c_str());
    TH1F* inputWeight7 = (TH1F*)file_wz->Get("ttbarselectionproducer/wgtcounter");
    wgtcounter_wz = inputWeight7->GetBinContent(1);
    TChain* tree_wz = (TChain*) file_wz->Get("btagana/ttree");

    TFile* file_zz = TFile::Open((samplepath+"/MC13TeV_ZZ/"+sampleroot).c_str());
    TH1F* inputWeight8 = (TH1F*)file_zz->Get("ttbarselectionproducer/wgtcounter");
    wgtcounter_zz = inputWeight8->GetBinContent(1);
    TChain* tree_zz = (TChain*) file_zz->Get("btagana/ttree");

    float sf_dy=1.3;


    //CommPlotProducer4ttbar *m_data = new CommPlotProducer4ttbar(tree_data,1,1);
    //m_data->Loop(0,3, 30, 500, outdir+"output_data_mueg", 0, "");

    TString outdir="02May_test";

    //CommPlotProducer4ttbar *m_ttbar = new CommPlotProducer4ttbar(tree_ttbar,1,1);
    //m_ttbar->SetNorm(831.77*luminosity/wgtcounter_ttbar);  
    //m_ttbar->Loop(1,3, 30, 500, outdir+"/output_ttbar", inputWeight, "");

    CommPlotProducer4ttbar *m_dy1 = new CommPlotProducer4ttbar(tree_dy1,1,1);
    m_dy1->SetNorm(18610*sf_dy*luminosity/wgtcounter_dy1);
    m_dy1->Loop(2,3, 30, 500, outdir+"/output_dy1", inputWeight2, "");

    CommPlotProducer4ttbar *m_dy2 = new CommPlotProducer4ttbar(tree_dy2,1,1);
    m_dy2->SetNorm(6025*sf_dy*luminosity/wgtcounter_dy2);
    m_dy2->Loop(2,3, 30, 500, outdir+"/output_dy2", inputWeight3, "");

    CommPlotProducer4ttbar *m_st1 = new CommPlotProducer4ttbar(tree_st1,1,1);
    m_st1->SetNorm(35.85*luminosity/wgtcounter_st1);
    m_st1->Loop(3,3, 30, 500, outdir+"/output_st1", inputWeight4, "");

    CommPlotProducer4ttbar *m_st2 = new CommPlotProducer4ttbar(tree_st2,1,1);
    m_st2->SetNorm(35.85*luminosity/wgtcounter_st2);
    m_st2->Loop(3,3, 30, 500, outdir+"/output_st2", inputWeight5, "");

    CommPlotProducer4ttbar *m_ww = new CommPlotProducer4ttbar(tree_ww,1,1);
    m_ww->SetNorm(12.178*luminosity/wgtcounter_ww);
    m_ww->Loop(4,3, 30, 500, outdir+"/output_ww", inputWeight6, "");

    CommPlotProducer4ttbar *m_wz = new CommPlotProducer4ttbar(tree_wz,1,1);
    m_wz->SetNorm(47.13*luminosity/wgtcounter_wz);
    m_wz->Loop(5,3, 30, 500, outdir+"/output_wz", inputWeight7, "");

    CommPlotProducer4ttbar *m_zz = new CommPlotProducer4ttbar(tree_zz,1,1);
    m_zz->SetNorm(16.5*luminosity/wgtcounter_zz);
    m_zz->Loop(6,3, 30, 500, outdir+"/output_zz", inputWeight8, "");

}
