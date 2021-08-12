#pragma once
#include "BaseTemplateControllerCore.h"

class ControllerCoreForString : public BaseTemplateControllerCore<string>
{
public:
	using BaseTemplateControllerCore::BaseTemplateControllerCore;

	virtual bool randomiseTree(int size);

protected:
	

	// Inherited via BaseTemplateControllerCore
	std::string processGetValueFromStr(std::string str);

};

