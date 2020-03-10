#pragma once
#include <vector>
#include <string>
#include "CImage.h"
#include "SDL_ttf.h"
#include "CLabel.h"

class CListBox :
	public CImage
{
public:
	CListBox()=delete;
	CListBox(const char* name, glm::vec2 position, glm::vec2 objsize, CLayout* ref) :CImage(name, position, objsize, ref)
	{
		this->_ID = Object2DType::OBJECT2D_LISTBOX;
		this->Value = std::string("");
		this->_Padding = glm::vec2(10.f);
		this->CreateLabels();
	};
	
	void CreateLabels();
	void SetFont(TTF_Font* fon);
	void SetText(std::string val);
	std::string GetText() { return this->Value; };

	virtual void PostDraw() override;
	~CListBox();


private:
	int FontSize;
	std::string Value;
	std::shared_ptr<CLabel> Label;
};

