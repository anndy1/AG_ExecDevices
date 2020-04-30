// ver.0.0.1 2020-05-01

#ifndef AG_ExecDevices
#define AG_ExecDevices

#include <Arduino.h> // нужно для ардуино-функций
//-------------------  Класс "Термостат" -------------------

class Thermostat {
  public:
    Thermostat(uint8_t pin, bool active);
    Thermostat(uint8_t pin, bool active, bool _isCooler);
    Thermostat(uint8_t pin, bool active, int8_t paramMin, int8_t paramMax, int8_t threshold);
    Thermostat(uint8_t pin, bool active, int8_t paramMin, int8_t paramMax, int8_t threshold, bool _isCooler);
    // setters
    void   setParamMax(int8_t paramMax);
    void   setParamMin(int8_t paramMin);
    void   setParamCritical(int8_t paramCritical);
    void   setThreshold(int8_t threshold);
    void   setCommand(bool   isCommand);
    void   setOn();
    void   setOff();
    void   setPwm(uint8_t val);
    // getters
    bool   getIsState();
    bool   isStateChanged();
    bool   isCommandChanged();
    bool   isPwmChanged();
    uint8_t getPwm();
    //--------------------
    void   runParamMin();
    void   runParamMax();
    void   runParamMinMax();
    void   runAuto();
    void   run();
    void   runPwm();
    char   isStateChar[2]={'2','\0'}; // ---> move to Private
  private:
    //входные параметры --
    int8_t paramMin;
    int8_t paramMax;
    int8_t threshold;
    int8_t paramCritical;
    float  paramCurrent;
    bool   isModeAuto;            // 1 - Авто вкл. , 0 - ручной режим
    bool   isCommand;                 // 1 - вкл. ,      0 - выкл. Команда <- MQTT
    bool   isState;               // 1 - вкл. ,      0 - выкл. Реальное состояние вывода контроллера -> MQTT
    bool   isCooler;
    //char   isStateChar[2]={'2','\0'};
    uint8_t _pin;
    bool   _isActive;
    bool   _isOn, _isOff;
    bool   _isStateOld;
    bool   _isCommandOld;
    bool   _isCommandChanged;
    bool   _isStateChanged;
    uint32_t _timeLast = 0;
    uint8_t  _pwmValue = 0;
    bool   _pwmOld;
    bool   _pwmChanged;
};

#endif