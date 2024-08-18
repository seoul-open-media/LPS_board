// ПРИМЕР ДЛЯ ОДНОГО ИЛИ НЕСКОЛЬКИХ ДАТЧИКОВ ОСВЕЩЁННОСТИ:                             //
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
    uint8_t j=0;                                                                       // Определяем переменную для хранения порядкового номера датчика освещённости.
//  Проходим по всем найденным модулям:                                                //
    for(uint8_t i=0; i<sum; i++){                                                      // Модуль с N i может быть не датчиком освещённости (а, например, светодиодом), по этому мы и ввели переменную j которая считает только датчики освещённости.
//      Если очередной модуль i является датчиком освещённости:                        //
        if(Metro[i].model==MOD_DSL){                                                   // Тип модуля определяется по значению model.
            j++;                                                                       // Увеличиваем порядковый номер датчика.
            Serial.print  ("Датчик N ");                                               // Выводим текст.
            Serial.print  (j);                                                         // Выводим порядковый номер датчика освещённости.
            Serial.print  (": освещённость = ");                                       // Выводим текст.
            Serial.print  ( Metro[i].read( DSL_LUX ) );                                // Выводим освещённость.
            Serial.println(" лк.");                                                    // Выводим текст с переводом строки.
        }                                                                              // Благодаря наличию переменной j мы выводим номер датчика освещённости, а не порядковый номер модуля.
    }                                                                                  // Значит скетч будет корректно выводить номер датчика, даже если на шине I2C есть другие модули Metro (не являющиеся датчиками освещённости).
    Serial.println("========================================");                        // Выводим текст с переводом строки.
    delay(1000);                                                                       // Ставим задержку на 1 секунду. Это предотвратит быстрое заполнение монитора строками "Датчик N ... освещённость = ... лк.".
}                                                                                      //