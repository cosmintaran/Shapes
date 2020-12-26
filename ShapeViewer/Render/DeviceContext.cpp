#include "stdafx.h"
#include "DeviceContext.h"
#include "core/Log.h"
#include <glm/gtc/type_ptr.hpp>


namespace SV::GS {

	DeviceContext::DeviceContext(wxWindow* parent, wxWindowID id, const int* attribList, const wxPoint& pos, const wxSize& size, long style, const wxString& name, const wxPalette& palette)
		:wxGLCanvas(parent, id, attribList, pos, size, style, name, palette)
		, _polygonRenderData(new RenderData<Vertex>())
		, _lineRenderData(new RenderData<Vertex>())
		, _viewProjectionMatrix(1.0f)
	{
		m_context = std::make_unique<wxGLContext>(this);
		SetCurrent(*m_context);
		InitializeGLAD();
		SetupPrimitiveGraphics();
	}

	DeviceContext::~DeviceContext()
	{
		SetCurrent(*m_context);

		//delete outline 
		glDeleteVertexArrays(1, &_lineRenderData->VertexArrayRenderID);
		glDeleteBuffers(1, &_lineRenderData->VertexBufferRenderID);
		glDeleteTextures(1, &_lineRenderData->WhiteTexture);
		delete _lineRenderData;

		//delete polygons
		glDeleteVertexArrays(1, &_polygonRenderData->VertexArrayRenderID);
		glDeleteBuffers(1, &_polygonRenderData->VertexBufferRenderID);
		glDeleteTextures(1, &_polygonRenderData->WhiteTexture);
		delete _polygonRenderData;
	}

	void DeviceContext::InitializeGLAD()
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
		//		glEnable(GL_DEPTH_TEST);
		glDisable(GL_DEPTH_TEST);

		glLineWidth(1.35f);
		glEnable(GL_PRIMITIVE_RESTART);
		glPrimitiveRestartIndex(0xFFFF);


		CAD_INFO("OpenGl initializated");
		CAD_INFO("OPENGL VERSION: {0}", (char*)glGetString(GL_VERSION));
		CAD_INFO("SHADER VERSION: {0}", (char*)glGetString(GL_SHADING_LANGUAGE_VERSION));

		SV::GS::SetGLDebugLogLevel(SV::GS::DebugLogLevel::Notification);
		SV::GS::EnableGLDebugging();

	}

	void DeviceContext::SetupPrimitiveGraphics()
	{
		/*Line setup*/
		_lineRenderData->Buffer = new Vertex[MaxVertexCount];
		_lineRenderData->IndexBufferArray = new unsigned int[MaxIndexCount];

		glGenVertexArrays(1, &_lineRenderData->VertexArrayRenderID);
		glBindVertexArray(_lineRenderData->VertexArrayRenderID);

		glGenBuffers(1, &_lineRenderData->VertexBufferRenderID);
		glBindBuffer(GL_ARRAY_BUFFER, _lineRenderData->VertexBufferRenderID);
		glBufferData(GL_ARRAY_BUFFER, MaxVertexCount * sizeof(Vertex), nullptr, GL_DYNAMIC_DRAW);

		glGenBuffers(1, &_lineRenderData->IndexBufferRenderID);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _lineRenderData->IndexBufferRenderID);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, MaxIndexCount * sizeof(unsigned int), nullptr, GL_DYNAMIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, Position));

		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, Color));

		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		/*Polygon setup*/
		_polygonRenderData->Buffer = new Vertex[MaxVertexCount];
		_polygonRenderData->IndexBufferArray = new unsigned int[MaxIndexCount];

		glGenVertexArrays(1, &_polygonRenderData->VertexArrayRenderID);
		glBindVertexArray(_polygonRenderData->VertexArrayRenderID);

		glGenBuffers(1, &_polygonRenderData->VertexBufferRenderID);
		glBindBuffer(GL_ARRAY_BUFFER, _polygonRenderData->VertexBufferRenderID);
		glBufferData(GL_ARRAY_BUFFER, MaxVertexCount * sizeof(Vertex), nullptr, GL_DYNAMIC_DRAW);

		glGenBuffers(1, &_polygonRenderData->IndexBufferRenderID);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _polygonRenderData->IndexBufferRenderID);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, MaxIndexCount * sizeof(unsigned int), nullptr, GL_DYNAMIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, Position));


		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, Color));

		_shader = std::unique_ptr<Shader>(Shader::FromGLSLTextFiles("res\\shaders\\outline_glsl.vert", "res\\shaders\\outline_glsl.frag"));
		glUseProgram(_shader->GetRendererID());

		glUseProgram(0);
		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	}

	void DeviceContext::SetupTextureGraphics()
	{
		_polygonRenderData->Buffer = new Vertex[MaxVertexCount];

		glGenVertexArrays(1, &_polygonRenderData->VertexArrayRenderID);
		glBindVertexArray(_polygonRenderData->VertexArrayRenderID);

		glGenBuffers(1, &_polygonRenderData->VertexBufferRenderID);
		glBindBuffer(GL_ARRAY_BUFFER, _polygonRenderData->VertexBufferRenderID);
		glBufferData(GL_ARRAY_BUFFER, MaxVertexCount * sizeof(Vertex), nullptr, GL_DYNAMIC_DRAW);

		glGenBuffers(1, &_polygonRenderData->IndexBufferRenderID);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _polygonRenderData->IndexBufferRenderID);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, MaxIndexCount * sizeof(unsigned int), nullptr, GL_DYNAMIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, Position));


		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, Color));


		//glEnableVertexAttribArray(2);
		//glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(TexVertex), (const void*)offsetof(TexVertex, TextCoords));


		//glEnableVertexAttribArray(3);
		//glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(TexVertex), (const void*)offsetof(TexVertex, TextId));


		//1x1 texture
		//glGenTextures(1, &_polygonRenderData->WhiteTexture);
		//glBindTexture(GL_TEXTURE_2D, _polygonRenderData->WhiteTexture);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		//unsigned int color = 0xFFFFFFFF;
		//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, &color);
		//_polygonRenderData->TexturesSlots[0] = _polygonRenderData->WhiteTexture;

		//for (unsigned int i = 1; i < MaxTextures; ++i) {
		//	_polygonRenderData->TexturesSlots[i] = 0;
		//}

		//_shader = std::unique_ptr<Shader>(Shader::FromGLSLTextFiles("res\\shaders\\glsl.vert", "res\\shaders\\glsl.frag"));
		//glUseProgram(_shader->GetRendererID());
		//auto loc = glGetUniformLocation(_shader->GetRendererID(), "u_Textures");
		//int samplers[32];
		//for (int i = 0; i < 32; ++i)
		//	samplers[i] = i;

		//glUniform1iv(loc, 32, samplers);

		//glUseProgram(0);
		//glBindVertexArray(0);
		//glBindBuffer(GL_ARRAY_BUFFER, 0);
		//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

	void DeviceContext::ClearColor(const glm::vec4& color)
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor(color.r, color.g, color.b, color.a);
	}

	void DeviceContext::UpdateScene(const glm::mat4& cameraControler)
	{
		_viewProjectionMatrix = cameraControler;
		SetCurrent(*m_context);
		BeginBatch();
	}

	void DeviceContext::EndScene()
	{
		EndBatch();
		SwapBuffers();
	}

	void DeviceContext::DrawPolygon(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& _indecies, const glm::vec4& color)
	{
		std::for_each(vertices.cbegin(), vertices.cend(), [&](const Vertex& vertex) {
			*_polygonRenderData->BufferPtr = vertex;
			_polygonRenderData->BufferPtr->Color = color;
			_polygonRenderData->BufferPtr++;
			});


		std::for_each(_indecies.cbegin(), _indecies.cend(), [&](unsigned int id) {

			*_polygonRenderData->IndexBufferArrayPtr = id + _polygonRenderData->VertexCount;
			_polygonRenderData->IndexBufferArrayPtr++;

			});

		_polygonRenderData->VertexCount += (unsigned int)vertices.size();

		EndBatch();
		BeginBatch();
	}

	void DeviceContext::DrawLine(const std::vector<Vertex>& contour, const glm::vec4& color)
	{
		for (size_t i = 0; i < contour.size(); ++i) {

			*_lineRenderData->BufferPtr = contour[i];
			_lineRenderData->BufferPtr->Color = color;
			_lineRenderData->BufferPtr++;

			*_lineRenderData->IndexBufferArrayPtr = i + _lineRenderData->VertexCount;
			_lineRenderData->IndexBufferArrayPtr++;
		}

		*_lineRenderData->IndexBufferArrayPtr = 0xFFFF;
		_lineRenderData->IndexBufferArrayPtr++;
		_lineRenderData->VertexCount += (unsigned int)contour.size();

		EndBatch();
		BeginBatch();
	}

	void DeviceContext::DrawPolygon(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& _indecies)
	{
		//if (_polygonRenderData->VertexCount >= MaxIndexCount || (vertices.size() + _polygonRenderData->VertexCount) > MaxIndexCount) {
		//	EndBatch();
		//	BeginBatch();
		//}

		std::for_each(vertices.cbegin(), vertices.cend(), [&](const Vertex& vertex) {
			*_polygonRenderData->BufferPtr = vertex;
			_polygonRenderData->BufferPtr++;
			});


		std::for_each(_indecies.cbegin(), _indecies.cend(), [&](unsigned int id) {

			*_polygonRenderData->IndexBufferArrayPtr = id + _polygonRenderData->VertexCount;
			_polygonRenderData->IndexBufferArrayPtr++;

			});

		_polygonRenderData->VertexCount += (unsigned int)vertices.size();

		EndBatch();
		BeginBatch();
	}

	void DeviceContext::DrawLine(const std::vector<Vertex>& contour)
	{
		//if (_lineRenderData->VertexCount >= MaxIndexCount || (contour.size() + _lineRenderData->VertexCount) > MaxIndexCount) {
		//	EndBatch();
		//	BeginBatch();
		//}

		for (size_t i = 0; i < contour.size(); ++i) {
			*_lineRenderData->BufferPtr = contour[i];
			_lineRenderData->BufferPtr++;
			*_lineRenderData->IndexBufferArrayPtr = i + _lineRenderData->VertexCount;
			_lineRenderData->IndexBufferArrayPtr++;
		}

		*_lineRenderData->IndexBufferArrayPtr = 0xFFFF;
		_lineRenderData->IndexBufferArrayPtr++;
		_lineRenderData->VertexCount += (unsigned int)contour.size();

		EndBatch();
		BeginBatch();
	}

	void DeviceContext::DrawTexture()
	{
	}

	void DeviceContext::Resize(const wxSize& size)
	{
		SetSize(size);
		glViewport(0, 0, size.GetWidth(), size.GetHeight());
	}

	void DeviceContext::BeginBatch()
	{
		_polygonRenderData->BufferPtr = _polygonRenderData->Buffer;
		_lineRenderData->BufferPtr = _lineRenderData->Buffer;

		_polygonRenderData->IndexBufferArrayPtr = _polygonRenderData->IndexBufferArray;
		_lineRenderData->IndexBufferArrayPtr = _lineRenderData->IndexBufferArray;

		_polygonRenderData->VertexCount = 0;
		_lineRenderData->VertexCount = 0;
	}


	void DeviceContext::EndBatch()
	{
		//Draw fill triangles
		GLsizeiptr bufferSize = _polygonRenderData->BufferPtr - _polygonRenderData->Buffer;
		GLsizeiptr indexSize = 0;

		glUseProgram(_shader->GetRendererID());
		int location = _shader->GetUniformLocation("u_ViewProjectionMatrix");
		glUniformMatrix4fv(location, 1, false, glm::value_ptr(_viewProjectionMatrix));

		if (bufferSize > 0) {
			glBindBuffer(GL_ARRAY_BUFFER, _polygonRenderData->VertexBufferRenderID);
			glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(Vertex) * bufferSize, _polygonRenderData->Buffer);

			glBindVertexArray(_polygonRenderData->VertexArrayRenderID);

			GLsizeiptr indexSize = _polygonRenderData->IndexBufferArrayPtr - _polygonRenderData->IndexBufferArray;
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _polygonRenderData->IndexBufferRenderID);
			glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, sizeof(unsigned int) * indexSize, _polygonRenderData->IndexBufferArray);

			glDrawElements(GL_TRIANGLES, indexSize, GL_UNSIGNED_INT, nullptr);
		}


		//Draw lines
		bufferSize = _lineRenderData->BufferPtr - _lineRenderData->Buffer;
		if (bufferSize > 0) {

			glBindBuffer(GL_ARRAY_BUFFER, _lineRenderData->VertexBufferRenderID);
			glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(Vertex) * bufferSize, _lineRenderData->Buffer);

			indexSize = _lineRenderData->IndexBufferArrayPtr - _lineRenderData->IndexBufferArray;
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _lineRenderData->IndexBufferRenderID);
			glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, sizeof(unsigned int) * indexSize, _lineRenderData->IndexBufferArray);

			glBindVertexArray(_lineRenderData->VertexArrayRenderID);

			//remove last added primitive restart
			glDrawElements(GL_LINE_LOOP, indexSize - 1, GL_UNSIGNED_INT, /*(void*) bufferSize*/ nullptr);
		}

		//for (unsigned short i = 0; i < _polygonRenderData->TextureSlotIndex; ++i)
		//{
		//	glActiveTexture(GL_TEXTURE0 + i);
		//	glBindTexture(GL_TEXTURE_2D, _polygonRenderData->TexturesSlots[i]);
		//}


		//glBindVertexArray(_renderData->VertexArray);
		//GLint start = 0;
		//GLint end = 2;
		//glMultiDrawArrays(GL_TRIANGLES, &start, &end, 1);
		//glDrawElements(GL_TRIANGLES, _renderData->IndexCount, GL_UNSIGNED_INT, nullptr);


		//glBindVertexArray(_polygonRenderData->VertexArray);
		//glDrawArrays(GL_TRIANGLES, 0, _polygonRenderData->IndexCount);
		//_polygonRenderData->IndexCount = 0;
		////_polygonRenderData->TextureSlotIndex = 1;




		//glUseProgram(_shader->GetRendererID());
		//location = glGetUniformLocation(_shader->GetRendererID(), "u_ViewProjectionMatrix");
		//glUniformMatrix4fv(location, 1, false, glm::value_ptr(_viewProjectionMatrix));



		//glBindVertexArray(_lineRenderData->VertexArray);
		//glDrawArrays(GL_LINES, 0, 2);
		//_lineRenderData->IndexCount = 0;
		//_polygonRenderData->TextureSlotIndex = 1;




	}

}
