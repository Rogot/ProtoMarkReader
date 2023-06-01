#pragma once
#ifndef GRADIENT_H
#define GRADIENT_H

#include <vector>
#include <gtkmm/main.h>
#include <gtkmm.h>
#include <cairomm/cairomm.h>

//#include "PMRMainWindow.h"

class Gradient {
public:

	struct Color {
		double r;
		double g;
		double b;
	};

	struct PointG {
		double x;
		double y;
		Color col;
	};

protected:
	class GradPoint {
		friend class Gradient;
		double _pos;
		Color _col;
	};


public:
	std::vector<GradPoint> gradient;

public:
	Gradient() {

	}

	void add_grad(double n1, double n2, Color col1, Color col2);
	Color interpolate(double pos);
};

#endif // !GRADIENT_H
