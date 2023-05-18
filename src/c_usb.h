#pragma once
#ifndef C_USB_H
#define C_USB_H

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <vector>
#include <fcntl.h>
#include <errno.h>
#include <termios.h>
#include <unistd.h>

#include <fstream>
#include <iostream>

#include <atomic>
#include <thread>
#include <mutex>

//#include <chrono>

#define COORD_NUM       3
#define COORD_BYTES_NUM 6
#define BUF_SIZE        10000

extern std::atomic<bool> IO_flag;
void signal_handler_IO(int status);

class CUsb {
public:

	typedef struct DATA_XYZ {
		uint16_t x;
		uint16_t y;
		//uint16_t z;
		uint16_t dutyCycle;
	} t_DATA;

	std::vector<t_DATA> data_buffer;
	std::mutex m_usb_mutex;

private:

	union {
		char read_buffer_ch[BUF_SIZE];
		t_DATA read_buffer_f[BUF_SIZE];
	} union_var;

	bool m_usb_open;

	int fd; /* File Descriptor */
	char *m_port_name; /* port name entered in space */
	char *m_file_name; /* file name entered in space */

	int bytes_read; /* Number of bytes read */
	int total_bytes;

	int idx_coord; /* Index for storage coordinates in each frame */
	uint16_t prev_coord[COORD_NUM], cur_coord[COORD_NUM]; /* values for find dublicates in
	 dublicate data
	 */
	long int total_read = 0;
	unsigned int arr_idx_current = 0;
	size_t bytes_left = 0;
	uint8_t data_rxd = 0;
	t_DATA prev;
	t_DATA cur;
	std::ofstream out; /* for storage data in file */

	std::thread* m_usb_thread = NULL;

	struct sigaction saio;
	struct termios SerialPortSettings;


public:
	int open_usb(const char *port_name, const char *file_name);
	int close_usb();
	void read_data_usb_buf();

	CUsb(){
		bytes_left = 0;
		data_rxd = 0;
		arr_idx_current = 0;
		bytes_left = 0;
		m_usb_open = false;
		std::thread* m_usb_thread = NULL;
	}

	~CUsb(){
		delete m_usb_thread;
	}

	/* setters */
	void set_file_name(char* file_name);
	void set_file_name(std::string& file_name);

	void set_port_name(char* port_name);
	void set_port_name(std::string& port_name);

	/* getters */
	char* get_file_name();
	char* get_port_name();
};

#endif // !C_USB_H
