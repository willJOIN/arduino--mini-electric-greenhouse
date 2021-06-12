#include <DHT.h>
#include <LiquidCrystal_I2C.h>
#define DHTPIN 7
#define DHTTYPE DHT22
#define cooler1 2
#define cooler2 3
#define pinResist 12

DHT dht(DHTPIN, DHTTYPE);
LiquidCrystal_I2C lcd(0x27, 20, 4);
boolean a = LOW, b = HIGH;
float h = 0, t = 0;
long previousMillis = 0;
long interval = 2000;

const int pinSensor = A0;
const int pinValve = 8;
const int limitDry = 65;
const int limitSoaked = 80;
const int timeWatering = 5;
int humidityGround = 0;

void setup() {
  lcd.begin();
  lcd.backlight();
  dht.begin();
  pinMode(cooler1, OUTPUT);
  pinMode(cooler2, OUTPUT);
  pinMode(pinValve, OUTPUT);
  pinMode(pinResist, OUTPUT);
  digitalWrite(pinValve, HIGH);
}

void loop() {
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis > interval) {
    previousMillis = currentMillis;
    for (int i = 0; i < 5; i++) {
      t = dht.readTemperature();
      h = dht.readHumidity();
      lcd.clear();
      lcd.setBacklight(HIGH);
      lcd.setCursor(0, 2);
      lcd.print("Humidity Ground: ");
      humidityGround = analogRead(pinSensor);
      humidityGround = map(humidityGround, 1023, 0, 0, 100);
      lcd.print(humidityGround);
      lcd.print(" %    ");
      lcd.setCursor(0, 0);
      lcd.print("Temperature:");
      lcd.setCursor(0, 1);
      lcd.print("Air Humidity:");
      lcd.setCursor(14, 0 );
      lcd.print(t);
      lcd.setCursor(19, 0 );
      lcd.print("C");
      lcd.setCursor(14, 1);
      lcd.print(h);
      delay(500);
    }
    if (t <= 16 && a == LOW) {
      digitalWrite (pinResist, LOW);
      a = HIGH;
      b = LOW;
    }
    else if (t >= 22 && b == LOW) {
      digitalWrite(pinResist, HIGH);
      a = LOW;
      b = HIGH;
    }
    if (humidityGround < limitDry) {
      lcd.setCursor(0, 2);
      lcd.print("    Regando    ");
      digitalWrite(pinValve, LOW);
      delay(timeWatering * 1000);
      digitalWrite(pinValve, HIGH);
    }
    if (humidityGround > limitSoaked) {
      lcd.setCursor(0, 2);
      lcd.print("Soaked   ");
      delay(3000);
    }
    if (t >= 22 && a == LOW) { 
      digitalWrite(cooler1, LOW);
      digitalWrite(cooler2, LOW);
      a = HIGH;
      b = LOW;
    }
    else if (t <= 16 && b == LOW) {
      digitalWrite(cooler1, HIGH);
      digitalWrite(cooler2, HIGH);
      a = LOW;
      b = HIGH;
    }
  }
}
