#include <pybind11/embed.h>
//guarantees Python's headers get fully processed before Qt's slots macro exists at all. 
#include "pyadapter.h"
#include <QDebug>

namespace py= pybind11;

PyAdapter::PyAdapter() {
    qDebug() << "Hello from PyAdapter" << this;
    py::initialize_interpreter();
    qDebug() << "Python interpreter initialized" << this;

    py::exec("print('hello from embedded python')");



    // Make our test folder visible to Python's import system
    py::module_::import("sys").attr("path").attr("insert")(0, "../python_models");

    // Import the file and call the function inside it
    py::module_ greet_module = py::module_::import("greet");
    py::object result = greet_module.attr("greet")("World");

    // Convert Python's return value into something C++/Qt can print
    std::string message = result.cast<std::string>();
    qDebug() << "From Python:" << QString::fromStdString(message);

}

PyAdapter::~PyAdapter() {
    qDebug() << "PyAdapter destructor - shutting down Python interpreter" << this;
    py::finalize_interpreter();
}

QString PyAdapter::ModelID(void) {
    return QString("PyAdapter");
}

int PyAdapter::NChem(void) { return 0;
}

void PyAdapter::CellHouseKeeping(CellBase *c) {
}

void PyAdapter::CelltoCellTransport(Wall *w, double *dchem_c1, double *dchem_c2) {
}

void PyAdapter::WallDynamics(Wall *w, double *dw1, double *dw2) {
}

void PyAdapter::CellDynamics(CellBase *c, double *dchem) {
}

void PyAdapter::OnDivide(ParentInfo *parent_info, CellBase *daughter1, CellBase *daughter2) {
}

void PyAdapter::SetCellColor(CellBase *c, QColor *color) {
}