#include "canvas.h"
#include <cassert>

using namespace std;
using namespace agl;

Canvas::Canvas(int w, int h) : _canvas(w, h)
{
}

Canvas::~Canvas()
{
}

void Canvas::save(const std::string& filename)
{
   _canvas.save(filename);
}

void Canvas::begin(PrimitiveType type)
{
}

void Canvas::end()
{
}

void Canvas::vertex(int x, int y)
{
}

void Canvas::color(unsigned char r, unsigned char g, unsigned char b)
{
}

void Canvas::background(unsigned char r, unsigned char g, unsigned char b)
{
}

