// ===========================
// Lighthouse Simulator
// Arduino Nano
// ===========================

const int lampPin = 3;            // Lamp/LED control pin
const int selectorPins[8] = {4,5,6,7,8,9,10,11}; // Rotary switch input pins

// Set true if your LED or relay module is active-low
const bool ACTIVE_LOW = false;     

int currentProgram = 0;
int stepIndex = 0;
unsigned long lastChange = 0;
bool lampOn = true;

// ---------------------------
// Program definitions
// Times in milliseconds, ON/OFF alternated
// ---------------------------

// Program table and visual representation (ASCII):
//
// Pos | Program       | Sequence (ON=█ OFF= ) 
// ----+---------------+----------------------------
// 0   | Krinica Morska | ██ ██ ██                  (2 ON –2 OFF –2 ON –6 OFF)
// 1   | Hel            | █████ █████               (5 ON –5 OFF)
// 2   | Jastarnia      | ██ ██ ███████ █████████   (2 ON –2 OFF –7 ON –9 OFF)
// 3   | Rozewie        | █ ███████████             (0.1 ON –2.9 OFF)
// 4   | Stilo          | █ ██ █ ██ █ ██████        (0.3 ON –2.2 OFF –0.3 ON –2.2 OFF –0.3 ON –6.7 OFF)
// 5   | Czołpino       | ██ █ ██ ███               (2 ON –1 OFF –2 ON –3 OFF)
// 6   | Jarosławiec    | █ ███ █ █████             (0.5 ON –2 OFF –0.5 ON –6 OFF)
// 7   | Darłowo        | ██ ██ ██ █████████        (2 ON –2 OFF –2 ON –9 OFF)

const unsigned long krinica[] = {2000,2000,2000,6000};                       // Krinica Morska
const unsigned long hel[] = {5000, 5000};                 // Hel
const unsigned long jastarnia[] = {2000, 2000, 7000, 9000};     // Jastarnia
const unsigned long rosewie[] = {100, 2900};     // Rozewie
const unsigned long stilo[] = {300,2200,300,2200,300,6700};                       // Stilo
const unsigned long czolpino[] = {2000, 1000, 2000, 3000};     // Czolpino
const unsigned long jaroslawiec[] = {500, 2000, 500, 6000};                 // Jaroslawiec
const unsigned long darlowo[] = {2000, 2000, 2000, 9000};     // Darlowo

#define ARRAY_LEN(x) (sizeof(x)/sizeof(x[0]))

const unsigned long* programs[8] = {krinica, hel, jastarnia, rosewie, stilo, czolpino, jaroslawiec, darlowo};
const int programLengths[8] = {
  ARRAY_LEN(krinica),
  ARRAY_LEN(hel),
  ARRAY_LEN(jastarnia),
  ARRAY_LEN(rosewie),
  ARRAY_LEN(stilo),
  ARRAY_LEN(czolpino),
  ARRAY_LEN(jaroslawiec),
  ARRAY_LEN(darlowo)
};
// ---------------------------
// Function to turn lamp ON/OFF according to ACTIVE_LOW
// ---------------------------
void setLamp(bool on) {
  if (ACTIVE_LOW) digitalWrite(lampPin, on ? LOW : HIGH);
  else            digitalWrite(lampPin, on ? HIGH : LOW);
  digitalWrite(LED_BUILTIN, on ? HIGH : LOW);
}

// ---------------------------
void setup() {
  pinMode(lampPin, OUTPUT);
  for(int i=0;i<8;i++) pinMode(selectorPins[i], INPUT_PULLUP);
  
  lampOn = true;
  setLamp(true);
  lastChange = millis();
}

// ---------------------------
void loop() {
  // Check rotary switch
  for(int i=0;i<8;i++){
    if(digitalRead(selectorPins[i]) == LOW){
      if(currentProgram != i){
        currentProgram = i;
        stepIndex = 0;
        lampOn = true;
        setLamp(true);
        lastChange = millis();
      }
    }
  }

  // Handle blinking sequence
  unsigned long now = millis();
  if(now - lastChange >= programs[currentProgram][stepIndex]){
    lampOn = !lampOn;
    setLamp(lampOn);
    lastChange = now;
    stepIndex++;
    if(stepIndex >= programLengths[currentProgram]) stepIndex = 0;
  }
}
