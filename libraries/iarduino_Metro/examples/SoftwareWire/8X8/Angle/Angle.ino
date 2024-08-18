// ПРИМЕР ПОВОРОТА ИЗОБРАЖЕНИЯ НА СВЕТОДИОДНОМ ИНДИКАТОРЕ НА 0°, 90°, 180° и 270°.     //
// Требуется установить библиотеку <iarduino_I2C_Software.h>                           // https://iarduino.ru/file/627.html
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
#include <iarduino_Metro.h>                                                            // Подключаем библиотеку для работы с модулями метро.
                                                                                       //
SoftTwoWire sWire(3,4);                                                                // Создаём объект программной шины I2C указав выводы которым будет назначена роль линий: SDA, SCL.
uint8_t sum;                                                                           // Объявляем переменную для хранения количества найденных модулей Metro.
uint8_t image[8];                                                                      // Объявляем массив для вывода изображения. Массив имеет размер 8 байт (это строки), а в каждом байте по 8 бит (это столбцы).
                                                                                       //
void setup(){                                                                          //
//  Определяем все модули на шине I2C и сохраняем их количество в переменную sum:      //
    sum=iarduino_Metro_Start(&sWire);                                                  // Функция принимает два необязательных параметра: ссылка на объект шины I2C (по умолчанию &Wire), номер адресного вывода (по умолчанию 12).
//  Теперь нам доступен массив Metro содержащий sum элементов для управления модулями. // Metro[0] управляет ближайшим к Arduino модулем, Metro[1] управляет следующим модулем и т.д до последнего модуля Metro[sum-1].
                                                                                       //
//  Проверяем наличие модулей Metro на шине I2C:                                       //
    while(sum==0){;}                                                                   // Если модули Metro не обнаружены, то входим в бесконечный цикл while, тогда цикл loop выполняться не будет.
    while(Metro[0].model!=MOD_8X8){;}                                                  // Если ближайший к Arduino модуль Metro не является светодиодным индикатором, то так же входим в бесконечный цикл while.
                                                                                       //
//  Создаём изображение (капля):                                                       //
    image[0] = 0b00111100;                                                             // Верхняя строка
    image[1] = 0b01111110;                                                             //
    image[2] = 0b11001111;                                                             //
    image[3] = 0b10000110;                                                             //
    image[4] = 0b10000000;                                                             //
    image[5] = 0b10000000;                                                             //
    image[6] = 0b01000000;                                                             //
    image[7] = 0b00100000;                                                             // Нижняя строка
                                                                                       //
    delay(100);                                                                        //
                                                                                       //
//  Выводим изображение на дисплей:                                                    //
    Metro[0].on(image);                                                                // Выводим на дисплей изображение массива image.
}                                                                                      //
                                                                                       //
void loop(){                                                                           //
//  Устанавливаем изображение в 0°:                                                    //
    Metro[0].on(X8_ANGLE_0);                                                           // Изображение не изменится но повернётся в положения по умолчанию.
    delay(100);                                                                        // Ждём 0,1 секунду.
//  Устанавливаем изображение в 90°:                                                   //
    Metro[0].on(X8_ANGLE_90);                                                          // Изображение не изменится но повернётся на 90° по часовой стрелке от положения по умолчанию.
    delay(100);                                                                        // Ждём 0,1 секунду.
//  Устанавливаем изображение в 180°:                                                  //
    Metro[0].on(X8_ANGLE_180);                                                         // Изображение не изменится но повернётся на 180° по часовой стрелке от положения по умолчанию.
    delay(100);                                                                        // Ждём 0,1 секунду.
//  Устанавливаем изображение в 270°:                                                  //
    Metro[0].on(X8_ANGLE_270);                                                         // Изображение не изменится но повернётся на 270° по часовой стрелке от положения по умолчанию.
    delay(100);                                                                        // Ждём 0,1 секунду.
}                                                                                      //