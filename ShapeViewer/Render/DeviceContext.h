#pragma once
#include "OpenGLDebug.h"
#include <memory> 
#include <glm/vec4.hpp>
#include "Render/RenderData.h"
#include "Shader.h"

#include "wx/glcanvas.h"
#include <Widgets/Shapes/Triangle.h>

namespace SV::GS {
	
	class DeviceContext : public wxGLCanvas {

	public:
		DeviceContext(wxWindow* parent, wxWindowID id = wxID_ANY,
			const int* attribList = 0,
			const wxPoint& pos = wxDefaultPosition,
			const wxSize& size = wxDefaultSize, long style = 0L,
			const wxString& name = L"GLCanvas",
			const wxPalette& palette = wxNullPalette);
		virtual~DeviceContext();
		void UpdateScene(const glm::mat4& _camera);
		void ClearColor(const glm::vec4& color);
		void EndScene();
		void DrawPolygon(const std::vector<Vertex>& mesh, const std::vector<unsigned int>& _indecies, const glm::vec4& color);
		void DrawLine(const std::vector<Vertex>& contour, const glm::vec4& color);
		void DrawPolygon(const std::vector<Vertex>& mesh, const std::vector<unsigned int>& _indecies);
		void DrawLine(const std::vector<Vertex>& contour);
		void DrawTexture(/*Texture*/);
		void Resize(const wxSize& size);

	private:

		RenderData<Vertex>* _polygonRenderData;
		RenderData<Vertex>* _lineRenderData;
		RenderData<TexVertex>* _texRenderData;
		std::unique_ptr<wxGLContext> m_context;
		std::unique_ptr<Shader> _shader;
		glm::mat4 _viewProjectionMatrix;

	private:

		void InitializeGLAD();
		void SetupPrimitiveGraphics();
		void SetupTextureGraphics();
		void BeginBatch();
		void EndBatch();

		DeviceContext(const DeviceContext& tc) = delete;
		DeviceContext(DeviceContext&& tc) = delete;
		DeviceContext& operator=(const DeviceContext& tc) = delete;
		DeviceContext& operator=(DeviceContext&& tc) = delete;
	};
}