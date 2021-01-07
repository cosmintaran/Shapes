#include "stdafx.h"
#include "DeviceContext.h"
#include "core/Log.h"
#include <glm/gtc/type_ptr.hpp>


namespace SV::GS {

	DrawingContext::DrawingContext(wxWindow* parent, wxWindowID id, const int* attribList, const wxPoint& pos, const wxSize& size, long style, const wxString& name, const wxPalette& palette)
		:wxGLCanvas(parent, id, attribList, pos, size, style, name, palette)
		, _viewProjectionMatrix(1.0f)
	{
		m_context = std::make_unique<wxGLContext>(this);
		SetCurrent(*m_context);
		InitializeGLAD();
		SetupPrimitiveGraphics();		
	}

	DrawingContext::~DrawingContext()
	{
		SetCurrent(*m_context);
	}

	void DrawingContext::InitializeGLAD()
	{

		if (!gladLoadGL())
		{
			CAD_ASSERT(false, "Faild to init GLAD");
			throw std::exception("Faild to init GLAD");
		}

		int width, height;
		GetSize(&width, &height);

		glViewport(0, 0, width, height);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glDisable(GL_DEPTH_TEST);

		glLineWidth(1.35f);
		glEnable(GL_PRIMITIVE_RESTART);
		glPrimitiveRestartIndex(0xFFFF);
		
		CAD_INFO("OpenGl initializated");
		CAD_INFO("OPENGL VERSION: {0}", (char*)glGetString(GL_VERSION));
		CAD_INFO("SHADER VERSION: {0}", (char*)glGetString(GL_SHADING_LANGUAGE_VERSION));

#if _DEBUG
		SV::GS::SetGLDebugLogLevel(SV::GS::DebugLogLevel::Notification);
		SV::GS::EnableGLDebugging();
#endif

	}

	void DrawingContext::SetupPrimitiveGraphics()
	{
		_shader = std::unique_ptr<Shader>(Shader::FromGLSLTextFiles("res\\shaders\\outline_glsl.vert", "res\\shaders\\outline_glsl.frag"));
		int location = _shader->GetUniformLocation("u_ViewProjectionMatrix");
		glUniformMatrix4fv(location, 1, false, glm::value_ptr(_viewProjectionMatrix));
	}

	void DrawingContext::ClearColor(const glm::vec4& color)
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor(color.r, color.g, color.b, color.a);
	}

	void DrawingContext::UpdateScene(const glm::mat4& cameraControler)
	{
		_viewProjectionMatrix = cameraControler;
		glUseProgram(_shader->GetRendererID());
		int location = _shader->GetUniformLocation("u_ViewProjectionMatrix");
		glUniformMatrix4fv(location, 1, false, glm::value_ptr(_viewProjectionMatrix));
		SetCurrent(*m_context);
	}

	void DrawingContext::EndScene()
	{
		SwapBuffers();
	}

	void DrawingContext::Resize(const wxSize& size)
	{
		SetSize(size);
		glViewport(0, 0, size.GetWidth(), size.GetHeight());
	}
}
