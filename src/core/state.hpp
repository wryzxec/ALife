#pragma once

#include "grid.hpp"

/*
 * State is the current world.
 * A field is spatial information (convolution, neighbour count, etc.) which is
 * used to update the state.
 */

using State = Grid<double>;
using Field = Grid<double>;
