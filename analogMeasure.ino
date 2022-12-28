
const float ANALOG_REF = 1.1;
const float ANALOG_MAX_COUNT = 1023.0;
const float VOLTAGE_RATE = 10.4;
const float MIN_POWER_VOLTAGE = 4.3;
const float MAX_POWER_VOLTAGE = 5.8;
const float MAX_ZERO_CURRENT_FAN = 0.05;
const float MIN_CURRENT_FAN = 0.2;
const float MAX_CURRENT_FAN = 0.6;
const int POWER_VOLTAGE_MEASURE_PIN = A0;

const int TIMEOUT_CHECK_ANALOG_VALUE = 10;


void setupAnalogandIdle(){
  analogReference(INTERNAL);
  for(int i = 0; i < 5; i++){
    delay(20);
    checkNormalPowerVoltage();
  }
  Serial.println(F("analog idle done"));
}


bool seriouslyCheckErrorPowerVoltage(){
	if(checkNormalPowerVoltage()){
		return false;
	}

	Serial.println(F("start seriously check power voltage:"));

	delay(TIMEOUT_CHECK_ANALOG_VALUE);
	if(checkNormalPowerVoltage() == false){
		Serial.println(F("seriously problem power voltage!"));
		return true;
	}

	return false;
}


bool checkNormalPowerVoltage(){

  const float voltage = getVoltageFromPin(POWER_VOLTAGE_MEASURE_PIN);
  bool result = false;
  if ( MIN_POWER_VOLTAGE < voltage && voltage < MAX_POWER_VOLTAGE){
    result = true;
  } else {
   printResultCheckAnalog(F("voltage"), voltage, result);
	}

  return result;
}


float getVoltageFromPin(int pin){
  const int sensorValue = analogRead(pin);
  const float voltage = VOLTAGE_RATE * sensorValue * ANALOG_REF / ANALOG_MAX_COUNT;
  return voltage;
}

void printResultCheckAnalog(String title, float value, bool result){
	Serial.print( title );
	Serial.print(F(" : "));
   	Serial.print(value);
   	Serial.print(F(" - normal : "));
   	Serial.println(result); 
}
