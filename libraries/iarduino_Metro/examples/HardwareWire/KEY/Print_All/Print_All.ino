// ПРИМЕР ВЫВОДА ВСЕХ СОБЫТИЙ И СОСТОЯНИЙ ДЛЯ ОДНОЙ КНОПОКИ:                                         //
                                                                                                     //
// (2RM) Реле электромеханическое на 2 канала (Metro-модуль):                                        //
// (8X8) LED Матрица 8x8 - I2C (Metro-модуль):                                                       // https://iarduino.ru/shop/Expansion-payments/led-matrica-8x8---i2c-metro-modul.html
// (BUZ) Зуммер - I2C (Metro-модуль):                                                                // https://iarduino.ru/shop/Expansion-payments/zummer---i2c-metro-modul.html
// (DHT) Датчик температуры и влажности - I2C (Metro-модуль):                                        // https://iarduino.ru/shop/Sensory-Datchiki/metro_temperature_humidity.html
// (DSL) Датчик освещенности, люксметр - I2C (Metro-модуль):                                         // https://iarduino.ru/shop/Sensory-Datchiki/metro_luxmeter.html
// (EXT) Расширитель выводов - I2C (Metro-модуль):                                                   // https://iarduino.ru/shop/Expansion-payments/metro-expander.html
// (KEY) Кнопка - I2C (Metro-модуль):                                                                // https://iarduino.ru/shop/Expansion-payments/knopka---i2c-metro-modul.html
// (RES) Потенциометр - I2C (Metro-модуль):                                                          //
// (RGB) RGB светодиод - I2C (Metro-модуль):                                                         // https://iarduino.ru/shop/Expansion-payments/rgb-svetodiod---i2c-metro-modul.html
// Плата управления Metro Leonardo:                                                                  // https://iarduino.ru/shop/boards/metro-leonardo.html
// Плата управления Metro ESP:                                                                       // https://iarduino.ru/shop/boards/metro-esp.html
// Информация о подключении модулей к шине I2C:                                                      // https://wiki.iarduino.ru/page/i2c_connection/
                                                                                                     //
#include <Wire.h>                                                                                    // Подключаем библиотеку для работы с аппаратной шиной I2C, до подключения библиотеки iarduino_Metro.
#include <iarduino_Metro.h>                                                                          // Подключаем библиотеку.
                                                                                                     //
uint8_t sum;                                                                                         // Объявляем переменную для хранения количества найденных модулей Metro.
                                                                                                     //
void setup(){                                                                                        //
//  Определяем все модули на шине I2C и сохраняем их количество в переменную sum:                    //
    sum=iarduino_Metro_Start(&Wire); // &Wire1, &Wire2 ...                                           // Функция принимает два необязательных параметра: ссылка на объект шины I2C (по умолчанию &Wire), номер адресного вывода (по умолчанию 12).
//  Теперь нам доступен массив Metro содержащий sum элементов для управления модулями.               // Metro[0] управляет ближайшим к Arduino модулем, Metro[1] управляет следующим модулем и т.д до последнего модуля Metro[sum-1].
                                                                                                     //
//  Готовимся выводить информацию в монитор последовательного порта:                                 //
    Serial.begin(9600);                                                                              // Инициируем передачу данных по шине UART на скорости 9600 бод.
    while( !Serial );                                                                                // Ждём завершения инициализации шины UART.
                                                                                                     //
//  Проверяем наличие модулей Metro на шине I2C:                                                     //
    if( sum==0                  ){ Serial.println("На шине I2C нет модулей Metro."    ); while(1); } // Если модули Metro не обнаружены, то информируем о ошибке и входим в бесконечный цикл while, тогда цикл loop выполняться не будет.
    if( Metro[0].model!=MOD_KEY ){ Serial.println("Первым модулем должна быть кнопка."); while(1); } // Если ближайший к Arduino модуль Metro не является кнопкой, то так же входим в бесконечный цикл while.
                                   Serial.println("Кнопка обнаружена.");                             // Если условия предыдущих строк не выполнены, значит молудь Metro кнопка - успешно обнаружен.
                                                                                                     //
//  Задаём время удержания кнопки:                                                                   //
    Metro[0].set(9000,6000,3000);                                                                    // Функция set задаёт время удержания кнопки для срабатывания функции read с параметрами KEY_HOLD1, KEY_HOLD2 и KEY_HOLD3.
}                                                                                                    // Можно указывать время от 0 до 25500мс с шагом 100мс.
                                                                                                     //
void loop(){                                                                                         //
//  Выводим информацию о состоянии кнопки:                                                           //
    delay(500);                                                                                      //
    int i = Metro[0].read(KEY_TIME);                                                                 // Функция read с параметром KEY_TIME     возвращает время (в мс) которое показывает как долго кнопка нажата или отпущена.
    int j = Metro[0].read(KEY_TRIGGER);                                                              // Функция read с параметром KEY_TRIGGER  меняет своё состояние true/false при каждом новом нажатии на кнопку.
    if( Metro[0].read(KEY_CHANGED)  ){ Serial.print("Состояние кнопки изменилось!\r\n");      }      // Функция read с параметром KEY_CHANGED  возвращает true если состояние кнопки изменилось (бала нажата, стала отпущена и наоборот).
                                       Serial.print("Кнопка ");                                      //
    if( Metro[0].read(KEY_PUSHED)   ){ Serial.print((String)"нажимается, триггер="+j);        }else  // Функция read с параметром KEY_PUSHED   возвращает true если кнопка нажимается  (была отпущена, стала нажата).
    if( Metro[0].read(KEY_RELEASED) ){ Serial.print("отпускается");                           }else  // Функция read с параметром KEY_RELEASED возвращает true если кнопка отпускается (была нажата, стала отпущена).
    if( Metro[0].read(KEY_PRESSED)  ){ Serial.print((String)"нажата в течении "  +i+" мс");   }else  // Функция read с параметром KEY_PRESSED  возвращает true если кнопка нажата и false если отпущена.
                                     { Serial.print((String)"отпущена в течении "+i+" мс");   }      // 
    if( Metro[0].read(KEY_HOLD1)    ){ Serial.print(", это дольше чем заданное время HOLD1"); }else  // Функция read с параметром KEY_HOLD1    возвращает true если кнопка удерживается дольше времени HOLD1=9с, как было указано в Metro[0].set().
    if( Metro[0].read(KEY_HOLD2)    ){ Serial.print(", это дольше чем заданное время HOLD2"); }else  // Функция read с параметром KEY_HOLD2    возвращает true если кнопка удерживается дольше времени HOLD1=6с, как было указано в Metro[0].set().
    if( Metro[0].read(KEY_HOLD3)    ){ Serial.print(", это дольше чем заданное время HOLD3"); }      // Функция read с параметром KEY_HOLD3    возвращает true если кнопка удерживается дольше времени HOLD1=3с, как было указано в Metro[0].set().
                                       Serial.println(".");                                          //
    if( millis()%20000<=501 ){                                                                       // Условие выполняется 1 раз 20 секунд
      Serial.println((String)"За последние 20с, кнопка нажималась "+Metro[0].read(KEY_SUM)+" раз."); // Функция read с параметром KEY_SUM      возвращает количество совершённых нажатий после последнего чтения этого количества.
    }                                                                                                //
}                                                                                                    //
