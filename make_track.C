#include "ejungwoo.h"
using namespace ejungwoo;

void make_track(Double_t uniformity = 0, Int_t numClusters = 200)
{
  gstat(1);
  fstat(1);
  version("dedx_test");
  Int_t numEvents = 1000;
  //Double_t dedxMax = 0.005;
  Double_t dedxMax = 0.0004;

  auto file = new TFile(Form("cluster_uniformity%.0f.root",uniformity));
  auto tree = (TTree *) file -> Get("cluster");

  Double_t edep;
  tree -> SetBranchAddress("e",&edep);

  auto hist70 = new TH1D("dedx_track_truncated",";dedx (track, truncated high 30 %)",100,0,dedxMax);
  auto histall = new TH1D("dedx_track",";dedx (track)",100,0,dedxMax*2);
  auto histAn = new TH1D("dedx_track_an",";dedx (track weird equation)",100,0,dedxMax);

  auto histn = new TH1D("hist_ncluster",";number of clusters",200,1,201);

  Int_t idx = 0;

  for (auto event=0; event<numEvents; ++event)
  {
    Double_t dedxArray[200] = {0};
    Int_t countNon0 = 0;
    for (auto icluster=0; icluster<numClusters; ++icluster)
    {
      tree -> GetEntry(idx++);

      if (edep==0)
        continue;

      dedxArray[countNon0++] = edep;
    }
    histn -> Fill(countNon0);
    
    std::sort(dedxArray, dedxArray+countNon0);

    Double_t emean70 = 0;
    for (auto icluster=0; icluster<Int_t(0.7*countNon0); ++icluster)
      emean70 += dedxArray[icluster]/countNon0;

    Double_t emean = 0;
    for (auto icluster=0; icluster<countNon0; ++icluster)
      emean += dedxArray[icluster]/countNon0;

    Double_t emeanAn = 0;
    for (auto icluster=0; icluster<countNon0; ++icluster)
      emeanAn += 1./sqrt(dedxArray[icluster]);
    emeanAn = 1./((emeanAn/countNon0)*(emeanAn/countNon0));

    if (emean70!=0)
      hist70 -> Fill(emean70);
    if (emean!=0)
      histall -> Fill(emean);
    if (emeanAn!=0)
      histAn -> Fill(emeanAn);
  }

  auto cvs = cc4(Form("summary_uniformity%.0f_n%d",uniformity,numClusters),750,700);
  div(cvs,2,2);
  cvs->cd(1);
  auto histcluster = tp("dedx_cluster",tree,"e","e!=0",";dedx (cluster)",200,0,dedxMax*2);
  histcluster -> Draw();; 
  histcluster -> SetName(Form("uniformity%.0f_n%d",uniformity,numClusters));

  //cvs->cd(2); make(histn) -> Draw();

  cvs->cd(2);
  make(histAn);
  auto fAn = fitg(histAn,5);
  histAn -> SetName(Form("sigma/mean=%.2f%%",100*fAn->GetParameter(2)/fAn->GetParameter(1)));
  histAn -> Draw();
  fAn -> Draw("samel");

  cvs->cd(3);
  make(hist70);
  auto f70 = fitg(hist70,5);
  hist70 -> SetName(Form("sigma/mean=%.2f%%",100*f70->GetParameter(2)/f70->GetParameter(1)));
  hist70 -> Draw();
  f70 -> Draw("samel");

  cvs->cd(4);
  make(histall);
  auto fall = fitg(histall,1.5);
  histall -> SetName(Form("sigma/mean=%.2f%%",100*fall->GetParameter(2)/fall->GetParameter(1)));
  histall -> Draw();
  fall -> Draw("samel");

  //cvs->cd(4); make(histall) -> Draw();

  save(cvs,"pdf");
}
