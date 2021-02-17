#include "shape.h"

Shape::Shape(const Color& color, const int specular, const double& reflective, const double& gloss) :
color{ color },
specular{ specular },
reflective{ reflective },
gloss{ gloss }{
}

const Color& Shape::getColor() const{
	return color;
}

const int Shape::getSpecular() const{
	return specular;
}

const double& Shape::getRef() const{
	return reflective;
}

const double& Shape::getGloss() const{
	return gloss;
}

Shape::~Shape() = default;








Ball::Ball(const Color& color, const int specular, const double& reflective, const double& gloss, const double& radius, const Vector3d& center) :
Shape{ color, specular, reflective, gloss },
radius{ radius },
center{ center }{
}

const double& Ball::getRadius() const{
	return radius;
}

const Vector3d& Ball::getCenter() const{
	return center;
}

const Vector3d Ball::getNormal(const Vector3d& point) const {
	return point - center;
}

const double Ball::intersect(const Vector3d& camera, const Vector3d& ray) const{
	Vector3d toCentre{ camera - center };

	double a = ray * ray;
	double b = 2 * (toCentre * ray);
	double с = toCentre * toCentre - radius * radius;
	double discr = b * b - 4 * a * с;

	//Вернуть -1, если не пересекается
	if (discr < 0) {
		return -1;
	}

	//вернуть меньшей корень
	double t = (-b + std::sqrt(discr)) / (2 * a);
	if (t >(-b - std::sqrt(discr)) / (2 * a)) {
		t = (-b - std::sqrt(discr)) / (2 * a);
	}

	return t;
}

Ball* const Ball::clone_shape_by_reference() const{
	return new Ball{ *this };
}

Ball::~Ball() = default;








Plane::Plane(const Color& color, const int& specular, const double& reflective, const double& gloss, const double& distance, const Vector3d& normal) :
Shape{ color, specular, reflective, gloss },
distance{ distance },
normal{ normal }{
}

const double& Plane::getDistance() const{
	return distance;
}

const Vector3d Plane::getNormal(const Vector3d& onSurf) const{
	return normal;
}

const double Plane::intersect(const Vector3d& camPos, const Vector3d& ray) const{
	return -((distance + (camPos * normal)) / (ray * normal));
}

Plane* const Plane::clone_shape_by_reference() const{
	return new Plane{ *this };
}

Plane::~Plane() = default;