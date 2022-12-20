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

		void PushLayer(Layer* layer);//layer �����뵽LayerStack��ǰһ��
		void PushOverlay(Layer* overlay);//overlay�����뵽LayerStack�ĺ�һ�룬����������һ��������ָ��m_LayerInsertά��ǰһ��LayerStack�Ĳ���λ��
										//֮��������������Ϊ����overlay������ϣ����������layer֮�ϣ�������Ⱦ���̵����
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


