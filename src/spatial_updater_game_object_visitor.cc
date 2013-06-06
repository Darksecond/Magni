#include "spatial_updater_game_object_visitor.h"

#include "light.h"

spatial_updater_game_object_visitor::spatial_updater_game_object_visitor() : _stack()
{
    _stack.push(spatial());
}

void spatial_updater_game_object_visitor::start_visit(game_object& go) { start(go); }
void spatial_updater_game_object_visitor::start_visit(light& l) { start(l); }
void spatial_updater_game_object_visitor::end_visit(game_object& go) { end(go); }
void spatial_updater_game_object_visitor::end_visit(light& l) { end(l); }

void spatial_updater_game_object_visitor::start(game_object& go)
{
    _stack.push(_stack.top() * go.local());
    go.global() = _stack.top();
}

void spatial_updater_game_object_visitor::end(game_object& go)
{
    _stack.pop();
}