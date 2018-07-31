//////////////////////////////////////////////////////////////////////
// Usage:
//  > root -l StudyStatUncertainty.C
//////////////////////////////////////////////////////////////////////

#include "AtlasStyle.C"
#include "AtlasUtils.C"
#include "Math/QuantFuncMathCore.h"
#include <vector>
#include <map>

const float Nbkg=0.82;
const float Nsig=2.49;
const float Nobs=1;

void SetStyle()
{
  TStyle* atlasStyle=AtlasStyle();
  gROOT->SetStyle("ATLAS");
  gROOT->ForceStyle();
  gStyle->SetPadGridX(1);
  gStyle->SetPadGridY(1);
}

void SetHistoStyle(TH1* h,const int markerstyle, const int markercolor, TString Ytitle)
{
  h->GetYaxis()->SetTitle(Ytitle);
  h->GetYaxis()->SetTitleSize(0.08);
  h->GetYaxis()->SetTitleOffset(1);
  h->GetXaxis()->SetTitle("Lumi");
  h->SetMarkerStyle(markerstyle);
  h->SetMarkerColor(markercolor);
  h->SetMarkerSize(1.5);
  h->SetLineColor(markercolor);
}

double theoGaussApprox(const float Nbkg, const float Nsig, const float Nobs)
{
  double quantile=ROOT::Math::gaussian_quantile(1-0.05*ROOT::Math::gaussian_cdf((Nobs-Nbkg)/TMath::Sqrt(Nbkg)),1);
  //double quantile=ROOT::Math::gaussian_quantile(1-0.05,1);                    

  return (Nobs-Nbkg)/Nsig + quantile*quantile/(2*Nsig)*(1+TMath::Sqrt(1+4*Nobs/quantile/quantile));
}


double theoGaussBadApprox(const float Nbkg, const float Nsig)
{
  return 1.64*TMath::Sqrt(Nbkg)/Nsig;
}

double theoPoisson(const float Nbkg, const float Nsig, const float Nobs)
{
  return (0.5*ROOT::Math::chisquared_quantile(1-0.05*(1-ROOT::Math::chisquared_cdf(2*Nbkg,2*(Nobs+1))),2*(Nobs+1))-Nbkg)/Nsig;
}

TH1* MakeRatio(const TH1* h1, const TH1* h2, const TString name, const TString Ytitle)
{
  TH1* h_ratio = (TH1F*) h1->Clone(name);
  h_ratio->Divide(h2);
  h_ratio->GetYaxis()->SetRangeUser(0.95,1.28);
  h_ratio->GetXaxis()->SetLabelSize(0.12);
  h_ratio->GetYaxis()->SetLabelSize(0.12);
  h_ratio->GetXaxis()->SetTitleSize(0.14);
  h_ratio->GetXaxis()->SetTitleOffset(1);
  h_ratio->GetYaxis()->SetTitleSize(0.12);
  h_ratio->GetYaxis()->SetTitleOffset(0.5);
  h_ratio->GetYaxis()->SetNdivisions(509);
  h_ratio->GetYaxis()->CenterTitle();
  h_ratio->GetYaxis()->SetTitle(Ytitle);
  h_ratio->GetXaxis()->SetTitle("Lumi");
  return h_ratio;
}

void plotLimitVsLumiAnalyticalBadApprox()
{
  SetStyle();

  float lumii = 1;
  float lumif = 20;

  int nbins = lumif-lumii+1;
  TH1F* h_LimitExpTheoPoissonVsLumi = new TH1F("h_LimitExpTheoPoissonVsLumi","h_LimitExpTheoPoissonVsLumi",nbins,lumii-0.5,lumif+0.5);
  TH1F* h_LimitTheoGaussBadApproxVsLumi = new TH1F("h_LimitTheoGaussBadApproxVsLumi","h_LimitTheoGaussBadApproxVsLumi",nbins,lumii-0.5,lumif+0.5);
  
  //std::vector<TF1*> vecFunc;
  std::map<float,TF1*> vecFunc;
  TH1* hFromVec = (TH1*) h_LimitExpTheoPoissonVsLumi->Clone("hFromVec");
  
  //int NoPoint=0;
  for(float i=lumii; i<lumif; i+=1) {
    float lumi = (float) i;
    float Nbkg2=Nbkg*lumi;
    float Nsig2=Nsig*lumi;
    float Nobs2=Nobs*lumi;
    cout << "lumi = " << lumi << endl;
    cout << "Nbkg = " << Nbkg2 << endl;
    cout << "Nsig = " << Nsig2 << endl;
    cout << "Nobs = " << Nobs2 << endl;
    double limitexp_theoPoisson = theoPoisson(Nbkg2,Nsig2,Nbkg2);
    double limit_theoGaussBadApprox = theoGaussBadApprox(Nbkg2,Nsig2);
    h_LimitExpTheoPoissonVsLumi->Fill(lumi,limitexp_theoPoisson);
    h_LimitTheoGaussBadApproxVsLumi->Fill(lumi,limit_theoGaussBadApprox);
    
    TF1* f = new TF1("f","TMath::Sqrt([0]/x)*[1]",lumi,lumif);
    f->SetParameters(i,h_LimitExpTheoPoissonVsLumi->GetBinContent(h_LimitExpTheoPoissonVsLumi->GetXaxis()->FindBin(lumi)));
    //vecFunc.push_back(f);
    vecFunc[lumi]=f;

    if(lumi==lumii*2)
      hFromVec->Fill(lumi,vecFunc[lumii]->Eval(lumi));
    else if(lumi==lumii*4)
      hFromVec->Fill(lumi,vecFunc[lumii*2]->Eval(lumi));
    else if(lumi==lumii*8)
      hFromVec->Fill(lumi,vecFunc[lumii*4]->Eval(lumi));
    else if(lumi==lumii*16)
      hFromVec->Fill(lumi,vecFunc[lumii*8]->Eval(lumi));

    //if(NoPoint>0) {
    //hFromVec->Fill(lumi,vecFunc[NoPoint-1]->Eval(lumi));
    //}
    //NoPoint++;
  }

  SetHistoStyle(h_LimitExpTheoPoissonVsLumi,25,kBlue,"#mu_{up}");
  SetHistoStyle(h_LimitTheoGaussBadApproxVsLumi,22,kBlack,"#mu_{up} (obs.)");

  TCanvas* c1 = new TCanvas("c1","c1",600,400);
  c1->SetBottomMargin(0.15);
  TPad* pPad1=new TPad("p1","",0.00,0.30,1.00,1.00);
  TPad* pPad2=new TPad("p2","",0.00,0.00,1.00,0.30);
  pPad1->SetBottomMargin(0.02);
  pPad1->Draw();  
  pPad2->SetTopMargin(0);
  pPad2->SetBottomMargin(0.3);
  pPad2->Draw();
  pPad1->cd();
  h_LimitExpTheoPoissonVsLumi->GetXaxis()->SetLabelOffset(0.025);
  h_LimitExpTheoPoissonVsLumi->Draw("p");
  h_LimitTheoGaussBadApproxVsLumi->Draw("psame");
  TLegend* leg1 = new TLegend(0.55,0.7,0.92,0.9);
  leg1->SetFillColor(kWhite);
  leg1->SetLineColor(kBlack);
  leg1->SetBorderSize(1);
  leg1->AddEntry(h_LimitExpTheoPoissonVsLumi,"Analytical (expected)","p");
  leg1->AddEntry(h_LimitTheoGaussBadApproxVsLumi,"1.64#times#sqrt{b}/s","p");
  leg1->Draw();
  pPad2->cd();
  TH1* hratio1 = MakeRatio(h_LimitExpTheoPoissonVsLumi,h_LimitTheoGaussBadApproxVsLumi,"hratio1","ratio");
  hratio1->GetYaxis()->SetRangeUser(0,3.4);
  hratio1->SetMarkerStyle(8);
  hratio1->SetMarkerColor(kBlack);
  hratio1->GetXaxis()->SetLabelOffset(0.025);
  hratio1->Draw("p");
  
  TCanvas* c2 = new TCanvas("c2","c2",600,400);
  TPad* pPad3=new TPad("p3","",0.00,0.30,1.00,1.00);
  TPad* pPad4=new TPad("p4","",0.00,0.00,1.00,0.30);
  pPad3->SetBottomMargin(0.02);
  pPad3->Draw();  
  pPad4->SetTopMargin(0);
  pPad4->SetBottomMargin(0.3);
  pPad4->Draw();
  pPad3->cd();
  h_LimitExpTheoPoissonVsLumi->Draw("p");
  h_LimitTheoGaussBadApproxVsLumi->Draw("psame");

  cout << "size : " << vecFunc.size() << "  " << vecFunc[1] << endl;

  int color=kMagenta+4;
  for(float i=lumii; i<lumif; i+=1) {
    float lumi = (float) i;
    vecFunc[lumi]->SetLineColor(color);
    vecFunc[lumi]->SetNpx(1e3);
    vecFunc[lumi]->SetLineWidth(1);
    vecFunc[lumi]->Draw("same");
    color--;
  }

  //for(int entry=0; entry<vecFunc.size(); ++entry) {
  //vecFunc[entry]->SetLineColor(color);
  //vecFunc[entry]->SetNpx(1e3);
  //vecFunc[entry]->SetLineWidth(1);
  //vecFunc[entry]->Draw("same");
  //color--;
  //}

  TLegend* leg2 = new TLegend(0.55,0.6,0.92,0.9);
  leg2->SetFillColor(kWhite);
  leg2->SetLineColor(kBlack);
  leg2->SetBorderSize(1);
  leg2->AddEntry(h_LimitExpTheoPoissonVsLumi,"Analytical (expected)","p");
  leg2->AddEntry(h_LimitTheoGaussBadApproxVsLumi,"1.64#times#sqrt{b}/s","p");
  leg2->AddEntry(vecFunc[0],"#sqrt{L} scaling","l");
  leg2->Draw();

  pPad4->cd();
  TH1* hratio3 = MakeRatio(hFromVec,h_LimitExpTheoPoissonVsLumi,"hratio3","Prediction/Analyt.");
  hratio3->GetXaxis()->SetLabelOffset(0.025);
  hratio3->Draw("p");

  c1->SaveAs("plotLimitVsLumiAnalyticalBadApprox1.pdf");
  c2->SaveAs("plotLimitVsLumiAnalyticalBadApprox2.pdf");
}

