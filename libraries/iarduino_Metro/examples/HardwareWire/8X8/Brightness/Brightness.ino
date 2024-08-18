// ПРИМЕР ВКЛЮЧЕНИЯ ВСЕХ ПИКСЕЛЕЙ СВЕТОДИОДНОГО ИНДИКАТОРА И ИЗМЕНЕНИЯ ИХ ЯРКОСТИ:     //
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
uint8_t sum;                                                                           // Объявляем  переменную для хранения количества найденных модулей Metro.
int     i=7;                                                                           // Определяем переменную для хранения яркости дисплея.
bool    f=true;                                                                        // Определяем флаг указывающий на увеличение яркости дисплея.
                                                                                       //
void setup(){                                                                          //
//  Определяем все модули на шине I2C и сохраняем их количество в переменную sum:      //
    sum=iarduino_Metro_Start(&Wire); // &Wire1, &Wire2 ...                             // Функция принимает два необязательных параметра: ссылка на объект шины I2C (по умолчанию &Wire), номер адресного вывода (по умолчанию 12).
//  Теперь нам доступен массив Metro содержащий sum элементов для управления модулями. // Metro[0] управляет ближайшим к Arduino модулем, Metro[1] управляет следующим модулем и т.д до последнего модуля Metro[sum-1].
                                                                                       //
//  Проверяем наличие модулей Metro на шине I2C:                                       //
    while(sum==0){;}                                                                   // Если модули Metro не обнаружены, то входим в бесконечный цикл while, тогда цикл loop выполняться не будет.
    while(Metro[0].model!=MOD_8X8){;}                                                  // Если ближайший к Arduino модуль Metro не является светодиодным индикатором, то так же входим в бесконечный цикл while.
                                                                                       //
    delay(100);                                                                        //
                                                                                       //
//  Включаем дисплей:                                                                  //
    Metro[0].on();                                                                     // Если функция on() вызвана без параметров, то весь дисплей будет залит.
}                                                                                      //
                                                                                       //
void loop(){                                                                           //
//  Применяем яркость дисплея:                                                         //
    Metro[0].on(i);                                                                    // Если функция on() вызвана с параметром от 0 до 10, то она установит яркость без изменения изображения дисплея.
    delay(500);                                                                        // Ждём пол секунды.
//  Изменяем значение яркости:                                                         //
    if(f){ i++; if(i>10){i=10; f=false;} }                                             // Увеличиваем значение яркости.
    else { i--; if(i<0 ){i=0;  f=true; } }                                             // Уменьшаем   значение яркости.
}                                                                                      //