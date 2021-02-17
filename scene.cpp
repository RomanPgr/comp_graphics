#include "scene.h"

Scene::Scene() :
canvas{ nullptr },
displ_progress{ false },
num_thread{ std::thread::hardware_concurrency() },
unit_progress{ nullptr } {
	num_thread = (num_thread >= 1 ? num_thread : 1);
	unit_progress = new std::atomic<uint16_t>[num_thread];
	for (auto& t : is_set){
		t = false;
	}

	for (uint32_t i{ 0 }; i < num_thread; ++i){
		unit_progress[i] = 0;
	}
}

void Scene::add_shapes(const std::vector<Shape*>& new_shapes){
	for (const auto& shape : new_shapes){
		shapes.push_back(shape->clone_shape_by_reference());
	}
}

void Scene::add_shapes(const Shape& shape){
	shapes.push_back(shape.clone_shape_by_reference());
}

void Scene::add_light_source(const std::vector<LightSource*>& new_sources){
	for (const auto& source : new_sources){
		light_sources.push_back(source->clone_source_by_reference());
	}
}

void Scene::add_light_source(const LightSource* const source){
	light_sources.push_back(source->clone_source_by_reference());
}

void Scene::set_canvas(const uint16_t w, const uint16_t h){
	delete canvas;
	canvas = new Image(w, h);
	is_set[0] = true;
}

void Scene::set_camera(const Vector3d& cam){
	camera = cam;
	is_set[1] = true;
}

void Scene::set_background(const Color& color){
	background = color;
	is_set[2] = true;
}

void Scene::set_ambient_lighting(const double& d){
	ambient_lighting = d;
	is_set[3] = true;
}

void Scene::set_distance(const double& d){
	distance = d;
	is_set[4] = true;
}

void Scene::set_anti_aliasing(const double& d){
	anti_aliasing = d;
	is_set[5] = true;
}
void Scene::set_near_borfder(const double& d){
	near_borfder = d;
	is_set[6] = true;
}

void Scene::set_far_border(const double& d){
	far_border = d;
	is_set[7] = true;
}

void Scene::set_num_fuzzy_rays(uint32_t n){
	num_fuzzy_rays = n;
	is_set[8] = true;
}

void Scene::set_depth(uint8_t n){
	depth = n;
	is_set[9] = true;
}

void Scene::set_rest_at_default(){
	if (!is_set[0]){
		set_canvas(300, 200);
	}
	if (!is_set[1]){
		set_camera(Vector3d{ 0, 0, 0 });
	}
	if (!is_set[2]){
		set_background(Color{ 0x00, 0x00, 0x00 });
	}
	if (!is_set[3]){
		set_ambient_lighting(0.2);
	}
	if (!is_set[4]){
		set_distance((canvas->getW() + canvas->getH()) / 2.0);
	}
	if (!is_set[5]){
		set_anti_aliasing(0.25);
	}
	if (!is_set[6]){
		set_near_borfder(0.1);
	}
	if (!is_set[7]){
		set_far_border(1000000.0 /*std::numeric_limits<double>::max()*/);
	}
	if (!is_set[8]){
		set_num_fuzzy_rays(3);
	}
	if (!is_set[9]){
		set_depth(3);
	}
	for (auto& b : is_set){
		b = true;
	}
}

void Scene::display_progress(bool b){
	displ_progress = b;
}

Scene::~Scene(){
	for (const auto& shape : shapes){
		delete shape;
	}

	for (const auto& source : light_sources){
		delete source;
	}

	delete canvas;
	//delete[] unit_progress;
	//wtf???
}

const Image& Scene::get_visualisation() const{
	return *canvas;
}











//Вернуть указатель на ближайший объект
const Shape* const Scene::find_intersection(double& res, const Vector3d& camera, const Vector3d& ray) const{
	const Shape* nearest_figure{ nullptr };
	double curr_dist;
	res = std::numeric_limits<double>::max();

	for (const auto& shape : shapes) {
		curr_dist = shape->intersect(camera, ray);
		if ((curr_dist - near_borfder > 0) &&
			(far_border - curr_dist > 0) &&
			(res - curr_dist > 0)) {

			nearest_figure = shape;
			res = curr_dist;
		}
	}

	return nearest_figure;
}

//Расчёт освещённости пикселя
const double Scene::illumination_pixel(
	const Vector3d& point,
	const Vector3d& normal,
	const Vector3d& ray,
	const Shape* const closest) const{

	double n, r, intense = 0;
	double trash;
	Vector3d light_direct;
	const Shape* shadowObj{ nullptr };

	//расчёт освещённости идёт для каждой позиции в сетке, на которую разбит протяжённый источник света
	for (const auto& light : light_sources){
		for (int i = 0; i < light->getGrid(); i++) {
			light_direct = light->get_part(i) - point;
			light_direct.normalization();

			shadowObj = find_intersection(trash, point, light_direct);
			if (shadowObj != nullptr) {
				continue;
			}

			//Расчёт, сколько света падает в точку
			n = normal * light_direct;
			if (n > 0) {
				intense += light->get_intensity_point(i) * n / normal.Length();
			}

			//Расчёт блеска
			if ((closest->getSpecular() != 0)) {
				Vector3d reflection = light_direct.reflect(normal);
				reflection.normalization();
				r = reflection * ray;
				if (r > 0) {
					intense += light->get_intensity_point(i) * pow((r / (ray.Length())), closest->getSpecular());
				}
			}
		}
	}

	return intense;
}

//функция для расчёта цвета в точке
const Color Scene::recursive_color_in_point(
	const Vector3d& camPos,
	const Vector3d& ray,
	const uint32_t depth) {

	double closestT;

	Color color, reflectedColor, totalReflectedColor, totalColor;
	Vector3d refRay;
	const Shape* closest;

	//Ищем ближайший объект
	closest = find_intersection(closestT, camPos, ray);

	//Возвращаем цвет фона
	if (closest == nullptr) {
		return background;
	}

	//Нормаль для работы с отражениями
	const Vector3d onSurf{ camPos + ray * closestT };
	Vector3d normal{ (closest->getNormal(onSurf)) };
	normal.normalization();

	color = closest->getColor() * (illumination_pixel(onSurf, normal, -ray, closest) + ambient_lighting);

	if ((depth == 0) || (closest->getRef() < 0)) {
		return color;
	}

	//Расчитываем цвет отражения
	if (closest->getRef() > 0) {
		for (uint32_t i = 0; i < num_fuzzy_rays; i++) {
			refRay = (-ray).reflect(normal) + Vector3d::rand() * closest->getGloss();

			//Рекурсивный вызов
			reflectedColor = recursive_color_in_point(onSurf, refRay, depth - 1);

			//Среднее значение цвета
			totalReflectedColor += Color((int)(reflectedColor.getR() * 1 / ((double)(num_fuzzy_rays))),
				(int)(reflectedColor.getG() * 1 / ((double)(num_fuzzy_rays))),
				(int)(reflectedColor.getB() * 1 / ((double)(num_fuzzy_rays))));
		}
	}

	color = color * (1 - closest->getRef()) + totalReflectedColor * (closest->getRef());
	color.fix();

	return color;
}

void Scene::render(){
	set_rest_at_default();

	const auto H = canvas->getH();
	const uint32_t current_num_thread = num_thread < H ? num_thread : H;

	uint16_t begin{ 0 };
	uint16_t end{ 0 };
	double diff{ H / static_cast<double>(current_num_thread) };

	std::thread *threads = new std::thread[current_num_thread + 1];
	threads[current_num_thread] = std::thread(&Scene::display_render_progress, this, current_num_thread);

	for (uint16_t i{ 0 }; i < current_num_thread; ++i){
		begin = end;
		end = static_cast<uint16_t>(std::round(diff * (i + 1)));
		threads[i] = std::thread(&Scene::main_render, this, current_num_thread, i);
	}

	threads[current_num_thread].join();
	for (uint32_t i{ 0 }; i < current_num_thread; ++i){
		threads[i].join();
		unit_progress[i] = 0;
	}

	delete[] threads;
}

//Основная функция рендера
void Scene::main_render(uint16_t num_thread, uint16_t thread_id) {
	set_rest_at_default();

	const double sqr_anti_aliasing{ anti_aliasing * anti_aliasing };
	const uint16_t H{ canvas->getH() };
	const uint16_t W{ canvas->getW() };

	uint32_t num_pixel;

	for (uint16_t i = thread_id; i < H; i += num_thread) {
		num_pixel = W * i;
		for (uint16_t j = 0; j < W; ++j) {
			Color color{ 0, 0, 0 };

			//Антиалиасинг
			for (double newY = i - H / 2.0; newY < i - H / 2.0 + 1.0; newY += anti_aliasing) {
				for (double newX = j - W / 2.0; newX < j - W / 2.0 + 1.0; newX += anti_aliasing) {
					Vector3d ray{ newX, -newY, distance };
					ray.normalization();
					color += recursive_color_in_point(camera, ray, depth);
				}
			}

			canvas->setPixel(color * sqr_anti_aliasing, num_pixel++);
		}
		unit_progress[thread_id]++;
	}
}

void Scene::display_render_progress(uint16_t num) const{
	if (!displ_progress){
		return;
	}

	std::stringstream str{ "" };
	uint32_t count{ 2 };
	std::cout << "Rendering: 0%" << std::flush;

	const uint16_t MAX{ canvas->getH() };
	uint16_t sum{ 0 };

	while (count < 4){
		for (uint16_t i{ 0 }; i < num; ++i){
			sum += unit_progress[i];
		}

		{
			str.str("");
			str << 100 * sum / MAX << "%";
			for (uint32_t m = 0; m < count; m++){
				std::cout << "\b";
			}
			count = str.str().length();
			(std::cout << str.str()).flush();
		}
		sum = 0;
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}
	std::cout << std::endl;
	return;
}