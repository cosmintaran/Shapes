#pragma once
#include <glm/gtc/matrix_transform.hpp>
#include <glm/mat4x4.hpp>
#include <wx/event.h>

namespace SV::GS{

	class OrotographicCamera {

	public:
		OrotographicCamera(float left, float right, float bottom, float top, float nearest = -1.0f, float farest = 1.0f);
		void SetProjection(float left, float right, float bottom, float top);
		void SetProjection(const glm::mat4& projection);

		const glm::vec3& GetPosition()const { return m_Position; }
		void SetPosition(const glm::vec3& position) { m_Position = position; RecalculateViewMatrix();}

		[[nodiscard]] float GetRotation()const { return m_Rotation; }
		void SetRotation(float rotation) { m_Rotation = rotation; RecalculateViewMatrix();}

		[[nodiscard]] const glm::mat4& GetProjectionMatrix()const { return m_ProjectionMatrix; }
		[[nodiscard]] const glm::mat4& GetViewMatrix()const { return m_ViewMatrix; }
		[[nodiscard]] const glm::mat4& GetViewProjectionMatrix()const { return m_ViewProjectionMatrix; }
		[[nodiscard]] glm::mat4 GetInversProjection()const { return glm::inverse(m_ProjectionMatrix); }

	private:
		void RecalculateViewMatrix();

	private:

		glm::mat4 m_ProjectionMatrix;
		glm::mat4 m_ViewMatrix;
		glm::mat4 m_ViewProjectionMatrix;

		glm::vec3 m_Position = {0.0f, 0.0f, 0.0f};
		float m_Rotation = 0.0f;
	};
}
