void errorSignalBadPower(){
	clearPowerOutput();
		while(true){
			buzzPowerError();
			blinkSet(true);

			delay(1111);
			buzzSignalStop();
			blinkSet(false);

			delay(1111);

			if(seriouslyCheckErrorPowerVoltage() == false){
				Serial.println(F("seriously problem Power Voltage solved+"));
				break;
			}
		}
}

bool _checkExceptionPowerVoltage(){
	if(seriouslyCheckErrorPowerVoltage()){
		Serial.println(F("seriously problem Power Voltage !"));
		errorSignalBadPower();
		return true;
	}
	return false;
}

void signalingNoTemperatureSensor(){
	disableWorkOutput();

	fanOn(true);

	buzzNoTempSensor();
	blinkSet(true);

	delay(500);

	buzzSignalStop();
	blinkSet(false);
}

void signalingMinTemperature(){
	buzzMinTemp();
	blinkSet(true);

	delay(50);

	blinkSet(false);

	buzzSignalStop();
}


void signalingMaxTemperature(){
	disableWorkOutput();

	fanOn(true);

	buzzMaxTemp();
	blinkSet(false);

	delay(150);

	buzzSignalStop();
	blinkSet(true);
}