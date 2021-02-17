#ifndef SHAPES_H
#define SHAPES_H

#include "color.h"
#include "vector3d.h"

class Shape { 
public:
	explicit  Shape(const Color& color, const int specular, const double& reflective, const double& gloss);

	const Color& getColor() const;  //Цвет
	const int getSpecular() const;  //Блики
	const double& getRef() const;   //Отражение
	const double& getGloss() const; //Матовость

	virtual const double intersect(const Vector3d& camPos, const Vector3d& ray) const = 0; //проверка на пересечение с объектом. Возвращает параметр t для уравнения луча, чтобы найти точку пересечения. //camPos - позиция камеры (начальный вектор для луча), ray - направление луча
	virtual const Vector3d getNormal(const Vector3d&) const = 0; //возвращает нормаль к поверхности объекта в точке
	virtual Shape* const clone_shape_by_reference() const = 0; //Возвращает константную ссылку на клон фигуры
	
	virtual ~Shape();

protected:
	Color color;            
	int specular = 0;        
	double reflective = 0;  
	double gloss = 0;        
};

class Ball : public Shape {
public:
	explicit Ball(const Color& color, const int specular, const double& reflective, const double& gloss, const double& radius, const Vector3d& center);
	
	const double& getRadius() const;
	const Vector3d& getCenter() const;

	const Vector3d getNormal(const Vector3d& onSurf) const override;
	const double intersect(const Vector3d& camPos, const Vector3d& ray) const override; //проверка на пересечение луча со сферой - решение квадратного уравнения
	Ball* const clone_shape_by_reference() const override;
	
	~Ball() override;

protected:
	double radius;
	Vector3d center; //центр сферы
};

class Plane : public Shape {
public:
	explicit Plane(const Color& color, const int& specular, const double& reflective, const double& gloss, const double& distance, const Vector3d& normal);
	
	const double& getDistance() const;

	const Vector3d getNormal(const Vector3d& onSurf) const override;
	const double intersect(const Vector3d& camPos, const Vector3d& ray) const override;
	Plane* const clone_shape_by_reference() const override;
	
	~Plane() override;

protected:
	Vector3d normal;      //Нормаль к плоскости
	double distance;      //Расстояние от плоскости до начала координат вдоль положительного направления нормали

};

#endif