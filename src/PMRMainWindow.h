#pragma once
#ifndef PMRMAINWINDOW_H
#define PMRMAINWINDOW_H

#include <gtkmm/main.h>
#include <gtkmm.h>
#include <cairomm/cairomm.h>
#include <gtkmm/button.h>
#include <gtkmm/window.h>
#include <gtkmm/label.h>
#include <stdlib.h>
#include <signal.h>

#include "CDrawingArea.h"

#define UI "part1.glade"
#define DEFAULT_COM_PORT_NAME "/dev/ttyACM0"


class PMRMainWindow: public Gtk::Window {
	friend void signal_handler_IO(int status);

protected:
	Glib::RefPtr<Gtk::Builder> m_builder;
	Gtk::Entry *m_entry_coord_x, *m_entry_coord_y, *m_entry_coord_z;
	Gtk::Button *m_button_open_port, *m_button_close_port,
				*m_button_upload_points, *m_button_clear_points;
	CDrawingArea *m_drawing_area;
	CUsb *m_c_usb;
	Gtk::ImageMenuItem *m_menu_file_open;
	Gtk::MenuBar *mbar;
	Gtk::Entry *m_com_select;

public:
	//Signal handlers:
	void OnButtonOpenPortClicked();
	void OnButtonClosePortClicked();
	void OpenFileActivate();
	void UploadPointsClicked();
	void ClearPointsClicked();

public:
	PMRMainWindow(BaseObjectType *cobject,
				const Glib::RefPtr<Gtk::Builder> &builder);
	/** "quit" action handler. */
	void OnQuit() {
		hide();
	}
	~PMRMainWindow();

//	void uint16_to_double(std::vector<CUsb::t_DATA>& src, std::vector<Point>& dst);
};

#endif // PMRMAINWINDOW_H
