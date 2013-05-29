#include "HUDElementVisitor.h"

#include <GLM/glm.hpp>
#ifdef __APPLE__
    #include <GLEW/glew.h>
    #include <GLFW/glfw.h>
#endif
#ifdef _WIN32
    #include <GL/glew.h>
    #include <GL/glfw.h>
#endif // _WIN32
#include <vector>

#include "Text.h"
#include "Program.h"
#include "Texture.h"

using namespace Ymir;

HUDElementVisitor::HUDElementVisitor(Program& _p, Texture& _holstein) : p(_p), holstein(_holstein)
{
}

void HUDElementVisitor::visit(Text& text)
{
    std::vector<glm::vec2> vertices;
	std::vector<glm::vec2> UVs;

    for ( unsigned int i=0 ; i < text.text.size(); i++ )
    {
        glm::vec2 vertex_up_left    = glm::vec2( text.bottom_left.x+i*text.size            , text.bottom_left.y+text.size );
		glm::vec2 vertex_up_right   = glm::vec2( text.bottom_left.x+i*text.size + text.size, text.bottom_left.y+text.size );
		glm::vec2 vertex_down_right = glm::vec2( text.bottom_left.x+i*text.size + text.size, text.bottom_left.y           );
		glm::vec2 vertex_down_left  = glm::vec2( text.bottom_left.x+i*text.size            , text.bottom_left.y           );

		vertices.push_back(vertex_up_left   );
		vertices.push_back(vertex_down_left );
		vertices.push_back(vertex_up_right  );

		vertices.push_back(vertex_down_right);
		vertices.push_back(vertex_up_right);
		vertices.push_back(vertex_down_left);

		char character = text.text[i];
		float uv_x = (character%16)/16.0f;
		float uv_y = (character/16)/16.0f;

		glm::vec2 uv_up_left    = glm::vec2( uv_x           , uv_y );
		glm::vec2 uv_up_right   = glm::vec2( uv_x+1.0f/16.0f, uv_y );
		glm::vec2 uv_down_right = glm::vec2( uv_x+1.0f/16.0f, (uv_y + 1.0f/16.0f) );
		glm::vec2 uv_down_left  = glm::vec2( uv_x           , (uv_y + 1.0f/16.0f) );
		UVs.push_back(uv_up_left   );
		UVs.push_back(uv_down_left );
		UVs.push_back(uv_up_right  );

		UVs.push_back(uv_down_right);
		UVs.push_back(uv_up_right);
		UVs.push_back(uv_down_left);
    }

    ProgramContext pc{p};

    //create VBO's, VAO
    GLuint vertex_VBO, UV_VBO, VAO;

    glGenBuffers(1, &vertex_VBO);
    glGenBuffers(1, &UV_VBO);
    glGenVertexArrays(1, &VAO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec2), &vertices[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(p.attrib("vertexPosition_screenspace"));
    glVertexAttribPointer(p.attrib("vertexPosition_screenspace"), 2, GL_FLOAT, GL_FALSE, 0, (void*)0 );

    glBindBuffer(GL_ARRAY_BUFFER, UV_VBO);
    glBufferData(GL_ARRAY_BUFFER, UVs.size() * sizeof(glm::vec2), &UVs[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(p.attrib("vertexUV"));
	glVertexAttribPointer(p.attrib("vertexUV"), 2, GL_FLOAT, GL_FALSE, 0, (void*)0 );

    TextureContext h{holstein, GL_TEXTURE0};
    p.setUniform("myTextureSampler", 0);

    glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glDrawArrays(GL_TRIANGLES, 0, (GLsizei)vertices.size() );

	glDisable(GL_BLEND);

	glDisableVertexAttribArray(p.attrib("vertexPosition_screenspace"));
	glDisableVertexAttribArray(p.attrib("vertexUV"));

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &vertex_VBO);
    glDeleteBuffers(1, &UV_VBO);
}