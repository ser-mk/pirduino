const int TABLET_PIN = 3;//2;-v2
const int FAN_PIN = 4;//3; -v2
const int CAMERA_PIN = 5;
const int TORCH_PIN = A3; //17; -v2
const int BUS_PIN = 6; //A3


void initPinOutput(){

  Serial.println(F("init output pin"));

  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(TABLET_PIN, OUTPUT);
  pinMode(CAMERA_PIN, OUTPUT);
  pinMode(TORCH_PIN, OUTPUT);
  pinMode(FAN_PIN, OUTPUT);
  pinMode(BUS_PIN, OUTPUT);
}


void clearPowerOutput(){
	tabletOn(false);
	fanOn(false);
	highLightOn(false);
	cameraOn(false);
}

  void blinkSet(bool set){
    if(set){
    digitalWrite(LED_BUILTIN, HIGH);
  } else {
    digitalWrite(LED_BUILTIN, LOW);
  }
}

void tabletOn(bool set){
  if(set){
    digitalWrite(TABLET_PIN, LOW);
  } else {
    digitalWrite(TABLET_PIN, HIGH);
  }
}

void cameraOn(bool set){
  if(set){
    digitalWrite(CAMERA_PIN, HIGH);
  } else {
    digitalWrite(CAMERA_PIN, LOW);
  }
}

void highLightOn(bool set){
  if(set){
    digitalWrite(TORCH_PIN, LOW);
  } else {
    digitalWrite(TORCH_PIN, HIGH);
  }
}

void fanOn(bool set){
  if(set){
    digitalWrite(FAN_PIN, LOW);
  } else {
    digitalWrite(FAN_PIN, HIGH);
  }
}

const int BIT_GENERAL_LOOP = 1;
const int BIT_THERMAL_LOOP = 2;
static int MASK_FAN_BIT = 1 + 2;

static int bitFan = 0;

void setFanBit(bool set, int bit){

  if(set){
    bitFan |= bit;
  } else {
    bitFan &= (~bit);
  }

  bit &= MASK_FAN_BIT;

  fanOn(bitFan != 0);
}

void fanOnPiLoop(bool set){
  setFanBit(set, BIT_GENERAL_LOOP);
}

void busSet(bool set){
  if(set){
    digitalWrite(BUS_PIN, HIGH);
  } else {
    digitalWrite(BUS_PIN, LOW);
  }
}

void disableWorkOutput(){
	tabletOn(false);
	highLightOn(false);
	cameraOn(false);
}