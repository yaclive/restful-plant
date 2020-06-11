#define HWSERIAL Serial1

elapsedMillis plantTimer;
const int timerDur = 500;
int timerVarSelect = 0;

const int moistSensorPin = A2;
const int lightSensorPin = A1;

int moistSensorVal = 0;
int lightSensorVal = 0;

int plantHealth = 100;
int plantSatasfaction = 0; // -2 very unsatasfied -1 unsatasfied 0 neutral 1 satasfied 2 very satasfied
int plantSatasMult = 1; // How much satasfaction influences health
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
}

void updatePlant() {
  
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

  if (plantTimer >= timerDur) {
    plantTimer = 0;

    readSensors();
    updatePlant();
    sendDataToESP(timerVarSelect);

    timerVarSelect++;
    timerVarSelect %= 6;
  }
  
  //delay(10);
}
