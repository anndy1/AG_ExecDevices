
#include "AG_ExecDevices.h"

// конструктор: на входе pin - номер вывода, 
//              active - логический уровень включения: 
//              1 - (1-вкл., 0-выкл.), 0 - (1-выкл., 0-вкл.) 
Thermostat:: Thermostat(uint8_t pin, bool active) {
  //isStateChar = "2";
  isModeAuto = 1;
  _pin =    pin;
  _isActive=  active;
  isCooler = 0;
  if (_isActive) {
    _isOn =   1;
    _isOff =  0; }
  else {
    _isOn =   0;
    _isOff =  1; 
  }
  paramCurrent = 777.0;  
  pinMode(_pin,OUTPUT);
  setOff();
  delay(100);
  _isStateOld = getIsState();
  isCommandChanged();
}

// конструктор: на входе pin - номер вывода, 
//              active - логический уровень включения: 
//              1 - (1-вкл., 0-выкл.), 0 - (1-выкл., 0-вкл.) 
//              _isCooler - 1 кондиционер, 0 - нагреватель
Thermostat:: Thermostat(uint8_t pin, bool active, bool _isCooler) {
  //isStateChar = "2";
  isModeAuto = 1;
  _pin    = pin;
  _isActive = active;
  isCooler = _isCooler;
  if (_isActive) {
    _isOn =   1;
    _isOff =  0; }
  else {
    _isOn =   0;
    _isOff =  1; 
  }
  paramCurrent = 777.0;  
  pinMode(_pin,OUTPUT);
  setOff();
  delay(100);
  _isStateOld = getIsState();
  isCommandChanged();
}

// конструктор: на входе pin - номер вывода, 
//              active - логический уровень включения: 
//              1 - (1-вкл., 0-выкл.), 0 - (1-выкл., 0-вкл.)
//              _paramMin, _paramMax, _threshold - минимум значения (температуры), максимум, гистерезис 
Thermostat:: Thermostat(uint8_t pin, bool active, int8_t _paramMin, int8_t _paramMax, int8_t _threshold) {
  //isStateChar = "2";
  isModeAuto = 1;
  _pin =    pin;
  _isActive=  active;
  isCooler = 0;
  if (_isActive) {
    _isOn =   1;
    _isOff =  0; }
  else {
    _isOn =   0;
    _isOff =  1; 
  }
  paramMin = _paramMin;
  paramMax = _paramMax;
  threshold = _threshold;
  paramCurrent = 777.0;  
  pinMode(_pin,OUTPUT);
  setOff();
  delay(100);
  _isStateOld = getIsState();
  isCommandChanged();
}

// конструктор: на входе pin - номер вывода, 
//              active - логический уровень включения: 
//              1 - (1-вкл., 0-выкл.), 0 - (1-выкл., 0-вкл.)
//              _paramMin, _paramMax, _threshold - минимум значения (температуры), максимум, гистерезис 
//              _isCooler - 1 кондиционер, 0 - нагреватель
Thermostat:: Thermostat(uint8_t pin, bool active, int8_t _paramMin, int8_t _paramMax, int8_t _threshold, bool _isCooler) {
  //isStateChar = "2";
  isModeAuto = 1;
  _pin    = pin;
  _isActive = active;
  isCooler = _isCooler;
  if (_isActive) {
    _isOn =   1;
    _isOff =  0; }
  else {
    _isOn =   0;
    _isOff =  1; 
  }
  paramMin = _paramMin;
  paramMax = _paramMax;
  threshold = _threshold;
  paramCurrent = 777.0;  
  pinMode(_pin,OUTPUT);
  setOff();
  delay(100);
  _isStateOld = getIsState();
  isCommandChanged();
}

///---------------------------------------------------------------///

void Thermostat:: setParamMax(int8_t paramMax){
    this-> paramMax = paramMax;
  }

void Thermostat:: setParamMin(int8_t paramMin){
    this-> paramMin = paramMin;
  }

void Thermostat:: setParamCritical(int8_t paramCritical){
    this-> paramCritical = paramCritical;
  }

void Thermostat:: setThreshold(int8_t threshold){
    this-> threshold = threshold;
  }

void Thermostat:: setCommand(bool isCommand){
    this-> isCommand = isCommand;
  }
  
void Thermostat:: setOn(){
    digitalWrite(_pin, _isOn);
  }

void Thermostat:: setOff(){
    digitalWrite(_pin, _isOff);
  }

bool Thermostat:: getIsState() {
    if (_isActive) {
      if (digitalRead(_pin))  {isState=1; dtostrf((isState), 1, 0, isStateChar);}
      else                    {isState=0; dtostrf((isState), 1, 0, isStateChar);}
      }
    else {
      if (digitalRead(_pin)) {isState=0; dtostrf((isState), 1, 0, isStateChar);}
      else                   {isState=1; dtostrf((isState), 1, 0, isStateChar);}
    }
    return isState;    
  }

bool Thermostat:: isCommandChanged(){
    if (_isCommandOld != isCommand){
      _isCommandOld=isCommand;
      _isCommandChanged = true;
    }
    else _isCommandChanged = false;
    return _isCommandChanged;
  }

// можно использовать вместо getIsState()
bool Thermostat:: isStateChanged(){ 
    bool i = getIsState(); 
    if (i != _isStateOld) {
      _isStateOld = i;
      _isStateChanged = true;
    }
    else _isStateChanged = false;
    return _isStateChanged;
  }

// для нагревателя использовать обе функции: runParamMin(); runParamMax(); либо только runParamMin() для "Анти-замерзания"
// runParamMinparamMax() для нагревателя включает "Анти-замерзание" свм по себе. 

// Только для нагревателя. Включ если темп < paramMin, paramMin+threshold - отключается 
void Thermostat:: runParamMin(){       
    if (!isCommand) {
      if (paramCurrent <= (float)(paramMin)) setOn();
      if (paramCurrent >= (float)(paramMin+threshold)) setOff(); 
    }
  };

// нагрев:      включ. если темп <= paramMax; отключ. темп >= paramMax-threshold
// кондиционер: включ. если темп >= paramMax; отключ. темп <= paramMax-threshold
void Thermostat:: runParamMax(){       
    if (isCommand) {
      if (!isCooler){
        if (paramCurrent <= (float)(paramMax-threshold)) setOn();
        if (paramCurrent >= (float)(paramMax)) setOff(); 
      }
      else {
        if (paramCurrent <= (float)(paramMax-threshold)) setOff();
        if (paramCurrent >= (float)(paramMax)) setOn();      
      }
    }
  };

// используется, в основном, для кондиционера и вентилятора
// нагрев:      от paramMin до paramMax
// кондиционер: включ. если темп >= paramMax; отключ. темп <= paramMin
void Thermostat:: runParamMinMax(){
    if (isCommand) {
      if (!isCooler){
        if (paramCurrent >= (float)paramMax) setOff();
        if (paramCurrent <= (float)paramMin) setOn();  
      }
      else {
        if (paramCurrent >= (float)paramMax) setOn();
        if (paramCurrent <= (float)paramMin) setOff(); 
      }
    }
    else {
      if (!isCooler) runParamMin();
      else setOff();
    }
  };

void Thermostat:: runAuto(){
    if (isModeAuto){
      if (isCommand) {
        if (!isCooler){
          if (paramCurrent >= (float)paramMax) setOff();
          if (paramCurrent <= (float)paramMin) setOn();  
        }
        else {
          if (paramCurrent >= (float)paramMax) setOn();
          if (paramCurrent <= (float)paramMin) setOff(); 
        }
      }
      else {
        if (!isCooler) runParamMin();
        else setOff();
      }
    }
    else{
    }
  };

void Thermostat:: run(){
  if (isCommand) {
    setOn();
  }
  else {
    setOff();
  }
}

bool Thermostat:: isPwmChanged(){
  if (_pwmChanged) return true;
  else return false;
}

void Thermostat:: setPwm(uint8_t val){
    _pwmValue  = val;
  if (_pwmValue != _pwmOld){
    _pwmOld = _pwmValue;
    _pwmChanged = true;
  } 
  else {
    _pwmChanged = false;
  }
}

uint8_t Thermostat:: getPwm(){
  return _pwmValue;
}

void Thermostat:: runPwm(){
  uint16_t pwmMap;
  if (_isActive){
    pwmMap = map(_pwmValue, 0, 20, 0, 1023);
  }
  else {
    pwmMap = map(_pwmValue, 0, 20, 1023, 0);
  }
    if (isCommand) {
    analogWrite(_pin, pwmMap);
  }
  else {
    setOff();
  }
}