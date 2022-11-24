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
    gStyle->SetPadGridX(true);
    gStyle->SetPadGridY(true);
    gStyle->SetPadTickX(true);
    gStyle->SetPadTickY(true);
    TGraphErrors* gr = new TGraphErrors("data.dat","%lg %lg %lg %lg");
    gr->SetTitle("Caratteristica I-V Transistor emettitore comune e I_{B}=200#muA;V_{CE} [mV]; I_{C} [mA]");
    gr->Draw("APE");
    

    TLegend* leg = new TLegend(0.78, 0.85, .99, .99);
    leg->AddEntry(gr,"Punti sperimentali","lep");
    leg->AddEntry(gr->GetFunction("fit"),"Fit","l");
    leg->Draw("SAME");  
    

    double s1{0},sx{0},sy{0},sxx{0},sxy{0},d{0},min{100},max{-1};
    int NPoints{gr->GetN()};

    for(int i{0};i<NPoints-12;++i){
        double e = gr->GetErrorX(i);
        double y = gr->GetX()[i];
        double x = gr->GetY()[i];

        s1+=1/(e*e);
        sx+=x/(e*e);
        sy+=y/(e*e);
        sxx+=x*x/(e*e);
        sxy+=x*y/(e*e);

        if(x<min)min=x;
        if(x>max)max=x;

    }

    d=s1*sxx-sx*sx;

    TF1* fl = new TF1("fitl","-[0]/[1] + x/[1]",0,5000);
    fl->SetParameters((sy*sxx-sx*sxy)/d,(s1*sxy-sx*sy)/d);
    fl->Draw("SAME");
    //gr->Draw("SAMEPE");
    
    std::fstream in;
    in.open("fit.txt",std::fstream::out);
    in<<"Linear fit method results:\n VA="<<(sy*sxx-sx*sxy)/d<<"+/-"<<sqrt(sxx/d)<<"mV   VCE/IC="<<(s1*sxy-sx*sy)/d<<"+/-"<<sqrt(s1/d)<<" V/A\ng="
    <<d/(s1*sxy-sx*sy)<<"+/-"<<d/(s1*sxy-sx*sy)*d/(s1*sxy-sx*sy)*sqrt(s1/d)<<"\n";
}