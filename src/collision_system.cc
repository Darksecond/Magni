#include "collision_system.h"
#include "bounding_volume.h"
#include "bounding_sphere.h"
#include "game.h"
#include "game_object.h"
#include "scene.h"

bool collision_system::step(std::shared_ptr<scene>& active_scene)
{
    if(!active_scene) return true;
    auto& linear_view = active_scene->linear_view();
    for(auto it = linear_view.begin(); it != linear_view.end(); ++it)
    {
        auto inner_it = it;
        ++inner_it;
        for(; inner_it != linear_view.end(); ++inner_it)
        {
            std::shared_ptr<bounding_volume> collider = (*it)->collider();
            std::shared_ptr<bounding_volume> other_collider = (*inner_it)->collider();
            if(collider && other_collider && collider->intersects(*other_collider))
            {
                if((*it)->get_behaviour())
                    (*it)->get_behaviour()->on_collision(*(*inner_it));
                if((*inner_it)->get_behaviour())
                    (*inner_it)->get_behaviour()->on_collision(*(*it));
            }
        }
    }
    return true;
}