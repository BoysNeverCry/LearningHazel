#include <Hazel.h>
#include <stdio.h>

#include "template.h"
#include <string>


class Sandbox : public Hazel::Application
{
public:
	Sandbox()
	{

	}

	~Sandbox()
	{

	}

};

Hazel::Application* Hazel::CreateApplication()
{
	sum<float>(1, 2);
	return new Sandbox();
}

