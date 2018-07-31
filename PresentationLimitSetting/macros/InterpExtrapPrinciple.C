#include "AtlasStyle.C"
#include "AtlasUtils.C"

void SetStyle()
{
  TStyle* atlasStyle=AtlasStyle();
  gROOT->SetStyle("ATLAS");
  gROOT->ForceStyle();
}

void InterpExtrapPrinciple()
{
  SetStyle();

  TH1F* h = new TH1F("h","h",1e6,-3,3);
  h->SetMarkerSize(0);
  h->SetLineWidth(0);
  h->SetLineStyle(kDashed);
  h->GetXaxis()->SetTitle("#eta");
  h->GetYaxis()->SetTitle("N(#eta)");
  h->GetYaxis()->SetTitleSize(0.05);
  h->GetYaxis()->SetTitleOffset(1.1);
  h->GetXaxis()->SetTitleSize(0.06);
  h->GetXaxis()->SetTitleOffset(1.06);
  h->GetXaxis()->SetRangeUser(-3,3);
  h->GetYaxis()->SetRangeUser(0,35);

  h->Fill(-1,15*0.7);
  h->Fill(0,15*1);
  h->Fill(1,15*1.4);

  TGraph* graph = new TGraph(3);
  graph->SetPoint(0,-1,15*0.7);
  graph->SetPoint(1,0,15*1);
  graph->SetPoint(2,1,15*1.4);
  graph->SetMarkerStyle(24); 
  graph->SetMarkerSize(2);

  TMarker* marker = new TMarker();
  marker->SetMarkerStyle(24);
  marker->SetMarkerSize(2);

  TCanvas* c1 = new TCanvas("c1","c1",800,600);
  marker->SetMarkerStyle(24);

  h->DrawClone("hist");
  marker->DrawMarker(-1,15*0.7);
  marker->DrawMarker(0,15);
  marker->DrawMarker(1,15*1.4);

  TCanvas* c2 = new TCanvas("c2","c2",800,600);
  
  h->Draw("p");
  h->Fit("pol2");
  TF1 *pol2 = h->GetFunction("pol2");
  pol2->SetNpx();
  pol2->SetLineColor(kBlue);
  pol2->Draw("same");

  TF1* pol3 = new TF1("pol3","15+7.4094*x+0.750024*x*x-2.1594*x*x*x",-3,3);
  pol3->SetLineColor(kRed);
  pol3->SetNpx(1e3);
  pol3->Draw("same");
  graph->Draw("psame");

  c1->SaveAs("InterpExtrapPrinciple1.png");
  c2->SaveAs("InterpExtrapPrinciple2.png");
  c1->SaveAs("InterpExtrapPrinciple1.pdf");
  c2->SaveAs("InterpExtrapPrinciple2.pdf");
}
