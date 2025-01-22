/*
  SignalProcessor - Пример обработки сигналов с микрофона через RC-цепочки
  Автор: [Киселев Иван]
  Версия: 1.0.0
  Дата: [23.01.2025]

  Описание схемы:
  =================
  1. Микрофон подключен к двум RC-цепочкам:
     - ФВЧ (Высокочастотный фильтр):
       Два конденсатора по 10 нФ последовательно-> A0 -> Резистор 10 кОм -> Земля.
       Дополнительно пин A0 подтянут резистором 10 кОм к +3.3 В.
     - ФНЧ (Низкочастотный фильтр):
       Резистор 10 кОм -> A1 -> Два конденсатора по 100 нФ параллельно к земле.

  2. Сигналы с A0 (ФВЧ) и A1 (ФНЧ) проходят обработку:
     - Фильтр AC: удаление постоянной составляющей.
     - Усиление: сигнал усиливается в 3 раза.
     - Сглаживание: для уменьшения шума.

  3. Обработанные сигналы нормализуются в диапазон 8 бит (0–255) и управляют:
     - Светодиодом HP (LED_HP, пин 5) для ФВЧ.
     - Светодиодом LP (LED_LP, пин 6) для ФНЧ.

  Пины:
  -----
  - PIN_RC_HP (A0): Входной сигнал RC-цепочки ФВЧ.
  - PIN_RC_LP (A1): Входной сигнал RC-цепочки ФНЧ.
  - LED_HP (5): ШИМ-управление яркостью светодиода для ФВЧ.
  - LED_LP (6): ШИМ-управление яркостью светодиода для ФНЧ.
*/

#include <kid_Signal.h>

// Определяем пины
#define PIN_RC_HP A0   // Пин для RC-цепочки ФВЧ
#define PIN_RC_LP A1   // Пин для RC-цепочки ФНЧ
#define LED_HP 5       // Пин для ШИМ-светодиода, связанного с сигналом HP
#define LED_LP 6       // Пин для ШИМ-светодиода, связанного с сигналом LP

// Объекты Signal для каждой RC-цепочки
Signal signalHP; // Для RC-цепочки ФВЧ
Signal signalLP; // Для RC-цепочки ФНЧ

void setup() {
  // Инициализация последовательного порта для отладки
  Serial.begin(9600);

  // Настройка фильтров для RC-цепочки ФВЧ
  signalHP.ac = Signal::AC(10);     // Фильтр AC с окном из 10 значений
  signalHP.amp.setGain(3.0);        // Усиление в 3 раза
  signalHP.amp.setMin(0);           // Минимальное значение усиления
  signalHP.amp.setMax(1023);        // Максимальное значение усиления
  signalHP.smooth.setSmooth(0.5);   // Коэффициент сглаживания

  // Настройка фильтров для RC-цепочки ФНЧ
  signalLP.ac = Signal::AC(10);     // Фильтр AC с окном из 10 значений
  signalLP.amp.setGain(3.0);        // Усиление в 3 раза
  signalLP.amp.setMin(0);           // Минимальное значение усиления
  signalLP.amp.setMax(1023);        // Максимальное значение усиления
  signalLP.smooth.setSmooth(0.5);   // Коэффициент сглаживания

  // Настройка пинов для ШИМ-управления светодиодами
  pinMode(LED_HP, OUTPUT);
  pinMode(LED_LP, OUTPUT);
}

void loop() {
  // Считываем сигналы с аналоговых пинов
  int rawValueHP = analogRead(PIN_RC_HP); // Вход с микрофона через ФВЧ
  int rawValueLP = analogRead(PIN_RC_LP); // Вход через ФНЧ

  // Обработка сигнала RC-цепочки ФВЧ
  float acValueHP = signalHP.ac.apply(rawValueHP);        // Фильтр AC
  float amplifiedValueHP = signalHP.amp.apply(acValueHP); // Усиление сигнала
  float smoothedValueHP = signalHP.smooth.apply(amplifiedValueHP); // Сглаживание

  // Обработка сигнала RC-цепочки ФНЧ
  float acValueLP = signalLP.ac.apply(rawValueLP);        // Фильтр AC
  float amplifiedValueLP = signalLP.amp.apply(acValueLP); // Усиление сигнала
  float smoothedValueLP = signalLP.smooth.apply(amplifiedValueLP); // Сглаживание

  // Приводим сигнал к диапазону 8 бит (0–255)
  int pwmValueHP = map(smoothedValueHP, 0, 1023, 0, 255); // RC ФВЧ
  int pwmValueLP = map(smoothedValueLP, 0, 1023, 0, 255); // RC ФНЧ

  // Управляем яркостью светодиодов
  analogWrite(LED_HP, pwmValueHP); // Светодиод HP
  analogWrite(LED_LP, pwmValueLP); // Светодиод LP

  // Вывод данных в Serial Monitor для отладки
  Serial.print("HP (RAW): ");
  Serial.print(rawValueHP);
  Serial.print(", HP (Filtered): ");
  Serial.print(smoothedValueHP);
  Serial.print(", PWM HP: ");
  Serial.println(pwmValueHP);

  Serial.print("LP (RAW): ");
  Serial.print(rawValueLP);
  Serial.print(", LP (Filtered): ");
  Serial.print(smoothedValueLP);
  Serial.print(", PWM LP: ");
  Serial.println(pwmValueLP);

  delay(50); // Небольшая задержка
}
