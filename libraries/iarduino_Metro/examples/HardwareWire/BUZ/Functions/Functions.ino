// ПРИМЕР ВЫВОДА МЕЛОДИЙ НА ЗВУКОИЗЛУЧАТЕЛЬ:                                           //
// Воспроизведение мелодий показано в примерах Functions и Melody.                     //
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
//  Проверяем наличие модулей Metro на шине I2C:                                       //
    while(sum==0){;}                                                                   // Если модули Metro не обнаружены, то входим в бесконечный цикл while, тогда цикл loop выполняться не будет.
    while(Metro[0].model!=MOD_BUZ){;}                                                  // Если ближайший к Arduino модуль Metro не является звукоизлучателем, то так же входим в бесконечный цикл while.
}                                                                                      //
                                                                                       //
void loop(){                                                                           //
//  Включаем сигнал «провал» (неудача, ошибка):                                        //
    Metro[0].on( MELODY(1) );                                                          // Зуммер однократно воспроизведёт мелодию № 1 длительностью 400 мс.
    delay(1000);                                                                       // Ждём 1 секунду.
//  Включаем сигнал «внимание»:                                                        //
    Metro[0].on( MELODY(2) );                                                          // Зуммер однократно воспроизведёт мелодию № 2 длительностью 500 мс.
    delay(1000);                                                                       // Ждём 1 секунду.
//  Включаем сигнал «успех»:                                                           //
    Metro[0].on( MELODY(3) );                                                          // Зуммер однократно воспроизведёт мелодию № 3 длительностью 400 мс.
    delay(1000);                                                                       // Ждём 1 секунду.
//  Воспроизводим фрагмент мелодии «Star Wars»:                                        //
    Metro[0].on( MELODY(4) );                                                          // Зуммер однократно воспроизведёт мелодию № 4 длительностью 10 сек.
    delay(11000);                                                                      // Ждём 11 секунд.
//  Воспроизводим фрагмент мелодии «Simpsons»:                                         //
    Metro[0].on( MELODY(7) );                                                          // Зуммер однократно воспроизведёт мелодию № 7.
    while( Metro[0].read() ){};                                                        // Ждём завершения воспроизведения мелодии. Время ожидания зависит от длительности мелодии.
    delay(1000);                                                                       // Ждём 1 секунду.
//  Воспроизводим 5 тональных сигналов:                                                //
    Metro[0].on(3000,200,100,5);                                                       // Воспроизводим звуковые сигналы с частотой звука 3000 Гц, длительность сигналов 200 мс, паузы между сигналами 100 мс и количество сигналов = 5.
    delay(2000);                                                                       // Ждём 2 секунды, так как воспроизведение сигналов займёт время (200мс+100мс)*5 = 1500 мс.
}                                                                                      //