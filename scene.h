#ifndef SCENE_H
#define SCENE_H

#include <vector>
#include <array>
#include <thread>
#include <atomic>
#include <iostream>
#include <sstream>
#include <chrono>
#include <cmath>

#include "color.h"
#include "vector3d.h"
#include "shape.h"
#include "image.h"
#include "lightsource.h"

class Scene{
public:
	Scene();

	void add_shapes(const std::vector<Shape*>&);
	void add_shapes(const Shape&);
	void add_light_source(const std::vector<LightSource*>&);
	void add_light_source(const LightSource* const);

	void set_canvas(const uint16_t w, const uint16_t h);
	void set_camera(const Vector3d&);
	void set_background(const Color&);
	void set_ambient_lighting(const double&);
	void set_distance(const double&);
	void set_anti_aliasing(const double&);
	void set_near_borfder(const double&);
	void set_far_border(const double&);
	void set_num_fuzzy_rays(uint32_t);
	void set_depth(uint8_t);	
	void set_rest_at_default();
	void display_progress(bool b);

	void Scene::render();
	const Image& get_visualisation() const;

	~Scene();

private:
	std::vector<Shape*> shapes;
	std::vector<LightSource*> light_sources;
	Image* canvas;            //Холст
	Vector3d camera;          //Позиция камеры
	Color background;         //Цвет фона
	double ambient_lighting;  //Интенсивность окружающего света
	double distance;          //Расстояние от камеры до холста
	double anti_aliasing;     //Коэффициент антиалиасинга.
	double near_borfder;      //Ближняя граница сцены
	double far_border;        //Дальняя граница сцены
	uint32_t num_fuzzy_rays;  //Количество лучей для нечётких отражений
	uint8_t depth;            //глубина отражений (сколько раз отражаем луч)
	
	std::array<bool, 10> is_set;   //Было ли установлено соответствующее поле
	bool displ_progress{ false };  //Отображать ли прогресс рендера
	std::atomic<uint16_t> *unit_progress;
	uint32_t num_thread;

	void main_render(uint16_t num_thread, uint16_t thread_id);
	const Color Scene::recursive_color_in_point(const Vector3d& camera, const Vector3d& ray, uint32_t depth);
	const double illumination_pixel(const Vector3d& point, const Vector3d& normal, const Vector3d& ray, const Shape* const closest) const;
	const Shape* const find_intersection(double& res, const Vector3d& camera, const Vector3d& ray) const;
	void display_render_progress(uint16_t) const;
};

#endif