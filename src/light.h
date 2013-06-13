#pragma once

#include "game_object.h"

class light : public game_object
{
public:
    struct attenuation_t
    {
        float constant;
        float linear;
        float exp;
    };
    
    explicit light(const std::string& name, const glm::vec3& position = glm::vec3(0.0f));
    
    virtual void accept(game_object_visitor& v);
    
    const attenuation_t& attenuation() const;
    void set_attenuation(float constant, float linear, float exp);
private:
    attenuation_t _attenuation;
};