#pragma once

#include "Hazel/Core/Input.h"

namespace Hazel {

	class WindowsInput :public Input
	{
	protected:
		virtual bool IsKeyPressedImpl(int keycode) override;//对于不同平台有不同的实现

		virtual bool IsMouseButtonPressedImpl(int button) override;
		virtual std::pair<float,float> GetMousePositionImpl() override;
		virtual float GetMouseXImpl() override;
		virtual float GetMouseYImpl() override;
	};

}