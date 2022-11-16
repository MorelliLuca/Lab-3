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
    gr->SetTitle("Caratteristica I-V diodio al germanio ;ddp ai capi del Diodo [mV];Corrente nel diodo [mA]");
    gr->Draw("APE");

    TF1* f = new TF1("fit","[0]*(e^(x/[1])-1)");
    f->SetParameters(1e-6,50);

    gr->Fit("fit","","",50,150);
    

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