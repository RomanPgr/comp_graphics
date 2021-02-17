#include "vector3d.h"

Vector3d::Vector3d(const double& x, const double& y, const double& z) : x{ x }, y{ y }, z{ z } {
}

Vector3d::Vector3d(const Vector3d& v) : x{ v.x }, y{ v.y }, z{ v.z }{
}

Vector3d& Vector3d::operator=(const Vector3d& v){
	x = v.x;
	y = v.y;
	z = v.z;
	return *this;
}

void Vector3d::normalization() {
	double len{ this->Length() };
	x /= len;
	y /= len;
	z /= len;
}

const Vector3d Vector3d::operator+(const Vector3d& v) const {
	return Vector3d{ x + v.x, y + v.y, z + v.z };
}

Vector3d& Vector3d::operator+=(const Vector3d& v){
	x += v.x;
	y += v.y;
	z += v.z;
	return *this;
}

const Vector3d Vector3d::operator-(const Vector3d& v) const {
	return Vector3d{ x - v.x, y - v.y, z - v.z };
}

const Vector3d Vector3d::operator-() const {
	return Vector3d{ -x, -y, -z };
}

const Vector3d Vector3d::operator*(const double& d) const {
	return Vector3d{ x * d, y * d, z * d };
}

const double Vector3d::operator*(const Vector3d& v) const{
	return x * v.x + y * v.y + z * v.z;
}

const Vector3d Vector3d::reflect(const Vector3d& normal) const{
	return normal * (2 * (normal * (*this))) - (*this);
}

const Vector3d operator*(const double& d, const Vector3d& v){
	return Vector3d{ v.x * d, v.y * d, v.z * d };
}

const double Vector3d::Length() const{
	return std::sqrt(x * x + y * y + z * z);
}

const Vector3d Vector3d::rand(){
	Vector3d t{ std::rand() / static_cast<double>(RAND_MAX),
		        std::rand() / static_cast<double>(RAND_MAX),
		        std::rand() / static_cast<double>(RAND_MAX) };
	t.normalization();
	return t;
}

Vector3d::~Vector3d() = default;