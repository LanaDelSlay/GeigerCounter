#include <LiquidCrystal.h>

unsigned long counts;
unsigned long previousCPM;
unsigned long previousMillis;

#define LOG_PERIOD 60000 //60,000 = 60 seconds
#define INPUT_PIN 2

LiquidCrystal lcd(7, 8, 9, 10, 11, 12);

void setup() { 
  
  lcd.begin(16, 2);
  counts = 0;
  Serial.begin(9600);
  pinMode(INPUT_PIN, INPUT);
  attachInterrupt(digitalPinToInterrupt(INPUT_PIN), iterateCounter, FALLING); //define external interrupts
  Serial.println("Init");
}

void initDisplay(){
  
    lcd.setCursor(0, 0);
    lcd.clear();
}

void iterateCounter() {
  
  counts++;
}
void loop() { 

  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis > LOG_PERIOD) {
    previousMillis = currentMillis;
    Serial.print("CPM: "); Serial.println(counts);
    previousCPM = counts;
    counts = 0;
    initDisplay();

  } else {
    lcd.setCursor(0,0);
    lcd.print("CPM:");  lcd.setCursor(4,0);  lcd.print(previousCPM); lcd.setCursor(7,0); lcd.print("Time:"); lcd.setCursor(13,0);lcd.print(millis()/1000); 
    lcd.setCursor(0,1); 
    lcd.print("Click Count:"); lcd.setCursor(13,1); lcd.print(counts);
  }
}
