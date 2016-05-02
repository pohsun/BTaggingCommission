{
    gROOT->ProcessLine(".L ../TTbarSelector.C++"); 
    gROOT->ProcessLine(".L CommPlotProducer4ttbar.C++"); 
    gROOT->ProcessLine(".x runCode4ttbarNominal.C++");
}
