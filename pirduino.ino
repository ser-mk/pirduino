#include <OneWire.h>

void setup() {
  buzzStartWork();
  //start serial connection
  Serial.begin(9600);
  
  initPinOutput();
  clearPowerOutput();

  setupAnalogandIdle();

  delay(400);
  buzzSignalStop();
  
  firstPowerTest();

}

void firstPowerTest(){

	Serial.println(F("start first test"));
	clearPowerOutput();
	delay(50);

	_checkExceptionPowerVoltage();

	tabletOn(true);
	cameraOn(true);
	delay(1000);
	highLightOn(true);
	delay(1000);
	fanOn(true);
	delay(5000);

	_checkExceptionPowerVoltage();

	cameraOn(false);
	fanOn(false);
	delay(500);
	highLightOn(false);
	delay(20);

	_checkExceptionPowerVoltage();

	Serial.println(F("first test done!"));
}

static const unsigned int TIMEOUT_MS_FAN_OFF = 10000;
static const unsigned long TIMEOUT_CAPTURE_OFF = 60000;

void loop(){

	tabletOn(true);

	Serial.println(F("listen START signal:"));

	// the program first polls the motion sensors to gather information about the presence of humans in the area
	while(seriouslyCheckStartSignaledSensor() == false){
		if(_checkExceptionPowerVoltage()) return; // check arduino's power
		delay(1);
		if(_checkTemperature(20*1000)) return; // check pirduino's temp
	}

	Serial.println(F("Turn On all:"));

	fanOnPiLoop(true);	// turn on the fan
	highLightOn(true);

	buzzSignalStart();
	delay(50);
	buzzSignalStop();

	cameraOn(true);

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
	buzzSignalStop();

	Serial.println(F("hold on 100 seconds. Wait new gamer. If they do, we can get back to playing the game as fast as possible. There's no need to restart the system."));

	const unsigned long end_time_wait = millis() + TIMEOUT_CAPTURE_OFF;

	while(end_time_wait > millis()){
		if(_checkExceptionPowerVoltage()) return;
		delay(10);
		if(_checkTemperature(1000)) return;
		waitRestartCamera();
		if(seriouslyCheckStartSignaledSensor()){
			Serial.println(F("find starting signal, goto start of the loop"));
			return;
		}
	}

	Serial.println(F("No new gamer. Turn Off cam and highLight(LED):"));

	cameraOn(false);
	highLightOn(false);

	Serial.print(F("FAN works 10 seconds for cooling"));


	int mi = 0;
	for( ; mi < TIMEOUT_MS_FAN_OFF; mi++){
		delay(10);
		if(seriouslyCheckStartSignaledSensor()){
			Serial.println(F("PreFan started"));
			break;
		}
		if(_checkExceptionPowerVoltage()) return;
	}

	Serial.print(F("TIMEOUT FAN end: "));
	Serial.println(mi);

	if(mi >= TIMEOUT_MS_FAN_OFF){
		Serial.println(F("Fan Off"));
		fanOnPiLoop(false);
	}
}
