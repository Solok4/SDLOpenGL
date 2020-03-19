#pragma once
#include "CButton.h"
#include <string>
#include <functional>
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
	void BindEnterFunc(std::function<void()> func);
	void OnEnter();
	bool GetCanEdit() { return this->CanEdit; };
	virtual void Tick(double delta) override;

private:
	std::string Value;
	TTF_Font* Font;
	bool CanEdit = false;
	std::function<void()> EnterFunc;

};

