#ifndef IMAGE_H
#define IMAGE_H

#include <string>
#include <cstdint>

#include "color.h"

class Image {
public:
	explicit Image(const uint16_t w, const uint16_t h);
	Image(const Image&) = delete;
	Image& operator=(const Image&) = delete;

	const uint16_t getW() const;
	const uint16_t getH() const;

	void setPixel(const Color& color, const int pointer);
	friend void operator<<(const std::string& name, const Image& image);
	
	~Image();

private:
	uint16_t width;
	uint16_t height;
	Color* data;
};

#endif