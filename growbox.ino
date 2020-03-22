#include <DHT.h> // Библиотека для работы с температурным датчиком
#include <Wire.h> // Библиотека для работы с I2C
#include <LiquidCrystal_I2C.h> // LCD1602 - Дисплей
#include <stdio.h>
#include <DS1302.h> // Библиотека для часов реального времени

#define DHTPIN 4 // Пин температурного датчика
#define DHTTYPE DHT22 // Тип температурного датчика

//Константы
const short onButton = 2; // Кнопка включения
const short mainLedRele = 3; // Пин упраления реле главным светодиодом
const short onLed = 12; // Светодиод внутри кнопки
const short resetLed = 13; // HDD светодиод
// Модули управляющие скоростью вращения куллеров через шим
const short mosModule1 = 11;
const short mosModule2 = 10; // Кулер охлаждения основного светодиода
const short mosModule3 = 5; // Кулер на передней панели
// Пины чясов реального времени
const short clk = 9;
const short dat = 8;
const short rst = 7;
DS1302 rtc(rst, dat, clk);
//Получение дня недели
String dayAsString(const Time::Day day) {
  switch (day) {
    case Time::kSunday: return "SU";
    case Time::kMonday: return "MO";
    case Time::kTuesday: return "TU";
    case Time::kWednesday: return "WE";
    case Time::kThursday: return "TH";
    case Time::kFriday: return "FR";
    case Time::kSaturday: return "SA";
  }
  return "NO";
}
// Функция получения даты
String getDate(Time tm) {
  char buf[25];
  snprintf(buf, sizeof(buf), "%02d-%02d-%04d",
           tm.date, tm.mon, tm.yr);
  return buf;
}
// Функция получения времени
String getTime(Time tm) {
  const String day = dayAsString(tm.day);
  
  char buf[25];
  snprintf(buf, sizeof(buf), "%02d:%02d:%02d %s",
           tm.hr, tm.min, tm.sec, day.c_str());
  return buf;
}

boolean onFlag = false; // Фллаг для определения нажатия кнопки
LiquidCrystal_I2C lcd(0x27,16,2);  // Устанавливаем дисплей

DHT dht(DHTPIN, DHTTYPE); // Установка температурного датчика


void setup() {
  
/*
// Установка времени
  rtc.halt(false); //запуск часов
  rtc.writeProtect(false); //снять защиту от записи
// Год, Месяц, День, Время, день недели
  Time t(2020, 3, 22, 19, 16, 00, Time::kSunday);
  rtc.time(t);
*/

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
pinMode(mosModule3, OUTPUT);

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
  
if (onFlag) {
    analogWrite(mosModule2, 255); // Если включен основной светодиод то включаем кулер охлаждения
} else {
    digitalWrite(mosModule2, LOW); // Выключаем кулер
}

// Управление скростью вращения куллеров
analogWrite(mosModule3, 255);

// Считывание показаний с температурного датчика
int t = round(dht.readTemperature()); // Температура
int h = round(dht.readHumidity()); // Влажность

// Печать данных о температуре-влажности на экран
lcd.setCursor(2, 0);
lcd.print(t);
lcd.setCursor(2, 1);
lcd.print(h);
// Печать даты-времени
Time tm = rtc.time(); // Получение даты-времени
lcd.setCursor(5, 0);
lcd.print(getTime(tm));
lcd.setCursor(5, 1);
lcd.print(getDate(tm));
}
