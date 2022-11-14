void IV(){
    TCanvas *canvas = new TCanvas("canvas", "Data", 1400, 700);
    TPad *pad1 = new TPad("pad1", "Graph", 0, 0, .5, 1);
    TPad *pad2 = new TPad("pad2", "Log", 0.5, 0, 1, 1);
    pad1->cd();
    gStyle->SetPadGridX(true);
    gStyle->SetPadGridY(true);
    TGraphErrors* gr = new TGraphErrors("data.dat","%lg %lg %lg");
    gr->SetTitle("Caratteristica I-V ;ddp ai capi del Diodo [mV];Corrente nel diodo [mA]");
    gr->Draw("APE");

    TF1* f = new TF1("fit","[0]*(e^(x/[1])-1)");
    f->SetParameters(1e-6,50);

    gr->Fit("fit");
    

    TLegend* leg = new TLegend(0.78, 0.85, .99, .99);
    leg->AddEntry(gr,"Punti sperimentali","lep");
    leg->AddEntry(gr->GetFunction("fit"),"Fit","l");
    leg->Draw("SAME");
    

    canvas->cd();
    pad1->Draw();
    
    std::fstream in;
    in.open("fit.txt",std::fstream::out);
    in<<"Fit function is like : I = I0(exp(v/etaV)-1) \n"<<"I0="<<gr->GetFunction("fit")->GetParameter("0")<<"+/-"<<gr->GetFunction("fit")->GetParError(0)<<"mA   etaV="<<gr->GetFunction("fit")->GetParameter("1")<<"+/-"<<gr->GetFunction("fit")->GetParError(1)<<" mV\n";
}