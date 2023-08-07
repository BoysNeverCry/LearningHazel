#pragma once
#include "Entity.h"

namespace Hazel {

	class ScriptableEntity
	{
		private:
			Entity m_Entity;
			friend class Scene;
		public:
			template<typename T>
			T& GetComponent()
			{
				return m_Entity.GetComponent<T>();
			}
			virtual ~ScriptableEntity() {}

		protected:
			virtual void OnCreate() {}
			virtual void OnDestroy() {}
			virtual void OnUpdate(Timestep ts) {}


	};
}