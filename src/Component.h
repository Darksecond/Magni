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
    class Component : public BaseComponent
    {
    public:
        Derived* parent = nullptr;
        
        static const Type type()
        {
            static Type _type = type_counter++;
            return _type;
        }
    };
};