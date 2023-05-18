#pragma once
#ifndef CDRAWINGAREA_H_
#define CDRAWINGAREA_H_

#include <gtkmm/main.h>
#include <gtkmm.h>
#include <cairomm/cairomm.h>
#include "c_usb.h"

#define NORMAL_LENGTH 20
#define SCROLL_SCALE_STEP 0.25
#define DS_NORM_MAX	65535

//extern std::atomic<bool> IO_flag;

class CUsb;

class PMRMainWindow;

struct Point {
	double x;
	double y;
	double dutyCycle;
};

class CDrawingArea: public Gtk::DrawingArea {

public:

	PMRMainWindow *m_window_main;
	Gtk::Entry *m_entry_x;
	Gtk::Entry *m_entry_y;


	bool mb_down = false;
	int height, width;
	double tx = 0, ty = 0, scale = 1, mbx = 0, mby = 0, mbtx = 0, mbty = 0;
	Gtk::Entry *m_entry_layer;
	double Xmax = std::numeric_limits<double>::lowest(); //job maximum X
	double Ymax = std::numeric_limits<double>::lowest(); //job maximum Y
	double Xmin = std::numeric_limits<double>::max(); //job minimum X
	double Ymin = std::numeric_limits<double>::max(); //job minimum Y

	std::vector<Point> points;

	CDrawingArea(BaseObjectType *cobject, const Glib::RefPtr<Gtk::Builder> &builder) :
			Gtk::DrawingArea(cobject) {
		add_events(Gdk::EventMask::SCROLL_MASK);
		add_events(Gdk::EventMask::BUTTON_PRESS_MASK);
		add_events(Gdk::EventMask::BUTTON_RELEASE_MASK);
		add_events(Gdk::EventMask::POINTER_MOTION_MASK);
		signal_scroll_event().connect(sigc::mem_fun(*this, &CDrawingArea::on_scroll_event));
		signal_motion_notify_event().connect(sigc::mem_fun(*this, &CDrawingArea::on_motion_notify_event));
		signal_button_press_event().connect(sigc::mem_fun(*this, &CDrawingArea::on_button_press));
		signal_button_release_event().connect(sigc::mem_fun(*this, &CDrawingArea::on_button_release));
	}

private:
	virtual bool on_draw(const Cairo::RefPtr<Cairo::Context> &cr);
	virtual bool on_scroll_event(GdkEventScroll *event);
	virtual bool on_button_press(GdkEventButton *event);
	virtual bool on_button_release(GdkEventButton *event);
	virtual bool on_motion_notify_event(GdkEventMotion *event);

public:

	void FitView();

	void uint16_to_double(const std::vector<CUsb::t_DATA>& src);
};

#endif /* CDRAWINGAREA_H_ */
