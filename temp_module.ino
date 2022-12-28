
const static OneWire ds1(10);
const static OneWire ds2(2);//16 v2

static byte data[12];
static byte addr[8];
static float celsius;
static int pre_times = 0;

const static int HIST_MAX = 5;

const int MAX_TEMPERATURE_THRESHOLD = 60;
const int TEMPERATURE_THRESHOLD_FOR_FAN = 40;
const int MIN_TEMPERATURE_THRESHOLD = 8;
const int MAX_NO_SENSOR_TIMES = 5;
const int MAX_TEMPERATURE_THRESHOLD_TIMES = 5;
const int MIN_TEMPERATURE_THRESHOLD_TIMES = 5;


static int qty_min_temperature_threshold = 0;

enum STATUS_TEMPERATURE {ALL_NORMAl, ONE_NORMAL, ABSENT_SENSOR, BREAK_SENSOR, OLD_SENSOR};

static unsigned long last_millis_checking = 0;

bool _checkTemperature(const unsigned long timeout_ms){

	const unsigned long time_yet_ms = millis() - last_millis_checking;

	if(time_yet_ms < timeout_ms)
		return false;

	last_millis_checking = millis(); 

	bool wasError = false;
	pre_times = 0;
	int tempMax = 0;
	int histeresis = HIST_MAX;
	int qty_no_sensor = 0;
	int qty_max_temperature_threshold = 0;

	while(true){

		int t1;
		const STATUS_TEMPERATURE state1 = getTemperature(t1, ds1);
		int t2;
		const STATUS_TEMPERATURE state2 = getTemperature(t2, ds2);	

		if(state1 == ONE_NORMAL && state2 == ONE_NORMAL){
			tempMax = t1 > t2 ? t1 : t2;
		} else {
			if(state1 == ONE_NORMAL){
				tempMax = t1;
				Serial.println(F("Only sensor 1 work"));
			} else if(state2 == ONE_NORMAL){
				tempMax = t2;
				Serial.println(F("Only sensor 2 work"));
			} else {
				if(qty_no_sensor > MAX_NO_SENSOR_TIMES){
					Serial.println(F("!all temperature sensor don't work!"));
					signalingNoTemperatureSensor();
					wasError = true;
					delay(1000);
				} else {
					qty_no_sensor++;
					Serial.print(F("No temperature sensor for analitics. repeat ask bus "));
					Serial.println(qty_no_sensor);
				}
				continue;
			}
		}

		Serial.print(F("max measurement temperature : "));
		Serial.println(tempMax);

		if(tempMax > MAX_TEMPERATURE_THRESHOLD){
			if (qty_max_temperature_threshold > MAX_TEMPERATURE_THRESHOLD_TIMES)
			{
				Serial.println(F("! Max temperature !"));
				signalingMaxTemperature();
				wasError = true;
			} else {
				Serial.print(F("Max temperature. repeat measurement "));
				Serial.println(qty_max_temperature_threshold);
				qty_max_temperature_threshold++;
			}
			continue;
		}

		if(tempMax > TEMPERATURE_THRESHOLD_FOR_FAN){
			setFanBit(true, BIT_THERMAL_LOOP);
		} else {
			setFanBit(false, BIT_THERMAL_LOOP);
		}

		if(tempMax < MIN_TEMPERATURE_THRESHOLD){
			if (qty_min_temperature_threshold > MIN_TEMPERATURE_THRESHOLD_TIMES)
			{
				Serial.println(F("! Min temperature !"));
				signalingMinTemperature();
			} else {
				Serial.print(F("Min temperature. repeat measurement "));
				Serial.println(qty_min_temperature_threshold);
				qty_min_temperature_threshold++;
			}
		} else {
			qty_min_temperature_threshold = 0;
		}

		break;
	}

	return wasError;
}


int getTemperature(int & temperature, const OneWire & ds) {

ds.reset_search();

if ( !ds.search(addr)) {
	Serial.println(F("No more addresses!"));
	return ABSENT_SENSOR;
}

if (OneWire::crc8(addr, 7) != addr[7]) {
	Serial.println(F("CRC is not valid!"));
	return BREAK_SENSOR;
}

// первый байт определяет чип
switch (addr[0]) {
case 0x10:
	Serial.println(" Chip = DS18S20"); // или более старый DS1820
	return OLD_SENSOR;
case 0x28:
	//Serial.println(" Chip = DS18B20");
	break;
case 0x22:
	//Serial.println(" Chip = DS1822");
	break;
default:
	Serial.println(F("Device is not a DS18x20 family device."));
	return BREAK_SENSOR;
}

ds.reset();
ds.select(addr);
ds.write(0x44); // начинаем преобразование, используя ds.write(0x44,1) с "паразитным" питанием

delay(1000); // 750 может быть достаточно, а может быть и не хватит
// мы могли бы использовать тут ds.depower(), но reset позаботится об этом
//present = 
ds.reset();
ds.select(addr);
ds.write(0xBE);

busSet(true);

/*
Serial.print(F(" Data = "));
Serial.print(present, HEX);
Serial.print(F(" "));
*/
for (byte i = 0; i < 9; i++) { // нам необходимо 9 байт
	data[i] = ds.read();
	/*
	Serial.print(data[i], HEX);
	Serial.print(" ");
	*/
}
/*
Serial.print(F(" CRC="));
Serial.print(OneWire::crc8(data, 8), HEX);
Serial.println();
*/
// конвертируем данный в фактическую температуру
// так как результат является 16 битным целым, его надо хранить в
// переменной с типом данных "int16_t", которая всегда равна 16 битам,
// даже если мы проводим компиляцию на 32-х битном процессоре
int16_t raw = (data[1] << 8) | data[0];
byte cfg = (data[4] & 0x60);
// при маленьких значениях, малые биты не определены, давайте их обнулим
if (cfg == 0x00) raw = raw & ~7; // разрешение 9 бит, 93.75 мс
else if (cfg == 0x20) raw = raw & ~3; // разрешение 10 бит, 187.5 мс
else if (cfg == 0x40) raw = raw & ~1; // разрешение 11 бит, 375 мс

//// разрешение по умолчанию равно 12 бит, время преобразования - 750 мс
celsius = (float)raw / 16.0;

temperature = int(celsius);
/*
Serial.print(F(" Temperature = "));
Serial.print(temperature);
Serial.println(F(" C"));
*/

busSet(false);

return ONE_NORMAL;
}