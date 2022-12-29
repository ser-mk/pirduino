const int BUZZER_PIN = 11;

const int FREQ_POWER_ERROR = 4000;
const int FREQ_MAX_TEMPERATURE = 100;
const int FREQ_MIN_TEMPERATURE = 7000;
const int FREQ_NO_TEMPERATURE_SENSOR = 1000;
const int FREQ_START_WORK = 2000;
const int FREQ_CAMERA_RESET = 3000;
const int FREQ_SIGNAL_WORK_START = 500;
const int FREQ_SIGNAL_WORK_STOP = 300;


void buzzSignalStop(){
	noTone(BUZZER_PIN);
	pinMode(BUZZER_PIN, INPUT);
}

void buzzPowerError(){
	buzzSignalStop();
	tone(BUZZER_PIN, FREQ_POWER_ERROR);
}

void buzzNoTempSensor(){
	buzzSignalStop();
	tone(BUZZER_PIN, FREQ_NO_TEMPERATURE_SENSOR);
}

void buzzMaxTemp(){
	buzzSignalStop();
	tone(BUZZER_PIN, FREQ_MAX_TEMPERATURE);
}

void buzzMinTemp(){
	buzzSignalStop();
	tone(BUZZER_PIN, FREQ_MIN_TEMPERATURE);
}

void buzzStartWork(){
	buzzSignalStop();
	tone(BUZZER_PIN, FREQ_START_WORK);
}

void buzzCameraReset(){
	buzzSignalStop();
	tone(BUZZER_PIN, FREQ_CAMERA_RESET);
}

void buzzSignalStart(){
	buzzSignalStop();
	tone(BUZZER_PIN, FREQ_SIGNAL_WORK_START);
}

void buzzSignalStop(){
	buzzSignalStop();
	tone(BUZZER_PIN, FREQ_SIGNAL_WORK_STOP);
}