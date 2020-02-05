#pragma once
#include "CButton.h"
#include <string>
class CTextBox :
	public CButton
{
public:
	CTextBox()=delete;
	CTextBox(const char* name, glm::vec2 position, glm::vec2 objsize, CLayout* ref);
	~CTextBox();

	std::string GetValue();
	void SetValue(std::string value);

	virtual void Tick(double delta) override;

private:
	std::string Value;
	TTF_Font* Font;
	bool IsFresh = false;
};

