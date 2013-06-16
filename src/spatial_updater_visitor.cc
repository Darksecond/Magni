#include "spatial_updater_visitor.h"

#include "light.h"
#include "camera.h"
#include "model.h"

spatial_updater_visitor::spatial_updater_visitor() : _stack()
{
    _stack.push(spatial());
}

void spatial_updater_visitor::start_visit(game_object& go) { start(go); }
void spatial_updater_visitor::start_visit(light& l) { start(l); }
void spatial_updater_visitor::start_visit(camera& l) { start(l); }
void spatial_updater_visitor::start_visit(model& l) { start(l); }
void spatial_updater_visitor::end_visit(game_object& go) { end(go); }
void spatial_updater_visitor::end_visit(light& l) { end(l); }
void spatial_updater_visitor::end_visit(camera& l) { end(l); }
void spatial_updater_visitor::end_visit(model& l) { end(l); }

void spatial_updater_visitor::start(game_object& go)
{
    _stack.push(_stack.top() * go.local());
    go.update_global(_stack.top());
    //go.global() = _stack.top();
}

void spatial_updater_visitor::end(game_object& go)
{
    _stack.pop();
}