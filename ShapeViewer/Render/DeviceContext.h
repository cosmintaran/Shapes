#pragma once
#include "OpenGLDebug.h"
#include <glm/vec4.hpp>
#include "Render/RenderData.h"
#include "Shader.h"
#include "wx/glcanvas.h"

namespace SV::GS {

	class DrawingContext : public wxGLCanvas {

	public:
		DrawingContext(wxWindow* parent, wxWindowID id = wxID_ANY,
			const int* attribList = 0,
			const wxPoint& pos = wxDefaultPosition,
			const wxSize& size = wxDefaultSize, long style = 0L,
			const wxString& name = L"GLCanvas",
			const wxPalette& palette = wxNullPalette);
		virtual~DrawingContext();
		void UpdateScene(const glm::mat4& _camera);
		void ClearColor(const glm::vec4& color);
		void EndScene();
		void Resize(const wxSize& size);

		void SetDrawColor(float r, float g, float b, float a)
		{
			int location = _shader->GetUniformLocation("u_Color");
			glUniform4f(location, r,g,b,a);
		}

	private:
		std::unique_ptr<wxGLContext> m_context;
		std::unique_ptr<Shader> _shader;
		glm::mat4 _viewProjectionMatrix;

	private:

		void InitializeGLAD();
		void SetupPrimitiveGraphics();
		DrawingContext(const DrawingContext& tc) = delete;
		DrawingContext(DrawingContext&& tc) = delete;
		DrawingContext& operator=(const DrawingContext& tc) = delete;
		DrawingContext& operator=(DrawingContext&& tc) = delete;
	};
}
