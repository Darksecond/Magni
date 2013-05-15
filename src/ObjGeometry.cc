#include "ObjGeometry.h"

#include <sstream>
#include <tuple>
#include <iostream>

using namespace Ymir;

ObjGeometry::ObjGeometry(std::istream& stream)
{
    load(stream);
    interlace();

    //clean
    _v.clear();
    _vn.clear();
    _vt.clear();
}

void ObjGeometry::load(std::istream& stream)
{
    std::string line;
    while(stream.good() && !stream.eof() && std::getline(stream, line))
    {
        std::string key;
        std::stringstream str(line);
        str >> key >> std::ws;
        if (key == "v")
        {
            //v 0.437119 1.311790 0.819443
            float x, y, z;
            str >> x >> std::ws >> y >> std::ws >> z;
            _v.push_back(std::make_tuple(x, y, z));
        }
        else if (key == "vn")
        {
            //vn -0.455134 0.525083 0.719125
            float x, y, z;
            str >> x >> std::ws >> y >> std::ws >> z;
            _vn.push_back(std::make_tuple(x, y, z));
        }
        else if (key == "vt")
        {
            //vt -0.114168 0.546974
            float u, v;
            str >> u >> std::ws >> v;
            _vt.push_back(std::make_tuple(u, v));
        }
        else if (key == "f")
        {
            //f 151/103/427 97/104/428 95/100/429
            for(int i = 0; i < 3; i++)
            {
                std::string face_str;
                str >> face_str >> std::ws;
                std::stringstream face(face_str);
                int v, vt, vn;
                std::string sv, svt, svn;

                std::getline(face, sv, '/');
                std::getline(face, svt, '/');
                std::getline(face, svn);
                std::stringstream(sv) >> v;
                if(svt != "")
                    std::stringstream(svt) >> vt;
                else
                    vt = 0;
                std::stringstream(svn) >> vn;

                _f.push_back(std::make_tuple(face_str, v, vt, vn));
            }
        }
    }
}

void ObjGeometry::interlace()
{
    for(auto& f : _f)
    {
        auto it = indice_mapping.find(std::get<0>(f));
        if(it != indice_mapping.end())
        {
            //in map
            _indices.push_back(it->second);
        }
        else
        {
            //niet in map

            //maak vertex
            auto v =  _v [std::get<1>(f) - 1];
            std::tuple<float, float> vt;
            if(std::get<2>(f) > 0)
            {
                vt = _vt[std::get<2>(f) - 1];
            }
            else
            {
                vt = std::make_tuple(0,0);
            }
            auto vn = _vn[std::get<3>(f) - 1];

            Vertex vert;
            vert.x = std::get<0>(v);
            vert.y = std::get<1>(v);
            vert.z = std::get<2>(v);
            vert.nx = std::get<0>(vn);
            vert.ny = std::get<1>(vn);
            vert.nz = std::get<2>(vn);
            vert.u = std::get<0>(vt);
            vert.v = std::get<1>(vt);
            _vertices.push_back(vert);

            //voeg toe aan indices
            _indices.push_back(_vertices.size() - 1);

            //voeg toe aan map
            indice_mapping.insert({std::get<0>(f), _vertices.size() - 1});
        }
    }
}

const std::vector<Vertex>& ObjGeometry::vertices() const
{
    return _vertices;
}

const std::vector<Indice>& ObjGeometry::indices() const
{
    return _indices;
}
