#include "deferred_render_visitor.h"

deferred_render_visitor::deferred_render_visitor()
{
    //g-buffer
}

deferred_render_visitor::~deferred_render_visitor()
{
}

void deferred_render_visitor::start_visit(light& l)
{
}

void deferred_render_visitor::start_visit(camera& c)
{
}

void deferred_render_visitor::start_visit(model& m)
{
}

void deferred_render_visitor::start_frame()
{
}

void deferred_render_visitor::end_frame()
{
}