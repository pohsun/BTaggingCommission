{
    gROOT->ProcessLine(".L DrawCommPlot4ttbar.C++");

    ////// In BTV-15-001:

    //Draw("track_IPs" , "3D IP significance of tracks",1); 
    //Draw("sv_flight3DSig","SV 3D flight distance significance",1); 
    //Draw("tagvarCSV_vertexmass_cat0","SV mass [GeV]",0); 
    //Draw("JP" ,"JP Discriminator",1); 
    //Draw("CSVv2","CSVv2 Discriminator",1); 
    //Draw("CSV" ,"CSVv2(AVR) Discriminator",1); 
    //Draw("JBP" ,"JBP Discriminator",1); 
    //Draw("SoftMu" ,"SM Discriminator",1); 
    //Draw("SoftEl" ,"SE Discriminator",1); 
    //Draw("cMVAv2","cMVAv2 Discriminator",1);

    //DrawTTbar("nbtag_all_afterJetSel_CSVv2M_SFapplied","number of b-tagged jets (CSVv2M)",0); 
    //DrawTTbar("nbtag_all_afterJetSel_CSVv2M","number of b-tagged jets (CSVv2M)",0);

    ////// In AN-16-036:

    Draw("jet_pt_all" ,"Jet pT",1); 
    Draw("jet_eta" ,"Jet eta", 0); 
    Draw("trk_multi_sel" , "Number of selected tracks in the jets",0); 
    Draw("track_pt" , "Track p_{T}",1); 
    Draw("track_nHit" , "number of hits",0); 
    Draw("track_HPix" , "Number of hits in the Pixel",0); 
    Draw("track_chi2" , "Normalized #chi^{2} of tracks" ,1); 
    Draw("track_dist" , "Track distance to the jet axis" ,1); 
    Draw("track_len" , "Track decay length",1); 
    Draw("track_IP" , "3D IP of tracks",1); 
    Draw("track_IPs" , "3D IP significance of tracks",1); 
    Draw("track_IPs" , "3D IP significance of tracks",0); 
    Draw("sv_multi_0","nr. of SV including bin 0",1); 
    Draw("sv_flight3DSig","SV 3D flight distance significance",1); 
    Draw("sv_deltaR_jet","Delta R between the jet and the SV direction.",0); 
    Draw("sv_phi","#phi",0); 
    Draw("sv_eta","#eta",0); 
    Draw("tagvarCSV_vertexmass_cat0","SV mass [GeV]",0); 
    Draw("tagvarCSV_vertexmass3trk_cat0","SV mass (at least 3 tracks) [GeV]",0); 
    Draw("tagvarCSV_vertexCategory","Vertex Category",1); 
    Draw("pfmuon_multi" , "number of pf muons", 1); 
    Draw("pfmuon_pt" , "p_{T} of pf muons [GeV]",1); 
    Draw("pfmuon_ptrel" , "p_{T} rel. of pf muons [GeV]",0); 
    Draw("JP" ,"JP Discriminator",1); 
    Draw("JBP" ,"JBP Discriminator",1); 
    Draw("CSVv2","CSVv2 Discriminator",1); 
    Draw("cMVAv2","cMVAv2 Discriminator",1); 
    Draw("CSV" ,"CSVv2(AVR) Discriminator",1); 
    Draw("CvsB" ,"C-tag CvsB Discriminator",1); 
    Draw("CvsL" ,"C-tag CvsL Discriminator",1); 

    //////// not anymore in the tree Draw("SoftEl" ,"SE Discriminator",1); Draw("SoftMu" ,"SM Discriminator",1); Draw("TCHP" ,"TCHP Discriminator",1); Draw("discri_ssche0", "SSVHE Discriminator", 1);
    Draw("SoftEl" ,"SE Discriminator",1); 
    Draw("SoftMu" ,"SM Discriminator",1); 
    Draw("TCHP" ,"TCHP Discriminator",1); 
    Draw("discri_ssche0", "SSVHE Discriminator", 1);

    DrawTTbar("njet_pt30" ,"number of jes", 1);
    DrawTTbar("nPV", "number of PV",0);
    DrawTTbar("nPV_unweighted", "number of PV (noWrt)",0);
    DrawTTbar("met", "MET (GeV)",0);
    DrawTTbar("mll", "M_{ll} (GeV)",0);
    DrawTTbar("njet","number of jets",0);
    //DrawTTbar("nbtag","number of btag jets",0);
    DrawTTbar("pt_e","Leading electron P_{T} (GeV)",0);
    DrawTTbar("pt_mu","Leading muon P_{T} (GeV)",0);
    DrawTTbar("pt_jet","Leading jet P_{T} (GeV)",0);
    DrawTTbar("nbtag_all_afterJetSel_CSVv2L","number of b-tagged jets (CSVv2L)",   1); 
    DrawTTbar("nbtag_all_afterJetSel_CSVv2M","number of b-tagged jets (CSVv2M)",   1); 
    DrawTTbar("nbtag_all_afterJetSel_CSVv2T","number of b-tagged jets (CSVv2T)",   1); 
    DrawTTbar("nbtag_all_afterJetSel_cMVAv2L","number of b-tagged jets (cMVAv2L)", 1); 
    DrawTTbar("nbtag_all_afterJetSel_cMVAv2M","number of b-tagged jets (cMVAv2M)", 1); 
    DrawTTbar("nbtag_all_afterJetSel_cMVAv2T","number of b-tagged jets (cMVAv2T)", 1); 
}
