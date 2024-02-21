# Plants_monitor
Контроль показателей комнатных растений

Описание:
---------------------

Гаджет отображает информацию:

- о температуре,
- влажности воздуха,
- влажности почвы комнатных растений,
- позволяет настроить уровень влажности для полива
- выводит индикатор необходимости полива (можно заменить на автополив)

![image](https://github.com/Kirill-Kasparov/Plants_monitor/assets/131332065/626eb38f-e1c0-458c-b8c6-2c9457e1d091)

Принцип работы:
---------------------
На экран выводится 2 строки с показателями.

1 строка - температура воздуха и влажность воздуха с датчика DHT22 (можно добавить давление, если заменить на датчик BME280).
2 строка - данные о влажности почвы, получаемые с 3-х одноименных датчиков влажности почвы).

Через потенциометр (крутилку) настраивается уровень влажности, при котором растению необходим полив. Показатель настроен от 0 до 9, где 0 - сухая почва, а 9 - датчик погружен в воду.
Верхний порог (9) настраивается программно, а нижний регулируется потенциометром. Как только показатель любого из датчиков опустится до 1 или 0, загорится красная лампочка, уведомляющая о необходимости полива. Как только показатель будет 2 и выше - лампочка гаснет автоматически.

Подсветка дисплея включается вручную нажатием кнопки, чтобы снизить энергопотребление и исключить лишние источники света в квартире.

Потребуется:
---------------------

1. Arduino Nano - 1 шт.
2. Датчик влажности почвы - 3 шт.
3. LCD 1602 + переходник I2C (плата на 4 пина для экрана) - 1 шт.
4. Датчик температуры и влажности DHT22 - 1 шт.
5. Потенциометр	10 kΩ - 1 шт.
6. Кнопка с фиксатором on/off или тумблер - 1 шт
7. Красный светодиод -	1 шт.
8. Резистор 220Ω - 1 шт.
9. Источник питания 5v - 1 шт.
10. Корпус для сбооки гаджета min 90х30х20 - 1шт.
11. Провода красный, черный, цветной - min 2 метра каждого

Реализация:
---------------------
1. Собмраем предварительный прототип на макетной плате по схеме:
   ![Распиновка](https://github.com/Kirill-Kasparov/Plants_monitor/assets/131332065/90a47e00-e70a-4b1a-81d9-68663413bffb)

Распиновка:
Для этих датчиков обязательно аналоговые порты:
А0 - датчик почвы 1
А1 - датчик почвы 2
А2 - датчик почвы 3
А3 - потенциометр
А4 - SDA порт экрана
A5 - SCL порт экрана

Можно в любой пин:
11 - датчик температуры и влажности
2 - красный светодиод
GND - общий провод
5V - можно запитать от платы, но мне не хочется испытывать её на прочность. Рекомендую запитать от внешнего источника.

2. Загружаем скетч:

```cpp
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
```
На макете должны получить результат:

![image](https://github.com/Kirill-Kasparov/Plants_monitor/assets/131332065/2297c29d-2ae5-4150-bbcf-cd6434a05c98)

3. Сборка корпуса:

   Минимальный размер корпуса 90х30х20. Я использовал губку для обуви 90х50х20
   Устанавливаем крупные детали.
   Вырезаем окно для дисплея, плотно монтируем на винты или жидкий клей. 4 пина сразу отгибаем в более свободное пространство.
   
   Устанавливаем контроллер Aeduino Nano, плотно фиксируем, чтобы не сорвать плату при подключении кабеля. У меня "Нано" отлично вошел поперек короба. Вырезаем отверстие для доступа к порту и возможности обновить программу.

   Фиксируем потенциометр (крутилку)

   Устанавливаем датчик температуры и влажности (на фото про него сразу не вспомнил)

   Добавляем кнопку или тумблер для управления подсветкой

![image](https://github.com/Kirill-Kasparov/Plants_monitor/assets/131332065/03ccb207-e460-4356-b872-8be2291afb39)


4. Прокладываем питание
   В корпус подаем "плюс" и "минус" на 4 устройства:
   - контроллер Ардуино
   - дисплей
   - потенциометр
   - датчик температуры и влажности

На общий "минус" можно подключить и светодиод, у меня он подключен к Ардуино.

За корпусом подаем "плюс" и "минус" на 3 датчика влажности почвы, чтобы сократить объем проводов.
Опционально датчик температуры и влажности можно вынести за пределы корпуса, но учитывайте возможность попадания воды от опрыскивания растений.

   ![image](https://github.com/Kirill-Kasparov/Plants_monitor/assets/131332065/60b1f907-8980-4e8b-bfcc-da1d26ef3d43)

5. Подключаем пины
   А3 - потенциометр
   А4 - SDA порт экрана
   A5 - SCL порт экрана
   11 - датчик температуры и влажности
   2 - красный светодиод (через резистор 220 Ом)

   ![image](https://github.com/Kirill-Kasparov/Plants_monitor/assets/131332065/215744bb-fd34-4739-92ab-57bd8d54cbe6)

Снимаем перемычку с дисплея, отвечающую за подсветку экрана. Вместо перемычки, используем кнопку с фиксатором или тумблер. Подключаем 2 пина, на которых была перемычка, через провода к кнопке.
![image](https://github.com/Kirill-Kasparov/Plants_monitor/assets/131332065/be0f753d-594f-464d-8745-a5f5d697407f)

На этом этапе корпус можно подключить и проверить работу всех устройств, кроме датчиков почвы.

   Выводим из корпуса 3 контактных кабеля для датчиков почвы:
   А0 - датчик почвы 1
   А1 - датчик почвы 2
   А2 - датчик почвы 3

   У меня датчики почвы и источник питания будут замурованы, так что для возможности отдельно отсоединить корпус для обновлений и доработок, в качестве клеммы использовал usb-разъем (что было под рукой).

![image](https://github.com/Kirill-Kasparov/Plants_monitor/assets/131332065/ad5accac-e8e9-4224-9a26-27c077824724)

Осталось подключить датчики почвы. Область, выделенную красным, рекомендую замотать изолентой, чтобы защитить от брызг при поливе и опрыскивании:

![image](https://github.com/Kirill-Kasparov/Plants_monitor/assets/131332065/cb737d20-18f3-42e4-ab6f-090ea02aceee)

Готовый результат монтажа:
![image](https://github.com/Kirill-Kasparov/Plants_monitor/assets/131332065/87442b84-60e9-4a6c-a3bf-3b2b38f166d0)

![image](https://github.com/Kirill-Kasparov/Plants_monitor/assets/131332065/bd3597c7-459a-42c6-8224-d1faaffb7090)

![image](https://github.com/Kirill-Kasparov/Plants_monitor/assets/131332065/49b7d70f-c20c-465f-a555-abf1d1fe4cab)
