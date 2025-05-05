#include "UTPars.h"
#include "GetBestVoltage.h"
#include <iostream>

//OutputFile_294InputFiles.root" ){
void GetBestVoltage(TString filename = "CCEScanResults_D2025-04-11_T04-06-55.root"){
  
  if(batchMode){ gROOT->SetBatch(kTRUE); }
  filename = inputDir + filename;
  gErrorIgnoreLevel = kWarning;
  TFile *f = new TFile(filename);
  cout << "Opening file: " << filename << endl;
  getSensInfo();
  getadcs(f);

  //return;

  TString s;
  TLatex *myLatex = new TLatex();
  myLatex->SetTextFont(42); myLatex->SetTextColor(1); myLatex->SetTextAlign(12); 
  myLatex->SetTextSize(0.090);

  std::string current_date_time = getCurrentDateTime();
  if(testDir){current_date_time = "test";}
  TString tag = current_date_time;
  
  // create plots directory
  std::string dirPath = "results";
  if(createDTSubDir){ dirPath = dirPath + "_" + current_date_time; }

  TString subdir = dirPath;
  if (createDirectoryIfNotExists(dirPath)) { 
    std::cout << "Directory '" << dirPath << "' created (or already existed)." << std::endl;
  } else { std::cerr << "Failed to create directory '" << dirPath << "'." << std::endl; }

  ofstream outCSV;
  ofstream outCSV_A;
  ofstream outCSV_C;
  ofstream outCSV_A_SLIM;
  ofstream outCSV_C_SLIM;
  TString CSV_name = "CCEScanSettings_"+current_date_time;
  TString CSV_name_A = "CCEScanSettings_A_"+current_date_time;
  TString CSV_name_C = "CCEScanSettings_C_"+current_date_time;
  TString CSV_name_A_SLIM = "CCEScanSettings_A_SLIM_"+current_date_time;
  TString CSV_name_C_SLIM = "CCEScanSettings_C_SLIM_"+current_date_time;
  TString fout_name = "CCEScanSummary_"+current_date_time;

  TString outdir = topLevelDir + "/" + subdir;
  std::cout << "Writing results to: " << outdir << std::endl;
  cout << endl;
  outCSV.open(outdir+"/"+CSV_name+".csv");
  outCSV_A.open(outdir+"/"+CSV_name_A+".csv");
  outCSV_C.open(outdir+"/"+CSV_name_C+".csv");
  outCSV_A_SLIM.open(outdir+"/"+CSV_name_A_SLIM+".csv");
  outCSV_C_SLIM.open(outdir+"/"+CSV_name_C_SLIM+".csv");

  s = "Steve-A-side Date-Time " + tag + " " + codeVersion;
  outCSV_A_SLIM << "board,  ch,  id, " << s << endl;
  s = "Steve-C-side Date-Time " + tag + " " + codeVersion;
  outCSV_C_SLIM << "board,  ch,  id, " << s << endl;

  TFile *fout  = new TFile(outdir+"/"+fout_name+".root","RECREATE");
  TH1F* vmaxmin0 = new TH1F("vmaxmin0","#DeltaV (max - min)",100,0,200);
  TH1F* vmaxmin = new TH1F("vmaxmin","#DeltaV (max - min)",100,0,200);
  TH1F* vbsens[4];
  TH1F* vbgroup[4];
  TH1F* hadc[4];
  TH1F* hadcG[4];
  for(int k=0; k<4; k++){
    vbsens[k] = new TH1F(Form("vbsens_%d",k),"V_{set}^{sensor}, Type "+sType[k],50,100,500);
    vbgroup[k] = new TH1F(Form("vbgroup_%d",k),"V_{set}^{group}, Type "+sType[k],50,100,500);
    hadc[k] = new TH1F(Form("hadc_%d",k),"Mean ADC at V_{best}^{sensor}, Type "+sType[k],100,0,20);
    hadcG[k] = new TH1F(Form("hadcG_%d",k),"Mean ADC at V_{group}}, Type "+sType[k],100,0,20);
  }

  TH2Poly* GvbiasPlot[4];
  TH2Poly* SvbiasPlot[4];
  TH2Poly* GSvbiasPlot[4];
  TH2Poly* adcPlot[4];
  TH2Poly* adc5Plot[4];
  TH2Poly* groupPlot[4];
  GvbiasPlot[0] = getPoly("Group_Vbias_UTaX","Group V_{bias}, UTaX");
  GvbiasPlot[1] = getPoly("Group_Vbias_UTaU","Group V_{bias}, UTaU");
  GvbiasPlot[2] = getPoly("Group_Vbias_UTbV","Group V_{bias}, UTbV");
  GvbiasPlot[3] = getPoly("Group_Vbias_UTbV","Group V_{bias}, UTbX");
  SvbiasPlot[0] = getPoly("Sensor_Vbias_UTaX","Sensor V_{bias}, UTaX");
  SvbiasPlot[1] = getPoly("Sensor_Vbias_UTaU","Sensor V_{bias}, UTaU");
  SvbiasPlot[2] = getPoly("Sensor_Vbias_UTbV","Sensor V_{bias}, UTbV");
  SvbiasPlot[3] = getPoly("Sensor_Vbias_UTbV","Sensor V_{bias}, UTbX");
  GSvbiasPlot[0] = getPoly("GroupSensorDiff_Vbias_UTaX","Group-Sensor V_{bias}, UTaX");
  GSvbiasPlot[1] = getPoly("GroupSensorDiff_Vbias_UTaU","Group-Sensor V_{bias}, UTaU");
  GSvbiasPlot[2] = getPoly("GroupSensorDiff_Vbias_UTbV","Group-Sensor V_{bias}, UTbV");
  GSvbiasPlot[3] = getPoly("GroupSensorDiff_Vbias_UTbV","Group-Sensor V_{bias}, UTbX");
  adcPlot[0] = getPoly("MeanADC_UTaX","#LT ADC#GT, UTaX");
  adcPlot[1] = getPoly("MeanADC_UTaU","#LT ADC#GT, UTaU");
  adcPlot[2] = getPoly("MeanADC_UTbV","#LT ADC#GT, UTbV");
  adcPlot[3] = getPoly("MeanADC_UTbX","#LT ADC#GT, UTbX");
  adc5Plot[0] = getPoly("MeanADCge5_UTaX","#LT ADC #GT(#geq5), UTaX");
  adc5Plot[1] = getPoly("MeanADCge5_UTaU","#LT ADC #GT(#geq5), UTaU");
  adc5Plot[2] = getPoly("MeanADCge5_UTbV","#LT ADC #GT(#geq5), UTbV");
  adc5Plot[3] = getPoly("MeanADCge5_UTbX","#LT ADC #GT(#geq5), UTbX");
  groupPlot[0] = getPoly("Group_UTaX","Group Num, UTaX");
  groupPlot[1] = getPoly("Group_UTaX","Group Num, UTaU");
  groupPlot[2] = getPoly("Group_UTaX","Group Num, UTbV");
  groupPlot[3] = getPoly("Group_UTaX","Group Num, UTbX");

  // Loop over ech HV group, and get best operating voltage for each sensor in the group
  c2 = new TCanvas(Form("c2_%d",0),"ADC vs HV",1400,0,500,400);
  TCanvas *cc[300];
  int isen, ityp, iV;   
  float vgroupMax;
  float vgroupMaxSave;
  float vgroupMax1;
  float vgroupMax2;
  float vgroupMin;
  float adcMax, dV;
  TString fname, s2;
  TArrow *l;
  TArrow* l2;
  int nGroup = nHVGroup;
  int hvProb[300] = {0};
  int nhvProb = 0;
  int nSens, ilay;
  int noSensFitInfo[n_sensors] = {0};
  int num_noSensFitInfo = 0;
  int checkGroup[nHVGroup] = {0};
  int nSenSel[nHVGroup] = {0};
  float vminG[nHVGroup] = {-1};
  float vmaxG[nHVGroup] = {-1};
  float vmaxG0[nHVGroup] = {-1};
  int nZero = 0;  
  int zeroHVGroup[nHVGroup] = { -1 } ;
  
  int iSkip[9] = {0};
  int iVMax = -1;
  int nThrow, jBest;
  int nToss[nHVGroup] = {0};
  string mystr, mystr2, mystr3;
  bool aSide;
  for(int i=0; i<nGroup; i++){
    vgroupMax = -1;
    vgroupMax1 = -1;
    vgroupMax2 = -1;
    vgroupMin = 1000;
    checkGroup[i] = 0;
    iVMax = -1;
    for(int j=0; j<9; j++){
      iSkip[j] = 0;
    }
    //for(int i=0; i<nHVGroup; i++){     
     std::vector<int> ivec = getCanvasSize(nsensHVGroup[i]);
     cc[i] = new TCanvas(Form("cc_%d",i),Form("Charge vs V_{bias}, HV Group %d",i),1.*ivec[0],1.*ivec[1]);
     cc[i]->Divide(ivec[2],ivec[3]);
     nSens = 0;
     for(int j=0; j<nsensHVGroup[i]; j++){
       cc[i]->cd(j+1);
       isen = sensInHVGroup[i][j];
       ityp = sensType[isen];
       TCanvas* c = getBestVoltageLin(f,isen,ityp);
       cc[i]->cd(j+1);
       c->DrawClonePad();
       if(best_Voltage < minBestVoltage){ 
         noSensFitInfo[isen] =  failReason;
         num_noSensFitInfo++;
         continue; 
       }
       //if(best_ADC[isen] < 5.5){ continue; }
       best_V[isen] = best_Voltage;
       l = new TArrow(best_Voltage,best_ADC[isen]-5,best_Voltage,best_ADC[isen]-0.5,0.02,"|>");
       //l->SetArrowSize(0.03);
       l->SetLineColor(4);
       l->SetFillColor(4);
       l->SetLineWidth(2);
       cc[i]->cd(j+1);
       l->Draw();
       s = Form("V_{sen}=%3.0f V",best_Voltage);
       myLatex->SetTextColor(4);
       myLatex->DrawLatex(15,16,s);      
       myLatex->SetTextColor(1);
       myLatex->DrawLatex(125,4,senName[isen]);
       myLatex->DrawLatex(125,1.5,"Type " + sType[ityp]);      
       cc[i]->Update();      
       senshv[i][j] = best_Voltage;
       vbsens[ityp]->Fill(senshv[i][j]);
       nSens++;
       if(best_Voltage > vgroupMax){
         vgroupMax  = best_Voltage;
         adcMax = best_ADC[isen];
         iVMax = j;
       }
       if(best_Voltage < vgroupMin){
         vgroupMin  = best_Voltage;
       }
       if(setThresh5){
         Thresh5Group[nThresh5] = i;
         Thresh5[nThresh5] = isen;
         nThresh5++;      
       }
     }  
     nSenSel[i] = nSens;
     vgroupMaxSave = vgroupMax;
     dV = vgroupMax - vgroupMin;
     nThrow = 0;
     vminG[i] = vgroupMin;
     if(vgroupMax>150 && dV > deltaThresh && nSens>=2){// Something bad fit, large dV
       checkGroup[i] = 1;
       iSkip[iVMax] = 1;
       vgroupMax2 = 150;
       iVMax = -1;
       nThrow++;
       hvProb[nhvProb++]=i;
       for(int j=0; j<nsensHVGroup[i]; j++){
         if(iSkip[j]==0 && senshv[i][j] > vgroupMax2){
           vgroupMax2 = senshv[i][j];
           iVMax = j; 
         }
       }
       dV = vgroupMax2 - vgroupMin;
       vgroupMax1 = 0;
       if(dV>deltaThresh && nSens-nThrow >=3 && iVMax >= 0){ 
         iSkip[iVMax] = 1; nThrow++;
         for(int j=0; j<nsensHVGroup[i]; j++){
           if(iSkip[j]==0 && senshv[i][j] > vgroupMax1){ // Get highest V of remaining ones
             vgroupMax1 = senshv[i][j];
           }
         }
         vgroupMax2 = vgroupMax1;
       }
       nToss[i] = nThrow;
       //cout << "Change Vmax: " << i << ", Vmin: "<< vgroupMin << ", Old Vmax: " << vgroupMaxSave << ", New Vmax: " 
       //	    << vgroupMax1 << ", nThrow = " << nThrow << ", nSens = " << nSens << endl;
       vgroupMax = vgroupMax2;
     }
     vmaxG[i] = vgroupMax;
     vmaxG0[i] = vgroupMaxSave;
     vmaxmin0->Fill(vgroupMaxSave-vgroupMin);     
     vmaxmin->Fill(vgroupMax-vgroupMin);     
     iV = (vgroupMax + 10.0)/10;
     grouphv[i] = vgroupMax;
     grouphv[i] = 10.0*iV;
     if(vgroupMax < vgroupMaxSave){grouphv[i] = grouphv[i] + vOff; }
     for(int j=0; j<nsensHVGroup[i]; j++){
       cc[i]->cd(j+1);
       isen = sensInHVGroup[i][j];
       s = Form("V_{grp}=%3.0f V",grouphv[i]);
       myLatex->SetTextColor(2);
       myLatex->DrawLatex(260,16,s);
       myLatex->SetTextColor(1);
       if(senshv[i][j] > 10){
         l2 = new TArrow(grouphv[i],best_ADC[isen]-5,grouphv[i],best_ADC[isen],0.02,"|>");
         l2->SetLineColor(2);
         l2->SetLineWidth(2);
         l2->SetFillColor(2);
         l2->Draw();
       }
     }
     for(int j=0; j<nsensHVGroup[i]; j++){
       isen = sensInHVGroup[i][j];
       best_VG[isen] = grouphv[i];
       ilay = layerNum[isen];
       GvbiasPlot[ilay]->Fill(xsen[isen],ysen[isen],grouphv[i]);
       SvbiasPlot[ilay]->Fill(xsen[isen],ysen[isen],floor(senshv[i][j]));
       GSvbiasPlot[ilay]->Fill(xsen[isen],ysen[isen],floor(grouphv[i]-senshv[i][j]));
       jBest = getBestVoltage(grouphv[i]);
       groupPlot[ilay]->Fill(xsen[isen],ysen[isen],(i+0.1)*1.0);
       if(jBest>=0){ 
         adcPlot[ilay]->Fill(xsen[isen],ysen[isen],adcVal[isen][jBest]);
         adc5Plot[ilay]->Fill(xsen[isen],ysen[isen],adcVal5[isen][jBest]);
       } 
     }
     isen = sensInHVGroup[i][0];
     ityp = sensType[isen];
     vbgroup[ityp]->Fill(grouphv[i]);
     mystr = hvGroupName[isen].Data();
     mystr2 = "";
     mystr3 = "";
     if(grouphv[i] < 10){
       mystr3 = "No bias can be determined -- HV set to default value";
       zeroHVGroup[nZero] = i;
       nZero++;
       grouphv[i] = defVoltageForHVGroup;
     }
     if(hvGroupSen[i]>=0 && hvGroupSen[i]<=3){mystr2 = "Type"+sType[hvGroupSen[i]];}
     aSide = (hvGroupName[isen].Contains("AT") || hvGroupName[isen].Contains("AB"));
     s = Form("Group %d, %s,  %6.3f, %d, %d, %s, %3.0f",i,mystr2.c_str(),caenModChID[i],caenModNum[i],
	      caenChNum[i],mystr.c_str(),grouphv[i]);
     s2 = Form("%d,  %d, %6.3f, %3.0f,  %s",caenModNum[i],caenChNum[i],caenModChID[i],grouphv[i],mystr3.c_str());
     cout << s;
     outCSV << s;
     if(aSide){
       outCSV_A << s;
       outCSV_A_SLIM << s2 << endl;
     }else{
       outCSV_C << s;
       outCSV_C_SLIM << s2 << endl;
     }
     for(int k=0; k<nsensHVGroup[i]; k++){
       isen = sensInHVGroup[i][k];
       if(isen < 0 || isen > n_sensors-1){
         cout << "Bad sensor ID! " << hvGroupName[i] << " " << k << endl;
         continue;
       }
       s = Form("%3.0f",senshv[i][k]);
       cout << ", " << senName[isen] << ", " << s;
       outCSV << ", " << senName[isen] << ", " << s;
       if(aSide){outCSV_A << ", " << senName[isen] << ", " << s;}
       else{outCSV_C << ", " << senName[isen] << ", " << s;}
     }
     if(checkGroup[i]>0){
       //cout << " ....Large DEV!! " << ", " << vgroupMin << ", " << vgroupMax << ", " << vgroupMax1; 
       outCSV << ",    ....Vgroup updated!! " << ", " << vgroupMin << ", " << vgroupMax << ", " << vgroupMaxSave;      
     if(aSide){outCSV_A << ",    ....Vgroup updated!! " << ", " << vgroupMin << ", " << vgroupMax << ", " << vgroupMaxSave;}
     else{outCSV_C << ",    ....Vgroup updated!! " << ", " << vgroupMin << ", " << vgroupMax << ", " << vgroupMaxSave; }
     }
     cout << endl;
     outCSV << endl;
     if(aSide){outCSV_A << endl;}
     else{outCSV_C << endl;}

     cc[i]->Update();
     cc[i]->Write();
     if(saveCanvas){
       fname  = Form("ADCvsHV_SensorsInGroup_%d",i);
       cc[i]->SaveAs(outdir+"/"+fname+".png");
       fname  = "ADCvsHV_AllSensorsInHVGroups";
       if(i==0){
         cc[i]->Print(outdir+"/"+fname+".pdf[");
         cc[i]->Print(outdir+"/"+fname+".pdf");
       }else if(i==nGroup-1){
         cc[i]->Print(outdir+"/"+fname+".pdf");
         cc[i]->Print(outdir+"/"+fname+".pdf]");
       }else{
         cc[i]->Print(outdir+"/"+fname+".pdf");
       }
     }     
   }

  fout->cd();
  gROOT->SetBatch(kFALSE); // Turn off batch mode

  ofstream outMess;
  TString MessageFile = "MessageFile_"+tag;
  outMess.open(outdir+"/"+MessageFile+".txt");

  TCanvas *csbias = new TCanvas("csbias","Sensor Bias Values",1000,1000);
  csbias->Divide(2,2);
  for(int i=0; i<4; i++){
    csbias->cd(i+1);
    addGraphics(vbsens[i],"Sensor bias values [V]","",1,0.05);
    vbsens[i]->SetStats(kTRUE);
    vbsens[i]->Draw();
    vbsens[i]->Write();
  }
  fname  = "SensorsBiasValuesBySensor";
  csbias->SaveAs(outdir+"/"+fname+".png");
  csbias->Write();

  TCanvas *csgroup = new TCanvas("csgroup","HV Group Bias Values",1000,1000);
  csgroup->Divide(2,2);
  for(int i=0; i<4; i++){
    csgroup->cd(i+1);
    addGraphics(vbgroup[i],"Group bias values [V]","",1,0.05);
    vbgroup[i]->SetStats(kTRUE);
    vbgroup[i]->Draw();
    vbgroup[i]->Write();
  }
  fname  = "SensorsBiasValuesByGroup";
  csgroup->SaveAs(outdir+"/"+fname+".png");
  csgroup->Write();

  // Get ADC value at best sensor voltage and group voltage (closest value to measured points)
  float dVMin, dVMinG, bb, bbG, dVG;
  static const int flagMax = 100;
  int iSenFlag[flagMax];
  int nFlag = 0;
  int iFlagStep[flagMax];
  int iStepG;
  TH2F *h2;
  TH1D *hp;
  int iBestG[n_sensors] = {-1};

  cout << endl;
  cout << "Sensors with Mean 3 < ADC < 10" << endl; 
  cout << "------------------------------" << endl;
  outMess << endl;
  outMess << "Sensors with Mean 3 < ADC < 10" << endl; 
  outMess << "------------------------------" << endl;
  for(int i=0; i<n_sensors; i++){    
    dVMin = 1000;
    dVMinG = 1000;
    iStepG = -1;
    for(int j=0; j<nCalStep; j++){
      dV = hv_vals[j]-best_V[i];
      dVG = hv_vals[j]-best_VG[i];
      if(fabs(dV) < dVMin){
        dVMin = fabs(dV);
        bb = adcVal[i][j];
      }
      if(fabs(dVG) < dVMinG){
        dVMinG = fabs(dVG);
        bbG = adcVal[i][j];
        iStepG = j;
      }
    }
    if(iStepG>=0){iBestG[i] = iStepG;}
    ityp = sensType[i];
    hadc[ityp]->Fill(bb);
    hadcG[ityp]->Fill(bbG);
    if(bbG < 10 && bbG > 3){
      if(nFlag<flagMax){
        iSenFlag[nFlag] = i;
        iFlagStep[nFlag] = iStepG;
        nFlag++;
      }
      cout << "nFlag, Num, SenID, SenName, MeanADC, V_best_sen, deltaV, V_best_Grp:  " 
           << nFlag << " " << i << " " << senName[i] << " " << bbG << " " << best_V[i] 
           << " " << dVMinG << " " << best_VG[i] << endl;      
      outMess << "nFlag, Num, SenID, SenName, MeanADC, V_best_sen, deltaV, V_best_Grp:  " 
              << nFlag << " " << i << " " << senName[i] << " " << bbG << " " << best_V[i] 
              << " " << dVMinG << " " << best_VG[i] << endl;      
    }    
  }

  if(nZero>0){
    cout << endl;
    cout << "HV Groups where no Bias Voltage was obtained, set to 250 V" << endl; 
    cout << "----------------------------------------------------------" << endl;    
    outMess << endl;
    outMess << "HV Groups where no Bias Voltage was obtained, set to 250 V" << endl; 
    outMess << "----------------------------------------------------------" << endl;    
    for(int i=0; i<nZero; i++){
      cout << "No: " << i << ", " << "HV Group " << zeroHVGroup[i] << endl;
      outMess << "No: " << i << ", " << "HV Group " << zeroHVGroup[i] << endl;
    }  
  }
  
 


  TCanvas *cadc = new TCanvas("cadc","Sensor ADC Values (at best Sensor HV Value)",1000,1000);
  cadc->Divide(2,2);
  for(int i=0; i<4; i++){
    cadc->cd(i+1);
    addGraphics(hadc[i],"Mean ADC [V] at V(sensor)","",1,0.05);
    hadc[i]->SetStats(kTRUE);
    hadc[i]->Draw();
    hadc[i]->Write();
  }
  fname  = "SensorMeanADCValuesBySensor";
  cadc->SaveAs(outdir+"/"+fname+".png");
  cadc->Write();

  TCanvas *cadcG = new TCanvas("cadcG","Sensor ADC Values (at best HV Group Value)",1000,1000);
  cadcG->Divide(2,2);
  for(int i=0; i<4; i++){
    cadcG->cd(i+1);
    addGraphics(hadcG[i],"Mean ADC [V] at V(group)","",1,0.05);
    hadcG[i]->SetStats(kTRUE);
    hadcG[i]->Draw();
    hadcG[i]->Write();
  }
  fname  = "SensorMeanADCValuesByGroup";
  cadcG->SaveAs(outdir+"/"+fname+".png");
  cadcG->Write();

  // Show distributions of low ADC sensors
  TCanvas *ca[10];
  int inc = (1.0*nFlag-0.1)/20 + 1;
  int is;
  TH1F* h1;
  for(int i=0; i<inc; i++){
    ca[i] = new TCanvas(Form("ca_%d",i),"Flagged Sensors",1600,1000);    
    ca[i]->Divide(5,4);
    for(int j=0; j<20; j++){
      ca[i]->cd(j+1);
      is = 20*i + j;
      if(is >= nFlag){ break; }
      isen = iSenFlag[is];
      s = Form("adcvshv_%d",isen);
      if(useMin5[isen]>0){s = Form("adcvshv5_%d",isen);}
      h1 = (TH1F*)f->Get(s);      
      h1->Draw("e");
    }
    fname  = Form("FlaggedSensors_ADCvsVbias_lt_10_%d",i);
    ca[i]->SaveAs(outdir+"/"+fname+".png");  
  }


  TCanvas *cb[10];
  int istep, ib;
  TH1D* hproj[100];
  //gStyle->SetPalette(55);
  //gStyle->SetPalette(kCool);
  //gStyle->SetPalette(kLightTerrain);
  //gStyle->SetPalette(kNeon);
  gStyle->SetStatW(0.35);
  gStyle->SetStatH(0.22);
  for(int i=0; i<inc; i++){
    cb[i] = new TCanvas(Form("cb_%d",i),"Flagged Sensors",1600,1000);    
    cb[i]->Divide(5,4);
    for(int j=0; j<20; j++){
      cb[i]->cd(j+1);
      is = 20*i + j;
      if(is >= nFlag){ break; }
      isen = iSenFlag[is];
      istep = iFlagStep[is];
      s = Form("h2chAll_%d",istep);
      h2 = (TH2F*)f->Get(s);   
      s2 = Form("hproj_%d",is);
      hproj[is] = new TH1D(s2,"",32,0,32);     
      hproj[is] = (TH1D*)h2->ProjectionY(s2,isen+1,isen+1);       
      if(useMin5[isen]>0){
	ib = hproj[is]->FindBin(4.5);
	hproj[is]->SetBinContent(ib,0.0);
      }      
      hproj[is]->Draw();
    }
    fname  = Form("FlaggedSensors_ADCDist_lt_10_%d",i);
    cb[i]->SaveAs(outdir+"/"+fname+".png");  
  }


  fout->cd();
  // Look at groups with "high" bias voltage, or large Vmax - Vmin
  cout << endl;
  cout << "Groups with 1 or 2 sensors dropped from Group HV determination" << endl;
  cout << "--------------------------------------------------------------" << endl;
  outMess << endl;
  outMess << "Groups with 1 or 2 sensors dropped from Group HV determination" << endl;
  outMess << "--------------------------------------------------------------" << endl;
  int ic = 1;
  for(int i=0; i<nGroup; i++){
    if(checkGroup[i] > 0){
      cout << "  HV modified: " << ic << " " << i 
           << " had large dV! VMin, VMax, NewVMax, setValue, nToss, nSens, nSenSel " 
           << vminG[i] << " " << vmaxG[i]  << " " << vmaxG0[i] << " " << grouphv[i] << " " << nToss[i] << " " 
           << nsensHVGroup[i] << " " << nSenSel[i] << endl;
      outMess << "  HV modified: " << ic << " " << i 
           << " had large dV! VMin, VMax, NewVMax, setValue, nToss, nSens, nSenSel " 
           << vminG[i] << " " << vmaxG[i]  << " " << vmaxG0[i] << " " << grouphv[i] << " " << nToss[i] << " " 
           << nsensHVGroup[i] << " " << nSenSel[i] << endl;
      ic++;
    }
  }
  cout << endl;
  cout << "Groups with HV > 260 V" << endl;
  cout << "----------------------" << endl;
  outMess << endl;
  outMess << "Groups with HV > 260 V" << endl;
  outMess << "----------------------" << endl;
  for(int i=0; i<nGroup; i++){
    if(grouphv[i] > 260){
      cout << "  Group # " << i << ", Group HV = " << grouphv[i] << ", nSens, nSensSel = " << nsensHVGroup[i] 
           << " " << nSenSel[i] << endl;
      outMess << "  Group # " << i << ", Group HV = " << grouphv[i] << ", nSens, nSensSel = " << nsensHVGroup[i] 
           << " " << nSenSel[i] << endl;
    }    
  }

  cout << endl;
  outMess << endl;
  if(nThresh5>0){
    cout << "********************************************************"<< endl;
    cout << "The following sensors used ADC >= 5 due to high noise, check plots! " << endl;
    cout << "------------------------------------------------------------------- " << endl;
    outMess << "********************************************************"<< endl;
    outMess << "The following sensors used ADC >= 5 due to high noise, check plots! " << endl;
    outMess << "------------------------------------------------------------------- " << endl;
    for(int i=0; i<nThresh5; i++){
      cout << "   HVGroup " << Thresh5Group[i] << ", Sensor " << Thresh5[i] << ", SenName = " 
           << senName[Thresh5[i]] << endl;  
      outMess << "   HVGroup " << Thresh5Group[i] << ", Sensor " << Thresh5[i] << ", SenName = " 
           << senName[Thresh5[i]] << endl;  
    }	
    cout << "********************************************************"<< endl;
    outMess << "********************************************************"<< endl;
  }

  cout << endl;
  cout << "Sensors with no fit information: " << num_noSensFitInfo << endl;
  cout << "------------------------------------" << endl;
  outMess << endl;
  outMess << "Sensors with no fit information: " << num_noSensFitInfo << endl;
  outMess << "------------------------------------" << endl;
  int ir;
  for(int i=0;i<n_sensors;i++){
    ir = noSensFitInfo[i];
    if(ir==0){ continue; }
    cout << "  SenID, Name = " << i << ", " << senName[i] << ", Reason Code: " << ir << " = " << failText[ir] << endl;
    outMess << "  SenID, Name = " << i << ", " << senName[i] << ", Reason Code: " << ir << " = " << failText[ir] << endl;
  
  }

  fout->cd();
  TLine *line = new TLine(0,-7,0,7);
  TLine *vline = new TLine(-9,0,9,0);
  line->SetLineWidth(3);
  line->SetLineColor(1);
  vline->SetLineWidth(3);
  vline->SetLineColor(1);
  myLatex->SetTextFont(42); myLatex->SetTextColor(1); myLatex->SetTextAlign(12); 
  myLatex->SetTextSize(0.050); myLatex->SetNDC(kTRUE);
  // Group Bias voltage map
  TCanvas *cgBias = new TCanvas("cgBias","Group Bias Voltages",1400,1200);
  cgBias->Divide(2,2);
  for(int i=0; i<4; i++){
    cgBias->cd(i+1); GvbiasPlot[i]->Draw("colz,text");  
    if(flipAC){ ReverseXAxis(GvbiasPlot[i]); }    
    GvbiasPlot[i]->GetZaxis()->SetRangeUser(VMin,VMax);
    myLatex->DrawLatex(0.15,0.93,"A Side");
    myLatex->DrawLatex(0.75,0.93,"C Side");    
    line->Draw(); vline->Draw();    
    GvbiasPlot[i]->Write();
  }
  cgBias->Update();
  cgBias->Write();
  fname  = "Group_Vbias";
  cgBias->SaveAs(outdir+"/"+fname+".png");

  // Sensor bias voltage map
  TCanvas *csBias = new TCanvas("csBias","Sensor Bias Voltages",1400,1200);
  csBias->Divide(2,2);
  for(int i=0; i<4; i++){
    csBias->cd(i+1); SvbiasPlot[i]->Draw("colz,text");  
    if(flipAC){ ReverseXAxis(SvbiasPlot[i]); }    
    SvbiasPlot[i]->GetZaxis()->SetRangeUser(VMin,VMax);
    myLatex->DrawLatex(0.15,0.93,"A Side");
    myLatex->DrawLatex(0.75,0.93,"C Side");    
    line->Draw();  vline->Draw();        
    SvbiasPlot[i]->Write();
  }
  csBias->Update();
  csBias->Write();
  fname  = "Sensor_Vbias";
  csBias->SaveAs(outdir+"/"+fname+".png");

  // Group-Sensor bias voltage map
  TCanvas *cgsBias = new TCanvas("cgsBias","Group-Sensor Bias Voltages",1400,1200);
  cgsBias->Divide(2,2);
  for(int i=0; i<4; i++){
    cgsBias->cd(i+1)->SetRightMargin(0.15); GSvbiasPlot[i]->Draw("colz,text");  
    if(flipAC){ ReverseXAxis(GSvbiasPlot[i]); }    
    GSvbiasPlot[i]->GetZaxis()->SetRangeUser(-100,100);
    line->Draw();  vline->Draw();        
    GSvbiasPlot[i]->Write();
  }
  cgsBias->Update();
  cgsBias->Write();
  fname  = "GroupSensorDiff_Vbias";
  cgsBias->SaveAs(outdir+"/"+fname+".png");

  // Mean ADC Map
  gStyle->SetPaintTextFormat("4.1f");
  //TColor::InvertPalette();
  TCanvas *cADC = new TCanvas("cADC","ADC Values",1400,1200);
  cADC->Divide(2,2);
  for(int i=0; i<4; i++){
    cADC->cd(i+1)->SetRightMargin(0.15); adcPlot[i]->Draw("colz,text");  
    if(flipAC){ ReverseXAxis(adcPlot[i]); }    
    adcPlot[i]->GetZaxis()->SetRangeUser(5,15);
    adcPlot[i]->SetMarkerSize(1.2);
    line->Draw();  vline->Draw();        
    adcPlot[i]->Write();
  }
  cADC->Update();
  cADC->Write();
  fname  = "MeanADCMap";
  cADC->SaveAs(outdir+"/"+fname+".png");

  // Mean ADC, ADC >= 5 Map
  TCanvas *cADC5 = new TCanvas("cADC5","ADC Values (ADC>=5)",1400,1200);
  cADC5->Divide(2,2);
  for(int i=0; i<4; i++){
    cADC5->cd(i+1)->SetRightMargin(0.15); adc5Plot[i]->Draw("colz,text");  
    if(flipAC){ ReverseXAxis(adc5Plot[i]); }    
    adc5Plot[i]->GetZaxis()->SetRangeUser(7,15);
    adc5Plot[i]->SetMarkerSize(1.2);
    line->Draw();  vline->Draw();        
    adc5Plot[i]->Write();
  }
  cADC5->Update();
  cADC5->Write();
  fname  = "MeanADCMap_gt_5";
  cADC5->SaveAs(outdir+"/"+fname+".png");

  // Mean ADC Map
  gStyle->SetPaintTextFormat("3.0f");
  TCanvas *cGRP = new TCanvas("cGRP","Group Number",1400,1200);
  cGRP->Divide(2,2);
  for(int i=0; i<4; i++){
    cGRP->cd(i+1)->SetRightMargin(0.15); groupPlot[i]->Draw("text");  
    if(flipAC){ ReverseXAxis(groupPlot[i]); }    
    groupPlot[i]->SetMarkerSize(1.2);
    line->Draw();  vline->Draw();        
    groupPlot[i]->Write();
  }
  cGRP->Update();
  cGRP->Write();
  fname  = "GoupNumMap";
  cGRP->SaveAs(outdir+"/"+fname+".png");


  // Plot 1D ADC distributions for all sensors in all HV groups.
  gROOT->SetBatch(kTRUE); // Turn ON batch mode
  TCanvas *cca[300];
  TH1D* h1adc[n_sensors];
  for(int i=0; i<n_sensors; i++){
    h1adc[i] = new TH1D(Form("h1adc_%d",i),"",32,0,32);
  }
  gStyle->SetOptStat("me");
  for(int i=0; i<nGroup; i++){
     std::vector<int> ivec = getCanvasSize(nsensHVGroup[i]);
     cca[i] = new TCanvas(Form("cca_%d",i),Form("Charge/sensor, HV Group %d",i),1.*ivec[0],1.*ivec[1]);
     cca[i]->Divide(ivec[2],ivec[3]);
     for(int j=0; j<nsensHVGroup[i]; j++){
       cca[i]->cd(j+1);
       isen = sensInHVGroup[i][j];
       ityp = sensType[isen];
       ib = iBestG[isen];
       if(ib < 0){ continue; }
       h2 = (TH2F*)f->Get(Form("h2chAll_%d",ib));
       h1adc[isen] = (TH1D*)h2->ProjectionY(Form("h1adc_%d",isen),isen+1,isen+1);       
       s = Form("V_{bias} = %3.0d V",hv_vals[ib]);
       s2 = Form(", HVG = %d",i);
       s = senName[isen] + ", " + s + s2;
       h1adc[isen]->SetTitle(s);
       addGraphics(h1adc[isen],"Charge [ADC]","",1,0.045);
       h1adc[isen]->SetStats(kTRUE);
       h1adc[isen]->SetLineWidth(1);
       h1adc[isen]->Draw();
     }
     cca[i]->Update();
     cca[i]->Write();
     if(saveCanvas){
       fname  = "ADCDist1D_AllSensorsInHVGroups";
       if(i==0){
         cca[i]->Print(outdir+"/"+fname+".pdf[");
         cca[i]->Print(outdir+"/"+fname+".pdf");
       }else if(i==nGroup-1){
         cca[i]->Print(outdir+"/"+fname+".pdf");
         cca[i]->Print(outdir+"/"+fname+".pdf]");
       }else{
         cca[i]->Print(outdir+"/"+fname+".pdf");
       }
     }     

  }
  


  TCanvas *cv = new TCanvas("cv","Vmax-Vmin",600,600);
  vmaxmin0->SetLineColor(1);
  vmaxmin->SetLineColor(2);
  vmaxmin->Draw();
  vmaxmin0->Draw("same");
  vmaxmin0->Write();
  vmaxmin->Write();

  cout << endl;
  std::cout << "Results Written out to: " << outdir << std::endl;


  //fout->Close();
  
}

