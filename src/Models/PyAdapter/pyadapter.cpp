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
        .def("cell_type", &CellBase::CellType)
        .def("set_chemical", &CellBase::SetChemical)
        .def("chemical", &CellBase::Chemical)
        .def("enlarge_target_area", &CellBase::EnlargeTargetArea)
        .def("area", &CellBase::Area)
        .def("centroid", &CellBase::Centroid)
        .def("divide", &CellBase::Divide);

    m.def("base_area", &CellBase::BaseArea);    

    py::class_<Vector>(m,"Vector")    
        .def_readonly("x", &Vector::x);

    py::class_<ParentInfo>(m, "ParentInfo");

    // py::class_<Parameter>(m, "Parameter")
    //     .def_readonly("cell_expansion_rate", &Parameter::cell_expansion_rate)
    //     .def_readonly("rel_cell_div_threshold", &Parameter::rel_cell_div_threshold);
    
    // m.attr("par") = py::cast(par, py::return_value_policy::reference);    
    
    }


PyAdapter::PyAdapter() {
    qDebug() << "Hello from PyAdapter" << this;

    py::initialize_interpreter();
    qDebug() << "Python interpreter initialized" << this;

    bridge = new PyBridge();
    pythonReady = false;

try{
    py::module_::import("virtualleaf");
    
    // Make our python_models folder visible to Python's import system
    py::module_::import("sys").attr("path").attr("insert")(0, "../python_models");

    // Import the test module, construct ONE instance, cache it for this plugin's lifetime
    // py::module_ test_module = py::module_::import("test_model");
    // bridge->instance = test_module.attr("TestModel")();

    py::module_ lateral_root = py::module_::import("lateral_root");
    bridge->instance = lateral_root.attr("lateral_root")();



    pythonReady = true; 
} catch (const py::error_already_set &e){
        qDebug() << "Python error during PyAdapter setup:" << e.what();
}
   

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
    if (!pythonReady)
    {
        return 0;
    }
    
    return bridge->instance.attr("n_chem")().cast<int>();}

void PyAdapter::CellHouseKeeping(CellBase *c) {
    if (!pythonReady) return;
    bridge->instance.attr("cell_housekeeping")(c);
}

void PyAdapter::CelltoCellTransport(Wall *w, double *dchem_c1, double *dchem_c2) {
    
}

void PyAdapter::WallDynamics(Wall *w, double *dw1, double *dw2) {
}

void PyAdapter::CellDynamics(CellBase *c, double *dchem) {
}

void PyAdapter::OnDivide(ParentInfo *parent_info, CellBase *daughter1, CellBase *daughter2) {
    if (!pythonReady) return;
    bridge->instance.attr("on_divide")(parent_info, daughter1, daughter2);

}

void PyAdapter::SetCellColor(CellBase *c, QColor *color) {
}