#include <iostream>
#include <fstream>

#include "color.h"
#include "vector3d.h"
#include "image.h"
#include "shape.h"
#include "lightsource.h"
#include "timer.h"
#include "scene.h"

int main()
{
	std::vector<LightSource*> s;
	std::vector<Shape*> v;

	s.push_back(new LineSource{ Vector3d{ +0550.0, +500.0, 1200.0 }, Vector3d{ +700.0, 0500.0, 1200.0 }, 0.9, 25 });
	s.push_back(new LineSource{ Vector3d{ -1000.0, 1000.0, 0010.0 }, Vector3d{ -900.0, 1000.0, 0010.0 }, 0.6, 25 });

	v.push_back(new Ball{ Color{ 0x46, 0x46, 0xC8 }, 1000, 0.5, 0.1, 250.0, Vector3d{ -800.0, -150.0, 2000.0 } });
	v.push_back(new Ball{ Color{ 0x0F, 0x50, 0xAA }, 0010, 0.0, 0.0, 070.0, Vector3d{ +100.0, +100.0, 1625.0 } });
	v.push_back(new Ball{ Color{ 0x8C, 0xD2, 0xF0 }, 1000, 0.1, 0.0, 150.0, Vector3d{ +300.0, +300.0, 1600.0 } });
	v.push_back(new Ball{ Color{ 0x00, 0x8C, 0x00 }, 1000, 0.0, 0.0, 150.0, Vector3d{ -500.0, +300.0, 2300.0 } });
	v.push_back(new Ball{ Color{ 0x8C, 0xC8, 0xF0 }, 1000, 0.9, 0.0, 150.0, Vector3d{ +500.0, -250.0, 1700.0 } });

	v.push_back(new Ball{ Color{ 0xC0, 0x00, 0x00 }, 20, 0.0, 0.0, 100.0, Vector3d{ -600, 350, 1500 } });
	v.push_back(new Ball{ Color{ 0x00, 0x00, 0xC0 }, 20, 0.0, 0.0, 100.0, Vector3d{ -500, 350, 1500 } });
	v.push_back(new Ball{ Color{ 0xC0, 0x00, 0x00 }, 20, 0.0, 0.0, 100.0, Vector3d{ -400, 350, 1500 } });
	v.push_back(new Ball{ Color{ 0x00, 0x00, 0xC0 }, 20, 0.0, 0.0, 100.0, Vector3d{ -300, 350, 1500 } });
	v.push_back(new Ball{ Color{ 0xC0, 0x00, 0x00 }, 20, 0.0, 0.0, 100.0, Vector3d{ -200, 350, 1500 } });

	v.push_back(new Plane{ Color{ 0xFF, 0xFF, 0xFF }, 20, 0.0, 0.0, 400.0, Vector3d{ 0.0, 1.0, 0.0 } });


	Scene sc;
	sc.add_light_source(s);
	sc.add_shapes(v);
	sc.set_canvas(300, 200);
	//sc.set_depth(0);
	//sc.set_ambient_lighting(0.34);
	sc.set_background(Color{ 0x80, 0x80, 0x80 });
	sc.display_progress(true);

	Timer time_class_render;
	sc.render();

	std::cout << "Render time: " << time_class_render.elapsed() << '\n';
	"q.bmp" << sc.get_visualisation();

	{
		for (const auto& i : s){
			delete i;
		}

		for (const auto& i : v){
			delete i;
		}
	}

	std::cin.get();

	return 0;
}