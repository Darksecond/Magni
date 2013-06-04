#pragma once

namespace Ymir
{
    class Entity;
    
    /**
     * a basecomponent, you can use this to make maps which contain multiple different components
     */
    class BaseComponent
    {
    public:
        typedef unsigned long Type;
    protected:
        static Type type_counter;
    };

    /**
     * A component, this is used to store data.
     * You can interact with the data with behaviors or engines.
     * uses http://en.wikipedia.org/wiki/Curiously_recurring_template_pattern
     */
    template<typename Derived>
    class Component : public BaseComponent
    {
    public:
        /**
         * Parent of this component.
         * This is set automatically by entity the component is contained in.
         */
        Derived* parent;
        
        Entity* entity;

        /**
         * This components' type. you can use this to build lists or maps of components.
         */
        static const Type type()
        {
            static Type _type = type_counter++;
            return _type;
        }
    };
};
