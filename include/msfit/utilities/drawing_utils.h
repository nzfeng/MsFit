#pragma once

#include <algorithm>

#include <gtkmm/drawingarea.h>

#include "msfit/utilities/vector2.h"

void draw_text(Gtk::DrawingArea& drawingArea, const Cairo::RefPtr<Cairo::Context>& cr, const Glib::ustring& text,
               const std::vector<Vector2>& cellCorners);