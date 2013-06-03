#include "HUDEngine.h"

#ifdef __APPLE__
    #include <GLEW/glew.h>
    #include <GLFW/glfw.h>
#endif
#ifdef _WIN32
    #include <GL/glew.h>
    #include <GL/glfw.h>
#endif // _WIN32
#include <iostream>

using namespace Ymir;

HUDEngine::HUDEngine(RenderEngine& renderer, ResourceManager<Texture>& texMan) : renderEngine(renderer), textureManager(texMan), elements()
{
    scene = nullptr;
    
    _selection_selecting = false;
    _selection_start = glm::vec3(0);
    _selection_end = glm::vec3(0);
    _selection_entities = std::list<std::shared_ptr<Entity>>();
}

void HUDEngine::registerEntity(Entity& ent)
{
}

void HUDEngine::unregisterEntity(Entity& ent)
{
}

void HUDEngine::update(int pass, double delta)
{
    if(pass != -1) return;
    
    if(glfwGetMouseButton(GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
    {
        int xpos, ypos;
        glfwGetMousePos(&xpos, &ypos);
        ypos = 600 - ypos;
        bool hud_clicked = false;
        
        //check left mouse button
        for(auto e : elements)
        {
            if(xpos > e->bottom_left.x &&
               ypos > e->bottom_left.y &&
               xpos < e->top_right.x &&
               ypos < e->top_right.y
               )
            {
                e->set_clicked(true);
                hud_clicked = true;
            }
            else
            {
                e->set_clicked(false);
            }
        }
        
        if(!hud_clicked && scene)
        {
            if(_selection_selecting)
            {
                _selection_end = renderEngine.get3DPositionFromMousePosition();
            }
            else
            {
                _selection_end = _selection_start = renderEngine.get3DPositionFromMousePosition();
                _selection_selecting = true;
            }
        }
    }
    else
    {
        for(auto e : elements)
            e->set_clicked(false);
        
        if(_selection_selecting)
        {
            _selection_selecting = false;
            if(glm::distance(_selection_start, _selection_end) < 0.01f)
            {
                _selection_entities = std::list<std::shared_ptr<Entity>>();
                _selection_entities.push_back(scene->getEntityAtPosition(_selection_start));
            }
            else
            {
                _selection_entities = scene->getEntitiesBetweenPoints(_selection_start, _selection_end);
            }
        }
    }
}

void HUDEngine::addComponent(Entity& ent, const BaseComponent::Type& component_type)
{
}

std::shared_ptr<Image> HUDEngine::addImage(const char* texture, glm::vec2 pos, float w, float h)
{
    std::shared_ptr<Texture> tex = textureManager.resource(texture);
    std::shared_ptr<Image> image = std::make_shared<Image>(pos, w, h, tex);
    
    elements.push_back(image);
    renderEngine.addText(image);
    
    return image;
}

std::shared_ptr<Text> HUDEngine::addText(const char* text, glm::vec2 pos, int size)
{
    std::shared_ptr<Text> text_element = std::make_shared<Text>(text, pos, size);
    
    elements.push_back(text_element);
    renderEngine.addText(text_element);
    
    return text_element;
}

void HUDEngine::removeElement(std::shared_ptr<HUDElement> element)
{
    elements.remove(element);
    renderEngine.texts.remove(element);
}

std::list<std::shared_ptr<Entity>> HUDEngine::selectedEntities()
{
    return _selection_entities;
}

std::shared_ptr<Entity> HUDEngine::selectedEntity()
{
    if(_selection_entities.empty())
        return nullptr;
    else
        return _selection_entities.front();
}
