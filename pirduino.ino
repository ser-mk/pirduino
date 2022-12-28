#include <OneWire.h>

void setup() {
  //start serial connection
  buzzStartWork();
  
  Serial.begin(9600);
  
  initPinOutput();
  clearPowerOutput();

  setupAnalogandIdle();

  delay(400);
  stopBuzz();
  
  firstPowerTest();

}

void firstPowerTest(){

	Serial.println(F("start first test"));
	clearPowerOutput();
	delay(50);

	_checkExceptionPowerVoltage();

	tabletSet(true);
	cameratSet(true);
	delay(1000);
	torchSet(true);
	delay(1000);
	fanSet(true);
	delay(5000);

	_checkExceptionPowerVoltage();

	cameratSet(false);
	fanSet(false);
	delay(500);
	torchSet(false);
	delay(20);

	_checkExceptionPowerVoltage();

	Serial.println(F("first test done!"));
}

static const unsigned int TIMEOUT_MS_FAN_OFF = 1000;
static const unsigned long TIMEOUT_CAPTURE_OFF = 100000;

void loop(){

	tabletSet(true);

	Serial.println(F("listen START signal:"));

	while(seriouslyCheckStartSignaledSensor() == false){
		if(_checkExceptionPowerVoltage()) return;
		delay(1);
		if(_checkTemperature(20*1000)) return;
	}

	Serial.println(F("Turn On all:"));

	setFanBitFromPILoop(true);
	torchSet(true);

	buzzSignalStart();
	delay(50);
	stopBuzz();

	cameratSet(true);

	delay(5000);

	Serial.println(F("listen STOP signal:"));

	while(seriouslyCheckStopSignaledSensor() == false){
		if(_checkExceptionPowerVoltage()) return;
		delay(10);
		if(_checkTemperature(1000)) return;
		waitRestartCamera();
	}

	buzzSignalStop();
	delay(50);
	stopBuzz();

	Serial.println(F("wait one minute..."));

	const unsigned long end_time_wait = millis() + TIMEOUT_CAPTURE_OFF;

	while(end_time_wait > millis()){
		if(_checkExceptionPowerVoltage()) return;
		delay(10);
		if(_checkTemperature(1000)) return;
		waitRestartCamera();
		if(seriouslyCheckStartSignaledSensor()){
			Serial.println(F("find start signal, goto start"));
			return;
		}
	}

	Serial.println(F("Turn Off cam and torch:"));

	cameratSet(false);
	torchSet(false);

	int mi = 0;
	for( ; mi < TIMEOUT_MS_FAN_OFF; mi++){
		delay(10);
		if(seriouslyCheckStartSignaledSensor()){
			Serial.println(F("PreFan started"));
			break;
		}
		if(_checkExceptionPowerVoltage()) return;
	}

	Serial.print(F("TIMEOUT FAN : "));
	Serial.println(mi);

	if(mi >= TIMEOUT_MS_FAN_OFF){
		Serial.println(F("Fan Off"));
		setFanBitFromPILoop(false);
	}
}
