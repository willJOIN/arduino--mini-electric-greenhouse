#include <DHT.h>
#include <LiquidCrystal_I2C.h>
#define DHTPIN 7
#define DHTTYPE DHT22
#define cooler1 2
#define cooler2 3
#define pinoResist 12
#define pinoValvula 8
#define pinoSensor A0

DHT dht(DHTPIN, DHTTYPE);
LiquidCrystal_I2C lcd(0x27, 20, 4);
boolean a = LOW, b = HIGH;
float h = 0, t = 0;
long previousMillis = 0;
long interval = 2000;
const int limiarSeco = 65;
const int limiarEncharcado = 80;
const int tempoRega = 5;
int umidadeSolo = 0;

void setup() {
  lcd.begin();
  lcd.backlight();
  dht.begin();
  pinMode(cooler1, OUTPUT);
  pinMode(cooler2, OUTPUT);
  pinMode(pinoValvula, OUTPUT);
  pinMode(pinoResist, OUTPUT);
  pinMode(pinoSensor, INPUT);
  digitalWrite(pinoValvula, HIGH);
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
      lcd.print("Umidade Solo: ");
      umidadeSolo = analogRead(pinoSensor);
      umidadeSolo = map(umidadeSolo, 1023, 0, 0, 100);
      lcd.print(umidadeSolo);
      lcd.print(" %    ");
      lcd.setCursor(0, 0);
      lcd.print("Temperatura:");
      lcd.setCursor(0, 1);
      lcd.print("Umidade Ar:");
      lcd.setCursor(14, 0 );
      lcd.print(t);
      lcd.setCursor(19, 0 );
      lcd.print("C");
      lcd.setCursor(14, 1);
      lcd.print(h);
      delay(500);
    }
    if (t <= 16 && a == LOW) {
      digitalWrite (pinoResist, LOW);
      a = HIGH;
      b = LOW;
    }
    else if (t >= 22 && b == LOW) {
      digitalWrite(pinoResist, HIGH);
      a = LOW;
      b = HIGH;
    }
    if (umidadeSolo < limiarSeco) {
      lcd.setCursor(0, 2);
      lcd.print("    Regando    ");
      digitalWrite(pinoValvula, LOW);
      delay(tempoRega * 1000);
      digitalWrite(pinoValvula, HIGH);
    }
    if (umidadeSolo > limiarEncharcado) {
      lcd.setCursor(0, 2);
      lcd.print("Encharcado   ");
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
