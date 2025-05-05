//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Wed Mar  5 07:58:11 2025 by ROOT version 6.34.04
// from TTree n/NTuple #n
// found on file: 309590_01.root
//////////////////////////////////////////////////////////

#ifndef CCEScan_h
#define CCEScan_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include "UTPars.h"

// Header file for the classes stored in the TTree if any.
class CCEScan {
public :
 
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain

// Fixed size dimensions of array or collections stored in the TTree if any.

   // Declaration of leaf types
   // Declaration of leaf types
   Int_t           eventType;
   Int_t           BXType;
   Int_t           triggerType;
   UShort_t        calibrationStep;
   UInt_t          nPVs;
   Int_t           nUTDigits;
   Float_t         hitChannelIDs[10000];   //[nUTDigits]
   Float_t         hitSIDs[10000];   //[nUTDigits]
   Float_t         hitXs[10000];   //[nUTDigits]
   Float_t         hitYs[10000];   //[nUTDigits]
   Float_t         hitZs[10000];   //[nUTDigits]
   Float_t         hitDxDy[10000];   //[nUTDigits]
   Float_t         hitClusterCharges[10000];   //[nUTDigits]
   Float_t         hitClusterSizes[10000];   //[nUTDigits]
   Int_t           nTracks;
   Float_t         trackPs[10000];   //[nTracks]
   Float_t         trackCStatePTs[10000];   //[nTracks]
   Float_t         trackXs[10000];   //[nTracks]
   Float_t         trackYs[10000];   //[nTracks]
   Float_t         trackTXs[10000];   //[nTracks]
   Float_t         trackTYs[10000];   //[nTracks]
   Float_t         tr2utlay0[10000];   //[nTracks]
   Float_t         tr2utlax0[10000];   //[nTracks]
   Float_t         tr2utlay1[10000];   //[nTracks]
   Float_t         tr2utlax1[10000];   //[nTracks]
   Float_t         tr2utlax2[10000];   //[nTracks]
   Float_t         tr2utlay2[10000];   //[nTracks]
   Float_t         tr2utlax3[10000];   //[nTracks]
   Float_t         tr2utlay3[10000];   //[nTracks]
   Float_t         tr2utnlay[10000];   //[nTracks]

   // List of branches
   TBranch        *b_eventType;   //!
   TBranch        *b_BXType;   //!
   TBranch        *b_triggerType;   //!
   TBranch        *b_calibrationStep;   //!
   TBranch        *b_nPVs;   //!
   TBranch        *b_nUTDigits;   //!
   TBranch        *b_hitChannelIDs;   //!
   TBranch        *b_hitSIDs;   //!
   TBranch        *b_hitXs;   //!
   TBranch        *b_hitYs;   //!
   TBranch        *b_hitZs;   //!
   TBranch        *b_hitDxDy;   //!
   TBranch        *b_hitClusterCharges;   //!
   TBranch        *b_hitClusterSizes;   //!
   TBranch        *b_nTracks;   //!
   TBranch        *b_trackPs;   //!
   TBranch        *b_trackCStatePTs;   //!
   TBranch        *b_trackXs;   //!
   TBranch        *b_trackYs;   //!
   TBranch        *b_trackTXs;   //!
   TBranch        *b_trackTYs;   //!
   TBranch        *b_tr2utlay0;   //!
   TBranch        *b_tr2utlax0;   //!
   TBranch        *b_tr2utlay1;   //!
   TBranch        *b_tr2utlax1;   //!
   TBranch        *b_tr2utlax2;   //!
   TBranch        *b_tr2utlay2;   //!
   TBranch        *b_tr2utlax3;   //!
   TBranch        *b_tr2utlay3;   //!
   TBranch        *b_tr2utnlay;   //!

   CCEScan(TTree *tree=0);
   virtual ~CCEScan();
   virtual Int_t    getHVID(TString layer, TString quad, int ihv);
   virtual Int_t    LayerNum(double zh);
   virtual Int_t    Cut(Long64_t entry);
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(TTree *tree);
   virtual void     Loop(bool batch = kFALSE);
   virtual bool     Notify();
   virtual void     Show(Long64_t entry = -1);
};

#endif

#ifdef CCEScan_cxx
CCEScan::CCEScan(TTree *tree) : fChain(0) 
{
// if parameter tree is not specified (or zero), connect the file
// used to generate this class and read the Tree.
  TString filenames[maxFiles];
  std::string line;

  ifstream InputFile;
  InputFile.open("./ROOTTupleList.inp");
  if (!InputFile.is_open()) {
    std::cerr << "Error opening file." << std::endl;
    return;
  }
  // Read in file to use
  std::getline(InputFile, line);
  TString nameOfInputFile = line;
  std::getline(InputFile, line);
  TString tupleLocation = line;
  cout << "******************************************************" << endl;  
  cout << "Using ROOT files in: " << nameOfInputFile << endl;
  cout << "Tuple Location:      " << tupleLocation << endl;
  cout << "******************************************************" << endl;  
  
  ifstream infile;
  infile.open(nameOfInputFile);
  if (!infile.is_open()) {
    std::cerr << "Error opening file." << std::endl;
    return;
  }
  int numFile = 0;
  TChain *chain = new TChain("n","n");
  while (std::getline(infile, line)) {
    std::cout << numFile << " " << line << std::endl;
    filenames[numFile] = line;
    chain->Add(filenames[numFile]+ "/" + tupleLocation);    
    numFile++;
  }
  cout << "Read in " << numFile << " files " << endl;
  infile.close();  

  tree = (TTree*)chain;
  Init(tree);

}

CCEScan::~CCEScan()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

Int_t CCEScan::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}
Long64_t CCEScan::LoadTree(Long64_t entry)
{
// Set the environment to read one entry
   if (!fChain) return -5;
   Long64_t centry = fChain->LoadTree(entry);
   if (centry < 0) return centry;
   if (fChain->GetTreeNumber() != fCurrent) {
      fCurrent = fChain->GetTreeNumber();
      Notify();
   }
   return centry;
}

void CCEScan::Init(TTree *tree)
{
   // The Init() function is called when the selector needs to initialize
   // a new tree or chain. Typically here the branch addresses and branch
   // pointers of the tree will be set.
   // It is normally not necessary to make changes to the generated
   // code, but the routine can be extended by the user if needed.
   // Init() will be called many times when running on PROOF
   // (once per file to be processed).

   // Set branch addresses and branch pointers
   if (!tree) return;
   fChain = tree;
   fCurrent = -1;
   fChain->SetMakeClass(1);

   fChain->SetBranchAddress("eventType", &eventType, &b_eventType);
   fChain->SetBranchAddress("BXType", &BXType, &b_BXType);
   fChain->SetBranchAddress("triggerType", &triggerType, &b_triggerType);
   fChain->SetBranchAddress("calibrationStep", &calibrationStep, &b_calibrationStep);
   fChain->SetBranchAddress("nPVs", &nPVs, &b_nPVs);
   fChain->SetBranchAddress("nUTDigits", &nUTDigits, &b_nUTDigits);
   fChain->SetBranchAddress("hitChannelIDs", hitChannelIDs, &b_hitChannelIDs);
   fChain->SetBranchAddress("hitSIDs", hitSIDs, &b_hitSIDs);
   fChain->SetBranchAddress("hitXs", hitXs, &b_hitXs);
   fChain->SetBranchAddress("hitYs", hitYs, &b_hitYs);
   fChain->SetBranchAddress("hitZs", hitZs, &b_hitZs);
   fChain->SetBranchAddress("hitDxDy", hitDxDy, &b_hitDxDy);
   fChain->SetBranchAddress("hitClusterCharges", hitClusterCharges, &b_hitClusterCharges);
   fChain->SetBranchAddress("hitClusterSizes", hitClusterSizes, &b_hitClusterSizes);
   fChain->SetBranchAddress("nTracks", &nTracks, &b_nTracks);
   fChain->SetBranchAddress("trackPs", trackPs, &b_trackPs);
   fChain->SetBranchAddress("trackCStatePTs", trackCStatePTs, &b_trackCStatePTs);
   fChain->SetBranchAddress("trackXs", trackXs, &b_trackXs);
   fChain->SetBranchAddress("trackYs", trackYs, &b_trackYs);
   fChain->SetBranchAddress("trackTXs", trackTXs, &b_trackTXs);
   fChain->SetBranchAddress("trackTYs", trackTYs, &b_trackTYs);
   fChain->SetBranchAddress("tr2utlay0", tr2utlay0, &b_tr2utlay0);
   fChain->SetBranchAddress("tr2utlax0", tr2utlax0, &b_tr2utlax0);
   fChain->SetBranchAddress("tr2utlay1", tr2utlay1, &b_tr2utlay1);
   fChain->SetBranchAddress("tr2utlax1", tr2utlax1, &b_tr2utlax1);
   fChain->SetBranchAddress("tr2utlax2", tr2utlax2, &b_tr2utlax2);
   fChain->SetBranchAddress("tr2utlay2", tr2utlay2, &b_tr2utlay2);
   fChain->SetBranchAddress("tr2utlax3", tr2utlax3, &b_tr2utlax3);
   fChain->SetBranchAddress("tr2utlay3", tr2utlay3, &b_tr2utlay3);
   fChain->SetBranchAddress("tr2utnlay", tr2utnlay, &b_tr2utnlay);

   Notify();
}

bool CCEScan::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return true;
}

void CCEScan::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}
Int_t CCEScan::Cut(Long64_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}
Int_t CCEScan::LayerNum(double zh){
  for(int j=0; j<4; j++){
    if(fabs(zh-zutlay[j])<25){ return j; }	  
  }
  return -1;

}

Int_t CCEScan::getHVID(TString layer, TString quad, int ihv){
  
  int ilnum = 0;
  int iquad = 0;
  int hvid = 0;
  if(layer=="UTaU"){ ilnum = 1;
  }else if(layer=="UTbV"){ ilnum = 2;
  }else if(layer=="UTbX"){ ilnum = 3;
  }
  if(quad=="CT"){ iquad = 1;
  }else if(quad=="CB"){ iquad = 2;
  }else if(quad=="AB"){ iquad = 3;
  }

  hvid = ilnum*4*nHVChan + nHVChan*iquad + ihv - 1;

  return hvid;
}

#endif // #ifdef CCEScan_cxx
