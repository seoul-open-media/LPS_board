// ПРИМЕР УПРАВЛЕНИЯ И ЧТЕНИЯ СОСТОЯНИЙ ДВУХ КАНАЛОВ ОДНОГО МОДУЛЯ РЕЛЕ:               //
                                                                                       //
// (2RM) Реле электромеханическое на 2 канала (Metro-модуль):                          //
// (8X8) LED Матрица 8x8 - I2C (Metro-модуль):                                         // https://iarduino.ru/shop/Expansion-payments/led-matrica-8x8---i2c-metro-modul.html
// (BUZ) Зуммер - I2C (Metro-модуль):                                                  // https://iarduino.ru/shop/Expansion-payments/zummer---i2c-metro-modul.html
// (DHT) Датчик температуры и влажности - I2C (Metro-модуль):                          // https://iarduino.ru/shop/Sensory-Datchiki/metro_temperature_humidity.html
// (DSL) Датчик освещенности, люксметр - I2C (Metro-модуль):                           // https://iarduino.ru/shop/Sensory-Datchiki/metro_luxmeter.html
// (EXT) Расширитель выводов - I2C (Metro-модуль):                                     // https://iarduino.ru/shop/Expansion-payments/metro-expander.html
// (KEY) Кнопка - I2C (Metro-модуль):                                                  // https://iarduino.ru/shop/Expansion-payments/knopka---i2c-metro-modul.html
// (RES) Потенциометр - I2C (Metro-модуль):                                            //
// (RGB) RGB светодиод - I2C (Metro-модуль):                                           // https://iarduino.ru/shop/Expansion-payments/rgb-svetodiod---i2c-metro-modul.html
// Плата управления Metro Leonardo:                                                    // https://iarduino.ru/shop/boards/metro-leonardo.html
// Плата управления Metro ESP:                                                         // https://iarduino.ru/shop/boards/metro-esp.html
// Информация о подключении модулей к шине I2C:                                        // https://wiki.iarduino.ru/page/i2c_connection/
                                                                                       //
#include <Wire.h>                                                                      // Подключаем библиотеку для работы с аппаратной шиной I2C, до подключения библиотеки iarduino_Metro.
#include <iarduino_Metro.h>                                                            // Подключаем библиотеку.
                                                                                       //
uint8_t sum;                                                                           // Объявляем переменную для хранения количества найденных модулей Metro.
void print_rele(void);                                                                 // Объявляем функцию вывода состояний реле.
                                                                                       //
void setup(){                                                                          //
    Serial.begin(9600);                                                                // Иницируем передачу данных в монитор последовательного порта на скорости 9600 бит/сек.
//  Определяем все модули на шине I2C и сохраняем их количество в переменную sum:      //
    sum=iarduino_Metro_Start(&Wire); // &Wire1, &Wire2 ...                             // Функция принимает два необязательных параметра: ссылка на объект шины I2C (по умолчанию &Wire), номер адресного вывода (по умолчанию 12).
//  Теперь нам доступен массив Metro содержащий sum элементов для управления модулями. // Metro[0] управляет ближайшим к Arduino модулем, Metro[1] управляет следующим модулем и т.д до последнего модуля Metro[sum-1].
                                                                                       //
//  Проверяем наличие модулей Metro на шине I2C:                                       //
    while(sum==0){;}                                                                   // Если модули Metro не обнаружены, то входим в бесконечный цикл while, тогда цикл loop выполняться не будет.
    while(Metro[0].model!=MOD_2RM){;}                                                  // Если ближайший к Arduino модуль Metro не является модулем реле, то так же входим в бесконечный цикл while.
}                                                                                      //
                                                                                       //
void loop(){                                                                           //
//  Включаем реле:                                                                     //
    Metro[0].on(1);         print_rele();                                              // Включаем 1 реле и выводим состояние в монитор последовательного порта.
    delay(1000);                                                                       // Ждём секунду.
    Metro[0].on(2);         print_rele();                                              // Включаем 2 реле и выводим состояние в монитор последовательного порта.
    delay(1000);                                                                       // Ждём секунду.
//  Отключаем реле:                                                                    //
    Metro[0].off(1);        print_rele();                                              // Отключаем 1 реле и выводим состояние в монитор последовательного порта.
    delay(1000);                                                                       // Ждём секунду.
    Metro[0].off(2);        print_rele();                                              // Отключаем 2 реле и выводим состояние в монитор последовательного порта.
    delay(1000);                                                                       // Ждём секунду.
//  Включаем все реле:                                                                 //
    Metro[0].on();          print_rele();                                              // Включаем все реле и выводим состояние в монитор последовательного порта.
    delay(1000);                                                                       // Ждём секунду.
//  Отключаем все реле:                                                                //
    Metro[0].off();         print_rele();                                              // Отключаем все реле и выводим состояние в монитор последовательного порта.
    delay(1000);                                                                       // Ждём секунду.
}                                                                                      //
                                                                                       //
void print_rele(void){                                                                 //
    Serial.print(Metro[0].read(1));                                                    // Выводим состояние 1 реле.
    Serial.print(Metro[0].read(2));                                                    // Выводим состояние 2 реле.
    Serial.print('-');                                                                 //
    Serial.println(Metro[0].read());                                                   // Выводим состояние всех реле.
}                                                                                      //
