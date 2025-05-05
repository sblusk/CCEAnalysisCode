#define CCEScan_cxx
#include "CCEScan.h"
#include <TH2.h>
#include <TProfile.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <iostream>

//using namespace std;
namespace fs = std::filesystem;

std::string getCurrentDateTime() {
    auto now = std::chrono::system_clock::now();
    auto in_time_t = std::chrono::system_clock::to_time_t(now);

    std::stringstream ss;
    ss << std::put_time(std::localtime(&in_time_t), "D%Y-%m-%d_T%H-%M-%S");
    return ss.str();
}

bool createDirectoryIfNotExists(const std::string& path) {
    if (!fs::exists(path)) {
        return fs::create_directories(path);
    }
    return true; // Directory already exists
}

std::vector<int> getCanvasSize(int nsen) {
    std::vector<int> vec(4);
    if(nsen==1){
      vec[0]=400;vec[1]=300; vec[2]=1; vec[3]=1;
    }else if(nsen==2){
      vec[0]=800;vec[1]=300; vec[2]=2; vec[3]=1;
    }else if(nsen==3){
      vec[0]=1200;vec[1]=300; vec[2]=3; vec[3]=1;
    }else if(nsen==4){
      vec[0]=800;vec[1]=600;vec[2]=2; vec[3]=2;
    }else if(nsen==5){
      vec[0]=1200;vec[1]=600;vec[2]=3; vec[3]=2;
    }else if(nsen==6){
      vec[0]=1200;vec[1]=600;vec[2]=3; vec[3]=2;
    }else if(nsen>=7){
      vec[0]=1200;vec[1]=900;vec[2]=3; vec[3]=3;
    }
    return vec;
}


void setMargins(TCanvas *c, int ip, double l, double b, double r){
  c->cd(ip)->SetLeftMargin(l);
  c->cd(ip)->SetBottomMargin(b);
  c->cd(ip)->SetRightMargin(r);
  c->cd(ip)->SetTickx();
  c->cd(ip)->SetTicky();

}
void addGraphics(TH1 *h, TString Xtitle, TString Ytitle = "Events", int iCol = 1,
                 double titleSize = 0.065, double xtoff = 1.0, double ytoff=1.0, int ls=1) {
  h->SetXTitle(Xtitle);
  // h->SetFillColor(30);
  int bw = h->GetBinWidth(1);
  h->SetXTitle(Xtitle);
  h->SetYTitle(Ytitle);
  h->SetStats(kFALSE);
  //h->SetMinimum(0.0);
  //h->SetMaximum(1.1*h->GetMaximum());
  h->SetTitleSize(0.1);
  h->SetLineColor(iCol);
  h->SetLineStyle(ls);
  h->SetMarkerColor(iCol);
  h->SetMarkerSize(1.00);
  h->SetMarkerStyle(20);
  h->GetXaxis()->SetTitleOffset(xtoff);
  h->GetYaxis()->SetTitleOffset(ytoff);
  h->GetXaxis()->SetTitleSize(titleSize);
  h->GetYaxis()->SetTitleSize(titleSize);
  h->GetXaxis()->SetLabelSize(titleSize-0.005);
  h->GetYaxis()->SetLabelSize(titleSize-0.005);
  h->SetNdivisions(505, "X");
  h->SetNdivisions(505, "Y");
  h->SetLineWidth(2);
  //h->SetTitle("");
}

void addGraphics(TProfile *h, TString Xtitle, TString Ytitle = "Events", int iCol = 1,
                 double titleSize = 0.065, double xtoff = 1.0, double ytoff=1.0) {
  h->SetXTitle(Xtitle);
  // h->SetFillColor(30);
  h->SetXTitle(Xtitle);
  h->SetYTitle(Ytitle);
  h->SetStats(kFALSE);
  h->SetMinimum(0.0);
  //h->SetMaximum(1.1*h->GetMaximum());
  h->SetTitleSize(0.1);
  h->SetLineColor(iCol);
  h->SetMarkerColor(iCol);
  h->SetMarkerSize(0.75);
  h->SetMarkerStyle(20);
  h->GetXaxis()->SetTitleOffset(xtoff);
  h->GetYaxis()->SetTitleOffset(ytoff);
  h->GetXaxis()->SetTitleSize(titleSize);
  h->GetYaxis()->SetTitleSize(titleSize);
  h->GetXaxis()->SetLabelSize(titleSize-0.005);
  h->GetYaxis()->SetLabelSize(titleSize-0.01);
  h->SetNdivisions(404, "X");
  h->SetNdivisions(505, "Y");
  h->SetLineWidth(2);
  h->SetTitle("");
}

void addGraphics(TH2F *h, TString Xtitle, TString Ytitle = "Events", int iCol = 1,
                 double titleSize = 0.05, double xtoff = 0.8, double ytoff=0.8, double msiz = 0.5) {
  h->SetXTitle(Xtitle);
  // h->SetFillColor(30);
  h->SetXTitle(Xtitle);
  h->SetYTitle(Ytitle);
  h->SetStats(kFALSE);
  h->SetMinimum(0.0);
  h->SetMaximum(1.1*h->GetMaximum());
  h->SetTitleSize(0.1);
  h->SetLineColor(iCol);
  h->SetMarkerColor(iCol);
  h->SetMarkerSize(msiz);
  h->SetMarkerStyle(20);
  h->GetXaxis()->SetTitleOffset(xtoff);
  h->GetYaxis()->SetTitleOffset(ytoff);
  h->GetXaxis()->SetTitleSize(titleSize);
  h->GetYaxis()->SetTitleSize(titleSize);
  h->GetXaxis()->SetLabelSize(titleSize-0.005);
  h->GetYaxis()->SetLabelSize(titleSize-0.01);
  h->SetNdivisions(505, "X");
  h->SetNdivisions(505, "Y");
  h->SetLineWidth(2);
  //h->SetTitle("");
}

void CCEScan::Loop(bool batch = kTRUE){
  if (fChain == 0) return;

  gROOT->SetBatch(kTRUE); 

  // create plots directory
  std::string dirPath = "plots";
  if (createDirectoryIfNotExists(dirPath)) { std::cout << "Directory '" << dirPath << "' created (or already existed)." << std::endl;
  } else { std::cerr << "Failed to create directory '" << dirPath << "'." << std::endl; }

  ofstream outfile;
  outfile.open("./HVCHMAP_SB.dat");
  ofstream outfile2;
  outfile2.open("./SensNumName.dat");
  ofstream outfile3;
  outfile3.open("./Chip2SensNumName.dat");

  //=================
  // Read in ECS map
  //=================
  string mystr, mystr1, mystr2, mystr3, mystr4;

  TFile *fmap = new TFile("fmap.root","RECREATE");
  TString mapfile = "./ecsmap_all.csv";
  TTree *map =  new TTree("map","new tree");
  map->ReadFile(mapfile,"chdev/C:chID/I:UTID/I:lay/I:stave/I:side/I:tb/I:mod/I:face/I:nASIC/I:hybrid/C:cASIC/C:DCB/C:SOL40d/C:SOL40name/C:GBTXd/C:TELL40d/C:TELL40l/C:PCIe/C:flav/C:sid/C:mar1/C:mar2/C:mar3/C:lvrd/C:lvrc/I:CAENc/C:CAENchid/D:HVpart/I:pol/I:chRid/I:fstripR/I:mstripR/I:lstripR/I:rChip/D:xflip/I:ym/F:xm/F:xc/F:salt/I:OLDsalt/I:oldChipID/I:oldChipMap/C:ASICind/I:dpGBTX/C:GBTXframe/C:TELL40mut/C:flavorMUT/C:TELL40Link/I:PCIeLink/I:Block/I:Lane/I:ASIC_mut/I:ASICIDmut/I:TELL40InCon/I");


  //return;
  bool found;
  int chID, radID, stave, mod, face, HVpart, lay;
  double radChip;
  double r_chip[nASIC];
  int rid_chip[nASIC];
  int nchip[nASIC];
  int stv[nASIC];
  int module[nASIC];
  int fac[nASIC];
  int sensLayer[n_sensors];
  int sensQuad[n_sensors];
  int sensType[n_sensors] = {-1};
  Char_t chdev[30];// = nullptr;
  int hvGroupNum[n_sensors];
  int hvGroupNumQ[n_sensors];
  TString chipName[nASIC];
  TString hvGroupName[nASIC];
  TString hvGroupName2[nASIC];
  TString ss;
  map->SetBranchAddress("chdev", &chdev); // Flavor
  map->SetBranchAddress("lay", &lay); // Layer
  map->SetBranchAddress("chID", &chID);  // Chip ID
  map->SetBranchAddress("chRid", &radID);  // Chip ID
  map->SetBranchAddress("rChip", &radChip); // Flavor
  map->SetBranchAddress("stave", &stave); // Stave = 0 - 7 (aX, aU), 0 - 8 (bV, bX)
  map->SetBranchAddress("mod", &mod);  // Module 0 = 7
  map->SetBranchAddress("face", &face);  // Face: 0 or 1
  map->SetBranchAddress("HVpart", &HVpart);  // Face: 0 or 1

  int chip2sen[nASIC]={0};   // Map of ASIC# to Sensor #
  TString senName[n4chip]={"0"};   // 
  double xb[1024]={0};       // Boundaries, in ASIC#, between sensors, 0, 4, 8, 16, 24 ... 
  map->GetEntry(0);
  //string firstChip = chdev;
  //firstChip = firstChip.substr(0,11);
  string firstChip = "blah";
  string lastChip;
  int sid = -1;
  int igrp, nsen;

  int firstStripInSensor[n_sensors];

  int nsensHVGroup[nHVGroup] = {0};
  int sensInHVGroup[nHVGroup][9];
  for(int i=0; i< nHVGroup; i++){
    for(int j=0; j<9; j++){
      sensInHVGroup[i][j]=-1;
    }
  }    
  bool typeA, typeB, typeCD;
  Long64_t numentries = map->GetEntriesFast();
  for(int j=0; j<numentries; j++){
    map->GetEntry(j);
    r_chip[chID] = radChip;
    rid_chip[chID] = radID;    
    mystr = chdev;
    chipName[j] = mystr;
    // Get HV group name
    mystr1 = mystr.substr(0,4);
    mystr2 = mystr.substr(6,2);
    mystr3 = to_string(HVpart);
    mystr4 = mystr1+"_"+mystr2+"_HV"+mystr3;

    lastChip = mystr.substr(0,11);
    if(lastChip != firstChip){
      sid++;
      hvGroupName[sid] = mystr4;
      igrp = getHVID(mystr1, mystr2, HVpart);    
      hvGroupNum[sid] = igrp;
      hvGroupNumQ[sid] = HVpart;
      nsen = nsensHVGroup[igrp];
      sensInHVGroup[igrp][nsen] = sid;  
      nsensHVGroup[igrp]++;
      //cout << sid << " " << hvGroupName[sid] << " " << hvGroupNum[sid] << " " << mystr1 << " " << mystr2 << " " << HVpart << endl;    
      senName[sid] = lastChip;
      xb[sid] = j;
      firstChip = lastChip;
      sensLayer[sid] = lay; // Layer
      sensType[sid] = 0; // Type A
      if(HVpart>=3 && HVpart<=5){
	sensType[sid] = 1; // Type B
      }else if(HVpart==2){
	sensType[sid] = 2; // Type C
      }else if(HVpart==1){
	sensType[sid] = 3; // Type D
      }      
      if(chipName[j].Contains("AT")){sensQuad[sid] = 0;
      }else if(chipName[j].Contains("CT")){sensQuad[sid] = 1;
      }else if(chipName[j].Contains("CB")){sensQuad[sid] = 2;
      }else if(chipName[j].Contains("AB")){sensQuad[sid] = 3;
      }
      firstStripInSensor[sid] = j*nChASIC;      
    }

    chip2sen[j] = sid;
    nchip[chID] = 4;
    stv[chID] = stave;
    module[chID] = mod;
    fac[chID] = face;
    if(mystr.find("W")!=string::npos || mystr.find("E")!=string::npos){nchip[chID] = 8;}    
  }
  sid++;
  xb[sid] = xb[sid-1] + 4;
  int nSens = sid;

  for(int i=0;i<nSens; i++){
    outfile2 << i << " " << senName[i] << endl;
  }
  outfile2.close();  

  for(int i=0;i<numentries; i++){
    outfile3 << i << " " << chipName[i] << " " << chip2sen[i] << " " << senName[chip2sen[i]] << endl;
  }
  outfile3.close();  

  int iname, id;
  cout << "nSens = "<< nSens << endl;  
  for(int i=0; i<nHVGroup; i++){
    //cout << "HV group: " << i << " " << hvGroupName[i] << " SenID(s)=  " ;
    outfile << "HV group: " << i << " " << hvGroupName[i] << " SenID(s)=  " ;
    for(int j=0; j<9; j++){
      id = sensInHVGroup[i][j];
      //cout << id << " " ;
      outfile << id << " " ;
    }
    //cout << " SenName(s)=  " ;
    outfile << " SenName(s)=  " ;
    for(int j=0; j<nsensHVGroup[i]; j++){
      id = sensInHVGroup[i][j];
      //cout << senName[id] << " ";
      outfile << senName[id] << " ";
    }
    //cout << endl;
    outfile << endl;
  }
  outfile.close();

  std::string current_date_time = getCurrentDateTime();
  std::cout << "Current date and time: " << current_date_time << std::endl;
  //return;
  TString fout_name = "CCEScanResults_"+current_date_time;
  TFile *fout = new TFile(fout_name+".root","RECREATE");

  TH1F* calibEv = new TH1F("calibEv","#Cal Events",15,0,15);  
  TH2F* nHitSensVsCalib = new TH2F("nHitSensVsCalib","#Hits vs (Calib,SensID",n_sensors,0,1.0*n_sensors,
                                   nCalStep,0,nCalStep*1.0);
  
  TH1F *hhv[4];
  for(int k=0; k<4; k++){
    hhv[k] = new TH1F(Form("hhv_%d",k),"HV group #",290,0,290);    
  }

  
  for(int k=0; k<n_sensors; k++){
    ss = hvGroupName[k];
    if(ss.Contains("UTaX")){hhv[0]->Fill(1.0*hvGroupNum[k]); }
    if(ss.Contains("UTaU")){hhv[1]->Fill(1.0*hvGroupNum[k]); }
    if(ss.Contains("UTbV")){hhv[2]->Fill(1.0*hvGroupNum[k]); }
    if(ss.Contains("UTbX")){hhv[3]->Fill(1.0*hvGroupNum[k]); }
  }
  

  TCanvas *ccc = new TCanvas("ccc","",1200,800);
  ccc->Divide(2,2);
  ccc->cd(1); hhv[0]->Draw();
  ccc->cd(2); hhv[1]->Draw();
  ccc->cd(3); hhv[2]->Draw();
  ccc->cd(4); hhv[3]->Draw();

  //return;
  
  int ich;
  int iL, iSen, isensor;
  float res[4], xtr[4], ytr[4];
  float xt[4], yt[4], minRes[4];
  float xh, yh, rr, sign, xtv, ytv, v, q;
  bool good;
  TString s1, s2;
  int nhitASIC[nASIC]={0};
  int iAsic;
  float stripBest[4];
  float chargeBest[4];


  ///===================
  // Create histograms
  ///===================
  TH1F* hres[4];
  TH1F* ubhres[4];
  TH1F* ubhres_best[4];
  TH1F* ubhresb[4];
  TH2F* hitXY[4];
  TH2F* nohitXY[4];
  TH2F* h2ch[nCalStep][4];
  TProfile* hpch[nCalStep][4];
  TH2F* h2chAll[nCalStep];
  TH2F* h2chAllASIC[nCalStep];
  TProfile* hpchAll[nCalStep];
  TProfile* hpchAll5[nCalStep];
  TProfile* hpchAll6[nCalStep];
  TProfile* hpchAll7[nCalStep];
  for(int i=0; i<4; i++){
    s1 = Form("hres_%d",i);
    hres[i] = new TH1F(s1,s1,100,-0.4,0.4);
    s1 = Form("ubhres_%d",i);
    ubhres[i] = new TH1F(s1,s1+", Layer "+sLay[i],100,-1,1);
    s1 = Form("ubhres_best_%d",i);
    ubhres_best[i] = new TH1F(s1,s1+", Layer "+sLay[i],100,-1,1);
    s1 = Form("ubhresb_%d",i);
    ubhresb[i] = new TH1F(s1,s1+", Layer "+sLay[i],100,-1,1);
    hitXY[i] = new TH2F(Form("hitXY_%d",i),"Y vs X with UT Hits, Layer "+sLay[i],100,-800,800,100,-700,700);
    nohitXY[i] = new TH2F(Form("nohitXY_%d",i),"Y vs X with UT Hits, Layer "+sLay[i],100,-800,800,100,-700,700);
    for(int k=0; k<nCalStep; k++){
      s1 = Form(", %d V",hv_vals[k]);
      h2ch[k][i] = new TH2F(Form("h2ch_%d_%d",k,i),"ADC vs sen#, Layer "+sLay[i]+s1,nSens,0,nSens*1.0,32,0,32);
      hpch[k][i] = new TProfile(Form("hpch_%d_%d",k,i),"ADC vs sen#, Layer "+sLay[i]+s1,nSens,0,nSens*1.0,0,32);
    }
  }
  for(int k=0; k<nCalStep; k++){
    s1 = Form(", %d V",hv_vals[k]);
    h2chAll[k] = new TH2F(Form("h2chAll_%d",k),"ADC vs sen#, "+s1,nSens,0,nSens*1.0,32,0,32);
    hpchAll[k] = new TProfile(Form("hpchAll_%d",k),"ADC vs sen#, "+s1,nSens,0,nSens*1.0,0,32);
    hpchAll5[k] = new TProfile(Form("hpchAll5_%d",k),"ADC vs sen#, "+s1,nSens,0,nSens*1.0,0,32);
    hpchAll6[k] = new TProfile(Form("hpchAll6_%d",k),"ADC vs sen#, "+s1,nSens,0,nSens*1.0,0,32);
    hpchAll7[k] = new TProfile(Form("hpchAll7_%d",k),"ADC vs sen#, "+s1,nSens,0,nSens*1.0,0,32);
    h2chAllASIC[k] = new TH2F(Form("h2chAllASIC_%d",k),"ADC vs sen#, "+s1,nASIC,0,nASIC*1.0,32,0,32);
  }
  TH1F* hitsperasic = new TH1F("hitsperasic","",130,0,130);   
  
  double xbin[nCalStep*2+1];
  int ib = 0;
  //cout << nCalStep << endl;
  for(int k=nCalStep-1; k>=0; k--){
    xbin[ib++] = 1.0*hv_vals[k]-2;
    xbin[ib++] = 1.0*hv_vals[k]+2;     
  }
  xbin[ib++] = 425;
  int nbin = ib-1;
  TString title, mys, mys2;
  TH1F* adcvshv[n_sensors];
  TH1F* adcvshv5[n_sensors];
  TH1F* adcvshv6[n_sensors];
  TH1F* adcvshv7[n_sensors];
  for(int k=0; k<n_sensors; k++){
    mys = Form(", HVG=%d",hvGroupNum[k]);
    mys2 = Form("ID=%d, ",k);
    title = "Mean ADC vs HV, "+mys2+senName[k]+mys;
    adcvshv[k] = new TH1F(Form("adcvshv_%d",k),title,nbin,xbin);			   
    addGraphics(adcvshv[k],"Bias voltage [V]","Mean Charge [ADC]",1,0.05,0.9,0.9);
    adcvshv[k]->GetYaxis()->SetRangeUser(0,18);
    adcvshv[k]->SetStats(kFALSE);  
    adcvshv5[k] = new TH1F(Form("adcvshv5_%d",k),title,nbin,xbin);			   
    addGraphics(adcvshv5[k],"Bias voltage [V]","Mean Charge [ADC]",1,0.05,0.9,0.9);
    adcvshv5[k]->GetYaxis()->SetRangeUser(0,18);
    adcvshv5[k]->SetStats(kFALSE);  
    adcvshv6[k] = new TH1F(Form("adcvshv6_%d",k),title,nbin,xbin);			   
    addGraphics(adcvshv6[k],"Bias voltage [V]","Mean Charge [ADC]",1,0.05,0.9,0.9);
    adcvshv6[k]->GetYaxis()->SetRangeUser(0,18);
    adcvshv6[k]->SetStats(kFALSE);  
    adcvshv7[k] = new TH1F(Form("adcvshv7_%d",k),title,nbin,xbin);			   
    addGraphics(adcvshv7[k],"Bias voltage [V]","Mean Charge [ADC]",1,0.05,0.9,0.9);
    adcvshv7[k]->GetYaxis()->SetRangeUser(0,18);
    adcvshv7[k]->SetStats(kFALSE);  
  }

  TH1F* adcvshvg[nHVGroup];
  for(int k=0; k<nHVGroup; k++){
    mys = Form(", HVG=%d",k);
    title = "Mean ADC vs HV, "+mys;
    adcvshvg[k] = new TH1F(Form("adcvshvg_%d",k),title,nbin,xbin);			   
    addGraphics(adcvshvg[k],"Bias voltage [V]","Mean Charge [ADC]",1,0.05,0.9,0.9);
    adcvshvg[k]->GetYaxis()->SetRangeUser(0,18);
    adcvshvg[k]->SetStats(kFALSE);  
  }
  TH2F* h2chAllg[nCalStep];
  TProfile* hpchAllg[nCalStep];
  TProfile* hpchAllg5[nCalStep];
  TProfile* hpchAllg6[nCalStep];
  TProfile* hpchAllg7[nCalStep];
  for(int k=0; k<nCalStep; k++){
    s1 = Form(", %d V",hv_vals[k]);
    h2chAllg[k] = new TH2F(Form("h2chAllg_%d",k),"ADC vs HV Group, "+s1,nHVGroup,0,nHVGroup*1.0,32,0,32);
    hpchAllg[k] = new TProfile(Form("hpchAllg_%d",k),"ADC vs HV Group, "+s1,nHVGroup,0,nHVGroup*1.0,0,32);
    hpchAllg5[k] = new TProfile(Form("hpchAllg5_%d",k),"ADC vs HV Group, "+s1,nHVGroup,0,nHVGroup*1.0,0,32);
    hpchAllg6[k] = new TProfile(Form("hpchAllg6_%d",k),"ADC vs HV Group, "+s1,nHVGroup,0,nHVGroup*1.0,0,32);
    hpchAllg7[k] = new TProfile(Form("hpchAllg7_%d",k),"ADC vs HV Group, "+s1,nHVGroup,0,nHVGroup*1.0,0,32);
  }

  // Histos of ADC vs Bias for Type B sensors
  TH2F* h2chTypeBStrip[nCalStep][nLayer][nTypeB];
  int il, is;
  for(int k=0; k<nCalStep; k++){
    for(int j=0; j<nLayer; j++){
      for(int i=0; i<nTypeB; i++){
      s1 = Form(", %d V",hv_vals[k]);
      s2 = Form(", Sen %d",i);
      s1 = s1 + ", "+sLay[j]+s2;
      h2chTypeBStrip[k][j][i] = new TH2F(Form("h2chTypeBStrip_%d_%d_%d",k,j,i),"ADC vs Strip, Type B, "+s1,nCh8ASIC,0,nCh8ASIC*1.0,32,0,32);   
      }
    }
  }


  //return;

  //========================  
  // Begin Loop over events
  //========================
  
   int ntrk = 0;
   Long64_t nentries = fChain->GetEntriesFast();
   //nentries = 5000;
   cout << "Looping over #entries =" << nentries << endl;   

   Long64_t nbytes = 0, nb = 0;
   for (Long64_t jentry=0; jentry<nentries;jentry++) {
      Long64_t ientry = LoadTree(jentry);
      if (ientry < 0) break;
      if(jentry%10000==0){ cout << "Entry = " << jentry << endl; }      
      nb = fChain->GetEntry(jentry);   nbytes += nb;
      if(BXType!=3 || nTracks<=3){ continue; }      
      iCal = calibrationStep;
      calibEv->Fill(1.0*iCal);      

      // Loop over tracks
      for(int itr = 0; itr < nTracks; itr++){
        if(trackPs[itr]<2000){ continue; }
        if(trackCStatePTs[itr]<200){ continue; }
        yt[0] = tr2utlay0[itr];
        xt[0] = tr2utlax0[itr];
        yt[1] = tr2utlay1[itr];
        xt[1] = tr2utlax1[itr];
        yt[2] = tr2utlay2[itr];
        xt[2] = tr2utlax2[itr];
        yt[3] = tr2utlay3[itr];
        xt[3] = tr2utlax3[itr];
	
        for(int k=0; k<4; k++){
          minRes[k] = 10.0;       
        }
        ntrk++;
        
        // Loop over UT digits
        for(int ii = 0; ii<nUTDigits; ii++){
          iSen = 0;
          if(fabs(hitYs[ii])<90 && fabs(hitXs[ii])<80) { iSen = 1; }  // C or D type
          //if(hitSensorTypes[ii] >= 67){ iSen = 1;}
          iL = LayerNum(hitZs[ii]);
          if(iL <0 || iL > 3){ continue; }
          ytv = trackYs[itr] + trackTYs[itr]*(hitZs[ii] - zut);
          xtv = trackXs[itr] + trackTXs[itr]*(hitZs[ii] - zut);
          ytv += yt[iL] - (trackYs[itr] + trackTYs[itr]*(zutlay[iL] - zut));
          xtv += xt[iL] - (trackXs[itr] + trackTXs[itr]*(zutlay[iL] - zut));
          rr = hitXs[ii] + hitDxDy[ii]*(ytv-hitYs[ii]) - xtv;
          if(fabs(rr)<fabs(minRes[iL])){ 
            minRes[iL] = rr; 
            stripBest[iL] = hitSIDs[ii];
            chargeBest[iL] = hitClusterCharges[ii];
          }
          good = (iSen==0 && fabs(hitYs[ii]-ytv)<52) || (iSen==1 && fabs(hitYs[ii]-ytv)<28);
          if(good){ 
            ubhres[iL]->Fill(rr);
            if(fabs(rr)<0.2){ hitXY[iL]->Fill(xt[iL],yt[iL]);}
            if(fabs(rr)>0.3&&fabs(rr)<0.5){ nohitXY[iL]->Fill(xt[iL],yt[iL]);}
          }else{ 
            ubhresb[iL]->Fill(rr);
          }
        } // End loop over digits

        // Get best hit for each track
        for(int k=0; k<4; k++){
          ubhres_best[k]->Fill(minRes[k]);
          if(fabs(minRes[k]) < 1.0){
            ich = stripBest[k]/nChASIC;
            isensor = chip2sen[ich];
            q = chargeBest[k]+0.5;
            if(q < 32){
              h2chAllASIC[iCal]->Fill(ich,q);
              h2ch[iCal][k]->Fill(isensor,q);
              hpch[iCal][k]->Fill(isensor,q);
              h2chAll[iCal]->Fill(isensor,q);
              hpchAll[iCal]->Fill(isensor,q);
              igrp = hvGroupNum[isensor];
              h2chAllg[iCal]->Fill(igrp,q);
              hpchAllg[iCal]->Fill(igrp,q);
	      if(q >= 5){
		hpchAll5[iCal]->Fill(isensor,q);
		hpchAllg5[iCal]->Fill(igrp,q);
	      }
	      if(q >= 6){
		hpchAll6[iCal]->Fill(isensor,q);
		hpchAllg6[iCal]->Fill(igrp,q);
	      }
	      if(q >= 7){
		hpchAll7[iCal]->Fill(isensor,q);
		hpchAllg7[iCal]->Fill(igrp,q);
	      }
	      if(isensor>=0 && sensType[isensor] == 1){ // Type B
		v = stripBest[k] - firstStripInSensor[isensor];
		il = sensLayer[isensor];
		is = sensQuad[isensor]*3 + hvGroupNumQ[isensor] - 3;
		h2chTypeBStrip[iCal][il][is]->Fill(v+0.5,q);
	      }
            }
          }
        }
      }// end of loop over tracks
   } // End of loop over tuple entries

 
   TCanvas *c[20];
   int ic = -1;

   for(int i=0; i<nCalStep; i++){
     for(int j=0; j<4; j++){
       addGraphics(hpch[i][j],"Sensor number","Charge");
       addGraphics(h2ch[i][j],"Sensor","Charge");
     }
   }

   double ne;   
   gStyle->SetOptStat(0);
   float hv, ev;
   for(int k=0; k<nCalStep; k++){
     for(int i=0; i<n_sensors; i++){
       v = hpchAll[k]->GetBinContent(i+1);
       ev =hpchAll[k]->GetBinError(i+1);
       hv = 1.0*hv_vals[k];
       ib = adcvshv[k]->FindBin(hv);
       ne = hpchAll[k]->GetBinEntries(i+1);
       nHitSensVsCalib->SetBinContent(i+0.5,k+0.5,ne);
       if(v>2 && ne>100){
         adcvshv[i]->SetBinContent(ib,v);
         adcvshv[i]->SetBinError(ib,ev);
       } 
       v = hpchAll5[k]->GetBinContent(i+1);
       ev =hpchAll5[k]->GetBinError(i+1);
       hv = 1.0*hv_vals[k];
       ib = adcvshv5[k]->FindBin(hv);
       ne = hpchAll5[k]->GetBinEntries(i+1);
       if(v>2 && ne>100){
         adcvshv5[i]->SetBinContent(ib,v);
         adcvshv5[i]->SetBinError(ib,ev);
       } 
       v = hpchAll6[k]->GetBinContent(i+1);
       ev =hpchAll6[k]->GetBinError(i+1);
       hv = 1.0*hv_vals[k];
       ib = adcvshv6[k]->FindBin(hv);
       ne = hpchAll6[k]->GetBinEntries(i+1);
       if(v>2 && ne>100){
         adcvshv6[i]->SetBinContent(ib,v);
         adcvshv6[i]->SetBinError(ib,ev);
       } 
       v = hpchAll7[k]->GetBinContent(i+1);
       ev =hpchAll7[k]->GetBinError(i+1);
       hv = 1.0*hv_vals[k];
       ib = adcvshv7[k]->FindBin(hv);
       ne = hpchAll7[k]->GetBinEntries(i+1);
       if(v>2 && ne>100){
         adcvshv7[i]->SetBinContent(ib,v);
         adcvshv7[i]->SetBinError(ib,ev);
       } 
     }
   }

   for(int k=0; k<nCalStep; k++){
     for(int i=0; i<nHVGroup; i++){
       v = hpchAllg[k]->GetBinContent(i+1);
       ev = hpchAllg[k]->GetBinError(i+1);
       hv = 1.0*hv_vals[k];
       ib = adcvshvg[k]->FindBin(hv);
       ne = hpchAllg[k]->GetBinEntries(i+1);
       if(v>2 && ne>100){
         adcvshvg[i]->SetBinContent(ib,v);
         adcvshvg[i]->SetBinError(ib,ev);
       } 
     }
   }

   // Loop over HV groups and make plots of ADC of all sensrs in the group vs HV
   TCanvas *cc[300];
   int isen;   
   TString fname;
   for(int i=0; i<nHVGroup; i++){
     std::vector<int> ivec = getCanvasSize(nsensHVGroup[i]);
     cc[i] = new TCanvas(Form("cc_%d",i),Form("Charge vs V_{bias}, HV Group %d",i),1.*ivec[0],1.*ivec[1]);
     cc[i]->Divide(ivec[2],ivec[3]);
     for(int j=0; j<nsensHVGroup[i]; j++){
       cc[i]->cd(j+1);
       isen = sensInHVGroup[i][j];
       adcvshv[isen]->Draw();         
     }
     cc[i]->Update();
     cc[i]->Write();
     if(saveCanvas){
       fname  = Form("ADCvsHV_SensorsInGroup_%d",i);
       cc[i]->SaveAs("./plots/"+fname+".png");
       fname  = "ADCvsHV_AllSensorsInHVGroups";
       if(i==0){
         cc[i]->Print("./plots/"+fname+".pdf[");
         cc[i]->Print("./plots/"+fname+".pdf");
       }else if(i==nHVGroup-1){
         cc[i]->Print("./plots/"+fname+".pdf");
         cc[i]->Print("./plots/"+fname+".pdf]");
       }else{
         cc[i]->Print("./plots/"+fname+".pdf");
       }
     }
   }


   TCanvas *cc1[20];
   for(int i=0; i<nHVGroup/18; i++){ // 288/18 = 16 canvases 
     cc1[i] = new TCanvas(Form("cc1_%d",i),Form("Charge vs V_{bias}, HV Group %d",i),1600,1200);
     cc1[i]->Divide(5,4);
     for(int j=0; j<18; j++){
       cc1[i]->cd(j+1);
       igrp = 18*i + j;
       adcvshvg[igrp]->Draw();         
     }
     cc1[i]->Update();
     cc1[i]->Write();
     if(saveCanvas){
       fname  = Form("ADCvsHV_HVGroup_%d",i);
       cc1[i]->SaveAs("./plots/"+fname+".png");
       fname  = "ADCvsHV_AllHVGroups";
       if(i==0){
         cc1[i]->Print("./plots/"+fname+".pdf[");
         cc1[i]->Print("./plots/"+fname+".pdf");
       }else if(i==nHVGroup/18-1){
         cc1[i]->Print("./plots/"+fname+".pdf");
         cc1[i]->Print("./plots/"+fname+".pdf]");
       }else{
         cc1[i]->Print("./plots/"+fname+".pdf");
       }
     }
   }

   int ncal = 2;
   gStyle->SetOptStat(0);
   for(int i=0; i<ncal; i++){
     ic++;
     c[ic] = new TCanvas(Form("c_%d",ic),"Charge vs Sensor#",1600,800);
     c[ic]->Divide(2,2);
     for(int j=0; j<4; j++){
       c[ic]->cd(j+1);
       h2ch[i][j]->Draw();
       hpch[i][j]->Draw("same");
     }
   }
   c[ic]->Write();   


   ic++;
   c[ic] = new TCanvas(Form("c_%d",ic),"Unbiased Residuals/layer",800,800);
   gStyle->SetOptStat("tenmruo");
   gStyle->SetStatW(0.4);
   c[ic]->Divide(2,2);
   for(int j=0; j<4; j++){
     c[ic]->cd(j+1)->SetGrid();
     ubhres[j]->SetMinimum(0);
     ubhres[j]->Draw();
     ubhresb[j]->SetLineColor(2);
     ubhresb[j]->Draw("same");
   }

   ic++;
   c[ic] = new TCanvas(Form("c_%d",ic),"Min Unbiased Residuals/layer",800,800);
   c[ic]->Divide(2,2);
   for(int j=0; j<4; j++){
     c[ic]->cd(j+1)->SetGrid();
     ubhres_best[j]->SetMinimum(0);
     ubhres_best[j]->Draw();
     v = ubhres_best[j]->Integral() / ntrk;
     std::cout << "Average Eff, Layer = " << j << ": " << v << std::endl;
   }

   ic++;
   c[ic] = new TCanvas(Form("c_%d",ic),"Y vs X without UT hit",800,800);
   c[ic]->Divide(2,2);
   for(int j=0; j<4; j++){
     c[ic]->cd(j+1);
     nohitXY[j]->Draw();
   }

   ic++;
   c[ic] = new TCanvas(Form("c_%d",ic),"Y vs X with UT hit",800,800);
   c[ic]->Divide(2,2);
   for(int j=0; j<4; j++){
     c[ic]->cd(j+1);
     hitXY[j]->Draw();
   }

   fout->Write();

			 

}
