#pragma once

#include "Geometry.h"

namespace Ymir
{
    class CubeGeometry : public Geometry
    {
        std::vector<Vertex> _vertices;
        std::vector<Indice> _indices;
    public:
        CubeGeometry()
        {
            std::vector<Vertex> verts
            {
                //  X     Y     Z       NX     NY     NZ       U     V
                // bottom
                {-1.0f,-1.0f,-1.0f,    0.0f, -1.0f,  0.0f,   0.0f, 0.0f},
                { 1.0f,-1.0f,-1.0f,    0.0f, -1.0f,  0.0f,   1.0f, 0.0f},
                {-1.0f,-1.0f, 1.0f,    0.0f, -1.0f,  0.0f,   0.0f, 1.0f},
                { 1.0f,-1.0f,-1.0f,    0.0f, -1.0f,  0.0f,   1.0f, 0.0f},
                { 1.0f,-1.0f, 1.0f,    0.0f, -1.0f,  0.0f,   1.0f, 1.0f},
                {-1.0f,-1.0f, 1.0f,    0.0f, -1.0f,  0.0f,   0.0f, 1.0f},
                
                // top
                {-1.0f, 1.0f,-1.0f,    0.0f,  1.0f,  0.0f,   0.0f, 0.0f},
                {-1.0f, 1.0f, 1.0f,    0.0f,  1.0f,  0.0f,   0.0f, 1.0f},
                { 1.0f, 1.0f,-1.0f,    0.0f,  1.0f,  0.0f,   1.0f, 0.0f},
                { 1.0f, 1.0f,-1.0f,    0.0f,  1.0f,  0.0f,   1.0f, 0.0f},
                {-1.0f, 1.0f, 1.0f,    0.0f,  1.0f,  0.0f,   0.0f, 1.0f},
                { 1.0f, 1.0f, 1.0f,    0.0f,  1.0f,  0.0f,   1.0f, 1.0f},
                
                // front
                {-1.0f,-1.0f, 1.0f,    0.0f,  0.0f,  1.0f,   1.0f, 0.0f},
                { 1.0f,-1.0f, 1.0f,    0.0f,  0.0f,  1.0f,   0.0f, 0.0f},
                {-1.0f, 1.0f, 1.0f,    0.0f,  0.0f,  1.0f,   1.0f, 1.0f},
                { 1.0f,-1.0f, 1.0f,    0.0f,  0.0f,  1.0f,   0.0f, 0.0f},
                { 1.0f, 1.0f, 1.0f,    0.0f,  0.0f,  1.0f,   0.0f, 1.0f},
                {-1.0f, 1.0f, 1.0f,    0.0f,  0.0f,  1.0f,   1.0f, 1.0f},
                
                // back
                {-1.0f,-1.0f,-1.0f,    0.0f,  0.0f, -1.0f,   0.0f, 0.0f},
                {-1.0f, 1.0f,-1.0f,    0.0f,  0.0f, -1.0f,   0.0f, 1.0f},
                { 1.0f,-1.0f,-1.0f,    0.0f,  0.0f, -1.0f,   1.0f, 0.0f},
                { 1.0f,-1.0f,-1.0f,    0.0f,  0.0f, -1.0f,   1.0f, 0.0f},
                {-1.0f, 1.0f,-1.0f,    0.0f,  0.0f, -1.0f,   0.0f, 1.0f},
                { 1.0f, 1.0f,-1.0f,    0.0f,  0.0f, -1.0f,   1.0f, 1.0f},
                
                // left
                {-1.0f,-1.0f, 1.0f,   -1.0f,  0.0f,  0.0f,   0.0f, 1.0f},
                {-1.0f, 1.0f,-1.0f,   -1.0f,  0.0f,  0.0f,   1.0f, 0.0f},
                {-1.0f,-1.0f,-1.0f,   -1.0f,  0.0f,  0.0f,   0.0f, 0.0f},
                {-1.0f,-1.0f, 1.0f,   -1.0f,  0.0f,  0.0f,   0.0f, 1.0f},
                {-1.0f, 1.0f, 1.0f,   -1.0f,  0.0f,  0.0f,   1.0f, 1.0f},
                {-1.0f, 1.0f,-1.0f,   -1.0f,  0.0f,  0.0f,   1.0f, 0.0f},
                
                // right
                { 1.0f,-1.0f, 1.0f,    1.0f,  0.0f,  0.0f,   1.0f, 1.0f},
                { 1.0f,-1.0f,-1.0f,    1.0f,  0.0f,  0.0f,   1.0f, 0.0f},
                { 1.0f, 1.0f,-1.0f,    1.0f,  0.0f,  0.0f,   0.0f, 0.0f},
                { 1.0f,-1.0f, 1.0f,    1.0f,  0.0f,  0.0f,   1.0f, 1.0f},
                { 1.0f, 1.0f,-1.0f,    1.0f,  0.0f,  0.0f,   0.0f, 0.0f},
                { 1.0f, 1.0f, 1.0f,    1.0f,  0.0f,  0.0f,   0.0f, 1.0f},
            };
            _vertices = verts;
            
            _indices.reserve(_vertices.size()); //reserve the amount of indices needed, to speed generation up
            for(int i = 0; i < _vertices.size(); i++)
                _indices.push_back(i);
        }
        
        const std::vector<Vertex>& vertices() const
        {
            return _vertices;
        }
        
        const std::vector<Indice>& indices() const
        {
            return _indices;
        }
    };
};