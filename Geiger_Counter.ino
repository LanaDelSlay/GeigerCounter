
#define PRINT_DEBUG_MESSAGES

//#include <SSD1306.h>

#define PERIOD_LOG 5                  //Logging period 
#define PERIOD_THINKSPEAK 60        // in seconds, >60

SSD1306  display(0x3c, 5, 4);


const int inputPin = 25;

volatile unsigned long counts = 0;                       // Tube events
int cpm = 0;                                             // CPM
int lastCounts = 0;
unsigned long lastCountTime;                            // Time measurement
unsigned long lastEntryThingspeak;

void IRAM_ATTR ISR_impulse() { // Captures count of events from Geiger counter board
  counts++;
}

void displayInit() {
  display.init();
  display.flipScreenVertically();
  display.setFont(ArialMT_Plain_24);
}

void displayInt(int dispInt, int x, int y) {
  display.setColor(WHITE);
  display.setTextAlignment(TEXT_ALIGN_CENTER);
  display.drawString(x, y, String(dispInt));
  display.setFont(ArialMT_Plain_24);
  display.display();
}

void displayString(String dispString, int x, int y) {
  display.setColor(WHITE);
  display.setTextAlignment(TEXT_ALIGN_CENTER);
  display.drawString(x, y, dispString);
  display.setFont(ArialMT_Plain_24);
  display.display();
}


/****reset***/
void software_Reset() // Restarts program from beginning but does not reset the peripherals and registers
{
  Serial.print("resetting");
  esp_restart();
}




void setup() {
  Serial.begin(115200);

  if (PERIOD_LOG > PERIOD_THINKSPEAK) {
    Serial.println("PERIOD_THINKSPEAK has to be bigger than PERIODE_LOG");
    while (1);
  }
  displayInit();
  displayString("Welcome", 64, 15);

  display.clear();
  displayString("Measuring", 64, 15);
  pinMode(inputPin, INPUT);                            // Set pin for capturing Tube events
  attachInterrupt(inputPin, ISR_impulse, FALLING);     // Define interrupt on falling edge
  lastEntryThingspeak = millis();
  lastCountTime = millis();
  Serial.println("Initialized");
}

void loop() {

  if (millis() - lastCountTime > (PERIOD_LOG * 1000)) {
    Serial.print("Counts: "); Serial.println(counts);
    cpm = (counts - lastCounts) / PERIOD_LOG ;
    lastCounts = counts;
    lastCountTime = millis();
    display.clear();
    displayString("Radioactivity", 64, 0);
    displayInt(cpm, 64, 30);
    if (cpm > 100 );
    Serial.print("cpm: "); Serial.println(cpm);
  }

  if (millis() - lastEntryThingspeak > (PERIOD_THINKSPEAK * 1000)) {
    Serial.print("Counts: "); Serial.println(counts);
    int averageCPM = (counts) / PERIOD_THINKSPEAK;
    lastEntryThingspeak = millis();
    counts=0;
    lastCounts=0;
  }
}
