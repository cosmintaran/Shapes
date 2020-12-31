#pragma once
#include<glad/glad.h>
#include <glm/glm.hpp>

struct VertexShadersSource;

namespace SV::GS{

	class Shader {

	public:
		
		~Shader();

		GLuint GetRendererID() { return m_RendererID; }
		int GetUniformLocation(const char* uniformName);

		static Shader* FromGLSLTextFiles(const std::string& vertexShaderPath, const std::string& fragmentShaderPath);

	private:
		Shader() = default;

		void LoadFromGLSLTextFiles(const std::string & vertexShaderPath, const std::string & fragmentShaderPath);
		GLuint CompileShader(GLenum type, const std::string & source);
	private:
		GLuint m_RendererID = 0;
		std::unordered_map<const char*, int> _uniformsCache;
	};
}
