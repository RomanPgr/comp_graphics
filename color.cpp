#include "color.h"

Color::Color(const int R, const int G, const int B) : R{ R }, G{ G }, B{ B }{
}

Color::Color(const Color& color) : R{ color.R }, G{ color.G }, B{ color.B }{
}

Color& Color::operator+=(const Color& color){
	R += color.R;
	G += color.G;
	B += color.B;
	return *this;
}

const Color Color::operator+(const Color& color) const {
	return Color{ R + color.R, G + color.G, B + color.B };
}

const Color Color::operator*(const double& d) const {
	return Color{ static_cast<int>(R * d), static_cast<int>(G * d), static_cast<int>(B * d) };
}

void Color::fix(){
	R = R > 255 ? 255 : R;
	G = G > 255 ? 255 : G;
	B = B > 255 ? 255 : B;
}

const int Color::getR() const{
	return R;
}

const int Color::getG() const{
	return G;
}

const int Color::getB() const{
	return B;
}

void Color::setR(int k){
	R = k;
}

void Color::setG(int k){
	G = k;
}

void Color::setB(int k){
	B = k;
}

Color::~Color() = default;