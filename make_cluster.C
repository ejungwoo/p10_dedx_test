#include "ejungwoo.h"
using namespace ejungwoo;

void make_cluster(Double_t uniformity = 4.)// mm
{
  auto rand = new TRandom3(0);

  auto file = new TFile("myrun.root");
  auto tree = (TTree *) file -> Get("step");

  gstat(0);
  Int_t worldSize = 1500;
  Int_t detectorSize = 1200;
  const Int_t numClusters = 240;

  version("dedx_test");
  auto cvs = cc("zx");
  cvs -> SetLogz();
  tp("zx",tree,"x:z","volumeID==1",";#it{z};#it{x}",200,-worldSize,worldSize,200,-1000,1000) -> Draw("colz");
  save(cvs,"pdf");

  Int_t event, volume;
  Double_t edep, x, z;

  tree -> SetBranchAddress("eventID",&event);
  tree -> SetBranchAddress("volumeID",&volume);
  tree -> SetBranchAddress("edep",&edep);
  tree -> SetBranchAddress("x",&x);
  tree -> SetBranchAddress("z",&z);


  auto file2 = new TFile(Form("cluster_uniformity%.0f.root",uniformity),"recreate");
  auto tree2 = new TTree("cluster","");

  Int_t id, iz;
  Double_t esum;

  tree2 -> Branch("id",&id);
  tree2 -> Branch("iz",&iz);
  tree2 -> Branch("e",&esum);

  Int_t n = tree -> GetEntries();
  id = 0;

  Double_t dzCluster = 2*detectorSize/numClusters;
  cout <<  dzCluster -uniformity << " " << dzCluster + uniformity << endl;
  Double_t eArray[numClusters] = {0};

  for (auto i=0; i<n; ++i) {

    tree -> GetEntry(i);

    if (event!=id)
    {
      cout << id << endl;
      for (auto j=0; j<numClusters; ++j) {
        iz = j;
        esum = eArray[j];
        tree2 -> Fill();
        eArray[j] = 0;
      }
    }

    id = event;

    if (x < -50 || x > 50)
      continue;

    z = z+detectorSize;

    auto iz2 = Int_t(z/dzCluster);
    auto z2 = z - iz2*(2*dzCluster);

    Double_t dx = dzCluster; 

    if (uniformity != 0) {
      if (z2<dzCluster-uniformity) {
        iz2 = iz2*2;
        dx = dzCluster-uniformity;
      }
      else {
        iz2 = iz2*2+1;
        dx = dzCluster+uniformity;
      }
    }

    if (iz2 < 0 || iz2 >= numClusters)
      continue;

    eArray[iz2] += edep/dx;
  }

  {
    cout << id << endl;
    for (auto j=0; j<numClusters; ++j) {
      iz = j;
      esum = eArray[j];
      tree2 -> Fill();
      eArray[j] = 0;
    }
  }

  file2 -> cd();
  tree2 -> Write();
}
