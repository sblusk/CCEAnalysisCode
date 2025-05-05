bool saveCanvas = true;

static const int n4chip = 1048;
static const int n_sensors = 968;
static const int nASIC = 4192;
static const int nChASIC = 128;
static const int nCh4ASIC = 512;
static const int nCh8ASIC = 512;
static const int nHVGroup = 288;
static const int nHVChan = 18;
static const int maxGroup = 9;
static const int nCalStep = 13;
static const int nTypeB = 48;
static const int nQuad = 4;
static const int nLayer = 4;
static const int maxFiles = 1000;  

float zutlay[4] = {2317.5, 2372.5, 2597.5, 2652.5};
float zref = 2485.0;
float zut = zref;
TString sType[4] = {"A", "B", "C", "D"};
TString sLay[4] = {"AX", "AU", "BV", "BX"};
int hv_vals[nCalStep]={400, 350, 300, 250, 200, 175, 150, 125, 100, 75, 50, 10, 0};
int iCal;
