// ПРИМЕР ЧТЕНИЯ ЛОГИЧЕСКИЙ И АНАЛОГОВЫХ УРОВНЕЙ ОДНОГО РАСШИРИТЕЛЯ ВЫВОДОВ:           //
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
                                                                                       //
//  Готовимся выводить информацию в монитор последовательного порта:                   //
    Serial.begin(9600);                                                                // Инициируем связь с монитором последовательного порта на скорости 9600 бит/сек.
    while(!Serial){;}                                                                  // Ждём готовность к работе аппаратной шины UART.
}                                                                                      //
                                                                                       //
void loop(){                                                                           //
//  Считываем уровни с выводов модуля:                                                 //
    bool  pin0 = Metro[0].read(0, EXT_DIGITAL);                                        // Считываем логический уровень с 0 вывода.
    bool  pin1 = Metro[0].read(1, EXT_DIGITAL_UP);                                     // Считываем логический уровень с 1 вывода подтянув его к уровню логической  «1».
    bool  pin2 = Metro[0].read(2, EXT_DIGITAL_DN);                                     // Считываем логический уровень с 2 вывода прижав   его к уровню логического «0».
    int   pin3 = Metro[0].read(3, EXT_ANALOG);                                         // Считываем аналоговый уровень с 3 вывода.
    float volt = (float(pin3)/4095.0f)*3.3f;                                           // Переводим считанный аналоговый уровень в напряжение.
//  Выводим считанные уровни:                                                          //
    Serial.println((String)"На 0 выводе уровень лог. '"+pin0+"'.");                    //
    Serial.println((String)"На 1 выводе уровень лог. '"+pin1+"' (вывод подтянут).");   //
    Serial.println((String)"На 2 выводе уровень лог. '"+pin2+"' (вывод прижат).");     //
    Serial.println((String)"На 3 выводе уровень '"+pin3+" из 4095' ("+volt+" В).");    //
    Serial.println("====================================================");            //
    delay(500);                                                                        //
/*                                                                                     //
 *   Metro[0].read( вывод , тип ); - считать уровень с вывода.                         // Функция считывает указанный тип сигнала с указанного вывода.         Вывод автоматически конфигурируется в режим работы на вход.
 *   Metro[0].set( вывод , уровень, тип ); - установть уровень на выводе.              // Функция устанавливает уровень указанного типа на указанном выводе.   Вывод автоматически конфигурируется в режим работы на выход.
 *   Metro[0].frequency( частота ); - установить частоту работы ШИМ для всех выводов.  // Функция устанавливает частоту с которой будет выводиться сигнал ШИМ. Частота ШИМ не влияет на коэффициент заполнения ШИМ.
 */                                                                                    //
}                                                                                      //