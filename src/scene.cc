#include "scene.h"
#include "game_object.h"

scene::scene()
{
    _world = add_game_object(std::make_shared<game_object>("world"));
}

scene::scene(std::shared_ptr<game_object>& world)
{
    _world = add_game_object(world);
}

std::shared_ptr<game_object> scene::add_game_object(std::shared_ptr<game_object> object)
{
    if(_world)
        _world->add(object);
    _linear_view.push_back(object);
    
    return object;
}

std::shared_ptr<game_object> scene::add_game_object(std::shared_ptr<game_object> object, std::shared_ptr<game_object> parent)
{
    if(parent)
        parent->add(object);
    _linear_view.push_back(object);
    
    return object;
}

std::shared_ptr<game_object> scene::get_by_name(const std::string& name)
{
    for(auto go : _linear_view)
    {
        if(go->name() == name)
            return go;
    }
    return nullptr;
}

void scene::remove_game_object(std::shared_ptr<game_object> object)
{
    _to_remove.push_back(object);
}

void scene::remove_to_remove()
{
    for(auto object : _to_remove)
    {
        _world->remove(object);
        _linear_view.remove(object);
    }
    _to_remove.clear();
}
