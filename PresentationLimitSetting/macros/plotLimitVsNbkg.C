struct result
{
  TF1* fCLs;
  TF1* fCLsb;
};

result plotLimitVsNbkg(double Nobs, int color)
{
  TF1* fCLsb = new TF1("fCLsb","0.5*ROOT::Math::chisquared_quantile(1-0.05,2*([0]+1))-x",0,15);
  TF1* fCLs = new TF1("fCLs","0.5*ROOT::Math::chisquared_quantile(1-0.05*(1-ROOT::Math::chisquared_cdf(2*x,2*([0]+1))),2*([0]+1))-x",0,15);
  // x -> Nbkg
  // [0] -> Nobs
  fCLs->SetNpx(1e3);
  fCLsb->SetNpx(1e3);
  fCLsb->SetParameter(0,Nobs);
  fCLsb->SetLineColor(color);
  fCLs->SetLineColor(color);
  fCLs->SetLineWidth(2);
  fCLs->SetLineStyle(7);
  fCLs->SetParameter(0,Nobs);
  fCLsb->GetYaxis()->SetTitle("s_{up}");
  fCLsb->GetXaxis()->SetTitleOffset(1.4);
  fCLsb->GetXaxis()->SetTitleSize(0.045);
  fCLsb->GetYaxis()->SetTitleSize(0.045);
  fCLsb->GetXaxis()->SetTitle("#sum_{i} b_{i}");
  fCLsb->GetYaxis()->SetRangeUser(-5,13);
  fCLsb->GetXaxis()->SetRangeUser(0,15);
  fCLs->GetYaxis()->SetTitle("s_{up}");
  fCLs->GetXaxis()->SetTitleOffset(1.4);
  fCLs->GetXaxis()->SetTitle("#sum_{i} b_{i}");
  fCLs->GetXaxis()->SetTitleSize(0.045);
  fCLs->GetYaxis()->SetTitleSize(0.045);
  fCLs->GetYaxis()->SetRangeUser(-5,13);
  fCLs->GetXaxis()->SetRangeUser(0,15);
  result res;
  res.fCLs=fCLs;
  res.fCLsb=fCLsb;
  return res;
}

void plotLimitVsNbkg()
{
  gROOT->SetStyle("Plain");
  gStyle->SetOptTitle(0);
  gStyle->SetPadGridX(1);
  gStyle->SetPadGridY(1);

  TLatex latex1;
  latex1.SetTextSize(0.04);
  latex1.SetTextAlign(13);  //align at top
  latex1.SetTextColor(kBlue);

  TLatex latex2;
  latex2.SetTextSize(0.04);
  latex2.SetTextAlign(13);  //align at top
  latex2.SetTextColor(kRed);

  TLatex latex3;
  latex3.SetTextSize(0.04);
  latex3.SetTextAlign(13);  //align at top
  latex3.SetTextColor(kGreen+3);

  TLatex latex4;
  latex4.SetTextSize(0.04);
  latex4.SetTextAlign(13);  //align at top
  latex4.SetTextColor(kMagenta);

  TCanvas* c2 = new TCanvas("c2","c2",600,400);
  c2->SetBottomMargin(.15);
  result res = plotLimitVsNbkg(0,kBlue);
  res.fCLsb->Draw();
  res = plotLimitVsNbkg(1,kRed);
  res.fCLsb->Draw("same");
  res = plotLimitVsNbkg(3,kGreen+3);
  res.fCLsb->Draw("same");
  res = plotLimitVsNbkg(6,kMagenta);
  res.fCLsb->Draw("same");
  latex1.DrawLatex(0.7,0.2,"N_{obs}=0");
  latex2.DrawLatex(0.7,5.2,"N_{obs}=1");
  latex3.DrawLatex(0.7,8,"N_{obs}=3");
  latex4.DrawLatex(0.7,12.3,"N_{obs}=6");

  TCanvas* c3 = new TCanvas("c3","c3",600,400);
  c3->SetBottomMargin(.15);
  res = plotLimitVsNbkg(0,kBlue);
  res.fCLsb->Draw();
  res.fCLs->Draw("same");
  res = plotLimitVsNbkg(1,kRed);
  res.fCLsb->Draw("same");
  res.fCLs->Draw("same");
  res = plotLimitVsNbkg(3,kGreen+3);
  res.fCLsb->Draw("same");
  res.fCLs->Draw("same");
  res = plotLimitVsNbkg(6,kMagenta);
  res.fCLsb->Draw("same");
  res.fCLs->Draw("same");
  latex1.DrawLatex(0.7,0.2,"N_{obs}=0");
  latex2.DrawLatex(0.7,5.2,"N_{obs}=1");
  latex3.DrawLatex(0.7,8,"N_{obs}=3");
  latex4.DrawLatex(0.7,12.3,"N_{obs}=6");

  TLegend* leg = new TLegend(0.6,0.75,0.73,0.87);
  leg->SetFillColor(kWhite);
  leg->SetLineColor(kBlack);
  leg->SetBorderSize(1);
  leg->AddEntry(res.fCLs,"CL_{s}","l");
  leg->AddEntry(res.fCLsb,"CL_{s+b}","l");
  leg->Draw();

  c2->SaveAs("plotLimitVsNbkg_CLsb_1.pdf");
  c3->SaveAs("plotLimitVsNbkg_CLsb_2.pdf");
}
