
TH1F* setHisto(TF1* f, int color=0, int lineStyle=0)
{
  TH1F* hfirst = new TH1F(Form("%sFirst",f->GetName()),Form("%sFirst",f->GetName()),16,0,16);
  int N=1e6;
  hfirst->FillRandom(f->GetName(),N);
  hfirst->Scale(1/(float) N);
  TH1F* h = new TH1F(f->GetName(),f->GetName(),16,-0.5,15.5);
  cout << "Number of bins: " << hfirst->GetNbinsX() << endl;
  for(int i=1; i<=hfirst->GetNbinsX();++i) {
    cout << i << "  " << hfirst->GetXaxis()->GetBinCenter(i) << "  " << hfirst->GetBinContent(i) << endl;
    h->Fill(hfirst->GetXaxis()->GetBinCenter(i)-0.5,hfirst->GetBinContent(i));
  }
  h->SetLineWidth(4);
  //h->SetFillStyle(3002);
  if(color) {
    h->SetLineColor(color);
    //h->SetFillColor(color);
  }
  if(lineStyle) {
    h->SetLineStyle(lineStyle);
  }
  return h;
}

TH1F* setPval(TH1F* h) 
{
  TH1F* hpval = (TH1F*) h->Clone(Form("%sPval",h->GetName()));
  hpval->Reset();
  for(int i=1; i<=hpval->GetNbinsX();++i) {
    if(hpval->GetXaxis()->GetBinCenter(i)<=1) {
      cout << i << "  " << h->GetXaxis()->GetBinCenter(i) << "  " << h->GetBinContent(i) << endl;
      hpval->Fill(hpval->GetXaxis()->GetBinCenter(i),h->GetBinContent(i));
    }
  }
  hpval->SetLineWidth(0);
  hpval->SetFillStyle(3002);
  hpval->SetFillColor(h->GetLineColor());
  return hpval;
}

void poissonDistribution()
{
  gROOT->SetStyle("Plain");
  gStyle->SetOptTitle(0);
  gStyle->SetOptStat(0);

  TF1* f1 = new TF1("f1","ROOT::Math::poisson_pdf(x,0.82)",0,16);
  TF1* f2 = new TF1("f2","ROOT::Math::poisson_pdf(x,1.82)",0,16);
  TF1* f3 = new TF1("f3","ROOT::Math::poisson_pdf(x,4.82)",0,16);
  TF1* f4 = new TF1("f4","ROOT::Math::poisson_pdf(x,8.82)",0,16);
  TH1F* h1 = setHisto(f1,kBlack,7);
  TH1F* h2 = setHisto(f2,kBlue);
  TH1F* h3 = setHisto(f3,kBlue);
  TH1F* h4 = setHisto(f4,kBlue);
  h1->GetXaxis()->SetTitle("N");
  h1->GetYaxis()->SetTitle("P(N)");
  h1->GetYaxis()->SetRangeUser(0,0.5);

  TLatex latex;
  latex.SetTextSize(0.05);
  latex.SetTextAlign(13);  //align at top

  TLine* line = new TLine(1,-0.03,1,0.4);
  line->SetLineColor(kRed);
  line->SetLineWidth(2);

  TCanvas* c1 = new TCanvas("c1","c1",700,400);
  c1->SetBottomMargin(0.15);
  h1->Draw();
  line->Draw();  
  latex.DrawLatex(0.8,-0.05,"N_{obs}");

  TArrow* arr1 = new TArrow(4,0.4,2.5,0.28,0.02,"|>");
  arr1->SetAngle(40);
  TArrow* arr2 = new TArrow(6,0.32,4.5,0.2,0.02,"|>");
  arr2->SetAngle(40);
  TArrow* arr3 = new TArrow(9.5,0.28,8,0.16,0.02,"|>");
  arr3->SetAngle(40);

  TCanvas* c2 = new TCanvas("c2","c2",700,400);
  c2->SetBottomMargin(0.15);
  h1->Draw();
  h2->Draw("same");
  line->Draw();  
  latex.DrawLatex(0.8,-0.05,"N_{obs}");
  arr1->Draw();
  latex.DrawLatex(4.1,0.42,"s=1");

  TCanvas* c3 = new TCanvas("c3","c3",700,400);
  c3->SetBottomMargin(0.15);
  h1->Draw();
  TH1F* h2prime = (TH1F*) h2->Clone("h2prime");
  h2prime->SetLineColor(kBlue-10);
  h2prime->Draw("same");
  h3->Draw("same");
  line->Draw();  
  latex.DrawLatex(0.8,-0.05,"N_{obs}");
  arr1->Draw();
  arr2->Draw();
  latex.DrawLatex(4.1,0.42,"s=1");
  latex.DrawLatex(6.1,0.34,"s=4");

  TCanvas* c4 = new TCanvas("c4","c4",700,400);
  c4->SetBottomMargin(0.15);
  h1->Draw();
  h2prime->Draw("same");
  TH1F* h3prime = (TH1F*) h3->Clone("h3prime");
  h3prime->SetLineColor(kBlue-6);
  h3prime->Draw("same");
  h4->Draw("same");
  arr1->Draw();
  arr2->Draw();
  arr3->Draw();
  latex.DrawLatex(4.1,0.42,"s=1");
  latex.DrawLatex(6.1,0.34,"s=4");
  latex.DrawLatex(9.6,0.3,"s=8");
  line->Draw();  
  latex.DrawLatex(0.8,-0.05,"N_{obs}");

  TCanvas* c5 = new TCanvas("c5","c5",700,400);
  c5->SetBottomMargin(0.15);

  TH1F* h2pval = setPval(h2prime);
  TH1F* h3pval = setPval(h3prime);
  TH1F* h4pval = setPval(h4);
  h1->Draw();
  h2prime->Draw("same");
  h3prime->Draw("same");
  h4->Draw("same");
  h2pval->Draw("same");
  h3pval->Draw("same");
  h4pval->Draw("same");
  arr1->Draw();
  arr2->Draw();
  arr3->Draw();
  latex.DrawLatex(4.1,0.42,"s=1");
  latex.DrawLatex(6.1,0.34,"s=4");
  latex.DrawLatex(9.6,0.3,"s=8");
  line->Draw();  
  latex.DrawLatex(0.8,-0.05,"N_{obs}");

  TCanvas* c6 = new TCanvas("c6","c6",700,400);
  c6->SetBottomMargin(0.15);

  TH1F* h1pval = setPval(h1);
  h1->Draw();
  h2prime->Draw("same");
  h3prime->Draw("same");
  h4->Draw("same");
  h1pval->Draw("same");
  h2pval->Draw("same");
  h3pval->Draw("same");
  h4pval->Draw("same");
  arr1->Draw();
  arr2->Draw();
  arr3->Draw();
  latex.DrawLatex(4.1,0.42,"s=1");
  latex.DrawLatex(6.1,0.34,"s=4");
  latex.DrawLatex(9.6,0.3,"s=8");
  line->Draw();  
  latex.DrawLatex(0.8,-0.05,"N_{obs}");


  c1->SaveAs("poissonDistributionBkgOnly.pdf");
  c2->SaveAs("poissonDistributionSig1.pdf");
  c3->SaveAs("poissonDistributionSig4.pdf");
  c4->SaveAs("poissonDistributionSig8.pdf");
  c5->SaveAs("poissonDistributionSig8Pval.pdf");
  c6->SaveAs("poissonDistributionSig8PvalBkg.pdf");
}
