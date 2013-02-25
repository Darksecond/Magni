#pragma once

#include "Geometry.h"

#include <istream>
#include <utility>
#include <string>
#include <map>
#include <vector>

namespace Ymir
{
    class ObjGeometry : public Geometry
    {
        std::vector<Vertex> _vertices;
        std::vector<Indice> _indices;
        
        std::vector<std::tuple<float, float, float>> _v;
        std::vector<std::tuple<float, float, float>> _vn;
        std::vector<std::tuple<float, float>> _vt;
        std::vector<std::tuple<std::string, int, int, int>> _f;
        
        std::map<std::string, int> indice_mapping;
    public:
        ObjGeometry(std::istream& stream);
        
        void load(std::istream& stream);
        void interlace();
        
        virtual const std::vector<Vertex>& vertices() const;
        virtual const std::vector<Indice>& indices() const;
    };
};