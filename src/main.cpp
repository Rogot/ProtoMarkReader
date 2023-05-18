
#include "PMRMainWindow.h"

std::atomic<bool> IO_flag;

int main(int argc, char *argv[]) {

	std::cout << "APP thread work\t ID:" << std::this_thread::get_id() << std::endl;

	Gtk::Main app(argc, argv); /* init Gtk */

	Glib::RefPtr<Gtk::Builder>  builder = Gtk::Builder::create_from_file(UI);
	PMRMainWindow	*PMRWindow = 0;
	builder->get_widget_derived("window", PMRWindow);

	app.run(*PMRWindow);
	delete PMRWindow;

	return EXIT_SUCCESS;
}
