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
#include "utils.h"

#define UI "UIProtomarkReader.glade"
#define DEFAULT_COM_PORT_NAME "/dev/ttyACM0"


class PMRMainWindow: public Gtk::Window {
protected:
	std::mutex m_gradient_update_mutex;
	std::thread* m_gradient_thread = NULL;
	friend void signal_handler_IO(int status);

protected:
	Glib::RefPtr<Gtk::Builder> m_builder;
	Gtk::Button *m_button_open_port, *m_button_close_port,
				*m_button_upload_points, *m_button_clear_points;
	CDrawingArea *m_drawing_area;
//	GradientDrawingArea *m_gradient_drawing_area;
	CUsb *m_c_usb;
	Gtk::ImageMenuItem *m_menu_file_open;
	Gtk::ImageMenuItem *m_menu_file_save;
	/* COM page */
	Gtk::MenuBar *mbar;
	Gtk::Entry *m_com_select;

	/* Display page */
	Gtk::Entry *m_entry_min_r, *m_entry_min_g, *m_entry_min_b;
	Gtk::Entry *m_entry_max_r, *m_entry_max_g, *m_entry_max_b;
	Gtk::Entry *m_entry_min_argument_value, *m_entry_max_argument_value;
	Gtk::Button *m_button_add_gradient_boarders;
	Gtk::Button *m_button_clear_gradient_boarders;

	bool is_add_grad = false;
	bool is_clear_grad = false;

public:
	//Signal handlers:
	void OnButtonOpenPortClicked();
	void OnButtonClosePortClicked();
	void UploadPointsClicked();
	void ClearPointsClicked();
	void OnButtonAddGradientBoardClicked();
	void OnButtonClearGradientBoardClicked();

	void OnSaveActivate();
	void OnLoadActivate();

public:
	PMRMainWindow(BaseObjectType *cobject,
				const Glib::RefPtr<Gtk::Builder> &builder);
	/** "quit" action handler. */
	void OnQuit() {
		hide();
	}
	~PMRMainWindow();

public:
	/* Extra thread func */
	void update_points();
	double string_to_double(const std::string &s) {
		std::istringstream i(s);
		double x;
		if (!(i >> x))
			return 0;
		return x;
	}
};

#endif // PMRMAINWINDOW_H
