TString codeVersion = "v1.0";                                              // Code version
TString topLevelDir = "/home/sblusk/UT_PerfPaper/CCEScan/for_lxplus/";     // top level dir of output
TString inputDir = "/home/sblusk/UT_PerfPaper/CCEScan/for_lxplus/";        // dir of input file
bool testDir = false;  // Set to False when not running test, and want time-stamped subdir.

bool batchMode = true;
int messageLevel = 0;
bool createDTSubDir = true;
bool flipAC = true;
float VMin = 150;
float VMax = 350;
float defVoltageForHVGroup = 250;

float V_off[4] = {80, 60, 60, 60}; // Type A, B, C, D voltage offsets

float best_Voltage;
float best_ADC[n_sensors]={0};
float best_V[n_sensors]={0};
float best_VG[n_sensors]={0};
float adcGroup[nHVGroup];
int useMin5[n_sensors] = {0};
double xsen[n_sensors];
double ysen[n_sensors];
int layerNum[n_sensors];

float f1_low[] = { 48,  52,  35, 52};
float f1_hi[] =  {126, 126, 126, 126};
float f2_low[] = {199, 199, 199, 199};
float f2_hi[] =  {352, 352, 352, 352};

float deltaThresh = 70;
float vOff = 10.0;
int maxThrow = 2;
float minBestVoltage = 150;

float adcVal[n_sensors][nCalStep] = {0};
float adcVal5[n_sensors][nCalStep] = {0};
double r_chip[nASIC];
int rid_chip[nASIC];
int nchip[nASIC];
int stv[nASIC];
int module[nASIC];
int hvGroupSen[nHVGroup] = {-1};
int caenModNum[nHVGroup] = {-1};
int caenChNum[nHVGroup] = {-1};
float caenModChID[nHVGroup] = {-1};
int hvGroupNum[n_sensors];
TString hvGroupName[n_sensors];
TString hvGroupName2[n_sensors];
int nsensHVGroup[nHVGroup] = {0};
int sensInHVGroup[nHVGroup][maxGroup];
int sensType[n_sensors] = {-1};
int Thresh5[n_sensors] = {0};
int Thresh5Group[n_sensors] = {0};
int nThresh5 = 0;
bool setThresh5 = false;
int failReason = 0;
TString failText[5] = {"", "Bad SensID/Type"," <10 data in adcvshv", " <10 data in adcvshv5", "Slope<0.001"}; 

float senshv[nHVGroup][maxGroup] = {0};
float grouphv[nHVGroup] = {0};

int chip2sen[nASIC]={0};   // Map of ASIC# to Sensor #
TString senName[n_sensors]={"0"};   // 
int nSens;
TCanvas *c2;// = new TCanvas(Form("c2_%d",0),Form("Sens ID = %d",isen),1400,0,500,400);

namespace fs = std::filesystem;

Int_t getHVID(TString layer, TString quad, int ihv){
  
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

void getSensInfo(){

  TFile *fmap = new TFile("fmap.root","RECREATE");
  TString mapfile = "./ecsmap_all.csv";
  TTree *map =  new TTree("map","new tree");
  map->ReadFile(mapfile,"chdev/C:chID/I:UTID/I:lay/I:stave/I:side/I:tb/I:mod/I:face/I:nASIC/I:hybrid/C:cASIC/C:DCB/C:SOL40d/C:SOL40name/C:GBTXd/C:TELL40d/C:TELL40l/C:PCIe/C:flav/C:sid/C:mar1/C:mar2/C:mar3/C:lvrd/C:lvrc/I:CAENc/C:CAENchid/D:HVpart/I:pol/I:chRid/I:fstripR/I:mstripR/I:lstripR/I:rChip/D:xflip/I:ym/F:xm/F:xc/F:salt/I:OLDsalt/I:oldChipID/I:oldChipMap/C:ASICind/I:dpGBTX/C:GBTXframe/C:TELL40mut/C:flavorMUT/C:TELL40Link/I:PCIeLink/I:Block/I:Lane/I:ASIC_mut/I:ASICIDmut/I:TELL40InCon/I");


  //return;
  int chID, radID, stave, mod, face, HVpart, lay, side;
  double radChip, CAENchid;
  float xm, ym;
  TString ss;
  Char_t chdev[30];// = nullptr;

  map->SetBranchAddress("chdev", &chdev); // Flavor
  map->SetBranchAddress("chID", &chID);  // Chip ID
  map->SetBranchAddress("chRid", &radID);  // Chip ID
  map->SetBranchAddress("rChip", &radChip); // Flavor
  map->SetBranchAddress("stave", &stave); // Stave = 0 - 7 (aX, aU), 0 - 8 (bV, bX)
  map->SetBranchAddress("mod", &mod);  // Module 0 = 7
  map->SetBranchAddress("side", &side);  // Module 0 = 7
  map->SetBranchAddress("lay", &lay);  // layer
  map->SetBranchAddress("face", &face);  // Face: 0 or 1
  map->SetBranchAddress("HVpart", &HVpart);  // Face: 0 or 1
  map->SetBranchAddress("CAENchid", &CAENchid);  // CAEN ch id
  map->SetBranchAddress("xm", &xm);  // x middle of sensor
  map->SetBranchAddress("ym", &ym);  // y middle of sensor

  map->GetEntry(0);
  string firstChip = "blah";
  string lastChip;
  int sid = -1;
  int igrp, nsen;
  string mystr, mystr1, mystr2, mystr3, mystr4;


  for(int i=0; i< nHVGroup; i++){
    for(int j=0; j<9; j++){
      sensInHVGroup[i][j]=-1;
    }
  }  

  
  bool typeA, typeB, typeCD;
  Long64_t numentries = map->GetEntriesFast();
  int i1, i2;
  for(int j=0; j<numentries; j++){
    map->GetEntry(j);
    r_chip[chID] = radChip;
    rid_chip[chID] = radID;    
    mystr = chdev;
    // Get HV group name
    mystr1 = mystr.substr(0,4);
    mystr2 = mystr.substr(6,2);
    mystr3 = to_string(HVpart);
    mystr4 = mystr1+"_"+mystr2+"_HV"+mystr3;
    i1 = CAENchid;
    i2 = 1000*(CAENchid-i1+0.0001);
    //if(j < 200){ cout <<  CAENchid << " " << i1 << " " << i2 << endl;}

    lastChip = mystr.substr(0,11);
    if(lastChip != firstChip){
      sid++;
      layerNum[sid] = lay;
      hvGroupName[sid] = mystr4;
      //cout << chdev << " " << mystr4 << " " << hvGroupName[sid] << endl;
      igrp = getHVID(mystr1, mystr2, HVpart);    
      hvGroupNum[sid] = igrp;
      nsen = nsensHVGroup[igrp];
      sensInHVGroup[igrp][nsen] = sid;  
      caenModNum[igrp] = i1;
      caenChNum[igrp] = i2;
      caenModChID[igrp] = CAENchid;
      nsensHVGroup[igrp]++;
      //cout << sid << " " << igrp << " " << nsensHVGroup[igrp] << " " << hvGroupName[sid] << " " << hvGroupNum[sid] << " " << mystr1 << " " << mystr2 << " " << HVpart << endl;      
      senName[sid] = lastChip;
      firstChip = lastChip;
      if(HVpart == 1){sensType[sid] = 3; // Type D
      }else if(HVpart == 2){sensType[sid] = 2; // Type C
      }else if(HVpart >= 3 && HVpart<=5){sensType[sid] = 1; // Type B
      }else if(HVpart >= 6 && HVpart<=18){sensType[sid] = 0; // Type A
      }
      hvGroupSen[igrp] = sensType[sid]; // HV groups feed all the same type of sensors
      
      xsen[sid] = xm;
      if(flipAC){xsen[sid] = -1.0*xsen[sid];}         
      ysen[sid] = ym;
      //cout << j << " " << sid << " " << xsen[sid] << " " << ysen[sid] << endl;
    }

    chip2sen[j] = sid;
    nchip[chID] = 4;
    stv[chID] = stave;
    module[chID] = mod;
    if(mystr.find("W")!=string::npos || mystr.find("E")!=string::npos){nchip[chID] = 8;}    
    
    
  }
  sid++;
  nSens = sid;

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

void getadcs(TFile *f){

  TString s, s5;
  int ib;
  TH1F *h1;
  TH1F *h5;
  double v1, v2;
  for(int i=0; i<n_sensors; i++){
    s = Form("adcvshv_%d",i);
    if(useMin5[i]>0){s = Form("adcvshv5_%d",i);}
    s5 = Form("adcvshv5_%d",i);
    h1 = (TH1F*)f->Get(s);
    h5 = (TH1F*)f->Get(s5);
    for(int j=0; j<nCalStep; j++){
      ib = h1->FindBin(hv_vals[j]);
      v1 = h1->GetBinContent(ib);
      adcVal[i][j] = v1;
      v2 = h5->GetBinContent(ib);
      adcVal5[i][j] = v2;      
    }
  }

  return;
}

TCanvas* getBestVoltageLin(TFile *f, int isen, int ityp){

  TString s = Form("adcvshv_%d",isen);
  TH1F *h1 = (TH1F*)f->Get(s);
  best_Voltage = -1;
  best_ADC[isen] = 0;
  c2->cd();
  h1->Draw();
  if(isen<0 || ityp < 0){
    cout << "Error!! Sens ID, Type = " << isen << ", " << ityp << " ....  Skipping " << endl;
    failReason = 1;
    return c2;
  }
  if(h1->GetEntries() < 10){failReason = 2; return c2; }
  TH1F* h;
  TH2F* h2Low;
  TH2F* h2Hi;
  TH1D* hproj1;// = new TH1D("hproj1","",32,0,32);
  TH1D* hproj2;// = new TH1D("hproj2","",32,0,32);
  // Determine best ADC vs HV to use
  int ind1, ind2, j;
  double rm, x1, x2, y1, y2, sl, r1, r2, asym;
  int nBad = 0;

  for(int k=3; k<(nCalStep-1); k++){
    j = 2*k + 1;
    ind1 = (nCalStep-1) - k;
    ind2 = (nCalStep-1) - (k + 1);
    h2Low = (TH2F*)f->Get(Form("h2chAll_%d",ind1));
    h2Hi = (TH2F*)f->Get(Form("h2chAll_%d",ind2));
    hproj1 = (TH1D*)h2Low->ProjectionY("hproj1",isen+1,isen+1);    
    hproj2 = (TH1D*)h2Hi->ProjectionY("hproj2",isen+1,isen+1);    
    // Noise ratios
    r1 = hproj1->Integral(1,6) / hproj1->Integral();
    r2 = hproj2->Integral(1,6) / hproj2->Integral();
    //asym = (r2 - r1) / (r1 + r2);
    //if(r1>0.5 || (k==(nCalStep-1) && r2>0.5)){nBad++;}
    if(r1>0.3 || (k==(nCalStep-1) && r2>0.3)){nBad++;}
  }
  
  s = Form("adcvshv_%d",isen);
  setThresh5 = false;
  if(nBad > 6){
    setThresh5 = true;
    if(messageLevel>5){ cout << "Hight noise ifor ADC<5, requiring ADC>=5 " << endl; }
    s = Form("adcvshv5_%d",isen);
    useMin5[isen] = 1;
    h1 = (TH1F*)f->Get(s);
    h1->Draw();
    if(h1->GetEntries() < 10){ failReason = 3; return c2; }
  }
  delete hproj1;
  delete hproj2;
  h1 = (TH1F*)f->Get(s);

  double F1_LOW = f1_low[ityp];
  double F1_HI = f1_hi[ityp];
  double F2_LOW = f2_low[ityp];
  double F2_HI = f2_hi[ityp];
  TF1* f1 = new TF1("f1","[0]+[1]*x",F1_LOW,F1_HI);
  TF1* f2 = new TF1("f2","[0]+[1]*x",F2_LOW, F2_HI);
  f1->SetLineStyle(1);
  f2->SetLineStyle(1);
  f1->SetLineColor(2);
  f2->SetLineColor(2);
  f1->SetParameters(5,0.02);
  f2->SetParameters(15,-0.002);
  
  best_Voltage = -1; 
  best_ADC[isen] = -1;
  h1->Draw();
  //TCanvas *c2 = new TCanvas(Form("c2_%d",0),Form("Sens ID = %d",isen),1400,0,500,400);

  h1->Draw();
  h1->Fit(f1,"MERQ");
  h1->Fit(f2,"MERQ+");
  double* p1 = f1->GetParameters();
  double* p2 = f2->GetParameters();
  double vb = (p2[0]-p1[0]) / (p1[1]-p2[1]);
  if(p1[1]<0.001){
    if(messageLevel>5){ cout << " Slope for f1 is too small!! " << isen << ", slope = " << p1[1] << endl; }
    failReason = 4;
    return c2;
  }
  f1->SetLineStyle(2);
  f2->SetLineStyle(2);
  h1->Draw();
  f1->SetRange(F1_LOW,vb);
  f2->SetRange(vb,F2_HI);
  f1->DrawCopy("same");
  f2->DrawCopy("same");
  f1->SetLineStyle(1);
  f2->SetLineStyle(1);
  f1->SetRange(F1_LOW,F1_HI);
  f2->SetRange(F2_LOW,F2_HI);
  f1->Draw("same");
  f2->Draw("same");

  //cout << p1[0] << " " << p1[1] << " " << p2[0] << " " << p2[1] << " " << ", Intersection: " << vb << endl;
  
  float voff = V_off[ityp];
  best_Voltage = vb + voff; 
  best_ADC[isen] = f2->Eval(best_Voltage);

 
  // Print some info about a specificsensor, if necessary
  /*
  if(isen==690){ 
    cout << "**************************************************************************" << endl;
    cout << "Intersection, Best Voltage " << isen << " " << ityp << " " << vb << " " << best_Voltage << " " << best_ADC[isen] << endl;
    cout << "**************************************************************************" << endl;
  }
  */
  return c2;

}

std::string getCurrentDateTime() {
    auto now = std::chrono::system_clock::now();
    auto in_time_t = std::chrono::system_clock::to_time_t(now);

    std::stringstream ss;
    ss << std::put_time(std::localtime(&in_time_t), "D%Y-%m-%d_T%H-%M-%S");
    return ss.str();
}

TH2Poly* getPoly(TString name, TString title="") {

   TH2Poly *h2p = new TH2Poly();
   h2p->SetName(name);
   h2p->SetTitle(title);
   h2p->SetStats(kFALSE);
   h2p->SetMarkerSize(1.5);
   h2p->GetYaxis()->SetLabelSize(0.04);
   
   static const int nx1 = 2;
   static const int ny1 = 8;
   double x1L[nx1] = {-1,0};
   double x1H[nx1] = {0,1};
   double y1L[ny1] = {0.0,0.5,1.0,2.0,3.0,4.0,5.0,6.0};
   double y1H[ny1] = {0.5,1.0,2.0,3.0,4.0,5.0,6.0,7.0};

   static const int nx2 = 8;
   static const int ny2 = 7;
   double x2L[nx2] = {1,2,3,4,5,6,7,8};
   double x2H[nx2] = {2,3,4,5,6,7,8,9};
   double y2L[ny2] = {0.0,1.0,2.0,3.0,4.0,5.0,6.0};
   double y2H[ny2] = {1.0,2.0,3.0,4.0,5.0,6.0,7.0};
 
   for (int i = 0; i<nx1; i++) {
      for (int j = 0; j<ny1; j++) {
	//cout << i << " " << j << "      " << x1L[i] << " " << x1H[i] << "      " << y1L[j] << " " << y1H[j] << endl;
	h2p->AddBin(x1L[i], y1L[j], x1H[i], y1H[j]);
	h2p->AddBin(x1L[i], -y1L[j], x1H[i], -y1H[j]);
      }
   }
   for (int i = 0; i<nx2; i++) {
      for (int j = 0; j<ny2; j++) {
	//cout << i << " " << j << "      " << x2L[i] << " " << x2H[i] << "      " << y2L[j] << " " << y2H[j] << endl;
	h2p->AddBin( x2L[i],  y2L[j],  x2H[i],  y2H[j]);
	h2p->AddBin( x2L[i], -y2L[j],  x2H[i], -y2H[j]);
	h2p->AddBin(-x2L[i],  y2L[j], -x2H[i],  y2H[j]);
	h2p->AddBin(-x2L[i], -y2L[j], -x2H[i], -y2H[j]);
      }
   }
 
   h2p->GetXaxis()->SetTickLength(0);
   h2p->GetYaxis()->SetTickLength(0);


   /*
   TRandom ran;
   for (int i = 0; i<300000; i++) {
     h2p->Fill(ran.Gaus(0.,8), ran.Gaus(0.,5));
   }
   */
   return h2p;
}

void ReverseXAxis(TH2Poly * h){

  h->GetXaxis()->SetLabelOffset(999);
  h->GetXaxis()->SetTickLength(0);
  
  // Redraw the new axis 
  gPad->Update();
  TGaxis *newaxis = new TGaxis(gPad->GetUxmax(), 
                               gPad->GetUymin(),
                               gPad->GetUxmin(),
                               gPad->GetUymin(),
                               h->GetXaxis()->GetXmin(),
                               h->GetXaxis()->GetXmax(),
                               510,"-");
  newaxis->SetLabelOffset(-0.03);
  newaxis->Draw();
}

int getBestVoltage(double vg){
  float dV;
  float dVMin = 10000;
  int jbest = -1;
  for(int j=0; j<nCalStep; j++){
    dV = hv_vals[j]-vg;      
    if(fabs(dV) < dVMin){
      dVMin = fabs(dV);
      jbest = j;
    }
  }
  return jbest;  
}
