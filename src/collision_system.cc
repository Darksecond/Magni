#include "collision_system.h"
#include "bounding_volume.h"
#include "bounding_sphere.h"
#include "game.h"
#include "game_object.h"

bool collision_system::step()
{
    auto& linear_view = game::instance().linear_view();
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
                (*it)->get_behaviour()->on_collision(*(*inner_it));
            }
        }
    }
    return true;
}