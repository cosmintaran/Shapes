#pragma once

#include "Vertex.h"
#include <glad/glad.h>

namespace SV::GS {

    struct IndexBufferContainer {

        std::vector<unsigned int> IndexTrianglesBuffer;
        std::vector<unsigned int> IndexLinesBuffer;
        unsigned int IndexTrianglesBufferRenderID = 0;
        unsigned int IndexLinesBufferRenderID = 0;

    };

    template <class T>
    class IRenderShape {


    };

    template <>
    class IRenderShape<Vertex> {

    public:
        virtual void vDraw(DeviceContext& deviceContext)const = 0;

        virtual ~IRenderShape() {

            glDeleteVertexArrays(1, &_vertexArrayRenderID);
            glDeleteBuffers(1, &_bufferVertexRenderID);
            glDeleteBuffers(1, &indeciesContainer.IndexLinesBufferRenderID);
            glDeleteBuffers(1, &indeciesContainer.IndexTrianglesBufferRenderID);
        }

        virtual void GraphicsInitialization() {

            glGenVertexArrays(1, &_vertexArrayRenderID);
            glBindVertexArray(_vertexArrayRenderID);

            glGenBuffers(1, &_bufferVertexRenderID);
            glBindBuffer(GL_ARRAY_BUFFER, _bufferVertexRenderID);
            glBufferData(GL_ARRAY_BUFFER, _vertices.size() * sizeof(Vertex), &_vertices[0], GL_DYNAMIC_DRAW);

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
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, Position));

            //glEnableVertexAttribArray(1);
            //glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, Color));

            glBindVertexArray(0);
            glBindBuffer(GL_ARRAY_BUFFER, 0);

        }

    protected:
        std::vector<Vertex> _vertices;
        unsigned int _bufferVertexRenderID = 0;
        unsigned int _vertexArrayRenderID = 0;
        IndexBufferContainer indeciesContainer;
    };
}
