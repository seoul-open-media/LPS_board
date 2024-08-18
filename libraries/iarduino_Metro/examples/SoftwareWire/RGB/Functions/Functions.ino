// ПРИМЕР ВЫПОЛНЕНИЯ ПРЕДУСТАНОВЛЕННЫХ ФУНКЦИЙ ПОВЕДЕНИЯ ДЛЯ ОДНОГО СВЕТОДИОДА:        //
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
#include <iarduino_I2C_Software.h>                                                     // Подключаем библиотеку для работы с программной шиной I2C, до подключения библиотеки iarduino_Metro.
#include <iarduino_Metro.h>                                                            // Подключаем библиотеку.
                                                                                       //
SoftTwoWire sWire(3,4);                                                                // Создаём объект программной шины I2C указав выводы которым будет назначена роль линий: SDA, SCL.
uint8_t sum;                                                                           // Объявляем переменную для хранения количества найденных модулей Metro.
                                                                                       //
void setup(){                                                                          //
//  Определяем все модули на шине I2C и сохраняем их количество в переменную sum:      //
    sum=iarduino_Metro_Start(&sWire);                                                  // Функция принимает два необязательных параметра: ссылка на объект шины I2C (по умолчанию &Wire), номер адресного вывода (по умолчанию 12).
//  Теперь нам доступен массив Metro содержащий sum элементов для управления модулями. // Metro[0] управляет ближайшим к Arduino модулем, Metro[1] управляет следующим модулем и т.д до последнего модуля Metro[sum-1].
                                                                                       //
//  Проверяем наличие модулей Metro на шине I2C:                                       //
    while(sum==0){;}                                                                   // Если модули Metro не обнаружены, то входим в бесконечный цикл while, тогда цикл loop выполняться не будет.
    while(Metro[0].model!=MOD_RGB){;}                                                  // Если ближайший к Arduino модуль Metro не является светодиодом, то так же входим в бесконечный цикл while.
}                                                                                      //
                                                                                       //
void loop(){                                                                           //
//  Включаем светодиод на выполнение 1 функции:                                        //
    Metro[0].on( RGB(1) );                                                             // Светодиод будет плавно переливаться всеми цветами радуги.
    delay(10000);                                                                      // Ждём 10 секунд.
//  Включаем светодиод на выполнение 2 функции:                                        //
    Metro[0].on( RGB(2) );                                                             // Светодиод будет переключаться по цветам радуги: красный, жёлтый, зелёный, голубой, синий, фиолетовый и опять красный, ...
    delay(10000);                                                                      // Ждём 10 секунд.
//  Включаем светодиод на выполнение 3 функции:                                        //
    Metro[0].on( RGB(3) );                                                             // Светодиод будет включать случайные цвета (можно использовать как шарапнель ёлочной гирлянды).
    delay(10000);                                                                      // Ждём 10 секунд.
//  Включаем эелёный цвет:                                                             //
    Metro[0].on( 0x00FF00 );                                                           // Включаем зелёный цвет.
    delay(3000);                                                                       // Ждём 3 секунды.
//  Выключаем светодиод:                                                               //
    Metro[0].off();                                                                    // Выключаем светодиод.
    delay(1000);                                                                       // Ждём 1 секунду.
//  Количество функций светодиода зависит от версии его прошивки.                      //
}                                                                                      //