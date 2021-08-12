#pragma once
#include "BaseTemplateControllerCore.h"

class ControllerCoreForCharTree : public BaseTemplateControllerCore<char> {
public:
	using BaseTemplateControllerCore::BaseTemplateControllerCore;

	virtual bool randomiseTree(int size) override;

protected:
	virtual char processGetValueFromStr(std::string str) override;
};

