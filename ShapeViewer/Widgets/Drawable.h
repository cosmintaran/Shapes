#pragma once

#include "Render/DeviceContext.h"
#include "Render/Vertex.h"
#include "Layer.h"

namespace SV::Shapes {

	struct IndexBufferContainer {

		std::vector<unsigned int> IndexTrianglesBuffer;
		std::vector<unsigned int> IndexLinesBuffer;
		unsigned int IndexTrianglesBufferRenderID = 0;
		unsigned int IndexLinesBufferRenderID = 0;

	};

	class Drawable {

	public:
        Drawable(Layer* layer) :
            _layer(layer) {}

		virtual void vDraw (GS::DeviceContext& deviceContext) = 0;

        virtual ~Drawable() {
            glDeleteVertexArrays(1, &_vertexArrayRenderID);
            glDeleteBuffers(1, &_bufferVertexRenderID);
            glDeleteBuffers(1, &indeciesContainer.IndexLinesBufferRenderID);
            glDeleteBuffers(1, &indeciesContainer.IndexTrianglesBufferRenderID);
        }

    protected:
        virtual void GraphicsInitialization() {

            glGenVertexArrays(1, &_vertexArrayRenderID);
            glBindVertexArray(_vertexArrayRenderID);

            glGenBuffers(1, &_bufferVertexRenderID);
            glBindBuffer(GL_ARRAY_BUFFER, _bufferVertexRenderID);
            glBufferData(GL_ARRAY_BUFFER, _vertices.size() * sizeof(GS::Vertex), &_vertices[0], GL_DYNAMIC_DRAW);

            if (indeciesContainer.IndexTrianglesBuffer.size() > 0) {
                glGenBuffers(1, &indeciesContainer.IndexTrianglesBufferRenderID);
                glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indeciesContainer.IndexTrianglesBufferRenderID);
                glBufferData(GL_ELEMENT_ARRAY_BUFFER, indeciesContainer.IndexTrianglesBuffer.size() * sizeof(unsigned int), &indeciesContainer.IndexTrianglesBuffer.front(), GL_DYNAMIC_DRAW);
                glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
            }

            if (indeciesContainer.IndexLinesBuffer.size() > 0) {
                glGenBuffers(1, &indeciesContainer.IndexLinesBufferRenderID);
                glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indeciesContainer.IndexLinesBufferRenderID);
                glBufferData(GL_ELEMENT_ARRAY_BUFFER, indeciesContainer.IndexLinesBuffer.size() * sizeof(unsigned int), &indeciesContainer.IndexLinesBuffer.front(), GL_DYNAMIC_DRAW);
                glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
            }

            glEnableVertexAttribArray(0);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GS::Vertex), (const void*)offsetof(GS::Vertex, Position));

            glBindVertexArray(0);
            glBindBuffer(GL_ARRAY_BUFFER, 0);
            _isGraphicsEnabled = true;
        }
        virtual void PrepareToDraw() = 0;
	protected:
		std::vector<GS::Vertex> _vertices;
		Layer* _layer = nullptr;
		unsigned int _bufferVertexRenderID = 0;
		unsigned int _vertexArrayRenderID = 0;
		IndexBufferContainer indeciesContainer;
		bool _isGraphicsEnabled = false;
	};
}
