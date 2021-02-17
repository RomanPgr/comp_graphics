#ifndef COLOR_H
#define COLOR_H

class Color {
public:
	explicit Color(const int R = 0, const int G = 0, const int B = 0);
	Color(const Color& color);

	Color& operator+=(const Color& color);
	const Color operator+(const Color& color) const;
	const Color operator*(const double& d) const;
	void fix();

	const int getR() const;
	const int getG() const;
	const int getB() const;

	void setR(int k);
	void setG(int k);
	void setB(int k);
	
	~Color();

private:
	int R;
	int G;
	int B;
};

#endif