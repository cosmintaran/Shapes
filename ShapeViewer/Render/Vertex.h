#pragma once
#include<glm/vec2.hpp>
#include<glm/vec3.hpp> 
#include<glm/vec4.hpp> 
namespace SV::GS {


	struct Vertex {
		glm::vec3 Position;
	};

	struct TexVertex 
		: public Vertex {

		glm::vec2 TextCoords;
		float TextId;
	};

}
