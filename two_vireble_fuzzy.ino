/*
2 input fuzzy
*/
#define FIS_TYPE float
#define FIS_RESOLUSION 101
#define FIS_MIN -3.4028235E+38
#define FIS_MAX 3.4028235E+38
typedef FIS_TYPE(*_FIS_MF)(FIS_TYPE, FIS_TYPE*);
typedef FIS_TYPE(*_FIS_ARR_OP)(FIS_TYPE, FIS_TYPE);
typedef FIS_TYPE(*_FIS_ARR)(FIS_TYPE*, int, _FIS_ARR_OP);


// Number of inputs to the fuzzy inference system
const int fis_gcI = 2;
// Number of outputs to the fuzzy inference system
const int fis_gcO = 1;
// Number of rules to the fuzzy inference system
const int fis_gcR = 25;

FIS_TYPE g_fisInput[fis_gcI];
FIS_TYPE g_fisOutput[fis_gcO];


/////////////////////// cell one parameters 
 float sensorv = A2;
  float voltagevalue;
  int trans = 11;
  float sensorvT = A3;
  float voltagevalueT;
  int transT = 10;
  float currentvalue;
  float intres;
  ///////////////////// cell two parameters 
  float sensorv2 = A0;
  float voltagevalue2;
  int trans2 = 13;
  float sensorvT2 = A1;
  float voltagevalueT2;
  int transT2 = 12;
  float currentvalue2;
  float intres2;  

// Setup routine runs once when you press reset:
void setup()
{
  Serial.begin(9600);
  //////////////////////////
  pinMode(sensorv, INPUT);
  pinMode(trans, OUTPUT);
  pinMode(sensorvT, INPUT);
  pinMode(transT, OUTPUT);
  ///////////////////////////
  pinMode(sensorv2, INPUT);
  pinMode(trans2, OUTPUT);
  pinMode(sensorvT2, INPUT);
  pinMode(transT2, OUTPUT);
}

// Loop routine runs over and over again forever:
void loop()
{
  ////////////////////////////////////////////////////////////////////////////////////////////////////////cell one 
  sensorv = analogRead(A2);
  voltagevalue = (sensorv * 5.0 / 1024); // converts value to voltge
  Serial.print("V=");
  Serial.print(voltagevalue, 3);
  delay(1); // Wait for 1000 millisecond(s)
  ////////////////////////////////////////////////////////////// terminal voltage measuring and current calculation
  digitalWrite(transT, HIGH);
  sensorvT = analogRead(A3);
  voltagevalueT = (sensorvT * 5.0 / 1024); // converts value to voltge
  currentvalue = (voltagevalueT / 1.7)/10; // calculation of current using ohm's law
  Serial.print(" VT=");
  Serial.print(voltagevalueT, 3);
  Serial.print(" A=");
  Serial.print(currentvalue, 3);
  digitalWrite(transT, LOW);
  
    // Read Input: input1
    g_fisInput[0] = currentvalue;
    // Read Input: input2
    g_fisInput[1] = voltagevalue;

    g_fisOutput[0] = 0;

    fis_evaluate();

    //Set output vlaue: output
    Serial.print(" SOC=");
    Serial.print(g_fisOutput[0]);
    Serial.print("%"); 
  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////cell two
  sensorv2 = analogRead(A0);
  voltagevalue2 = (sensorv2 * 5.0 / 1024)*2-voltagevalue; // converts value to voltge
  Serial.print(" 2V=");
  Serial.print(voltagevalue2, 3);
  delay(1); // Wait for 1 millisecond(s)
  //////////////////////////////////////////////////////////////// terminal voltage measuring and current calculation
  digitalWrite(transT2, HIGH);
  sensorvT2 = analogRead(A1);
  voltagevalueT2 = (sensorvT2 * 5.0 / 1024)*2-voltagevalueT; // converts value to voltge
  currentvalue2 = voltagevalueT2 / 1.7; // calculation of current using ohm's law
  Serial.print(" 2VT=");
  Serial.print(voltagevalueT2, 3);
  Serial.print(" 2A=");
  Serial.print(currentvalue2, 3);
  digitalWrite(transT2, LOW);
  
  // Read Input: input1
    g_fisInput[0] = currentvalue2;
    // Read Input: input2
    g_fisInput[1] = voltagevalue2;

    g_fisOutput[0] = 0;

    fis_evaluate();

    //Set output vlaue: output
    Serial.print(" SOC2=");
    Serial.print(g_fisOutput[0]);
    Serial.println("%");
    delay(1000); // Wait for 1000 millisecond(s) 
}

//***********************************************************************
// Support functions for Fuzzy Inference System                          
//***********************************************************************
// Gaussian Member Function
FIS_TYPE fis_gaussmf(FIS_TYPE x, FIS_TYPE* p)
{
    FIS_TYPE s = p[0], c = p[1];
    FIS_TYPE t = (x - c) / s;
    return exp(-(t * t) / 2);
}

FIS_TYPE fis_prod(FIS_TYPE a, FIS_TYPE b)
{
    return (a * b);
}

FIS_TYPE fis_probor(FIS_TYPE a, FIS_TYPE b)
{
    return (a + b - (a * b));
}

FIS_TYPE fis_sum(FIS_TYPE a, FIS_TYPE b)
{
    return (a + b);
}

FIS_TYPE fis_array_operation(FIS_TYPE *array, int size, _FIS_ARR_OP pfnOp)
{
    int i;
    FIS_TYPE ret = 0;

    if (size == 0) return ret;
    if (size == 1) return array[0];

    ret = array[0];
    for (i = 1; i < size; i++)
    {
        ret = (*pfnOp)(ret, array[i]);
    }

    return ret;
}


//***********************************************************************
// Data for Fuzzy Inference System                                       
//***********************************************************************
// Pointers to the implementations of member functions
_FIS_MF fis_gMF[] =
{
    fis_gaussmf
};

// Count of member function for each Input
int fis_gIMFCount[] = { 5, 5 };

// Count of member function for each Output 
int fis_gOMFCount[] = { 25 };

// Coefficients for the Input Member Functions
FIS_TYPE fis_gMFI0Coeff1[] = { 0.0057024527323588, 0.161909812 };
FIS_TYPE fis_gMFI0Coeff2[] = { 0.0057024527323588, 0.175338062 };
FIS_TYPE fis_gMFI0Coeff3[] = { 0.0057024527323588, 0.188766312 };
FIS_TYPE fis_gMFI0Coeff4[] = { 0.0057024527323588, 0.202194562 };
FIS_TYPE fis_gMFI0Coeff5[] = { 0.0057024527323588, 0.215622812 };
FIS_TYPE* fis_gMFI0Coeff[] = { fis_gMFI0Coeff1, fis_gMFI0Coeff2, fis_gMFI0Coeff3, fis_gMFI0Coeff4, fis_gMFI0Coeff5 };
FIS_TYPE fis_gMFI1Coeff1[] = { 0.180480882561204, 2.5 };
FIS_TYPE fis_gMFI1Coeff2[] = { 0.180480882561204, 2.925 };
FIS_TYPE fis_gMFI1Coeff3[] = { 0.180480882561204, 3.35 };
FIS_TYPE fis_gMFI1Coeff4[] = { 0.180480882561204, 3.775 };
FIS_TYPE fis_gMFI1Coeff5[] = { 0.180480882561204, 4.2 };
FIS_TYPE* fis_gMFI1Coeff[] = { fis_gMFI1Coeff1, fis_gMFI1Coeff2, fis_gMFI1Coeff3, fis_gMFI1Coeff4, fis_gMFI1Coeff5 };
FIS_TYPE** fis_gMFICoeff[] = { fis_gMFI0Coeff, fis_gMFI1Coeff };

// Coefficients for the Output Member Functions
FIS_TYPE fis_gMFO0Coeff1[] = { 95.0111645848837, 9.39346086852979, -38.9639888325454 };
FIS_TYPE fis_gMFO0Coeff2[] = { 231.080983930788, -14.5931184976699, 0.37713714672752 };
FIS_TYPE fis_gMFO0Coeff3[] = { 12.8900028529417, 190.136032504134, 65.2869883804423 };
FIS_TYPE fis_gMFO0Coeff4[] = { 0.00119434654411491, 0.0207169252044731, 0.00638345295526875 };
FIS_TYPE fis_gMFO0Coeff5[] = { 4.82706776310933e-07, 8.34962390081396e-06, 2.49805390629388e-06 };
FIS_TYPE fis_gMFO0Coeff6[] = { -71.2871535248066, -18.3544556912874, 56.1219626223475 };
FIS_TYPE fis_gMFO0Coeff7[] = { -144.138874509909, -36.8408471066007, 130.141006861271 };
FIS_TYPE fis_gMFO0Coeff8[] = { -25.4055146498016, 125.153137894573, -323.091306319857 };
FIS_TYPE fis_gMFO0Coeff9[] = { 45.6395402465892, 794.652567384131, 237.909392875129 };
FIS_TYPE fis_gMFO0Coeff10[] = { 0.00889992717429327, 0.154903859410186, 0.0464932521908096 };
FIS_TYPE fis_gMFO0Coeff11[] = { 8.78534191210075, 144.362229942332, 28.6288651805417 };
FIS_TYPE fis_gMFO0Coeff12[] = { 18.8439402582757, 0.91371180618838, -12.7314993883137 };
FIS_TYPE fis_gMFO0Coeff13[] = { -1375.01806054001, -77.8578848242721, 509.094507463076 };
FIS_TYPE fis_gMFO0Coeff14[] = { -65.4895143101133, 167.828238101889, -83.9712660451702 };
FIS_TYPE fis_gMFO0Coeff15[] = { 3.73273711900156, 65.7830264900794, 18.2242399573941 };
FIS_TYPE fis_gMFO0Coeff16[] = { 1.19758732969061, 18.8533898988641, 6.32555285983867 };
FIS_TYPE fis_gMFO0Coeff17[] = { 108.08883955554, 54.8102001794354, -85.6635619210934 };
FIS_TYPE fis_gMFO0Coeff18[] = { 852.800034310297, -121.297069707331, 245.151422983018 };
FIS_TYPE fis_gMFO0Coeff19[] = { -975.248416909239, -112.964029071687, 722.282589468054 };
FIS_TYPE fis_gMFO0Coeff20[] = { -18.4025642141389, 47.6472947155247, -75.5443104437488 };
FIS_TYPE fis_gMFO0Coeff21[] = { 0.012502506916548, 0.21258594845641, 0.0656083496044226 };
FIS_TYPE fis_gMFO0Coeff22[] = { -48.398240085017, -791.707660335235, -229.917560953997 };
FIS_TYPE fis_gMFO0Coeff23[] = { 914.861662332078, -247.535879133107, 760.549310099555 };
FIS_TYPE fis_gMFO0Coeff24[] = { 785.567599156346, 69.4277308380287, -356.188617602956 };
FIS_TYPE fis_gMFO0Coeff25[] = { -12.7184469485522, 23.8938709968432, 1.72849858736261 };
FIS_TYPE* fis_gMFO0Coeff[] = { fis_gMFO0Coeff1, fis_gMFO0Coeff2, fis_gMFO0Coeff3, fis_gMFO0Coeff4, fis_gMFO0Coeff5, fis_gMFO0Coeff6, fis_gMFO0Coeff7, fis_gMFO0Coeff8, fis_gMFO0Coeff9, fis_gMFO0Coeff10, fis_gMFO0Coeff11, fis_gMFO0Coeff12, fis_gMFO0Coeff13, fis_gMFO0Coeff14, fis_gMFO0Coeff15, fis_gMFO0Coeff16, fis_gMFO0Coeff17, fis_gMFO0Coeff18, fis_gMFO0Coeff19, fis_gMFO0Coeff20, fis_gMFO0Coeff21, fis_gMFO0Coeff22, fis_gMFO0Coeff23, fis_gMFO0Coeff24, fis_gMFO0Coeff25 };
FIS_TYPE** fis_gMFOCoeff[] = { fis_gMFO0Coeff };

// Input membership function set
int fis_gMFI0[] = { 0, 0, 0, 0, 0 };
int fis_gMFI1[] = { 0, 0, 0, 0, 0 };
int* fis_gMFI[] = { fis_gMFI0, fis_gMFI1};

// Output membership function set

int* fis_gMFO[] = {};

// Rule Weights
FIS_TYPE fis_gRWeight[] = { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 };

// Rule Type
int fis_gRType[] = { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 };

// Rule Inputs
int fis_gRI0[] = { 1, 1 };
int fis_gRI1[] = { 1, 2 };
int fis_gRI2[] = { 1, 3 };
int fis_gRI3[] = { 1, 4 };
int fis_gRI4[] = { 1, 5 };
int fis_gRI5[] = { 2, 1 };
int fis_gRI6[] = { 2, 2 };
int fis_gRI7[] = { 2, 3 };
int fis_gRI8[] = { 2, 4 };
int fis_gRI9[] = { 2, 5 };
int fis_gRI10[] = { 3, 1 };
int fis_gRI11[] = { 3, 2 };
int fis_gRI12[] = { 3, 3 };
int fis_gRI13[] = { 3, 4 };
int fis_gRI14[] = { 3, 5 };
int fis_gRI15[] = { 4, 1 };
int fis_gRI16[] = { 4, 2 };
int fis_gRI17[] = { 4, 3 };
int fis_gRI18[] = { 4, 4 };
int fis_gRI19[] = { 4, 5 };
int fis_gRI20[] = { 5, 1 };
int fis_gRI21[] = { 5, 2 };
int fis_gRI22[] = { 5, 3 };
int fis_gRI23[] = { 5, 4 };
int fis_gRI24[] = { 5, 5 };
int* fis_gRI[] = { fis_gRI0, fis_gRI1, fis_gRI2, fis_gRI3, fis_gRI4, fis_gRI5, fis_gRI6, fis_gRI7, fis_gRI8, fis_gRI9, fis_gRI10, fis_gRI11, fis_gRI12, fis_gRI13, fis_gRI14, fis_gRI15, fis_gRI16, fis_gRI17, fis_gRI18, fis_gRI19, fis_gRI20, fis_gRI21, fis_gRI22, fis_gRI23, fis_gRI24 };

// Rule Outputs
int fis_gRO0[] = { 1 };
int fis_gRO1[] = { 2 };
int fis_gRO2[] = { 3 };
int fis_gRO3[] = { 4 };
int fis_gRO4[] = { 5 };
int fis_gRO5[] = { 6 };
int fis_gRO6[] = { 7 };
int fis_gRO7[] = { 8 };
int fis_gRO8[] = { 9 };
int fis_gRO9[] = { 10 };
int fis_gRO10[] = { 11 };
int fis_gRO11[] = { 12 };
int fis_gRO12[] = { 13 };
int fis_gRO13[] = { 14 };
int fis_gRO14[] = { 15 };
int fis_gRO15[] = { 16 };
int fis_gRO16[] = { 17 };
int fis_gRO17[] = { 18 };
int fis_gRO18[] = { 19 };
int fis_gRO19[] = { 20 };
int fis_gRO20[] = { 21 };
int fis_gRO21[] = { 22 };
int fis_gRO22[] = { 23 };
int fis_gRO23[] = { 24 };
int fis_gRO24[] = { 25 };
int* fis_gRO[] = { fis_gRO0, fis_gRO1, fis_gRO2, fis_gRO3, fis_gRO4, fis_gRO5, fis_gRO6, fis_gRO7, fis_gRO8, fis_gRO9, fis_gRO10, fis_gRO11, fis_gRO12, fis_gRO13, fis_gRO14, fis_gRO15, fis_gRO16, fis_gRO17, fis_gRO18, fis_gRO19, fis_gRO20, fis_gRO21, fis_gRO22, fis_gRO23, fis_gRO24 };

// Input range Min
FIS_TYPE fis_gIMin[] = { 0.161909812, 2.5 };

// Input range Max
FIS_TYPE fis_gIMax[] = { 0.215622812, 4.2 };

// Output range Min
FIS_TYPE fis_gOMin[] = { 0 };

// Output range Max
FIS_TYPE fis_gOMax[] = { 100 };

//***********************************************************************
// Data dependent support functions for Fuzzy Inference System           
//***********************************************************************
// None for Sugeno

//***********************************************************************
// Fuzzy Inference System                                                
//***********************************************************************
void fis_evaluate()
{
    FIS_TYPE fuzzyInput0[] = { 0, 0, 0, 0, 0 };
    FIS_TYPE fuzzyInput1[] = { 0, 0, 0, 0, 0 };
    FIS_TYPE* fuzzyInput[fis_gcI] = { fuzzyInput0, fuzzyInput1, };
    FIS_TYPE fuzzyOutput0[] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    FIS_TYPE* fuzzyOutput[fis_gcO] = { fuzzyOutput0, };
    FIS_TYPE fuzzyRules[fis_gcR] = { 0 };
    FIS_TYPE fuzzyFires[fis_gcR] = { 0 };
    FIS_TYPE* fuzzyRuleSet[] = { fuzzyRules, fuzzyFires };
    FIS_TYPE sW = 0;

    // Transforming input to fuzzy Input
    int i, j, r, o;
    for (i = 0; i < fis_gcI; ++i)
    {
        for (j = 0; j < fis_gIMFCount[i]; ++j)
        {
            fuzzyInput[i][j] =
                (fis_gMF[fis_gMFI[i][j]])(g_fisInput[i], fis_gMFICoeff[i][j]);
        }
    }

    int index = 0;
    for (r = 0; r < fis_gcR; ++r)
    {
        if (fis_gRType[r] == 1)
        {
            fuzzyFires[r] = 1;
            for (i = 0; i < fis_gcI; ++i)
            {
                index = fis_gRI[r][i];
                if (index > 0)
                    fuzzyFires[r] = fis_prod(fuzzyFires[r], fuzzyInput[i][index - 1]);
                else if (index < 0)
                    fuzzyFires[r] = fis_prod(fuzzyFires[r], 1 - fuzzyInput[i][-index - 1]);
                else
                    fuzzyFires[r] = fis_prod(fuzzyFires[r], 1);
            }
        }
        else
        {
            fuzzyFires[r] = 0;
            for (i = 0; i < fis_gcI; ++i)
            {
                index = fis_gRI[r][i];
                if (index > 0)
                    fuzzyFires[r] = fis_probor(fuzzyFires[r], fuzzyInput[i][index - 1]);
                else if (index < 0)
                    fuzzyFires[r] = fis_probor(fuzzyFires[r], 1 - fuzzyInput[i][-index - 1]);
                else
                    fuzzyFires[r] = fis_probor(fuzzyFires[r], 0);
            }
        }

        fuzzyFires[r] = fis_gRWeight[r] * fuzzyFires[r];
        sW += fuzzyFires[r];
    }

    if (sW == 0)
    {
        for (o = 0; o < fis_gcO; ++o)
        {
            g_fisOutput[o] = ((fis_gOMax[o] + fis_gOMin[o]) / 2);
        }
    }
    else
    {
        for (o = 0; o < fis_gcO; ++o)
        {
            FIS_TYPE sWI = 0.0;
            for (j = 0; j < fis_gOMFCount[o]; ++j)
            {
                fuzzyOutput[o][j] = fis_gMFOCoeff[o][j][fis_gcI];
                for (i = 0; i < fis_gcI; ++i)
                {
                    fuzzyOutput[o][j] += g_fisInput[i] * fis_gMFOCoeff[o][j][i];
                }
            }

            for (r = 0; r < fis_gcR; ++r)
            {
                index = fis_gRO[r][o] - 1;
                sWI += fuzzyFires[r] * fuzzyOutput[o][index];
            }

            g_fisOutput[o] = sWI / sW;
        }
    }
}
