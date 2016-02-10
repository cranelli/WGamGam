#include <TROOT.h>
#include <TStyle.h>
#include <TH2F.h>
#include <TString.h>
#include <TFile.h>
#include <TCanvas.h>
#include <TPad.h>
#include <TAxis.h>
#include <TGaxis.h>
#include <TPaveText.h>
#include <TBox.h>
#include <TWbox.h>
#include <TLine.h>
#include <TGraphAsymmErrors.h>
#include <TMarker.h>
#include <stdio.h>
#include <iostream>

#include "tdrstyle.C"
#include "CMS_lumi.C"

const double LOW_X=0.25;
const double UPP_X=2.80;


void drawTheory(int i, double mean, double nErr, double pErr, double vstep) {
    
    double lowY = (i+1)*vstep; // margins are 1 vstep each
    double uppY = (i+2)*vstep;

    TBox* band = new TBox(mean-nErr,lowY+0.1*vstep,mean+pErr,uppY-0.1*vstep);
    band->SetFillColor(kYellow-4);
    band->Draw();
    TLine* center = new TLine(mean, 0, mean, 1);
    center->SetLineStyle(2); // dash
    center->SetLineWidth(4);
    center->SetLineColor(2);
    center->Draw();
  return;
}

void drawBkgBox(int i, int aux[6], double vstep, TH2F* histo) {

  double lowY = (i+1)*vstep; // margins are 1 vstep each
  double uppY = (i+2)*vstep;

  double lowX =LOW_X;
  double uppX =UPP_X;
  //double lowX = histo->GetBinLowEdge(1);
  //double uppX = histo->GetBinLowEdge(histo->GetNbinsX()) +
  //              histo->GetBinWidth(histo->GetNbinsX());

  TBox* band = new TBox(lowX,lowY,uppX,uppY);
  band->SetFillColor(aux[1]);
  band->Draw();


  return;
}

void drawMeasurement(int i, double m[5], char label[2][100], int aux[5],
		     double vstep, TH2F* histo, TCanvas* canvas) {

 
    double lowY = (i+1)*vstep;
    double uppY = (i+2)*vstep;

  //double lowX = histo->GetBinLowEdge(1);
  //double uppX = histo->GetBinLowEdge(histo->GetNbinsX()) +
  //              histo->GetBinWidth(histo->GetNbinsX());
    double lowX = 0.25;
    double uppX = 2.80;
    double widthX = uppX - lowX;

 
  // y-range of the histogram is [0...1]

  double startX = lowX + 0.04*widthX;
  
  TPaveText* text = new TPaveText(startX, lowY,
				  startX, uppY, "BR");
  text->SetTextAlign(12);
  text->SetFillColor(aux[1]);
  text->SetTextColor(aux[0]);
  text->SetLineColor(1);
  text->SetBorderSize(0);
  TText* t0 = text->AddText("    ");
  t0->SetTextSize(0.08);
  t0->SetTextFont(aux[2]);
  TText* t1 = text->AddText(label[0]);
  t1->SetTextSize(0.08);
  t1->SetTextFont(aux[2]);
  TText* t2 = text->AddText(label[1]);
  t2->SetTextSize(0.08);
  t2->SetTextFont(aux[2]);
  text->Draw();


  double ypos = 0.5*(lowY+uppY);
  double mean = m[0];
  double nErr1 = m[1];
  double pErr1 = m[2];
  double nErr2 = sqrt(m[1]*m[1]+m[3]*m[3]);
  double pErr2 = sqrt(m[2]*m[2]+m[4]*m[4]);

  // draw TGraphAsymmErrors 1 (stat only) |---*---|
  TMarker* measurement = new TMarker(mean, ypos, aux[4]);
  measurement->SetMarkerColor(aux[0]);
  measurement->SetMarkerStyle(aux[4]);

  measurement->SetMarkerSize(1.5);
  //measurement->SetMarkerSize(1.75);
  measurement->Draw();

  double vsizeErr1 = 0.09*vstep;
  TLine* l1 = new TLine(mean, ypos, mean-nErr1, ypos);
  l1->SetLineWidth(aux[3]);
  l1->SetLineColor(aux[0]);
  l1->Draw();
  TLine* l2 = new TLine(mean, ypos, mean+pErr1, ypos);
  l2->SetLineWidth(aux[3]);
  l2->SetLineColor(aux[0]);
  l2->Draw();
  TLine* l3 = new TLine(mean-nErr1, ypos-vsizeErr1,
			mean-nErr1, ypos+vsizeErr1);
  l3->SetLineWidth(aux[3]);
  l3->SetLineColor(aux[0]);
  l3->Draw();
  TLine* l4 = new TLine(mean+pErr1, ypos-vsizeErr1,
			mean+pErr1, ypos+vsizeErr1);
  l4->SetLineWidth(aux[3]);
  l4->SetLineColor(aux[0]);
  l4->Draw();

  // overlay TGraphAsymmErrors 2 (stat+syst) |----*-----|
  double vsizeErr2 = 0.12*vstep; 
  TLine* l5 = new TLine(mean, ypos, mean-nErr2, ypos);
  l5->SetLineWidth(aux[3]);
  l5->SetLineColor(aux[0]);
  l5->Draw();
  TLine* l6 = new TLine(mean, ypos, mean+pErr2, ypos);
  l6->SetLineWidth(aux[3]);
  l6->SetLineColor(aux[0]);
  l6->Draw();
  TLine* l7 = new TLine(mean-nErr2, ypos-vsizeErr2,
			mean-nErr2, ypos+vsizeErr2);
  l7->SetLineWidth(aux[3]);
  l7->SetLineColor(aux[0]);
  l7->Draw();
  TLine* l8 = new TLine(mean+pErr2, ypos-vsizeErr2,
			mean+pErr2, ypos+vsizeErr2);
  l8->SetLineWidth(aux[3]);
  l8->SetLineColor(aux[0]);
  l8->Draw();

  // draw measurement label "XXX+/-YY+/-ZZ"
  TPaveText* num = new TPaveText(uppX-0.32*widthX,
				 lowY, uppX-0.02*widthX, uppY, "BR");
  num->SetTextAlign(12);
  num->SetFillColor(aux[1]);
  num->SetTextColor(aux[0]);
  num->SetLineColor(aux[0]);
  num->SetBorderSize(0);
  TString str;
  char s[100];

  sprintf(s, "%4.2f#color[%d]{X}", m[0], aux[1]); str +=s;
  if (m[1]==m[2]) { // sym. stat. errors
    sprintf(s, "#pm %4.2f",  m[1]); str +=s;
  } else {
    sprintf(s, "^{+%4.2f}",  m[2]); str +=s;
    sprintf(s, "_{-#color[%d]{|}%4.2f}", aux[1], m[1]); str +=s;
  }
  str += " (stat)";
  if (m[3]!=0.0 || m[4]!=0.0) {
    if (m[3]==m[4]) { // sym. syst. errors
      sprintf(s, "#color[%d]{X}#pm% 4.2f", aux[1], m[3]); str +=s;
    } else {
      sprintf(s, "#color[%d]{X}", aux[1]); str +=s;
      sprintf(s, "^{+%4.2f}",  m[4]); str +=s;
      sprintf(s, "_{-#color[%d]{|}%4.2f}", aux[1], m[3]); str +=s;
    }
  }
  str += " (syst)";


  TText* n0 = num->AddText(str);
  n0->SetTextFont(aux[2]);
  num->Draw();

  return;
}

/*
 * Main Function
 */
void ratio() {

  gStyle->SetFrameLineWidth(1); 
                                
  //Number of Measurements
  const int NUM = 2;
  
  //Measurements and uncertainties
  // mean, -stat, +stat, -syst, +syst  
  double m[NUM][5] = {
    0.98, 0.10, 0.10, 0.14, 0.14,
    //1.33, 0.32, 0.32, 0.22, 0.22
    1.26, 0.37, 0.37, 0.46, 0.46
  };
  
  //Theory and uncertainties
  // mean, -uncert, +uncert
  double t[NUM][3] = {
    1.0, 0.114, 0.114,
    1.0, 0.111, 0.111
  };

  // label text, sub-label text
  // Note: TString does not work, b/c one cannot pass an array of TStrings
  // as an argument to a function
  char label[NUM][2][100] = {
    "Z#gamma#gamma", "",
    //"W#gamma#gamma", ""
    "W#gamma#gamma (#mu)", ""
  };

  // format: 
  // # color, bgColor, fontSytle, linewidth, markerStyle 
  int aux[NUM][5] = {
    1,  10, 42, 2, 20,
    1,  10, 42, 2, 20
  };

  // determning the x size of the plot
  double lowX = LOW_X;
  double uppX = UPP_X;

  
  TH2F* lft = new TH2F("lft", "", 50, lowX, uppX, 1, 0.0, 1.0);

  // height = NUM*unitHeight + 2*spacers + 1*bottomMargin + 0.5*topMargin
  const double unitHeight = 50.0; // even number
  const double height = (double(NUM)+2.0)*unitHeight+100.0+30.0;
  // how much to step each time to cover the vertical range of the histo in
  // exactly NUM+2 steps
  const double vstep  = unitHeight/(height-100.0-30.0);
  const double width = 800.0;

  printf("Canvas: width=%d, height=%d\n",
	 TMath::Nint(width), TMath::Nint(height));
  printf("Y-step = %6.4f\n", vstep);

  
  // Set canvas and margins
  TCanvas* canvas = new TCanvas("canvas", "canvas", 200, 0,
				TMath::Nint(width), TMath::Nint(height));
  
  canvas->SetFillColor(10);
  canvas->SetRightMargin(20.0/width);
  canvas->SetLeftMargin(20.0/width);
  canvas->SetBottomMargin(56.0/height);
  canvas->SetTopMargin(30.0/height);
  canvas->Draw();
  canvas->cd();

  //printf("TopMargin   : %6.4f\n", canvas->GetTopMargin());
  //printf("BottomMargin: %6.4f\n", canvas->GetBottomMargin());

  TAxis* xaxis = lft->GetXaxis();
  TAxis* yaxis = lft->GetYaxis();
  xaxis->CenterTitle(kTRUE);  
  xaxis->SetTitleSize(0.07);  
  xaxis->SetTitleFont(62);    
  xaxis->SetTitleOffset(1.1); 
  xaxis->SetNdivisions(6,5,0);
    
  xaxis->SetLabelOffset(0.01);
  xaxis->SetLabelSize(0.05);
  xaxis->SetLabelFont(42);
  yaxis->SetLabelSize(0.0);
  yaxis->SetNdivisions(-1);

  lft->SetXTitle("Cross Section Ratio  #sigma_{Exp} / #sigma_{Theory}");
  lft->SetYTitle("");
  lft->SetStats(kFALSE);
  lft->SetTitle("");
  lft->Draw();

    
  // Draw Theory Bands
  for (int i=0; i!=NUM; ++i) {
    drawTheory(i, t[i][0], t[i][1], t[i][2], vstep);
  }

  canvas->RedrawAxis(); 
  gPad->RedrawAxis(); 

  // Draw Measurements
  for (int i=0; i!=NUM; ++i) {
    drawMeasurement(i, m[i], label[i], aux[i], vstep, lft, canvas);
  }

  //Set CMS Preliminary marker
  int iPeriod=2; // 8 TeV
  int iPos = 0; //
  CMS_lumi( canvas, iPeriod, iPos );

  // Print
  //canvas->Print("Ratio_CrossSec_Exp_Theory.pdf");
  canvas->Draw();
}
