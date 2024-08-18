// ПРИМЕР ДЛЯ ОДНОГО ИЛИ НЕСКОЛЬКИХ ДАТЧИКОВ ВЛАЖНОСТИ И ТЕМПЕРАТУРЫ:                  //
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
                                                                                       //
void setup(){                                                                          //
//  Определяем все модули на шине I2C и сохраняем их количество в переменную sum:      //
    sum=iarduino_Metro_Start(&Wire); // &Wire1, &Wire2 ...                             // Функция принимает два необязательных параметра: ссылка на объект шины I2C (по умолчанию &Wire), номер адресного вывода (по умолчанию 12).
//  Теперь нам доступен массив Metro содержащий sum элементов для управления модулями. // Metro[0] управляет ближайшим к Arduino модулем, Metro[1] управляет следующим модулем и т.д до последнего модуля Metro[sum-1].
                                                                                       //
//  Готовимся выводить информацию в монитор последовательного порта:                   //
    Serial.begin(9600);                                                                // Инициируем связь с монитором последовательного порта на скорости 9600 бит/сек.
    while(!Serial){;}                                                                  // Ждём готовность к работе аппаратной шины UART.
}                                                                                      //
                                                                                       //
void loop(){                                                                           //
    uint8_t j=0;                                                                       // Определяем переменную для хранения порядкового номера датчика влажности и температуры.
//  Проходим по всем найденным модулям:                                                //
    for(uint8_t i=0; i<sum; i++){                                                      // Модуль с N i может быть не датчиком влажности и температуры (а, например, светодиодом), по этому мы и ввели переменную j которая считает только датчики DHT.
//      Если очередной модуль i является датчиком влажности и температуры:             //
        if(Metro[i].model==MOD_DHT){                                                   // Тип модуля определяется по значению model.
            j++;                                                                       // Увеличиваем порядковый номер датчика.
            Serial.print  ("Датчик N ");                                               // Выводим текст.
            Serial.print  (j);                                                         // Выводим порядковый номер датчика влажности и температуры.
            Serial.print  (": температура ");                                          // Выводим текст.
            Serial.print  ( Metro[i].read( DHT_TEMPERATURE ) );                        // Выводим температуру.
            Serial.print  ("°C, влажность ");                                          // Выводим текст.
            Serial.print  ( Metro[i].read( DHT_HUMIDITY    ) );                        // Выводим влажность.
            Serial.println("%");                                                       // Выводим текст с переводом строки.
        }                                                                              // Благодаря наличию переменной j мы выводим номер датчика влажности и температуры, а не порядковый номер модуля.
    }                                                                                  // Значит скетч будет корректно выводить номер датчика, даже если на шине I2C есть другие модули Metro (не являющиеся датчиками влажности и температуры).
    Serial.println("========================================");                        // Выводим текст с переводом строки.
    delay(3000);                                                                       // Ставим задержку на 3 секунды. Выводить показания чаще нет смысла, так как показания чипа модуля (по умолчанию) обновляются 1 раз в 3 секунды.
}                                                                                      //