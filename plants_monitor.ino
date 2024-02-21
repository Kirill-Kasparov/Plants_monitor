#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <DHT.h>      // подключаем библиотека для датчика температуры и влажности

int AirValuePin = A3;  // Пин подключения потенциометра для переменной WaterValue
int AlarmLed = 2;   // Пин светодиода тревоги
DHT dht(11, DHT22);  // Пин датчика температуры и влажности
LiquidCrystal_I2C lcd(0x27,16,2);  // Устанавливаем дисплей


//датчики влажности почвы
int AirValue = 600;             // Максимальное значение сухого датчика
int WaterValue = 200;           // Минимальное значение погруженного датчика

int soilMoistureValue = 0;            // Создаем переменную soilMoistureValue
int soilMoisturePercent = 0;    

int soilMoistureValue2 = 0;            // Создаем переменную soilMoistureValue2
int soilMoisturePercent2 = 0;  

int soilMoistureValue3 = 0;            // Создаем переменную soilMoistureValue3
int soilMoisturePercent3 = 0;  

void setup() 
{
  Serial.begin(9600);                 // Открываем последовательную связь на скорости 9600
  pinMode(soilMoistureValue, INPUT);  // Устанавливаем вывод как вход.
  pinMode(soilMoistureValue2, INPUT);  // Устанавливаем вывод как вход.
  pinMode(soilMoistureValue3, INPUT);  // Устанавливаем вывод как вход.
  pinMode(AirValuePin, INPUT);  // Потенциометр
  pinMode(AlarmLed, OUTPUT);
  dht.begin();                // запускаем датчик DHT22
  
  lcd.init();                     
  lcd.backlight();// Включаем подсветку дисплея

}
void loop() 
{
// Снимаем порог влажности с потенциометра
int airPotValue = analogRead(AirValuePin);  // Считываем значение с потенциометра для AirValue
AirValue = map(airPotValue, 0, 1023, 300, 1000); // Маппинг значения на диапазон от 300 до 600

// Снимаем температуру и влажность воздуха
int f = dht.readHumidity();
int t = dht.readTemperature();

// Выводим темп. и влажн. воздуха
lcd.setCursor(0, 0);
lcd.print("Temp: ");
lcd.print(t);
lcd.print("c");

lcd.setCursor(9, 0);
lcd.print(" f: ");
lcd.print(f);
lcd.print("%");

  //Снимаем влажность почвы
soilMoistureValue = analogRead(A0);   // Считываем данные с порта A0 и записываем их в переменную
soilMoistureValue2 = analogRead(A1);   // Считываем данные с порта A0 и записываем их в переменную
soilMoistureValue3 = analogRead(A2);   // Считываем данные с порта A0 и записываем их в переменную

if (soilMoistureValue > AirValue || soilMoistureValue2 > AirValue || soilMoistureValue3 > AirValue) {
    // Находим ближайшее значение и присваиваем его переменной AirValue
    int closestValue = soilMoistureValue;
    if (abs(soilMoistureValue2 - AirValue) < abs(closestValue - AirValue)) {
      closestValue = soilMoistureValue2;
    }
    if (abs(soilMoistureValue3 - AirValue) < abs(closestValue - AirValue)) {
      closestValue = soilMoistureValue3;
    }
    AirValue = closestValue;}

soilMoisturePercent = map(soilMoistureValue, AirValue, WaterValue, 0, 9);
soilMoisturePercent2 = map(soilMoistureValue2, AirValue, WaterValue, 0, 9);
soilMoisturePercent3 = map(soilMoistureValue3, AirValue, WaterValue, 0, 9);

// Выводим влажность почвы
lcd.setCursor(0, 1);
lcd.print("1");
lcd.write(126);
lcd.print(soilMoisturePercent);

lcd.setCursor(5, 1);
lcd.print(" 2");
lcd.write(126);
lcd.print(soilMoisturePercent2);

lcd.setCursor(11, 1);
lcd.print(" 3");
lcd.write(126);
lcd.print(soilMoisturePercent3);

// Сигнал тревоги
if (soilMoisturePercent <= 1 || soilMoisturePercent2 <= 1 || soilMoisturePercent3 <= 1) {
    digitalWrite(AlarmLed, HIGH);
  } else {
    digitalWrite(AlarmLed, LOW);
  }

// Монитор
Serial.print("Temp: ");
Serial.print(t);
Serial.print("с f: ");
Serial.println(f);
Serial.print("1-");
Serial.print(soilMoisturePercent);
Serial.print("% 2-");
Serial.print(soilMoisturePercent2);
Serial.print("% 3-");
Serial.println(soilMoisturePercent3);
Serial.print("AirValue: ");
Serial.println(AirValue);
Serial.println(airPotValue);

delay(1000);
}


