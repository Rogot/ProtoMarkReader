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

//public:
//	class GradientDrawingArea : public Gtk::DrawingArea {
//	private:
//		int height, width;
//		double tx = 0, ty = 0, scale = 1;
//		std::vector<PointG> points;
//	public:
//		GradientDrawingArea(BaseObjectType *cobject,
//				const Glib::RefPtr<Gtk::Builder> &builder) :
//				Gtk::DrawingArea(cobject) {
//			for (int i = 0; i < 100; i++) {
//				PointG p;
//				p.x = i;
//				p.y = 0;
//				p.col.r = 0;
//				p.col.g = 0;
//				p.col.b = 0;
//				points.push_back(p);
//			}
//		}
//
//		void print_arc(const Cairo::RefPtr<Cairo::Context> &cr, int i);
//
//	private:
//		virtual bool on_draw(const Cairo::RefPtr<Cairo::Context> &cr);
//	};
public:
	std::vector<GradPoint> gradient;

public:
	Gradient();
	Gradient(const Gradient &other);
	virtual ~Gradient();

	void add_grad(double n1, double n2, Color col1, Color col2);
	Color interpolate(double pos);
};

#endif // !GRADIENT_H
