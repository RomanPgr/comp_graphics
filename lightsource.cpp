#include "lightsource.h"

LightSource::LightSource(const double& intensity, const int grid) :
	intensity{ intensity },
	grid{ grid }{
}

LightSource::LightSource(const LightSource& source) :
	LightSource{ source.intensity, source.grid }{
}

const int LightSource::getGrid() const{
	return grid;
}

LightSource::~LightSource() = default;







LineSource::LineSource(const Vector3d& begin, const Vector3d& end, const double& intensity, const int grid) :
	LightSource{ intensity, grid },
	begin{ begin },
	end{ end },
	intense_point{ intensity / grid }{
	light_poses = new Vector3d[grid];
	Vector3d step{ (end - begin) * (1.0 / grid) };
	Vector3d t{ begin };
	for (int i = 0; i < grid; i++) {
		light_poses[i] = t;
		t += step;
	}
}

LineSource::LineSource(const LineSource& source) :
	LineSource{source.begin, source.end, source.intensity, source.grid}{

}

const Vector3d LineSource::get_part(const int i) const{
	return light_poses[i];
}

const double LineSource::get_intensity_point(const int n) const{
	return intense_point;
}

LineSource* const LineSource::clone_source_by_reference() const{
	return new LineSource{ *this };
}

LineSource::~LineSource() {
	delete[] light_poses;
}