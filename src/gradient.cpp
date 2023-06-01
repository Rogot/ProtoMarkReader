#include "gradient.h"

Gradient::Color Gradient::interpolate(double pos) {
	int i = 0;
	while (pos > gradient[i]._pos && i < gradient.size()) {
		i++;
	}

	if (i == gradient.size()) {
		return {0,0,0};
	}

	Gradient::Color res_color;
	double numerator = pos - gradient[i - 1]._pos;
	double denominator = gradient[i]._pos - gradient[i - 1]._pos;

	res_color.r = gradient[i - 1]._col.r
			+ (gradient[i]._col.r - gradient[i - 1]._col.r) / (denominator)
					* (numerator);
	res_color.g = gradient[i - 1]._col.g
				+ (gradient[i]._col.g - gradient[i - 1]._col.g) / (denominator)
						* (numerator);
	res_color.b = gradient[i - 1]._col.b
				+ (gradient[i]._col.b - gradient[i - 1]._col.b) / (denominator)
						* (numerator);

	return res_color;
}

/*
 * Args: n1 - first limit,
 *    	 n2 - second limit,
 *    	 col1 - first color,
 *    	 col2 - second color.
 * Task: added gradient limit and from which color to which to switch
 * */

void Gradient::add_grad(double n1, double n2, Color col1,
	Color col2) {
	GradPoint gp;

	gp._pos = n1;
	gp._col = col1;

	gradient.push_back(gp);

	gp._pos = n2;
	gp._col = col2;

	gradient.push_back(gp);
}


