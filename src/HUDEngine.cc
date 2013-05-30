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
    _selectedEntity = nullptr;
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
            _selectedEntity = scene->getEntityAtPosition(renderEngine.get3DPositionFromMousePosition());
            //TODO track changes in selectedEntity
        }
    }
    else
    {
        for(auto e : elements)
            e->set_clicked(false);
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
