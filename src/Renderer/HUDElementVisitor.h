#pragma once

#include "../Program.h"
#include "../Texture.h"

namespace Ymir
{
    class Text;
    class Image;

    class HUDElementVisitor
    {
        Program& p;
        Texture& holstein;
    public:
        HUDElementVisitor(Program& p, Texture& holstein);
        void visit(Text& text);
        void visit(Image& image);
    };
};
