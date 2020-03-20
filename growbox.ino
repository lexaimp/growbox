#include <DHT.h> // Библиотека для работы с температурным датчиком
#include <Wire.h> // Библиотека для работы с I2C
#include <LiquidCrystal_I2C.h> // LCD1602 - Дисплей

#define DHTPIN 4 // Пин температурного датчика
#define DHTTYPE DHT22 // Тип температурного датчика

//Константы
const short onButton = 2; // Кнопка включения
const short mainLedRele = 3; // Пин упраления реле главным светодиодом
const short onLed = 12; // Светодиод внутри кнопки
const short resetLed = 13; // HDD светодиод
// Модули управляющие скоростью вращения куллеров через шим
const short mosModule1 = 11;
const short mosModule2 = 10;

boolean onFlag = false; 
LiquidCrystal_I2C lcd(0x27,16,2);  // Устанавливаем дисплей

DHT dht(DHTPIN, DHTTYPE); // Установка температурного датчика
void setup() {
// Дисплей
  lcd.init(); // Инициализация                    
  lcd.backlight(); // Включаем подсветку дисплея
  lcd.setCursor(0, 0);
  lcd.print("T=");
  lcd.setCursor(0, 1);
  lcd.print("H=");

// Кнопка включения
pinMode(onButton, INPUT);
// Светодиод кнопки включения
pinMode(onLed, OUTPUT);  
// Реле включения светодиода 220
pinMode(mainLedRele, OUTPUT);
// МосМодули
pinMode(mosModule1, OUTPUT);
pinMode(mosModule2, OUTPUT);


// Светодиод reset
pinMode(resetLed, OUTPUT);

// Температурный датчик
dht.begin();
}

void loop() {
  
// Работа с кнопкой включения
if (digitalRead(onButton) == HIGH && !onFlag) {
    digitalWrite(onLed, !digitalRead(onLed)); // Зажигаем или тушим светодиод кнопки
    digitalWrite(mainLedRele, !digitalRead(mainLedRele)); // Зажигаем или тушим светодиод 220
    onFlag = true;
  }
if (digitalRead(onButton) == LOW && onFlag) {
    onFlag = false;
  }

// Управление скростью вращения куллеров
analogWrite(mosModule2, 250);

// Считывание показаний с температурного датчика
float t = dht.readTemperature(); // Температура
float h = dht.readHumidity(); // Влажность

// Печать данных о температуре-влажности на экран
lcd.setCursor(2, 0);
lcd.print(t);
lcd.setCursor(2, 1);
lcd.print(h);
}
