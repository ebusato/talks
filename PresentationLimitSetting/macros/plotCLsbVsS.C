TF1* plotCLsbVsS(double Nbkg, double Nobs, int color, TString CLsOrCLsb="CLsb")
{
  TF1* fCLsb = 0;
  if(CLsOrCLsb=="CLsb") {
    fCLsb = new TF1("fCLsb","1-ROOT::Math::chisquared_cdf(2*(x+[1]),2*([0]+1))",0,6);
  }
  else if(CLsOrCLsb=="CLs") {
    fCLsb = new TF1("fCLs","(1-ROOT::Math::chisquared_cdf(2*(x+[1]),2*([0]+1)))/(1-ROOT::Math::chisquared_cdf(2*([1]),2*([0]+1)))",0,6);
  }
  else 
    cout << "ERROR !" << endl;
  // [0] -> Nobs
  // [1] -> Nbkg
  fCLsb->SetNpx(1e3);
  fCLsb->SetParameters(Nobs,Nbkg);
  if(CLsOrCLsb=="CLsb") fCLsb->GetYaxis()->SetTitle("CL_{s+b}");
  else if(CLsOrCLsb=="CLs") fCLsb->GetYaxis()->SetTitle("CL_{s}");
  else cout << "ERROR !" << endl; 
  fCLsb->SetLineColor(color);
  fCLsb->GetXaxis()->SetTitleOffset(1.4);
  fCLsb->GetXaxis()->SetTitleSize(0.045);
  fCLsb->GetXaxis()->SetTitle("s");
  fCLsb->GetXaxis()->SetRangeUser(0,6);
  fCLsb->GetYaxis()->SetRangeUser(0,0.85);
  fCLsb->GetYaxis()->SetTitleSize(0.045);
  return fCLsb;
}

void plotCLsbVsS()
{
  gROOT->SetStyle("Plain");
  gStyle->SetOptTitle(0);
  gStyle->SetPadGridX(1);
  gStyle->SetPadGridY(1);

  TCanvas* c1 = new TCanvas("c1","c1",600,400);
  c1->SetBottomMargin(.15);
  TF1* fCLsb = plotCLsbVsS(0.82,1,kBlue);
  fCLsb->Draw();

  TLine* line = new TLine(0,0.05,6,0.05);
  line->SetLineColor(kRed);
  line->SetLineWidth(2);
  line->Draw();

  TCanvas* c2 = new TCanvas("c2","c2",600,400);
  c2->SetBottomMargin(.15);
  TF1* fCLs = plotCLsbVsS(0.82,1,kMagenta,"CLs");
  fCLs->GetYaxis()->SetTitle("CL_{s} or CL_{s+b}");
  fCLs->Draw();
  fCLsb->Draw("same");
  line->Draw();
  TLegend* leg = new TLegend(0.6,0.75,0.73,0.87);
  leg->SetFillColor(kWhite);
  leg->SetLineColor(kBlack);
  leg->SetBorderSize(1);
  leg->AddEntry(fCLs,"CL_{s}","l");
  leg->AddEntry(fCLsb,"CL_{s+b}","l");
  leg->Draw();

  c1->SaveAs("plotCLsbVsS_1.pdf");
  c2->SaveAs("plotCLsbVsS_2.pdf");
}
