#include "ichart.h"

IChart::~IChart() {
    delete chart;
}

QChart * IChart::getChart() {
    return chart;
}
