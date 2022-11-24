void multigraph(){
    gStyle->SetPadGridX(true);
    gStyle->SetPadGridY(true);
    gStyle->SetPadTickX(true);
    gStyle->SetPadTickY(true);
    TMultiGraph *mg = new TMultiGraph("multiGraph","Caratteristica I-V di due transistor BJT");
    mg->SetTitle("Caratteristica I-V un transistor BJT con emettitore comune a due correnti di base diverse;V_{CE} [mV]; I_{C} [mA]");
    TGraphErrors* gr1 = new TGraphErrors("100 muA/data.dat","%lg %lg %lg %lg");
    TGraphErrors* gr2 = new TGraphErrors("200 muA/data.dat","%lg %lg %lg %lg");
    gr1->SetMarkerColor(4);
    gr2->SetMarkerColor(2);
    gr1->SetMarkerStyle(22);
    gr2->SetMarkerStyle(23);
    gr1->SetMarkerSize(1);
    gr2->SetMarkerSize(1);
    mg->Add(gr1);
    mg->Add(gr2);
    mg->Draw("APE");

    TF1* fl1 = new TF1("fitl1","-[0]/[1] + x/[1]",0,5000);
    fl1->SetParameters(-16127.3,985.067);
    fl1->SetLineColor(4);
    fl1->Draw("SAME");

    TF1* fl2 = new TF1("fitl2","-[0]/[1] + x/[1]",0,5000);
    fl2->SetParameters(-12024,414.023);
    fl2->SetLineColor(2);
    fl2->Draw("SAME");

    TLegend* leg = new TLegend(0.78, 0.85, .99, .99);
    leg->AddEntry(gr1,"Punti 100#muA","ep");
    leg->AddEntry(gr2,"Punti 200#muA","ep");
    leg->AddEntry(fl1,"Fit 100#muA","l");
    leg->AddEntry(fl2,"Fit 200#muA","l");
    leg->Draw("SAME");  
}