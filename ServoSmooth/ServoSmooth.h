#pragma once
#include <Arduino.h>
#include <Servo.h>
#include "smoothUtil.h"

/*	
	ServoSmooth - библиотека для плавного управления сервоприводами
	- Является дополнением к стандартной библиотеке Servo
	- Настройка максимальной скорости сервопривода
	- Настройка ускорения (разгон и торможение) сервопривода
	- При использовании ESC и БК мотора получаем "плавный пуск" мотора
	- Устанвока целевой позиции серво по углу (0-180) и длине импульса (500-2400)
	- Автоматическое отключение (detach) при достижении цели
	Читай документацию здесь: https://alexgyver.ru/servosmooth/	
	
	v1.1 - автоматическое отключение (detach) при достижении цели
	v1.2 - вкл/выкл автоотключения серво
	v1.3 - отдельный метод для установки и чтения текущего положения. Добавлен вариант метода attach
	v1.4 - улучшена совместимость
	v1.5 - исправлены getCurrent и getCurrentDeg
	v1.6 - чуть оптимизирована инициализация
	v1.7 - исправлен баг с низкой скоростью/ускорением, код оптимизирован
	v1.8 - улучшена стабильность
	v1.9 - добавлена настройка макс. угла серво
	v1.10 - исправлен баг когда текущая позиция совпадает с позицией таргета
	
	v2.0 - упрощён алгоритм
	v2.1 - добавлена смена направления
	v2.2 - фикс движения в инверсии (спасибо VICLER) и функций write (спасибо CheDima)
	
	v3.0 
	- Добавлен полностью новый, более плавный алгоритм
	- Почищен мусор
	- Добавлена поддержка PCA9685
	- "Плавность" вынесена в базовый класс для упрощения добавления поддержки новых библиотек серво
	
	v3.1 - оптимизирован и облегчён алгоритм, скор. и уск. задаются в градусах/сек
	
	Документация: https://alexgyver.ru/servosmooth/
	2020 by AlexGyver
*/

/*
	// =========== НАСЛЕДУЕМЫЕ МЕТОДЫ ==========
	void write(uint16_t angle);					// повернуть на угол. Аналог метода из библиотеки Servo
	void writeMicroseconds(uint16_t angle);		// повернуть на импульс. Аналог метода из библиотеки Servo
	void attach();	
	void attach(int pin, int target);			// аттач+установка позиции
	void attach(int pin, int min, int max, int target = 0);	// аналог метода из библиотеки Servo. min по умолч. 500, max 2400. target - положение (в углах или мкс, на которые серво повернётся при подключении)
	void start();								// attach + разрешает работу tick
	void stop();								// detach + запрещает работу tick
	
	boolean tick();								// метод, управляющий сервой, должен опрашиваться как можно чаще.
	// Возвращает true, когда целевая позиция достигнута.
	// Имеет встроенный таймер с периодом SS_SERVO_PERIOD
	
	boolean tickManual();						// метод, управляющий сервой, без встроенного таймера.
	// Возвращает true, когда целевая позиция достигнута
	
	void setSpeed(int speed);					// установка максимальной скорости (больше 0), градусов / с
	void setAccel(int accel);					// установка ускорения (1 - 30), условные величины
	void setTarget(int target);					// установка целевой позиции в мкс (~500 - 2400 серво, ~150-600 драйвер PCA9685)
	void setTargetDeg(int target);				// установка целевой позиции в градусах (0-макс. угол). Зависит от min и max
	void setAutoDetach(boolean set);			// вкл/выкл автоматического отключения (detach) при достижении угла. По умолч. вкл
	void setCurrent(int target);				// установка текущей позиции в мкс (~500 - 2400 серво, ~150-600 драйвер PCA9685)
	void setCurrentDeg(int target);				// установка текущей позиции в градусах (0-макс. угол). Зависит от min и max
	void setMaxAngle(int maxAngle);				// установка макс. угла привода
	int getCurrent();							// получение текущей позиции в мкс (~500 - 2400 серво, ~150-600 драйвер PCA9685)
	int getCurrentDeg();						// получение текущей позиции в градусах (0-макс. угол). Зависит от min и max
	int getTarget();							// получение целевой позиции в мкс (~500 - 2400 серво, ~150-600 драйвер PCA9685)
	int getTargetDeg();							// получение целевой позиции в градусах (0-макс. угол). Зависит от min и max
	
	void setDirection(bool _dir);				// смена направления поворота
*/

#define _SERVO_DEADZONE 10			// мёртвая зона (по микросекундам)
#define _SERVO_DEADZONE_SP 3		// мёртвая зона (по скорости)

class ServoSmooth : public Smooth {
public:
	ServoSmooth(int maxAngle = 180);
	using Smooth::attach;
	void attach(int pin);	
	void detach();
	void sendToDriver(uint16_t val);	
	Servo _servo;			
private:
};