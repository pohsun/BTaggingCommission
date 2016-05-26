{
    const int i=runN; 
    //const int i=1; 

    // Options for CommPlotProducer4ttbar::Loop
    int trig_data=3;
    float PtMin_Cut=30;   
    float PtMax_Cut=500;
    TString syst="";   
 
    // Samples defination
    //Data13TeV_MuonEG_2016B
    //MC13TeV_DY10to50
    //MC13TeV_DY50toInf
    //MC13TeV_SingleT_tW
    //MC13TeV_SingleTbar_tW
    //MC13TeV_TTJets_powheg_pythia8
    //MC13TeV_WW
    //MC13TeV_WZ
    //MC13TeV_ZZ
    //
    const int Nsamples=9;
    TString iname[Nsamples] = { "Data13TeV_MuonEG_2016B",        // 0
                                "MC13TeV_TTJets_powheg_pythia8", // 1
                                "MC13TeV_DY10to50",              // 2
                                "MC13TeV_DY50toInf",             // 3
                                "MC13TeV_SingleT_tW",            // 4
                                "MC13TeV_SingleTbar_tW",         // 5
                                "MC13TeV_WW",                    // 6
                                //"MC13TeV_WWTo2L2Nu",             // 6
                                "MC13TeV_WZ",                    // 7 
                                "MC13TeV_ZZ"                     // 8
                                }; 
    TString oname[Nsamples] = { "output_data_mueg",
                                "output_ttbar",
                                "output_dy1",
                                "output_dy2",
                                "output_st1",
                                "output_st2",
                                "output_ww",
                                "output_wz",
                                "output_zz" 
                                };
 
    int datatype[Nsamples] = {0,      1,     2,    2,     3,     3,      4,     5,    6 }; 
    //float xsec[Nsamples]   = {0, 831.77, 18610, 6025, 35.85, 35.85, 12.178, 47.13, 16.5 };
    float xsec[Nsamples]   = {0, 831.77, 18610, 6025, 35.85, 35.85, 118.7, 47.13, 16.5 };
    float luminosity = 215.829; // /pb see usage.txt 

    TString samplepath="root://eoscms.cern.ch//eos/cms/store/group/phys_btag/Commissioning/TTbar/Run2016/May2016_v0";
    TString sampleroot="MergedJetTree.root";

    // Run... 
    TFile* f = TFile::Open(samplepath+"/"+iname[i]+"/"+sampleroot);
    TChain* tree = (TChain*)f->Get("btagana/ttree");
    CommPlotProducer4ttbar *run = new CommPlotProducer4ttbar(tree,1,1);

    if( xsec[i] != 0 )
    {
        TH1F* inputWeight = (TH1F*)f->Get("ttbarselectionproducer/wgtcounter");
        double wgtcounter = inputWeight->GetBinContent(1);
        run->SetNorm(xsec[i]*luminosity/wgtcounter);  
        run->Loop( datatype[i], trig_data, PtMin_Cut, PtMax_Cut, oname[i], inputWeight, syst);
    }
    else
    {
        run->Loop( datatype[i], trig_data, PtMin_Cut, PtMax_Cut, oname[i], 0, syst);
    }
}
