# CCEAnalysisCode


Input Files:
============

* ROOTTupleList.inp:
    1) Location of InputFile_XXX.dat
    2) TupleLocation in ROOT files
    
 * InputFile_2025Start.dat: Provides list of ROOT files to process (text, with full path)


Runs interactiverly in ROOT. Initial code tested and ran in ROOT 6.34.06
========================================================================
 [ FYI:  2M events x 13 voltages  will take several hours ]


.L CCEScan.C
CCEScan s
s.Loop()

This will produce a single ROOT output (CCEResults_Date_Time.root) file that has date and time in file name.

This ROOT file is then analyzed with GetBestVoltage.{C,h}. 

In ROOT execute with:

.L GetBestVoltage.C
GetBestVoltage("CCEScanResults_D2025-04-11_T04-06-55.root")     [Provide root file output produced with CCEScan]

A number of files get produced in a subdir results_DATE_TIME. Most notably:
 * csv files. The one with _SLIM_ in the filename is the one that can be uploaded to the UT.
   The other ones give more detailed information on each sensor in each HV group
 * PDF files showing results of all 288 groups (ADCvsHV and 1D plots of the ADC distributions)
