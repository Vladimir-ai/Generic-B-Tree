#pragma once
#include "BaseTemplateControllerCore.h"

class ControllerCoreForIntTree : public BaseTemplateControllerCore<int> {
public:
	using BaseTemplateControllerCore::BaseTemplateControllerCore;

	virtual bool randomiseTree(int size) override;

protected:
	int processGetValueFromStr(std::string str) override;

};

