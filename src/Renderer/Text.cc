#include "Text.h"
#include "HUDElementVisitor.h"

#include <vector>

using namespace Ymir;

Text::Text(std::string text, glm::vec2 pos, int size) : text{text}, size{size}
{
    bottom_left = pos;
    top_right = glm::vec2(pos.x + (size * text.size() - 1 ),
                          pos.y + size);
}

void Text::accept(HUDElementVisitor& visitor)
{
    visitor.visit(*this);
}