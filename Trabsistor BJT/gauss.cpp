void gauss(){
    TH1F* h = new TH1F("Gauss","Distribuzione del parametro #beta nella regione lineare",18,130,185);
    h->Fill(179.9);
    h->Fill(179.5);
    h->Fill(175.9);
    h->Fill(174.9);
    h->Fill(171.2);
    h->Fill(168.7);
    h->Fill(166.6);
    h->Fill(163.1);
    h->Fill(160.4);
    h->Fill(158);
    h->Fill(155.7);
    h->Fill(153.5);
    h->Fill(153.5);
    h->Fill(150.9);
    h->Fill(148);
    h->Fill(148.8);
    h->Fill(146.7);
    h->Fill(143);
    h->Fill(141.1);
    h->Fill(136.5);
    h->Draw();
}