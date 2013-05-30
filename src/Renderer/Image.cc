#include "Image.h"
#include "HUDElementVisitor.h"

using namespace Ymir;


Image::Image(glm::vec2 bl, float width, float height, std::shared_ptr<Texture> tex) : image_texture(tex)
{
    bottom_left = bl;
    top_right = glm::vec2(bl.x+width, bl.y+height);
}

void Image::accept(HUDElementVisitor& visitor)
{
    visitor.visit(*this);
}