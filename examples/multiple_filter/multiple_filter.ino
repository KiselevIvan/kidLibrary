#include <kid_Signal.h>

// Создание объекта Signal
Signal signal;

void setup() {
  Serial.begin(9600);

  // Запись уровня фона с аналогового пина A0
  Serial.println("Запись уровня фона...");
  signal.bgRemove.record(A0, 10, 50); // 10 выборок с задержкой 50 мс
  Serial.print("Уровень фона установлен на: ");
  Serial.println(signal.bgRemove.getLevel());

  // Настройка остальных фильтров
  signal.smooth.setSmooth(0.5);      // Установка коэффициента сглаживания
  signal.amp.setGain(2.0);           // Установка коэффициента усиления
  signal.amp.setMin(0);              // Установка минимального значения усилителя
  signal.amp.setMax(1023);           // Установка максимального значения усилителя

  Serial.println("Настройка фильтров завершена!");
}

void loop() {
  // Считывание значения с аналогового пина
  int rawValue = analogRead(A0);

  // Применение фильтров
  float bgRemovedValue = signal.bgRemove.apply(rawValue);
  float smoothedValue = signal.smooth.apply(bgRemovedValue);
  float baseZeroValue = signal.baseZero.apply(smoothedValue);
  float amplifiedValue = signal.amp.apply(baseZeroValue);

  // Вывод данных в Serial Monitor
  Serial.print("Сырой сигнал: ");
  Serial.print(rawValue);
  Serial.print(", Удаление фона: ");
  Serial.print(bgRemovedValue);
  Serial.print(", Сглаживание: ");
  Serial.print(smoothedValue);
  Serial.print(", baseZero фильтр: ");
  Serial.print(baseZeroValue);
  Serial.print(", Усиление: ");
  Serial.println(amplifiedValue);

  delay(100); // Задержка для удобства чтения данных
}
