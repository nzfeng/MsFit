#pragma once

#include <algorithm>
#include <chrono>
#include <fstream>
#include <iostream>
#include <map>
#include <random>
#include <sstream>
#include <string>
#include <vector>

class Cell;
class Edge;
class Halfedge;

class Cell {

  public:
    // Constructors
    Cell();

    // State
    enum State { BLACK, WHITE };
    int nSides = 4;     // default cell shape is a square
    Glib::ustring data; // contents of the cell

    // Navigators
    Halfedge halfedge(); // default halfedge is the one at the "bottom"
};

class Halfedge {

  public:
    // Constructors
    Halfedge();

    // Navigators
    Halfedge next();
    Halfedge prev();
    Halfedge twin();
    // Currently I assume all cells are regular polygons, and they have an even number of sides, so that each side has a
    // side that is "across" from it (parallel).
    Halfedge across();
    Cell cell();
};

class Edge {
  public:
    // Constructors
    Edge();

    // Navigators
    Halfedge halfedge();
};