/*
 BMS
*/
#define FIS_TYPE float
#define FIS_RESOLUSION 101
#define FIS_MIN -3.4028235E+38
#define FIS_MAX 3.4028235E+38
typedef FIS_TYPE(*_FIS_MF)(FIS_TYPE, FIS_TYPE*);
typedef FIS_TYPE(*_FIS_ARR_OP)(FIS_TYPE, FIS_TYPE);
typedef FIS_TYPE(*_FIS_ARR)(FIS_TYPE*, int, _FIS_ARR_OP);



// Number of inputs to the fuzzy inference system
const int fis_gcI = 1;
// Number of outputs to the fuzzy inference system
const int fis_gcO = 1;
// Number of rules to the fuzzy inference system
const int fis_gcR = 5;

FIS_TYPE g_fisInput[fis_gcI];
FIS_TYPE g_fisOutput[fis_gcO];

/////////////////////////////
  float sensorv = A2;
  float voltagevalue;
  int trans = 11;
  float sensorvT = A3;
  float voltagevalueT;
  int transT = 10;
  float currentvalue;
  float intres;
  ////////////////////////////
  float sensorv2 = A0;
  float voltagevalue2;
  int trans2 = 13;
  float sensorvT2 = A1;
  float voltagevalueT2;
  int transT2 = 12;
  float currentvalue2;
  float intres2;
  ///////////////////////////
  float sensort = A4;
  float tempinc;
  //////////////////////////
  float sensort2 = A5;
  float tempinc2;
  int fanpin = 8;
  /////////////////////////
  int overd = 9;
  int charge_over =7;

void setup() {
  ///////////////////////////
  Serial.begin(9600);
  pinMode(sensorv, INPUT);
  pinMode(trans, OUTPUT);
  pinMode(sensorvT, INPUT);
  pinMode(transT, OUTPUT);
  ///////////////////////////
  pinMode(sensorv2, INPUT);
  pinMode(trans2, OUTPUT);
  pinMode(sensorvT2, INPUT);
  pinMode(transT2, OUTPUT);
  ///////////////////////////
  pinMode(sensort, INPUT);
  //////////////////////////
  pinMode(sensort2, INPUT);
  pinMode(fanpin, OUTPUT);
  //////////////////////////
  pinMode(overd, OUTPUT);
  pinMode(charge_over, OUTPUT);
}
void loop() {
  //////////////////////////////////////////////////////////// open voltage measuring
  sensorv = analogRead(A2);
  voltagevalue = (sensorv * 5.0 / 1024); // converts value to voltge
  Serial.print("V=");
  Serial.print(voltagevalue, 3);
  delay(1); // Wait for 1000 millisecond(s)
  ////////////////////////////////////////////////////////////// terminal voltage measuring and current calculation
  digitalWrite(transT, HIGH);
  sensorvT = analogRead(A3);
  voltagevalueT = (sensorvT * 5.0 / 1024); // converts value to voltge
  currentvalue = voltagevalueT / 1.7; // calculation of current using ohm's law
  Serial.print(" VT=");
  Serial.print(voltagevalueT, 3);
  Serial.print(" A=");
  Serial.print(currentvalue, 3);
  digitalWrite(transT, LOW);
  ////////////////////////////////////////////////////////////// internal resistence calculation
  intres = (voltagevalue - voltagevalueT) / currentvalue;
  Serial.print(" IR=");
  Serial.print(intres, 5);
  ///////////////////////////////////////////////////////////// overcharging control
  if (voltagevalue > 4.1)
  {
    digitalWrite(trans, HIGH);
  }
  else
  {
    digitalWrite(trans, LOW);
  }
  ////////////////////////////////////////////////////////////////// open voltage measuring
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
  ////////////////////////////////////////////////////////////////// internal resistence calculation
  intres2 = (voltagevalue2 - voltagevalueT2) / currentvalue2;
  Serial.print(" 2IR=");
  Serial.print(intres2, 5);
  //////////////////////////////////////////////////////////////////  overcharging control
  if (voltagevalue2 > 4.1)
  {
    digitalWrite(trans2, HIGH);
  }
  else
  {
    digitalWrite(trans2, LOW);
  }
  ///////////////////////////////////////////////////////////////////////// temperetura sensor and fan control
  sensort = analogRead(A4); // reads the velue given by the sensor
  tempinc = ((5.0 * sensort * 100.0) / 1024.0) - 50; // converts analog to celcius
  Serial.print(" T=");
  Serial.print((byte)tempinc);// will output C values
  if (tempinc >= 27)
  {
    digitalWrite(fanpin, HIGH);
  }
  else
  {
    digitalWrite(fanpin, LOW);
  }
  /////////////////////////////////////////////////////////////////////////// temperature sensor2 and fan control
  sensort2 = analogRead(A5); // reads the velue given by the sensor
  tempinc2 = ((5.0 * sensort2 * 100.0) / 1024.0) - 50; // converts analog to celcius
  Serial.print(" 2T=");
  Serial.print((byte)tempinc2);// will output C values
  delay(1000); // Wait for 1000 millisecond(s)
  if (tempinc2 >= 27)
  {
    digitalWrite(fanpin, HIGH);
  }
  else
  {
    digitalWrite(fanpin, LOW);
  }
  ////////////////////////////////////////////////////////////////////////////// over discharge protection
  if (voltagevalue <=2.52 || voltagevalue2 <=2.52)
  {
    digitalWrite(overd, HIGH);
    Serial.print("over discharge warning disconnet battery from load");
  }
  else
  {
    digitalWrite(overd,LOW);
  }
  ////////////////////////////////////////////////////////////////////////////// over charge CC/CV emergency disconnection
  if (voltagevalue >=4.19 || voltagevalue2 >=4.19)
  {
    digitalWrite(charge_over, HIGH);
    Serial.print("over charge warning disconnet battery from CC/CV");
  }
  else
  {
    digitalWrite(charge_over,LOW);
  }
  //////////////////////////////////////////////////////////////////////////////fuzzy cell soc 1
    // Read Input: input1
    g_fisInput[0] = voltagevalue;

    g_fisOutput[0] = 0;

    fis_evaluate();
// Set output vlaue: output
    Serial.print(" SOC=");
    Serial.print(g_fisOutput[0]);
    Serial.print("%");
    ////////////////////////////////////////////////// fuzzy cell soc 2
// Read Input: input2
    g_fisInput[0] = voltagevalue2;

    g_fisOutput[0] = 0;

    fis_evaluate();

    // Set output vlaue: output2
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
int fis_gIMFCount[] = { 5 };

// Count of member function for each Output 
int fis_gOMFCount[] = { 5 };

// Coefficients for the Input Member Functions
FIS_TYPE fis_gMFI0Coeff1[] = { 0.184686030914522, 2.50259498654868 };
FIS_TYPE fis_gMFI0Coeff2[] = { 0.196429419126853, 2.93524921646634 };
FIS_TYPE fis_gMFI0Coeff3[] = { 0.126170510056513, 3.30659833657337 };
FIS_TYPE fis_gMFI0Coeff4[] = { 0.170651987200151, 3.76501780674384 };
FIS_TYPE fis_gMFI0Coeff5[] = { 0.198610849413971, 4.18724214659661 };
FIS_TYPE* fis_gMFI0Coeff[] = { fis_gMFI0Coeff1, fis_gMFI0Coeff2, fis_gMFI0Coeff3, fis_gMFI0Coeff4, fis_gMFI0Coeff5 };
FIS_TYPE** fis_gMFICoeff[] = { fis_gMFI0Coeff };

// Coefficients for the Output Member Functions
FIS_TYPE fis_gMFO0Coeff1[] = { 24.8763402356449, -61.8393485325138 };
FIS_TYPE fis_gMFO0Coeff2[] = { 31.4115973060576, -87.188063212906 };
FIS_TYPE fis_gMFO0Coeff3[] = { 82.5810928539714, -263.942253509771 };
FIS_TYPE fis_gMFO0Coeff4[] = { 46.2970991975748, -104.456077491185 };
FIS_TYPE fis_gMFO0Coeff5[] = { 7.26546172524349, 69.747583902075 };
FIS_TYPE* fis_gMFO0Coeff[] = { fis_gMFO0Coeff1, fis_gMFO0Coeff2, fis_gMFO0Coeff3, fis_gMFO0Coeff4, fis_gMFO0Coeff5 };
FIS_TYPE** fis_gMFOCoeff[] = { fis_gMFO0Coeff };

// Input membership function set
int fis_gMFI0[] = { 0, 0, 0, 0, 0 };
int* fis_gMFI[] = { fis_gMFI0};

// Output membership function set

int* fis_gMFO[] = {};

// Rule Weights
FIS_TYPE fis_gRWeight[] = { 1, 1, 1, 1, 1 };

// Rule Type
int fis_gRType[] = { 1, 1, 1, 1, 1 };

// Rule Inputs
int fis_gRI0[] = { 1 };
int fis_gRI1[] = { 2 };
int fis_gRI2[] = { 3 };
int fis_gRI3[] = { 4 };
int fis_gRI4[] = { 5 };
int* fis_gRI[] = { fis_gRI0, fis_gRI1, fis_gRI2, fis_gRI3, fis_gRI4 };

// Rule Outputs
int fis_gRO0[] = { 1 };
int fis_gRO1[] = { 2 };
int fis_gRO2[] = { 3 };
int fis_gRO3[] = { 4 };
int fis_gRO4[] = { 5 };
int* fis_gRO[] = { fis_gRO0, fis_gRO1, fis_gRO2, fis_gRO3, fis_gRO4 };

// Input range Min
FIS_TYPE fis_gIMin[] = { 2.5 };

// Input range Max
FIS_TYPE fis_gIMax[] = { 4.2 };

// Output range Min
FIS_TYPE fis_gOMin[] = { 0 };

// Output range Max
FIS_TYPE fis_gOMax[] = { 100 };

//***********************************************************************
// Fuzzy Inference System                                                
//***********************************************************************
void fis_evaluate()
{
    FIS_TYPE fuzzyInput0[] = { 0, 0, 0, 0, 0 };
    FIS_TYPE* fuzzyInput[fis_gcI] = { fuzzyInput0, };
    FIS_TYPE fuzzyOutput0[] = { 0, 0, 0, 0, 0 };
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
