#pragma once

#include <memory>
#include <string>
#include <GLM/glm.hpp>

class scene;

//TODO behaviors (factory and method)
//TODO colliders (factory and method), allow to use the mesh aabb
//TODO redo behaviours so game_object can set their 'parent'

/**
 * This builder is stack based, this is how you can construct trees.
 * Use the 'end' function to end a element.
 */
class abstract_scene_builder
{
public:
    virtual ~abstract_scene_builder() = default;
    
    /**
     * Create a new camera.
     */
    virtual abstract_scene_builder& camera(const std::string& name, const glm::vec3& position) = 0;
    
    /**
     * Create a new light.
     * You can set the radius with the 'radius' method.
     */
    virtual abstract_scene_builder& light(const std::string& name, const glm::vec3& position) = 0;
    /**
     * Set a lights' radius.
     */
    virtual abstract_scene_builder& radius(float r) = 0;
    
    /**
     * Create a new model.
     * You need to set the material and mesh.
     */
    virtual abstract_scene_builder& model(const std::string& name, const glm::vec3& position) = 0;
    /**
     * Set a models' mesh.
     */
    virtual abstract_scene_builder& mesh(const std::string& mesh_name) = 0;
    /**
     * Set a models' material.
     */
    virtual abstract_scene_builder& material(const std::string& material_name) = 0;
    
    /**
     * Create a group element.
     * It is basically a generic 'group'.
     */
    virtual abstract_scene_builder& group(const std::string& name) = 0;
    
    /**
     * Finish current element.
     */
    virtual abstract_scene_builder& end() = 0;
    /**
     * Get the fully constructed scene.
     */
    virtual std::shared_ptr<scene> get_scene() = 0;
};