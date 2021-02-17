#ifndef LIGHT_SOURCE_H
#define LIGHT_SOURCE_H

#include "vector3d.h"

class LightSource{
public:
	explicit LightSource(const double& intensity, const int grid = 1);
	LightSource(const LightSource& source);

	const int getGrid() const;

	virtual const Vector3d get_part(const int n) const = 0;
	virtual const double get_intensity_point(const int n) const = 0;
	virtual LightSource* const clone_source_by_reference() const = 0;
	
	virtual ~LightSource();

protected:
	double intensity;
	int grid; 
};


class LineSource: public LightSource {
public:
	explicit LineSource(const Vector3d& begin, const Vector3d& end, const double& intensity, const int grid);
	LineSource(const LineSource& source);

	const Vector3d get_part(const int i) const override;
	const double get_intensity_point(const int n) const override;
	LineSource* const clone_source_by_reference() const override;

	~LineSource() override;

protected:
	Vector3d begin;        
	Vector3d end;             
	double intense_point; 
	Vector3d* light_poses;     
};

#endif