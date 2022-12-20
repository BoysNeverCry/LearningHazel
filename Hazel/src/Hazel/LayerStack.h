#pragma once
#include "Hazel/Core.h"
#include "Layer.h"

#include <vector>


namespace Hazel {

	//It is actually a wrapper over the vector of Layers. The Layer stack is owned by the application. The Layers will be deallocated when the app shuts down,
	class HAZEL_API LayerStack 
	{
	public:
		LayerStack();
		~LayerStack();

		void PushLayer(Layer* layer);//layer 被插入到LayerStack的前一半
		void PushOverlay(Layer* overlay);//overlay被插入到LayerStack的后一半，所以这里用一个单独的指针m_LayerInsert维护前一半LayerStack的插入位置
										//之所以这样做是因为对于overlay，我们希望它总是在layer之上，整个渲染过程的最后
		void PopLayer(Layer* layer);
		void PopOverlay(Layer* overlay);

		std::vector<Layer*>::iterator begin() { return m_Layers.begin(); } 
		std::vector<Layer*>::iterator end() { return m_Layers.end(); }
	private:
		std::vector<Layer*> m_Layers;
		//std::vector<Layer*>::iterator m_LayerInsert;
		unsigned int m_LayerInsertIndex = 0;


	};

}


