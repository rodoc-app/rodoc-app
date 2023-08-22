/// @file main.cpp The main file for starting the application.

#include "graphics/mainwindow.h"
#include "core/strings.h"
#include <iostream>

/// Main function for the application.
/// @param argc Number of arguments.
/// @param argv List of all arguments.
/// @return The exit code of the application.
int main(int argc, char ** argv){
	try{
		QApplication app(argc, argv);
        MainWindow* w = new MainWindow();
        w->showMaximized();
        #ifdef _WIN32
            FreeConsole();
		#endif
		return app.exec();
	} catch(const std::exception& e){
		std::cout << error::EXCEPTION_STR << e.what() << std::endl;
	} catch(...){
		std::cout << error::UNKNOWN_EXCEP_STR << std::endl;
	}
}
