#ifndef VECTOR3D_H
#define VECTOR3D_H

#include <iostream>
#include <cmath>

class Vector3d { //вектор или точка в пространстве
public:
	explicit Vector3d(const double& x = 0, const double& y = 0, const double& z = 0);
	Vector3d(const Vector3d& v);

	Vector3d& operator=(const Vector3d&);
	Vector3d& operator+=(const Vector3d&);
	const Vector3d operator+(const Vector3d&) const;
	const Vector3d operator-(const Vector3d&) const;
	const Vector3d operator-() const;
	const Vector3d operator*(const double&) const;
	const double operator*(const Vector3d&) const;
	friend const Vector3d operator*(const double&, const Vector3d&);

	const Vector3d reflect(const Vector3d& normal) const; //отражает луч вдоль нормали
	const double Length() const;
	void normalization();
	
	static const Vector3d rand(); //случайные единичные векторы

	~Vector3d();

private:
	double x;
	double y;
	double z;
};

#endif