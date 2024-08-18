// ЧТЕНИЕ ЛОГИЧЕСКИХ И АНАЛОГОВЫХ УРОВНЕЙ, УСТАНОВКА ЛОГИЧЕСКИХ УРОВНЕЙ И СИГНАЛОВ ШИМ://
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
                                                                                       //
void setup(){                                                                          //
//  Определяем все модули на шине I2C и сохраняем их количество в переменную sum:      //
    sum=iarduino_Metro_Start(&sWire);                                                  // Функция принимает два необязательных параметра: ссылка на объект шины I2C (по умолчанию &Wire), номер адресного вывода (по умолчанию 12).
//  Теперь нам доступен массив Metro содержащий sum элементов для управления модулями. // Metro[0] управляет ближайшим к Arduino модулем, Metro[1] управляет следующим модулем и т.д до последнего модуля Metro[sum-1].
                                                                                       //
//  Проверяем наличие модулей Metro на шине I2C:                                       //
    while(sum==0){;}                                                                   // Если модули Metro не обнаружены, то входим в бесконечный цикл while, тогда цикл loop выполняться не будет.
    while(Metro[0].model!=MOD_EXT){;}                                                  // Если ближайший к Arduino модуль Metro не является расширителем выводов, то так же входим в бесконечный цикл while.
}                                                                                      //
                                                                                       //
void loop(){                                                                           //
//  Считываем уровни с 0 и 1 вывода модуля:                                            //
    bool pin0=Metro[0].read(0,       EXT_DIGITAL);                                     // Считываем логический уровень с 0 вывода.
    int  pin1=Metro[0].read(1,       EXT_ANALOG );                                     // Считываем аналоговый уровень с 1 вывода.
//  Устанавливаем уровни на 2 и 3 выводе модуля:                                       //
              Metro[0].set (2, pin0, EXT_DIGITAL);                                     // Устанавливаем на 2 выводе логический уровень из переменной pin0.
              Metro[0].set (3, pin1, EXT_ANALOG );                                     // Устанавливаем на 3 выводе аналоговый уровень из переменной pin1 (сигнал ШИМ).
/*                                                                                     //
 *   Metro[0].read( вывод , тип ); - считать уровень с вывода.                         // Функция считывает указанный тип сигнала с указанного вывода.         Вывод автоматически конфигурируется в режим работы на вход.
 *   Metro[0].set( вывод , уровень, тип ); - установть уровень на выводе.              // Функция устанавливает уровень указанного типа на указанном выводе.   Вывод автоматически конфигурируется в режим работы на выход.
 *   Metro[0].frequency( частота ); - установить частоту работы ШИМ для всех выводов.  // Функция устанавливает частоту с которой будет выводиться сигнал ШИМ. Частота ШИМ не влияет на коэффициент заполнения ШИМ.
 *                                                                                     //
 *   Удобство данного примера заключается в том, что диапазон значений считанного      //
 *   аналогового уровня (0...4095) совпадает с диапазоном устанавливаемого сигнала ШИМ.//
 */                                                                                    //
}                                                                                      //