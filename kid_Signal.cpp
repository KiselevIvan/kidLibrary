#include "kid_Signal.h"
#include "limits.h"

// Конструктор для окна. Инициализирует размер окна и очищает его.
Signal::Window::Window(unsigned int _size = 10) {
  size = _size;
  values = new int[size];
  clear();
}

// Устанавливает новый размер окна и очищает его.
void Signal::Window::setSize(unsigned int _size) {
  if (_size == size)
    return;
  delete[] values; // Удаление старых данных
  size = _size;
  values = new int[size];
  clear();
}

// Очищает данные окна: сбрасывает счетчики и устанавливает значения по умолчанию.
void Signal::Window::clear() {
  count = 0;
  sum = 0;
  avg = 0;
  min = INT_MAX;
  max = INT_MIN;
  for (int i = 0; i < size; i++)
    values[i] = 0;
}

// Вычисляет минимальное и максимальное значения в окне.
void Signal::Window::calculate() {
  min = INT_MAX;
  max = INT_MIN;
  for (int i = 0; i < count; i++) {
    if (values[i] < min) min = values[i];
    if (values[i] > max) max = values[i];
  }
}

// Добавляет новое значение в окно и сдвигает старые значения.
void Signal::Window::stepMove(int _val) {
  if (count == size)
  {
    int popit = values[0];
    sum -= popit;
    for (byte i = 0; i < size - 1; i++) {
      values[i] = values[i + 1];
    }
    count--;
    if (popit == max || popit == min)
      calculate();
  }
  sum += _val;
  values[count] = _val;
  count++;
  if (_val < min) min = _val;
  if (_val > max) max = _val;
  avg = sum / count;
}

// Возвращает размер окна.
unsigned int Signal::Window::getSize() const {
  return size;
}

// Возвращает текущее количество элементов в окне.
unsigned int Signal::Window::getCount() const {
  return count;
}

// Возвращает сумму всех значений в окне.
int Signal::Window::getSum() const {
  return sum;
}

// Возвращает среднее значение.
int Signal::Window::getAvg() const {
  return avg;
}

// Возвращает минимальное значение.
int Signal::Window::getMin() const {
  return min;
}

// Возвращает максимальное значение.
int Signal::Window::getMax() const {
  return max;
}

// Записывает уровень фона, используя несколько выборок с пина.
void Signal::BgRemove::record(byte _pin, byte _samples = 10, int _timeDelay = 50) {
  int sum = 0;
  for (byte i = 0; i < _samples; i++) {
    sum += analogRead(_pin);
    delay(_timeDelay); // Задержка между измерениями
  }
  level = (sum / _samples); // Средний уровень фона
}

// Возвращает уровень фона.
int Signal::BgRemove::getLevel() const {
  return level;
}

// Устанавливает уровень фона.
void Signal::BgRemove::setLevel(int _level) {
  if (_level < 1024 && _level >= 0)
    level = _level;
}

// Применяет фильтр для удаления фона из значения.
float Signal::BgRemove::apply(float _val) {
  return _val - level;
}

// Конструктор для сглаживания, устанавливает коэффициент сглаживания.
Signal::Smooth::Smooth(float _smooth = 0.4) {
  smoothed = 0;
  smooth = _smooth;
}

// Применяет фильтр сглаживания к значению.
float Signal::Smooth::apply(float _val) {
  smoothed = (smooth * _val) + ((1 - smooth) * smoothed);
  return smoothed;
}

// Устанавливает новый коэффициент сглаживания.
void Signal::Smooth::setSmooth(float _smooth) {
  if (_smooth < 1 && _smooth > 0)
    smooth = _smooth;
}

// Возвращает текущий коэффициент сглаживания.
float Signal::Smooth::getSmooth() const {
  return smooth;
}

// Конструктор для фильтра AC, инициализирует окно.
Signal::AC::AC(int _samples = 10) {
  window = Window(_samples);
}

// Применяет фильтр AC, вычитая минимальное значение окна.
float Signal::AC::apply(float _val){
  window.stepMove(_val);
  if (window.getMin() > 0)
    return _val - window.getMin();
  else
    return _val;
}

// Конструктор для усилителя, устанавливает начальные параметры.
Signal::Amp::Amp(float _gate = 2, int _min = 0, int _max = 1023) {
  gain = _gate;
  min = _min;
  max = _max;
}

// Применяет усиление к значению с ограничением по минимальному и максимальному значениям.
float Signal::Amp::apply(float _val) {
  int amp = _val * gain;
  if (amp > max) {
    amp = max;
  }
  if (amp < min) {
    amp = min;
  }
  return amp;
}

// Устанавливает коэффициент усиления.
void Signal::Amp::setGain(float _gain) {
  gain = _gain;
}

// Возвращает коэффициент усиления.
float Signal::Amp::getGain() const {
  return gain;
}

// Устанавливает минимальное значение для усилителя.
void Signal::Amp::setMin(int _min) {
  min = _min;
}

// Возвращает минимальное значение.
int Signal::Amp::getMin() const {
  return min;
}

// Устанавливает максимальное значение для усилителя.
void Signal::Amp::setMax(int _max) {
  max = _max;
}

// Возвращает максимальное значение.
int Signal::Amp::getMax() const {
  return max;
}
