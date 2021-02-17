//#define _CRT_SECURE_NO_WARNINGS
#include "image.h"

#pragma pack(push, 1)
struct Head_BMP{
	uint16_t marker_bmp;
	uint32_t size_file;
	uint16_t reserved1;
	uint16_t reserved2;
	uint32_t offset_bits;

	uint32_t size_head;
	uint32_t width;
	uint32_t height;
	uint16_t planes;
	uint16_t bit_count;
	uint32_t compression;
	uint32_t size_image;
	uint32_t x_pels_per_meter;
	uint32_t y_pels_per_meter;
	uint32_t colors_used;
	uint32_t colors_important;

	explicit Head_BMP(uint16_t q1, uint32_t q2, uint16_t q3, uint16_t q4,
		              uint32_t q5, uint32_t q6, uint32_t q7, uint32_t q8,
		              uint16_t q9, uint16_t p1, uint32_t p2, uint32_t p3,
		              uint32_t p4, uint32_t p5, uint32_t p6, uint32_t p7) :
		marker_bmp(q1),       size_file(q2),        reserved1(q3),   reserved2(q4),
		offset_bits(q5),      size_head(q6),        width(q7),       height(q8),
		planes(q9),           bit_count(p1),        compression(p2), size_image(p3),
		x_pels_per_meter(p4), y_pels_per_meter(p5), colors_used(p6), colors_important(p7) {
	}
};
#pragma pack(pop)

Image::Image(const uint16_t w, const uint16_t h) : width{ w }, height{ h }{
	data = new Color[width * height];
}

const uint16_t Image::getW() const{
	return width;
}

const uint16_t Image::getH() const{
	return height;
}

void Image::setPixel(const Color& color, const int pointer) {
	data[pointer] = color;
}

void operator<<(const std::string& name, const Image& image){
	FILE* f;
#ifdef _WIN32
	fopen_s(&f, name.c_str(), "wb");
#else
	f = std::fopen(name.c_str(), "wb");
#endif

	const Head_BMP head(
		static_cast<uint16_t>(0x4D42),
		static_cast<uint32_t>(image.height * image.width * 3 + 54),
		static_cast<uint16_t>(0x0),
		static_cast<uint16_t>(0x0),
		static_cast<uint32_t>(0x36),

		static_cast<uint32_t>(0x28),
		static_cast<uint32_t>(image.width),
		static_cast<uint32_t>(image.height),
		static_cast<uint16_t>(0x1),
		static_cast<uint16_t>(0x18),
		static_cast<uint32_t>(0x0),
		static_cast<uint32_t>(image.height * image.width * 3),
		static_cast<uint32_t>(4000),
		static_cast<uint32_t>(4000),
		static_cast<uint32_t>(0x0),
		static_cast<uint32_t>(0x0));

	std::fwrite(static_cast<const uint8_t*>(static_cast<const void*>(&head)), 1, sizeof(Head_BMP), f);
	uint8_t t;
	for (int i = image.height - 1; i >= 0; --i){
		for (int j = 0; j < image.width; ++j){
			int k = i * image.width + j;
			t = static_cast<uint8_t>(image.data[k].getB());
			std::fwrite(&t, sizeof(t), 1, f);
			t = static_cast<uint8_t>(image.data[k].getG());
			std::fwrite(&t, sizeof(t), 1, f);
			t = static_cast<uint8_t>(image.data[k].getR());
			std::fwrite(&t, sizeof(t), 1, f);
		}
		t = 0;
		for (int j = 0; j < image.width * 3 % 4; ++j){
			std::fwrite(&t, sizeof(t), 1, f);
		}
	}

	std::fclose(f);
}

Image::~Image() {
	delete[] data;
}

//#undef _CRT_SECURE_NO_WARNINGS