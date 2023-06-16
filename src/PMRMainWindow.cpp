#include "PMRMainWindow.h"
//#include "c_usb.h"
//#include "CDrawingArea.h"

PMRMainWindow::PMRMainWindow(BaseObjectType *cobject,
		const Glib::RefPtr<Gtk::Builder> &builder) :
		Gtk::Window(cobject), m_builder(builder) {

	/* Retrieve all widgets */

	m_builder->get_widget_derived("drawing_area", m_drawing_area);
//	m_builder->get_widget_derived("gradient_drawing_area", m_gradient_drawing_area);
	m_builder->get_widget("entry_coord_x", m_drawing_area->m_entry_x);
	m_builder->get_widget("entry_coord_y", m_drawing_area->m_entry_y);
	m_builder->get_widget("entry_scale", m_drawing_area->m_entry_scale);

	/* COM page */
	m_builder->get_widget("button_open_port", m_button_open_port);
	m_builder->get_widget("button_close_port", m_button_close_port);
	m_builder->get_widget("com_select", m_com_select);
	m_builder->get_widget("button_upload_points", m_button_upload_points);
	m_builder->get_widget("button_clear_points", m_button_clear_points);

	/* Display page */
	m_builder->get_widget("entry_min_r", m_entry_min_r);
	m_builder->get_widget("entry_min_g", m_entry_min_g);
	m_builder->get_widget("entry_min_b", m_entry_min_b);

	m_builder->get_widget("entry_max_r", m_entry_max_r);
	m_builder->get_widget("entry_max_g", m_entry_max_g);
	m_builder->get_widget("entry_max_b", m_entry_max_b);

	m_builder->get_widget("entry_min_argument_value", m_entry_min_argument_value);
	m_builder->get_widget("entry_max_argument_value", m_entry_max_argument_value);
	m_builder->get_widget("button_add_gradient_boarders", m_button_add_gradient_boarders);
	m_builder->get_widget("button_clear_gradient_boarders", m_button_clear_gradient_boarders);

	m_c_usb = new CUsb();

	/* Menu */
	m_builder->get_widget("menu_file_save", m_menu_file_save);
	m_builder->get_widget("menu_file_open", m_menu_file_open);

	m_menu_file_save->signal_activate().connect(
			sigc::mem_fun(*this, &PMRMainWindow::OnSaveActivate));
	m_menu_file_open->signal_activate().connect(
				sigc::mem_fun(*this, &PMRMainWindow::OnLoadActivate));


	/* Connect signals. */
	m_button_open_port->signal_clicked().connect(
				sigc::mem_fun(*this, &PMRMainWindow::OnButtonOpenPortClicked));
	m_button_close_port->signal_clicked().connect(
				sigc::mem_fun(*this, &PMRMainWindow::OnButtonClosePortClicked));
	m_button_upload_points->signal_clicked().connect(
				sigc::mem_fun(*this, &PMRMainWindow::UploadPointsClicked));
	m_button_clear_points->signal_clicked().connect(
				sigc::mem_fun(*this, &PMRMainWindow::ClearPointsClicked));
	m_button_add_gradient_boarders->signal_clicked().connect(
				sigc::mem_fun(*this, &PMRMainWindow::OnButtonAddGradientBoardClicked));
	m_button_clear_gradient_boarders->signal_clicked().connect(
			sigc::mem_fun(*this, &PMRMainWindow::OnButtonClearGradientBoardClicked));

	/* Initial values */
	m_com_select->set_text(DEFAULT_COM_PORT_NAME);
	m_entry_min_r->set_text("0.0");
	m_entry_min_g->set_text("0.0");
	m_entry_min_b->set_text("0.0");
	m_entry_max_r->set_text("1.0");
	m_entry_max_g->set_text("0.0");
	m_entry_max_b->set_text("0.0");
	m_entry_min_argument_value->set_text("0.0");
	m_entry_max_argument_value->set_text("1.0");

}

PMRMainWindow::~PMRMainWindow(){
	delete m_c_usb;
}

#include <fstream>
#include <iostream>

void PMRMainWindow::OnSaveActivate(){
	std::ofstream out; /* for storage data in file */
//	out.open("./saveDataPMR_large.txt", std::ofstream::out | std::ofstream::trunc);
	out.open("./saveDataPMR_small.txt", std::ofstream::out | std::ofstream::trunc);
	for (int i = 0; i < m_drawing_area->points.size(); i++){
		out << m_drawing_area->points[i].x << ";"
			<< m_drawing_area->points[i].y << ";"
			<< m_drawing_area->points[i].dutyCycle << ";\n";
	}
	out.close();
}

void PMRMainWindow::OnLoadActivate(){
	std::ifstream input; /* for storage data in file */
//	input.open("./saveDataPMR_large.txt");
	input.open("./saveDataPMR_small.txt");

	if (input.is_open()) {

		std::cout << "File is open\n";

		std::string line;

		m_drawing_area->points.clear();

		while (getline(input, line)) {

			std::string token;
			Point p;

			std::istringstream iss(line);

			int i = 0;
			while (getline(iss, token, ';')) {
				if (i == 0)
					p.x = atoi(token.c_str());
				else if (i == 1)
					p.y = atoi(token.c_str());
				else if (i == 2) {
					p.dutyCycle = string_to_double(token);
					m_drawing_area->points.push_back(p);
				}
				i++;
			}
		}
		input.close();
	}
}

void PMRMainWindow::OnButtonOpenPortClicked(){
	std::string temp_port_name;

	temp_port_name = m_com_select->get_text();
	m_c_usb->set_port_name(temp_port_name);
	m_c_usb->set_file_name("./saveDataPMR_large.txt");
	int status = m_c_usb->open_usb(m_c_usb->get_port_name(), m_c_usb->get_file_name());

	if (status > 0) {

//		if (m_c_usb->get_file_name() != "") {
//			m_c_usb->out.open(m_c_usb->get_file_name(), std::ofstream::out | std::ofstream::trunc);
//		}
//
//		if (!m_c_usb->out.is_open()) {
//			printf("\n %s couldn't open", m_c_usb->get_file_name());
//		} else if (m_c_usb->out.is_open()) {
//			printf("\n %s is open ", m_c_usb->get_file_name());
//		}
	}

}

void PMRMainWindow::update_points() {
	std::cout << "SET_RGB thread work\t ID:" << std::this_thread::get_id()
			<< std::endl;
	while (1) {
		if (is_add_grad || is_clear_grad) {
			m_gradient_update_mutex.lock();
			m_drawing_area->set_rgb(m_drawing_area->points);
			m_gradient_update_mutex.unlock();
			is_add_grad = false;
			is_clear_grad = false;
		}
	}
}

void PMRMainWindow::OnButtonAddGradientBoardClicked() {

	if (m_gradient_thread == NULL) {
		m_gradient_thread = new std::thread(&PMRMainWindow::update_points,
				this);
	}

	m_drawing_area->gradient_interpol->add_grad(
			string_to_double(m_entry_min_argument_value->get_text()),
			string_to_double(m_entry_max_argument_value->get_text()),
			{ string_to_double(m_entry_min_r->get_text()),
					string_to_double(m_entry_min_g->get_text()),
					string_to_double(m_entry_min_b->get_text()) },
			{ string_to_double(m_entry_max_r->get_text()),
					string_to_double(m_entry_max_g->get_text()),
					string_to_double(m_entry_max_b->get_text())});
	is_add_grad = true;
}

void PMRMainWindow::OnButtonClearGradientBoardClicked(){

	if (m_gradient_thread == NULL) {
		m_gradient_thread = new std::thread(&PMRMainWindow::update_points,
				this);
	}

	m_drawing_area->gradient_interpol->gradient.clear();
	is_clear_grad = true;
}

void PMRMainWindow::OnButtonClosePortClicked(){
	int status;
	std::cout << "\nPort " << m_c_usb->get_port_name() << " is closed";
	status = m_c_usb->close_usb(); /* Close the serial port */
}

void PMRMainWindow::UploadPointsClicked(){
	m_drawing_area->uint16_to_double((*m_c_usb).data_buffer);
}

void PMRMainWindow::ClearPointsClicked(){
	m_drawing_area->points.clear();
}
