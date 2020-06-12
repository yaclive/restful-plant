#define HWSERIAL Serial1

elapsedMillis secondTimer;
int uptime = 0;
elapsedMillis ESPTimer;
elapsedMillis plantTimer;
const int ESPTimerDur = 500;
const int plantTimerDur = 1000 * 60 * 10; // 10 minutes
int ESPTimerVarSelect = 0;

const int moistSensorPin = A2;
const int lightSensorPin = A1;


int lightMin = 9999;
int lightMax = 0;

int moistSensorVal = 0;
int lightSensorVal = 0;

int plantHealth = 100;
int plantSatasfaction = 0; // -2 very unsatasfied -1 unsatasfied 0 neutral 1 satasfied 2 very satasfied
float plantSatasMult = 0.2; // How much satasfaction influences health
int plantAge = 0;
bool plantAlive = true;

void setup() {
  Serial.begin(9600);
  HWSERIAL.begin(115200);

  pinMode(moistSensorPin, INPUT);
  pinMode(lightSensorPin, INPUT);
}

void readSensors() {
  moistSensorVal = analogRead(moistSensorPin);
  lightSensorVal = analogRead(lightSensorPin);

  if (uptime < 86400) { // 1 day
    if (lightSensorVal > lightMax) {
      lightMax = lightSensorVal;
    }
    if (lightSensorVal < lightMin) {
      lightMin = lightSensorVal;
    }
  }
}

void updatePlant() {
  plantSatasfaction = 0;
  if (moistSensorVal < 950) { // Soil is moist
    plantSatasfaction++;
  }
  else if (moistSensorVal > 1000) {
    plantSatasfaction--;
  }
  if (lightSensorVal > (lightMax - 200)) { // Light room
    plantSatasfaction++;
  }
  else if (lightSensorVal < (lightMin + 200)) {
    plantSatasfaction--;
  }

  plantHealth += (plantSatasfaction * plantSatasMult);
  if (plantHealth > 100) {
    plantHealth = 100;
  }
  else if (plantHealth < 0) {
    plantHealth = 0;
    plantAlive = false;
  }
  plantAge ++;
}

void sendDataToESP(int varSelect) {
  switch(varSelect) {
    case 0:
      HWSERIAL.print("Moisture:" + String(moistSensorVal) + "\n");
      break;
    case 1:
      HWSERIAL.print("Light:" + String(lightSensorVal) + "\n");
      break;
    case 2:
      HWSERIAL.print("Health:" + String(plantHealth) + "\n");
      break;
    case 3:
      HWSERIAL.print("Satasfa:" + String(plantSatasfaction) + "\n");
      break;
    case 4:
      HWSERIAL.print("Age:" + String(plantAge) + "\n");
      break;
    case 5:
      HWSERIAL.print("Alive:" + String(plantAlive) + "\n");
      break;
    
  }
}

void loop() {
  int incomingByte;

  // Write usb serial to ESP
  if (Serial.available() > 0) {
    incomingByte = Serial.read();
    HWSERIAL.write(incomingByte);
  }
  // Write ESP serial to usb
  if (HWSERIAL.available() > 0) {
    incomingByte = HWSERIAL.read();
    Serial.write(incomingByte);
  }

  if (secondTimer >= 1000) {
    secondTimer = 0;
    
    uptime ++;
  }
  if (plantTimer >= plantTimerDur) {
    plantTimer = 0;

    if (plantAlive && uptime > 86400) updatePlant(); // Plant is alive and has been alive longer than a day
  }
  if (ESPTimer >= ESPTimerDur) {
    ESPTimer = 0;

    readSensors();
    sendDataToESP(ESPTimerVarSelect);

    ESPTimerVarSelect++;
    ESPTimerVarSelect %= 6;
  }
  
  //delay(10);
}
