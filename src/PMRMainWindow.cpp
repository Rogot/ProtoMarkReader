#include "PMRMainWindow.h"
//#include "c_usb.h"
//#include "CDrawingArea.h"

PMRMainWindow::PMRMainWindow(BaseObjectType *cobject,
		const Glib::RefPtr<Gtk::Builder> &builder) :
		Gtk::Window(cobject), m_builder(builder) {

	/* Retrieve all widgets */

	m_builder->get_widget_derived("drawing_area", m_drawing_area);
	m_builder->get_widget("entry_coord_x", m_drawing_area->m_entry_x);
	m_builder->get_widget("entry_coord_y", m_drawing_area->m_entry_y);
//		m_builder->get_widget("entry_coord_z", m_entry_coord_z);
	m_builder->get_widget("menu_file_open", m_menu_file_open);
	m_builder->get_widget("button_open_port", m_button_open_port);
	m_builder->get_widget("button_close_port", m_button_close_port);
	m_builder->get_widget("com_select", m_com_select);
	m_builder->get_widget("button_upload_points", m_button_upload_points);
	m_builder->get_widget("button_clear_points", m_button_clear_points);

	m_c_usb = new CUsb();

	/* Connect signals. */
	m_menu_file_open->signal_activate().connect(
			sigc::mem_fun(*this, &PMRMainWindow::OpenFileActivate));
	m_button_open_port->signal_clicked().connect(
				sigc::mem_fun(*this, &PMRMainWindow::OnButtonOpenPortClicked));
	m_button_close_port->signal_clicked().connect(
					sigc::mem_fun(*this, &PMRMainWindow::OnButtonClosePortClicked));
	m_button_upload_points->signal_clicked().connect(
						sigc::mem_fun(*this, &PMRMainWindow::UploadPointsClicked));
	m_button_clear_points->signal_clicked().connect(
							sigc::mem_fun(*this, &PMRMainWindow::ClearPointsClicked));

	/* Initial values */
	m_com_select->set_text(DEFAULT_COM_PORT_NAME);
}

PMRMainWindow::~PMRMainWindow(){
	delete m_c_usb;
}

void PMRMainWindow::OpenFileActivate(){

}

void PMRMainWindow::OnButtonOpenPortClicked(){
	std::string temp_port_name;

	temp_port_name = m_com_select->get_text();
	m_c_usb->set_port_name(temp_port_name);
	m_c_usb->set_file_name("./saveDataPMR.txt");
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
