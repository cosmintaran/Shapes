#pragma once
#include "Vertex.h"
#include <array>

namespace SV::GS {

	static constexpr size_t MaxTriangleCount = 10500;
	static constexpr size_t MaxVertexCount   = MaxTriangleCount * 3;
	static constexpr size_t MaxIndexCount    = MaxVertexCount * 3;
	static constexpr size_t MaxTextures      = 32;


	struct BaseRenderData {

		unsigned int VertexArrayRenderID = 0;
		unsigned int VertexBufferRenderID = 0;
		unsigned int IndexBufferRenderID = 0;

		unsigned int WhiteTexture = 0;
		unsigned int WhiteTextureSlot = 0;
		unsigned int VertexCount = 0;

		unsigned int* IndexBufferArray = nullptr;
		unsigned int* IndexBufferArrayPtr   = nullptr;

		virtual ~BaseRenderData() {
			delete[] IndexBufferArray;
		}

	};

	template<typename T>
	struct RenderData 
		: public BaseRenderData
	{
		T* Buffer = nullptr;
		T* BufferPtr = nullptr;
	};


	template<>
	struct RenderData<TexVertex>
		: public BaseRenderData
	{

		TexVertex* Buffer = nullptr;
		TexVertex* BufferPtr = nullptr;

		std::array<unsigned int, MaxTextures> TexturesSlots;
		unsigned int TextureSlotIndex = 1; // Zero is reserved for WhiteTexture

		~RenderData() {
			delete[] Buffer;
			Buffer = nullptr;
			BufferPtr = nullptr;
		}

	};

	template<>
	struct RenderData<Vertex> 
		: public BaseRenderData
	{

		Vertex* Buffer = nullptr;
		Vertex* BufferPtr = nullptr;

		~RenderData() {
			delete[] Buffer;
			Buffer = nullptr;
			BufferPtr = nullptr;
		}

	};
}
