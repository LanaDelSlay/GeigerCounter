#include <SSD1306.h>
#define LOG_PERIOD 60000 //60,000 = 60 seconds
#define INPUT_PIN 2
SSD1306  display(0x3c, 5, 4);

int counts = 0;
unsigned int previousCPM;
unsigned long previousMillis;
unsigned int elapsedSecs;

void displayString(String dispString, int x, int y) {
  display.setColor(WHITE);
  display.setTextAlignment(TEXT_ALIGN_LEFT);
  display.drawString(x, y, dispString);
  display.setFont(ArialMT_Plain_16);
  display.display();
}

void displayInt(int dispInt, int x, int y) {
  display.setColor(WHITE);
  display.setTextAlignment(TEXT_ALIGN_LEFT);
  display.drawString(x, y, String(dispInt));
  display.setFont(ArialMT_Plain_16);
  display.display();
}

void displayInit() {
  display.init();
  display.flipScreenVertically();
  display.setFont(ArialMT_Plain_16);
}

void setup() { 
  displayInit();
  counts = 0;
  Serial.begin(9600);
  pinMode(INPUT_PIN, INPUT);
  attachInterrupt(digitalPinToInterrupt(INPUT_PIN), iterateCounter, FALLING); //define external interrupts
  Serial.println("Init");
}


void iterateCounter() {
  
  counts++;
}

void loop() { 
    elapsedSecs = millis()/1000;
    unsigned long currentMillis = millis();
  if (currentMillis - previousMillis > LOG_PERIOD) {
    previousMillis = currentMillis;
    Serial.print("CPM: "); Serial.println(counts);
    previousCPM = counts;
    counts = 0;
    displayInit();

  } else {
    displayString("CPM: ", 0,0);
    displayString("Time:",50,0);
    displayString("Crnt CPM:",0,40);
    // following overlaps previous text with a black rectangle effectively erasing it.
    display.drawRect(0, 20, 100, 40);
    display.setColor(BLACK);
    display.fillRect(0, 20, 100, 40);
    display.setColor(WHITE);
    displayInt(elapsedSecs,50,20);
    displayInt(previousCPM,0,20);
    Serial.println(counts);
    displayInt(counts,80,40);
  }
}
