#pragma once
#include "glm/glm.hpp"

namespace Hazel {

	class Camera
	{
		private:
			glm::mat4 m_Projection;
			
		public:
			Camera() = default;
			Camera(const glm::mat4& projection)
				:	m_Projection(projection) {}

			glm::mat4 GetProjection() const{ return m_Projection;}

	};
}