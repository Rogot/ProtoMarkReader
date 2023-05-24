#include "PMRMainWindow.h"
#include <cmath>

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

	if(points.size()>0){
		//uint16_to_double(data_buffer,points);
		cr->move_to(points[0].x, double(height / scale ) - points[0].y);
		for(size_t i = 1; i < points.size(); i++)
		{
			if (points[i].x != 0 && points[i].y != 0){
				cr->move_to(points[i].x, double(height / scale ) - points[i].y);
//				cr->line_to(points[i].x, double(height / scale ) - points[i].y);
//				if (points[i].dutyCycle < 0.05)
//					cr->set_source_rgb(0, 0, 0);
				if (points[i].dutyCycle <= 0.3)
					cr->set_source_rgb(0, 0, points[i].dutyCycle / 0.3);
				else if (points[i].dutyCycle > 0.3 && points[i].dutyCycle <= 0.6)
					cr->set_source_rgb(0, (points[i].dutyCycle - 0.3) / 0.3,
							1);
				else if (points[i].dutyCycle > 0.6
						&& points[i].dutyCycle <= 0.95)
					cr->set_source_rgb((points[i].dutyCycle - 0.6) / 0.35, 1,
							0.4);
				else if (points[i].dutyCycle > 0.95)
					cr->set_source_rgb(points[i].dutyCycle, 0.5, 0.5);

				if (1 / scale > 1){
					cr->arc(points[i].x, double(height / scale) - points[i].y, 1 / scale,
							0, 2 * 3.14);
				} else {
					cr->arc(points[i].x, double(height / scale) - points[i].y, 1,
												0, 2 * 3.14);
				}
				cr->fill();

				if (1 / scale == 1){
					i += int(points.size() / scale * 100);
				}
//				cr->stroke();
			}
		}
//		cr->stroke();
	}
	return 0;
}

bool CDrawingArea::on_scroll_event(GdkEventScroll *event) {
	double x, y, dx, dy;
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
	queue_draw();
	return 0;
}

bool CDrawingArea::on_button_press(GdkEventButton *event) {
	printf("on_button_press\n");
	if (event->button == GDK_BUTTON_PRIMARY) {
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
//			points.push_back(p);

			if (src[i].x != 0 && src[i].y != 0) {
				points.push_back(p);
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
		queue_draw();
	}

}
