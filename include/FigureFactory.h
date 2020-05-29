#ifndef FIGUREFACTORY_H
#define FIGUREFACTORY_H

#include <memory>

#include "Figure.h"

namespace FigureFactory
{
    std::unique_ptr<Figure> create(const char* name, int x, int y);
}

#endif // FIGUREFACTORY_H
