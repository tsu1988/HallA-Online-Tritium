void cerL_check(Int_t run, Int_t flag){
  // Int_t run,flag;    
  // cout << "enter run_number: ";
  // cin >> run;
  // cout << "0=fastbus signal, 1=FADC signal: ";
  // cin >> flag;

  Double_t scale = 1;
  if (flag==1) scale=3;
 
  TString dir = "/chafs1/work1/tritium/Rootfiles/";
  TString dir2 = "/volatile/halla/triton/shujie/rootfiles/";
  TString fname =Form("tritium_%d.root",run);
  TString filename = dir2+fname;

 
  TChain *T = new TChain("T");
  T->Add(filename);

 
  cout << T->GetEntries() << endl;
  TCanvas *c1 = new TCanvas("c1","c1",1200,700);
  c1->Divide(5,2);
  TString name, tit, dr;
  Double_t min, max;
  Int_t max_bin;
  Int_t i;
  Double_t ped[10],peak[10];
  TH1F *h1[10], *hh[10], *hh1[10];
  TString cut = "(DL.evtypebits>>1)&1";
  //cut="";
  for(i=0;i<10;i++){
    h1[i] = new TH1F(Form("h%d",i),Form("h%d",i),300,0,10000);
  }
  for(i=0;i<10;i++){
    c1->cd(i+1);
    if (flag==0)
      T->Draw(Form("FbusL.cer.a_c[%d]>>h%d",i,i),cut);
    if (flag==1)
      T->Draw(Form("L.cer.a_c[%d]>>h%d",i,i),cut);
    max_bin = h1[i]->GetMaximumBin();
    ped[i] = h1[i]->GetBinCenter(max_bin);
    min = ped[i] - 100*scale;
    max = min + 400*scale;
    name = Form("gc_%d",i);
    tit = Form("gc_%d",i);
    hh[i] = new TH1F(name,tit,100,min,max);
    if(flag==0)
      dr = Form("FbusL.cer.a_c[%d]",i);
    if(flag==1)
      dr = Form("L.cer.a_c[%d]",i);
    dr = dr + ">>";
    dr = dr + name;
    T->Draw(dr,cut);
    //fit SPE
    ped[i]=0;
    hh[i]->GetXaxis()->SetRangeUser(ped[i],ped[i]+100*scale);
    Double_t dip=hh[i]->GetMinimumBin(); //find the dip b/w ped and SPE
    //cout<<ped[i]<<"  "<<dip<<endl;
    hh[i]->GetXaxis()->SetRangeUser(min,max);

    TF1 *f2 = new TF1("f2", "gaus", ped[i]+dip+20*scale, ped[i]+dip+100*scale);
    hh[i]->Fit(f2, "Rq");
    peak[i] = f2->GetParameter(1);
    TLine *l1 = new TLine(peak[i],0,peak[i],hh[i]->GetMaximum());
    l1->SetLineColor(2);
    l1->Draw();
    gPad->SetLogy();
    cout<<peak[i]<<"   ";
  }
}
