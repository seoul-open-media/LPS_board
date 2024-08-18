// ПРИМЕР ВЫВОДА ИНФОРМАЦИИ О ОБНАРУЖЕННЫХ МОДУЛЯХ METRO:                              //
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
//  Выводим информацию о найденных модулях:                                            //
                                      Serial.begin  (9600);                            // Инициируем связь с монитором последовательного порта на скорости 9600 бит/сек.
                                      while(!Serial){;}                                // Ждём готовность к работе аппаратной шины UART.
    if(sum==0){                       Serial.println(F("Модули линейки не найдены"));  // Если модули не найдены, то сообщаем об этом.
    }else{                            Serial.print  (F("Найдено модулей: "));          // Иначе, выводим:
                                      Serial.print  (sum);                             // - их количество,
                                      Serial.println(".");                             //
//      Проходим по всем найденным модулям:                                            //
        for(int i=0; i<sum; i++){     Serial.print  (i+1);                             // - номер по порядку,
                                      Serial.print  (F(")\tадрес = "));                //
                                      Serial.print  (Metro[i].address);                // - адрес на шине I2C,
                                      Serial.print  (F(",\tтип - "));                  // 
            switch(Metro[i].model){                                                    // - тип модуля (тип определяется по значению model):
                case MOD_KEY:         Serial.print  (F("кнопка   "));         break;   //
                case MOD_RGB:         Serial.print  (F("светодиод"));         break;   //
                case MOD_RES:         Serial.print  (F("потенциометр"));      break;   //
                case MOD_BUZ:         Serial.print  (F("звукоизлучатель"));   break;   //
                case MOD_DHT:         Serial.print  (F("датчик DHT"));        break;   //
                case MOD_DSL:         Serial.print  (F("датчик света"));      break;   //
                case MOD_EXT:         Serial.print  (F("расширитель"));       break;   //
                case MOD_8X8:         Serial.print  (F("матрица 8х8"));       break;   //
                case MOD_2RM:         Serial.print  (F("реле"));              break;   //
                default:              Serial.print  (F("не определён"));      break;   //
            }                         Serial.print  (F(",\tверсия "));                 // - выводим
                                      Serial.print  (Metro[i].version);                // - версию прошивки
                                      Serial.print  (F(", занимает "));                // - и выводим
                                      Serial.print  (Metro[i].size);                   // - занимаемое место в ОЗУ.
                                      Serial.println(F(" байт ОЗУ."));                 //
        }                                                                              //
    }                                                                                  //
}                                                                                      //
                                                                                       //
void loop(){}                                                                          //