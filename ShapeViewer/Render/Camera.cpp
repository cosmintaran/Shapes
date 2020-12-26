#include"stdafx.h"
#include"Camera.h"

namespace SV::GS {

	OrotographicCamera::OrotographicCamera(float left, float right, float bottom, float top, float nearest /*= -1.0f*/, float farest /*= 1.0f*/) 
		:m_ProjectionMatrix(glm::ortho(left,right,bottom,top,nearest, farest))
		,m_ViewMatrix(glm::ortho(left, right, bottom, top, nearest, farest))
	{
		RecalculateViewMatrix();
	}

	void OrotographicCamera::SetProjection(float left, float right, float bottom, float top)
	{
		m_ProjectionMatrix = glm::ortho(left, right, bottom, top, -1.0f,1.0f);
		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}

	void OrotographicCamera::RecalculateViewMatrix()
	{
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), m_Position) * glm::rotate(glm::mat4(1.0f), m_Rotation, glm::vec3(0.0f, 0.0f, 1.0f));
		m_ViewMatrix = glm::inverse(transform);
		m_ViewProjectionMatrix = m_ProjectionMatrix* m_ViewMatrix;
	}

}