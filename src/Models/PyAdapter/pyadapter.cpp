#include <pybind11/embed.h>
//guarantees Python's headers get fully processed before Qt's slots macro exists at all. 
#include "pyadapter.h"
#include <QDebug>

namespace py= pybind11;



// The real definition of PyBridge — only visible in this file.
struct PyAdapter::PyBridge {
    py::object instance;   // the cached Python object, e.g. a TestModel()
};

PYBIND11_EMBEDDED_MODULE(virtualleaf, m) {
    py::class_<CellBase>(m, "CellBase")
        .def("cell_type", &CellBase::CellType);
}


PyAdapter::PyAdapter() {
    qDebug() << "Hello from PyAdapter" << this;

    py::initialize_interpreter();
    qDebug() << "Python interpreter initialized" << this;

    bridge = new PyBridge();

    py::module_::import("virtualleaf");

    // Make our python_models folder visible to Python's import system
    py::module_::import("sys").attr("path").attr("insert")(0, "../python_models");

    // Import the test module, construct ONE instance, cache it for this plugin's lifetime
    py::module_ test_module = py::module_::import("test_model");
    bridge->instance = test_module.attr("TestModel")();

    qDebug() << "Cached Python object, ready to delegate calls" << this;
}

PyAdapter::~PyAdapter() {
    qDebug() << "PyAdapter destructor - shutting down Python interpreter" << this;

       // bridge->instance holds a live Python reference. It MUST be destroyed
    // while the interpreter is still running - deleting bridge here does that,
    // before finalize_interpreter() tears the interpreter down.
    delete bridge;
    bridge = nullptr;

    py::finalize_interpreter();

}



QString PyAdapter::ModelID(void) {
    std::string id = bridge->instance.attr("model_id")().cast<std::string>();
    return QString::fromStdString(id);}

int PyAdapter::NChem(void) {
    return bridge->instance.attr("n_chem")().cast<int>();}

void PyAdapter::CellHouseKeeping(CellBase *c) {
    bridge->instance.attr("cell_housekeeping")(c);
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