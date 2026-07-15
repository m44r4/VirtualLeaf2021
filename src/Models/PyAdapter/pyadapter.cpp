#include "pyadapter.h"
#include <QDebug>

PyAdapter::PyAdapter() {
    qDebug() << "Hello from PyAdapter";
}

QString PyAdapter::ModelID(void) {
    return QString("PyAdapter");
}

int PyAdapter::NChem(void) {
    return 0;
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