#include "c_usb.h"

void signal_handler_IO(int status) {
	IO_flag = true;
}

int CUsb::open_usb(const char *port_name, const char *file_name) {

	std::cout << "COM PORT NAME: " << port_name << std::endl;
	std::cout << "STORAGE FILE: " << file_name << std::endl;

	printf("\n +----------------------------------+");
	printf("\n |        Serial Port Read          |");
	printf("\n +----------------------------------+");

	fd = open(port_name, O_RDWR); /* ttyACMx is the FT232 based USB2SERIAL Converter   */
	/* O_RDWR   - Read/Write access to serial port       */
	/* Open in blocking mode,read will wait              */

	if (fd < 0) {
		printf("\n Error! in Opening %s \n", port_name);
		return 0;
	}

	fcntl(fd, F_SETOWN, getpid());
	fcntl(fd, F_SETFL, O_ASYNC | O_NDELAY);

	printf("\n Port %s Opened Successfully \n", port_name);

	/* --------- Setting the Attributes of the serial port using termios structure ---------*/

	saio.sa_handler = signal_handler_IO;
	saio.sa_flags = 0;
	saio.sa_restorer = NULL;
	sigaction(SIGIO, &saio, NULL);

	if (tcgetattr(fd, &SerialPortSettings) == 0) {
		cfmakeraw(&SerialPortSettings);
		int state = tcsetattr(fd, TCSANOW, &SerialPortSettings);

		if (state != 0) /* Set the attributes to the termios structure */
		{
			printf("\n ERROR! in Setting attributes \n");
			return 0;
		}
	}

	m_usb_open = true;

	if (m_usb_thread == NULL) {
		m_usb_thread = new std::thread(&CUsb::read_data_usb_buf, this);
	}
	return 1;
}

void CUsb::read_data_usb_buf() {
	//	out.open(file_name, std::ios::binary|std::ios::app);
	std::cout << "USB thread work\t ID:" << std::this_thread::get_id()
			<< std::endl;

	while (m_usb_open) {

		if (IO_flag) {
			IO_flag = false;
			std::cout << "\nREAD IS STARTED\n" << std::endl;
			int j = 0;
			m_usb_mutex.lock();
			do {
				data_rxd = 0;
				bytes_read = read(fd, &(union_var.read_buffer_ch[bytes_left]),
						1800);

				if (bytes_read > 0) {
					data_rxd = 1;
				}

				if (data_rxd) {
					for (int i = 0;
							i
									< (int) ((bytes_read + bytes_left)
											/ COORD_BYTES_NUM); i++) {
						data_buffer.push_back(union_var.read_buffer_f[i]);
					}
					bytes_left = (bytes_read + bytes_left) % COORD_BYTES_NUM;

					arr_idx_current += bytes_read;
					total_read += bytes_read;

					if (bytes_read % COORD_BYTES_NUM != 0) {
						int j = 0;
						for (int i = (int) (bytes_read / COORD_BYTES_NUM)
								* COORD_BYTES_NUM;
								i
										< (int) (bytes_read / COORD_BYTES_NUM)
												* COORD_BYTES_NUM + bytes_left;
								i++) {
							union_var.read_buffer_ch[j] =
									union_var.read_buffer_ch[i];
							j++;
						}
					}
				}
			} while (data_rxd);

			arr_idx_current = 0;

//		if (data_buffer.size() > 0) {
//
//			for (int i = 0; i < data_buffer.size(); i++) {
//				if (union_var.read_buffer_f[i].x != 0
//						|| union_var.read_buffer_f[i].y != 0) {
//					out << data_buffer[i].x << ";" << data_buffer[i].y << ";"
//							<< data_buffer[i].dutyCycle << ";\n";
//					//<< cur_coord[2] << ";\n";
//				}
//			}
//
//			data_buffer.clear();
////			out.close();
//		}
			m_usb_mutex.unlock();
			std::cout << "\nREAD IS END\n" << std::endl;
		}
	}
}

int CUsb::close_usb(){
	m_usb_open = false;
	return close(fd);
}

char* CUsb::get_file_name(){
	return m_file_name;
}

char* CUsb::get_port_name(){
	return m_port_name;
}

void CUsb::set_file_name(char* file_name){
	m_file_name = file_name;
}

void CUsb::set_file_name(std::string& file_name){
	m_file_name = const_cast<char*>(file_name.c_str());
}

void CUsb::set_port_name(char* port_name){
	m_port_name = port_name;
}

void CUsb::set_port_name(std::string& port_name){
	m_port_name = const_cast<char*>(port_name.c_str());
}
