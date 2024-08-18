// ПРИМЕР ПЛАВНОГО ПЕРЕЛИВА ВСЕХ ЦВЕТОВ РАДУГИ ПО ВСЕМ ПОДКЛЮЧЁННЫМ СВЕТОДИОДАМ:       //
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
uint8_t j;                                                                             // Объявляем переменную для хранения значения сдвига спектра цветов для всех светодиодов (от 0 до 255).
uint8_t k;                                                                             // Объявляем переменную для хранения положения сдвига спектра цвета для каждого светодиода на спектре j (зависит от количества светодиодов).
uint8_t r, g, b;                                                                       // Объявляем переменные для хранения цветов RGB для каждого светодиода.
uint8_t z=5;                                                                           // Определяем переменную указывающую задержку в мс (чем выше значение, тем медленнее перелив цветов).
                                                                                       //
void setup(){                                                                          //
//  Определяем все модули на шине I2C и сохраняем их количество в переменную sum:      //
    sum=iarduino_Metro_Start(&sWire);                                                  // Функция принимает два необязательных параметра: ссылка на объект шины I2C (по умолчанию &Wire), номер адресного вывода (по умолчанию 12).
//  Теперь нам доступен массив Metro содержащий sum элементов для управления модулями. // Metro[0] управляет ближайшим к Arduino модулем, Metro[1] управляет следующим модулем и т.д до последнего модуля Metro[sum-1].
}                                                                                      //
                                                                                       //
void loop(){                                                                           //
    j++;                                                                               // Смещаем спектр цветов для всех светодиодов.
//  Проходим по всем найденным модулям:                                                //
    for(uint16_t i=0; i<sum; i++){                                                     //
//      Определяем цвет для каждого модуля:                                            //
        k=((uint16_t)(i*256/sum)+j);                                                   // Определяем положение очередного светодиода на смещённом спектре цветов.
        if(k<85) {        b=0; r=k*3; g=255-r;}else                                    // Перелив от зелёного к красному, через жёлтый.
        if(k<170){k-=85;  g=0; b=k*3; r=255-b;}else                                    // Перелив от красного к синему  , через фиолетовый.
                 {k-=170; r=0; g=k*3; b=255-g;}                                        // Перелив от синего   к зелёному, через голубой.
//      Записываем цвет в модуль:                                                      //
        if(Metro[i].model==MOD_RGB){                                                   // Если модуль является светодиодом, то ...
           Metro[i].on( RGB(r,g,b) );                                                  // Включаем цвет r g b.
        }                                                                              //
    }                                                                                  //
    delay(z);                                                                          // Устанавливаем задержку.
}                                                                                      //