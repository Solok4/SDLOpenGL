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
	void SetCanEdit(bool canBe) { this->CanEdit = canBe; };
	virtual void OnEnter(std::string value);
	bool GetCanEdit() { return this->CanEdit; };
	virtual void Tick(double delta) override;

private:
	std::string Value;
	TTF_Font* Font;
	bool CanEdit = false;

};

