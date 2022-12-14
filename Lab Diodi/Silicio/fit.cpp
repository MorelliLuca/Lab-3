void calibration(){
    gStyle->SetPadGridX(true);
    gStyle->SetPadGridY(true);
    TGraphErrors* gr = new TGraphErrors("calibrazione.dat","%lg %lg %lg");
    gr->SetTitle("Calibrazione multimetro-oscilloscopio;ddp Multimetro [mV];ddp Oscilloscopio [mV]");
    gr->Draw("APE");

    double s1{0},sx{0},sy{0},sxx{0},sxy{0},d{0},min{100},max{-1};
    int NPoints{gr->GetN()};

    for(int i{0};i<NPoints;++i){
        double e = gr->GetErrorY(i);
        double x = gr->GetX()[i];
        double y = gr->GetY()[i];

        s1+=1/(e*e);
        sx+=x/(e*e);
        sy+=y/(e*e);
        sxx+=x*x/(e*e);
        sxy+=x*y/(e*e);

        if(x<min)min=x;
        if(x>max)max=x;

    }

    d=s1*sxx-sx*sx;

    TF1* f = new TF1("fit","[0]+[1]*x",min-1,max+1);
    f->SetParameters((sy*sxx-sx*sxy)/d,(s1*sxy-sx*sy)/d);
    f->Draw("SAME");


    TLegend* leg = new TLegend(0.78, 0.85, .99, .99);
    leg->AddEntry(gr,"Punti sperimentali","lep");
    leg->AddEntry(f,"Fit","l");
    leg->Draw("SAME");

    std::fstream in;
    in.open("calibrazione.txt",std::fstream::out);
    in<<"Fit function is like : y = mx +q \n"<<"q="<<(sy*sxx-sx*sxy)/d<<"+/-"<<sqrt(sxx/d)<<"mV   m="<<(s1*sxy-sx*sy)/d<<"+/-"<<sqrt(1/d)<<'\n';
}

void IV(){
    TCanvas *canvas = new TCanvas("canvas", "Data", 1400, 700);
    TPad *pad1 = new TPad("pad1", "Graph", 0, 0, .5, 1);
    TPad *pad2 = new TPad("pad2", "Log", 0.5, 0, 1, 1);
    pad1->cd();
    gStyle->SetPadGridX(true);
    gStyle->SetPadGridY(true);
    TGraphErrors* gr = new TGraphErrors("data.dat","%lg %lg %lg %lg");
    gr->SetTitle("Caratteristica I-V del diodo al silicio;ddp ai capi del Diodo [mV];Corrente nel diodo [mA]");
    gr->Draw("APE");

    TF1* f = new TF1("fit","[0]*(e^(x/[1])-1)");
    f->SetParameters(1e-6,50);

    gr->Fit("fit","E0","",400,625);
    

    TLegend* leg = new TLegend(0.78, 0.85, .99, .99);
    leg->AddEntry(gr,"Punti sperimentali","lep");
    leg->AddEntry(gr->GetFunction("fit"),"Fit","l");
    leg->Draw("SAME");
    pad1->SetGridx();
    pad1->SetGridy();
    gr->GetFunction("fit")->Draw("SAME");

    
    

    double s1{0},sx{0},sy{0},sxx{0},sxy{0},d{0},min{100},max{-1};
    int NPoints{gr->GetN()};

    for(int i{0};i<NPoints-5;++i){
        double e = gr->GetErrorX(i);
        double y = gr->GetX()[i];
        double x = log(gr->GetY()[i]);

        s1+=1/(e*e);
        sx+=x/(e*e);
        sy+=y/(e*e);
        sxx+=x*x/(e*e);
        sxy+=x*y/(e*e);

        if(x<min)min=x;
        if(x>max)max=x;

    }

    d=s1*sxx-sx*sx;

    TF1* fl = new TF1("fitl","e^(-[0]/[1])*(e^(x/[1]) - 1)",0,1000);
    fl->SetParameters((sy*sxx-sx*sxy)/d,(s1*sxy-sx*sy)/d);
    pad2->cd();
    pad2->SetGridx();
    pad2->SetGridy();
    pad2->SetLogy();
    TGraphErrors* grl = new TGraphErrors("data.dat","%lg %lg %lg %lg");
    grl->SetTitle("Caratteristica I-V del diodo al silicio fit lineare in scala semilogaritmica;ddp ai capi del Diodo [mV];Corrente nel diodo [mA]");
    grl->Draw("APE");
    leg->Draw("SAME");
    fl->Draw("SAME");

    canvas->cd();
    pad1->Draw();
    pad2->Draw();

    double correction{sqrt(gr->GetFunction("fit")->GetChisquare()/gr->GetFunction("fit")->GetNDF())};
    
    std::fstream in;
    in.open("fit.txt",std::fstream::out);
    in<<"Fit function is like : I = I0(exp(v/etaV)-1) \n"<<"I0="<<gr->GetFunction("fit")->GetParameter("0")<<"+/-"<<gr->GetFunction("fit")->GetParError(0)<<"mA   etaV="<<gr->GetFunction("fit")->GetParameter("1")<<"+/-"<<gr->GetFunction("fit")->GetParError(1)<<" mV\n"<<
      "Chisqr error correction I0="<<gr->GetFunction("fit")->GetParameter("0")<<"+/-"<<gr->GetFunction("fit")->GetParError(0)*correction<<"mA   etaV="<<gr->GetFunction("fit")->GetParameter("1")<<"+/-"<<gr->GetFunction("fit")->GetParError(1)*correction<<" mV\n"<<
      "Linear fit method results I0="<<exp(-1*(sy*sxx-sx*sxy)/(s1*sxy-sx*sy))<<"+/-"<<exp(-1*(sy*sxx-sx*sxy)/d/((s1*sxy-sx*sy)/d))*(sqrt(sxx/d)/((sy*sxx-sx*sxy)/d)+sqrt(s1/d)/((s1*sxy-sx*sy)/d))<<"mA   etaV="<<(s1*sxy-sx*sy)/d<<"+/-"<<sqrt(s1/d)<<" mV\n";
}