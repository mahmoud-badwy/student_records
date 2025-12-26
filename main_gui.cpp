#include <QApplication>
#include "MainWindow.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    
    // Set application metadata
    QApplication::setApplicationName("Student Information System");
    QApplication::setOrganizationName("Red-Black Tree");
    QApplication::setApplicationVersion("1.0");
    
    // Create and show main window
    MainWindow window;
    window.show();
    
    return app.exec();
}
