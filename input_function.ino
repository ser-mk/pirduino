
const int RADIO_SENSOR_PIN = 9;//12; -v2
const int PIR_SENSOR_PIN = 8;
const int DISABLE_CAMERA_PIN = 7;

void initInputPin(){

	Serial.println(F("init input pin"));

	pinMode(RADIO_SENSOR_PIN, INPUT);
	pinMode(PIR_SENSOR_PIN, INPUT);
	pinMode(DISABLE_CAMERA_PIN, INPUT);
}


int simpleCheckSensor(){
	int signaledSensor = 0;
	if (digitalRead(RADIO_SENSOR_PIN) == LOW){ //v3 board
		signaledSensor++;
	}

	if (digitalRead(PIR_SENSOR_PIN) == LOW){ //v3 board
		signaledSensor++;
	}

	return signaledSensor;
}

bool simpleDisableCameraPin(){
	return digitalRead(DISABLE_CAMERA_PIN) == HIGH ? false : true;
}

const int DELAY_MS_REMAINDER_DISABLE_CAMERA = 500;
const int DELAY_MS_SIGNAL_DISABLE_CAMERA = 500;
const int TIMEOUT_MS_CHECK_DISABLE_CAMERA = 10;


void waitRestartCamera(){
	if (simpleDisableCameraPin() == false){
		return;
	}
	delay(TIMEOUT_MS_CHECK_DISABLE_CAMERA);
	if (simpleDisableCameraPin() == false){
		return;
	}

	Serial.println(F("Restart camera -"));
	buzzCameraReset();
	cameraOn(false);
	delay(DELAY_MS_SIGNAL_DISABLE_CAMERA);
	buzzSignalStop();
	delay(DELAY_MS_REMAINDER_DISABLE_CAMERA);
	Serial.println(F("Ensable camera again +"));
	cameraOn(true);
}


const int TIMES_CHECK_SIGNAL_VALUE = 10;
const int THRESHOLD_START_SIGNALED = 7;
const int THRESHOLD_STOP_SIGNALED = 2;
const int TIMEOUT_CHECK_SENSOR = 500;



bool seriouslyCheckStartSignaledSensor(){
	if(simpleCheckSensor() == 0){
		return false;
	}

	Serial.println(F("start seriously check STARTED signaled sensor:"));

	delay(TIMEOUT_CHECK_SENSOR);
	const int ret = simpleCheckSensor();
	if(ret > 0){
		Serial.print(F("sensor seriously START signaled: "));
		Serial.println(ret);
		return true;
	}

	return false;
}

bool seriouslyCheckStopSignaledSensor(){
	if(simpleCheckSensor() > 0){
		return false;
	}

	Serial.println(F("start seriously check STOPED signaled sensor:"));

	delay(TIMEOUT_CHECK_SENSOR);
	const int ret = simpleCheckSensor();
	if(simpleCheckSensor() == 0){
		Serial.print(F("sensor seriously STOP signaled"));
		Serial.println(ret);
		return true;
	}

	return false;
}
