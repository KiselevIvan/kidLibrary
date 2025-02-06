#include <Arduino.h>
#ifndef SIGNAL_H
#define SIGNAL_H

// Класс Signal предоставляет различные методы для обработки сигналов.
class Signal {
  public:
    // Вложенный класс для обработки скользящего окна значений
    class Window {
      public:
        // Конструктор с заданным размером окна (по умолчанию 10)
        Window(unsigned int _size = 10);
        
        // Устанавливает новый размер окна
        void setSize(unsigned int _size);
        
        // Очищает все данные окна
        void clear();
        
        // Добавляет новое значение в окно и сдвигает старые значения
        void stepMove(int _val);
        
        // Вычисляет минимальное и максимальное значения в окне
        void calculate();
        
        // Получение размера окна
        unsigned int getSize() const;
        
        // Получение текущего количества элементов в окне
        unsigned int getCount() const;
        
        // Получение суммы всех значений в окне
        int getSum() const;
        
        // Получение среднего значения в окне
        int getAvg() const;
        
        // Получение минимального значения в окне
        int getMin() const;
        
        // Получение максимального значения в окне
        int getMax() const;
        
      private:
        unsigned int size; // Размер окна
        unsigned int count; // Текущее количество значений в окне
        int sum; // Сумма значений
        int avg; // Среднее значение
        int min; // Минимальное значение
        int max; // Максимальное значение
        int *values; // Массив значений окна
    };

    // Абстрактный класс для фильтров
    class Filter {
      public:
        // Абстрактный метод для применения фильтра
        virtual float apply(float _val) = 0;
    };

    // Фильтр для удаления фона с сигнала
    class BgRemove : public Filter {
      public:
        // Метод для записи уровня фона на основе выборки с пина
        void record(byte _pin, byte _samples = 10, int _timeDelay = 50);
        
        // Применяет удаление фона к значению
        float apply(float _val);
        
        // Получение уровня фона
        int getLevel() const;
        
        // Установка уровня фона
        void setLevel(int _level);
        
      private:
        int level; // Уровень фона
    };

    // Фильтр для сглаживания сигналов
    class Smooth: public Filter {
      public:
        // Конструктор с коэффициентом сглаживания
        Smooth(float _smooth = 0.4);
        
        // Применяет сглаживание к значению
        float apply(float _val) override;
        
        // Устанавливает коэффициент сглаживания
        void setSmooth(float _smooth);
        
        // Получение текущего коэффициента сглаживания
        float getSmooth() const ;
        
      private:
        float smoothed; // Сглаженное значение
        float smooth; // Коэффициент сглаживания
    };

    // Выравнивает сигнал так, чтобы минимум скользящего окна был нулём
    class baseZero: public Filter {
      public:
        // Конструктор с заданным количеством выборок для окна
        baseZero(int _samples = 10);
        
        // Применяет фильтр baseZero
        float apply(float _val) override;
        
        Window window; // Окно для сбора данных
    };

    // Фильтр усиления (амплификации)
    class Amp: public Filter {
      public:
        // Конструктор с заданными параметрами усиления, минимального и максимального значений
        Amp(float _gate = 2, int _min = 0, int _max = 1023);
        
        // Применяет усиление к значению
        float apply(float _val);
        
        // Установка коэффициента усиления
        void setGain(float _gain);
        
        // Получение коэффициента усиления
        float getGain() const;
        
        // Установка минимального значения для амплификации
        void setMin(int);
        
        // Получение минимального значения
        int getMin() const;
        
        // Установка максимального значения для амплификации
        void setMax(int _max);
        
        // Получение максимального значения
        int getMax() const;
        
      private:
        float gain; // Коэффициент усиления
        int min; // Минимальное значение
        int max; // Максимальное значение
    };

    BgRemove bgRemove; // Экземпляр фильтра BgRemove
    Smooth smooth; // Экземпляр фильтра Smooth
    baseZero baseZero; // Экземпляр фильтра baseZero
    Amp amp; // Экземпляр фильтра Amp
};

#endif
