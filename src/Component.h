#pragma once

namespace Ymir
{
    class BaseComponent
    {
    public:
        typedef unsigned long Type;
    protected:
        static Type type_counter;
    };

    template<typename Derived>
    class iComponent : public BaseComponent
    {
    public:
        static const Type type()
        {
            static Type _type = type_counter++;
            return _type;
        }
    };
};