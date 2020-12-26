#pragma once
#include <glm/glm.hpp>
namespace SV::GS {

	class Triangulator {

	private:
		Triangulator() = default;
		~Triangulator() = default;
	public:
		static void TriangulatePolygon(const std::vector<glm::vec3>& polygon);

	};
}