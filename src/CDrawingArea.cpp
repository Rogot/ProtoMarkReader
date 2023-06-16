#include "PMRMainWindow.h"
#include <cmath>

double CDrawingArea::calc_(double x1, double y1, double x2, double y2) {
	return sqrt(pow(abs(x1-x2), 2) + pow(abs(y1-y2), 2));
}

void CDrawingArea::print_arc(const Cairo::RefPtr<Cairo::Context> &cr, std::vector<Point>& points, int i) {
	cr->move_to(points[i].x, double(height / scale) - points[i].y);

	cr->set_source_rgb(points[i].r, points[i].g, points[i].b);

//	if (2 / scale >= 5) {
//		cr->arc(points[i].x, double(height / scale) - points[i].y, 2 / scale, 0,
//				2 * 3.14);
//	} else {
		cr->arc(points[i].x, double(height / scale) - points[i].y, 5 / scale, 0,
				2 * 3.14);
//	}
	cr->fill();
}

void CDrawingArea::print_line(const Cairo::RefPtr<Cairo::Context> &cr, std::vector<Point>& points, int i) {
	cr->line_to(points[i].x, double(height / scale ) - points[i].y);
	cr->set_source_rgb(points[i].r, points[i].g, points[i].b);
	cr->stroke();
	cr->move_to(points[i].x, double(height / scale ) - points[i].y);
}

bool CDrawingArea::on_draw(const Cairo::RefPtr<Cairo::Context> &cr) {
	std::valarray<double> dashed = { 5.0, 5.0 };
	std::valarray<double> solid = { };
	dashed[0] /= scale;
	dashed[1] /= scale;
	height = CDrawingArea::get_allocated_height();
	width = CDrawingArea::get_allocated_width();
	Glib::RefPtr<Gtk::StyleContext> context = CDrawingArea::get_style_context();
	cr->set_line_join(Cairo::LINE_JOIN_ROUND);
	cr->set_line_width(1 / scale);
	cr->scale(scale, scale);
	cr->translate(tx, ty);
	m_entry_scale->set_text(std::to_string(scale));

	double sx = width / scale;
	double sy = height / scale;
//	printf("Monitor settings, height=%i width=%i\n", height, width);
//	printf("tx=%f ty=%f\n", tx, ty);
//	printf("sx=%f sy=%f\n", sx, sy);
	point_draw = 0;
	int last_draw = 0;
	double dist;

	if (points_drawing.size() > 0) {
		cr->move_to(points_drawing[0].x,
				double(height / scale) - points_drawing[0].y);
		if (scale > 0.7 || scale < 0.26) {
			print_arc(cr, points_drawing, 0);
		}

		if (scale < 0.7) {
			for (size_t i = 1; i < points_drawing.size(); i++) {
				if (scale > 0.15) {
					print_line(cr, points_drawing, i);
//					print_arc(cr, i);
//					last_draw = i;
					point_draw++;
//					if (1 / scale != 1) {
//						i += int(5 / scale);
//					}
				} else {
					print_line(cr, points_drawing, i);
//					print_arc(cr, i);
//					last_draw = i;
					point_draw++;
//					if (1 / scale != 1) {
//						i += int(1 / scale);
//					}
				}
			}
		} else {
			for (size_t i = 1; i < points.size(); i++) {
				/* Отрисовка участка, попадающего в область видимости */
				if ((points[i].x <= sx - tx && points[i].y <= sy + ty)
						&& (points[i].x >= -tx && points[i].y >= ty)) {
					print_arc(cr, points, i);
					last_draw = i;
					point_draw++;
				}
			}
		}
		return 0;
	}
}

bool CDrawingArea::on_scroll_event(GdkEventScroll *event) {
	double x, y, dx, dy;
	int last_draw = 0;
	x = event->x;
	y = event->y;
	if (event->direction == GDK_SCROLL_UP) {
		scale = scale * (1 + SCROLL_SCALE_STEP);
		dx = x * SCROLL_SCALE_STEP;
		dy = -(height - y) * SCROLL_SCALE_STEP;
		tx -= dx / scale;
		ty -= dy / scale;
	} else if (event->direction == GDK_SCROLL_DOWN) {
		//printf("Scrolled DOWN at X=%f Y=%f tx=%f ty=%f\n", X, Y, tx, ty);
		scale = scale / (1 + SCROLL_SCALE_STEP);
		dx = -x * SCROLL_SCALE_STEP / (1 + SCROLL_SCALE_STEP);
		dy = (height - y) * SCROLL_SCALE_STEP / (1 + SCROLL_SCALE_STEP);
		tx -= dx / scale;
		ty -= dy / scale;
	}

	points_drawing.clear();

	points_drawing.push_back(points[0]);

	for (int i = 1; i < points.size(); i++){
		dist = calc_(points[i].x, points[i].y, points[last_draw].x, points[last_draw].y);
		if (dist > 6 / scale) {
			points_drawing.push_back(points[i]);
			last_draw = i;
		}
	}

	queue_draw();
	return 0;
}

bool CDrawingArea::on_button_press(GdkEventButton *event) {
	printf("on_button_press\n");
	if (event->button == GDK_BUTTON_PRIMARY) {
		printf("points drawed =%i\n", point_draw);
//		printf("points size=%i\n", points.size());
//		printf("points_small_scale size=%i\n", points_small_scale.size());
//		Point p;
//		p.x = event->x / scale - tx;
//		p.y = (height - event->y) / scale + ty;
//		points.push_back(p);
//		if(p.x > Xmax) Xmax = p.x;
//		if(p.x < Xmin) Xmin = p.x;
//		if(p.y > Ymax) Ymax = p.y;
//		if(p.y < Ymin) Ymin = p.y;
//		queue_draw();
	}
	if (event->button == GDK_BUTTON_MIDDLE) {
		mbx = event->x;
		mby = event->y;
		mbtx = tx;
		mbty = ty;
		mb_down = 1;
		//printf("Middle button pressed at x=%f y=%f\n", mbx, mby);
	}
	if (event->button == GDK_BUTTON_SECONDARY) {
		FitView();
		queue_draw();
	}
	return 0;
}

bool CDrawingArea::on_button_release(GdkEventButton *event) {
	printf("on_button_release\n");
	if (event->button == GDK_BUTTON_MIDDLE) {
		mb_down = 0;
		//printf("Middle button released at tx=%f ty=%f\n", tx, ty);
	}
	return 0;
}

bool CDrawingArea::on_motion_notify_event(GdkEventMotion *event) {
	m_entry_x->set_text(std::to_string(event->x / scale - tx));
	m_entry_y->set_text(std::to_string((height - event->y) / scale + ty));
	if (mb_down) {
		tx = mbtx + (event->x - mbx) / scale;
		ty = mbty + (event->y - mby) / scale;
		//printf("Mouse motion with middle button down, tx=%f ty=%f\n", tx, ty);
		queue_draw();
	}
	return 0;
}

void CDrawingArea::FitView() {
	if( ((Xmax-Xmin)>0) && ((Ymax-Ymin)>0) ){
		double sx, sy, s, dx, dy;
		tx = ((double) width / scale - (Xmax - Xmin)) / 2;
		ty = -((double) height / scale - (Ymax - Ymin)) / 2;
		sx = (double) width / (Xmax-Xmin) / scale;
		sy = (double) height / (Ymax-Ymin) / scale;
		s = std::min(sx, sy);

		if (s > 1) {
			scale *= s;
			dx = 0.5 * width * (s - 1);
			dy = -0.5 * height * (s - 1);
			tx -= dx / scale + Xmin;
			ty -= dy / scale - Ymin;
		} else {
			scale *= s;
			dx = -0.5 * width * (1 - s);
			dy = 0.5 * height * (1 - s);
			tx -= dx / scale + Xmin;
			ty -= dy / scale - Ymin;
		}
		printf("Xmin=%f\n", Xmin);
		printf("Xmax=%f\n", Xmax);
		printf("Ymin=%f\n", Ymin);
		printf("Ymax=%f\n", Ymax);
		printf("height=%i\n", height);
		printf("width=%i\n", width);
		printf("tx=%f\n", tx);
		printf("ty=%f\n", ty);
		queue_draw();
	}
}

void CDrawingArea::uint16_to_double(const std::vector<CUsb::t_DATA> &src) {
	Point p;

	if (!IO_flag) {
		for (int i = 0; i < src.size(); i++) {
			p.x = (double) src[i].x;
			p.y = (double) src[i].y;
			p.dutyCycle = (double) src[i].dutyCycle / (double) DS_NORM_MAX;

			if (src[i].x != 0 && src[i].y != 0) {
				points.push_back(p);
//				if (abs(p.x - src[i - 1].x) > 1 || abs(p.y - src[i - 1].y) > 1) {
//					points_small_scale.push_back(p);
//				}
				if (p.x > Xmax)
					Xmax = p.x;
				if (p.x < Xmin)
					Xmin = p.x;
				if (p.y > Ymax)
					Ymax = p.y;
				if (p.y < Ymin)
					Ymin = p.y;
				if (p.dutyCycle > DSmax)
					DSmax = p.dutyCycle;
				if (p.dutyCycle < DSmin)
					DSmin = p.dutyCycle;
			}
		}
//		std::sort()
		printf("points size=%i\n", points.size());
		printf("points_small_scale size=%i\n", points_small_scale.size());
		queue_draw();
	}
}

void CDrawingArea::set_rgb(std::vector<Point> &src) {

	Gradient::Color temp_col;

	if (gradient_interpol->gradient.size() == 0) {
		for (int i = 0; i < src.size(); i++) {
			src[i].r = 0.0f;
			src[i].g = 0.0f;
			src[i].b = 0.0f;
		}
	} else {

		for (int i = 0; i < src.size(); i++) {
			temp_col = gradient_interpol->interpolate(src[i].dutyCycle);
			src[i].r = temp_col.r;
			src[i].g = temp_col.g;
			src[i].b = temp_col.b;
		}
	}
}
