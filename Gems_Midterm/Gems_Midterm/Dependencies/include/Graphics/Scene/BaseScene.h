#pragma once

#include <string.h>
#include "../Debugger.h" 

namespace Scene
{
	class BaseScene
	{
	public:

		std::string mName = "UnNamed";

		virtual ~BaseScene() {} ;
		virtual void Start() = 0;
		virtual void Update() = 0;
		virtual void Render() = 0;
		virtual void Cleanup();
	};
}


