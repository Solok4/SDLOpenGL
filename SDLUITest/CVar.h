#pragma once

#include <functional>
#include <string>
#include <variant>

enum CvarType
{
	CVAR_VOID=0,
	CVAR_BOOL,
	CVAR_INTEGER,
	CVAR_FLOAT,
	CVAR_STRING,
	CVAR_MAX,
};

//class ICvar
//{
//public:
//	virtual CvarType GetDataType() {};
//	virtual char* GetName() {};
//	virtual char* GetHint() {};
//	virtual void SetCurrentValue(std::string value) {};
//};
//
//template <class T>
class CVar
{
public:
	CVar()=delete;
	//CVar(char* name,std::variant<bool,int,float> defaultvalue, char* hint) :_Name(name), _DefaultValue(defaultvalue), _Hint(hint)
	//{
	//	if (std::holds_alternative<bool>(defaultvalue))
	//	{
	//		this->_Type = CvarType::CVAR_BOOL;
	//		return;
	//	}
	//	if (std::holds_alternative<int>(defaultvalue))
	//	{
	//		this->_Type = CvarType::CVAR_INTEGER;
	//		return;
	//	}

	//	if (std::holds_alternative<float>(defaultvalue))
	//	{
	//		this->_Type = CvarType::CVAR_FLOAT;
	//		return;
	//	}
	//	/*if (std::holds_alternative<char*>(defaultvalue))
	//	{
	//		this->_Type = CvarType::CVAR_STRING;
	//		return;
	//	}*/
	//	this->_Type = CvarType::CVAR_VOID;
	//};
	//void AttachFunction(std::function<void(std::variant<bool, int, float>)> func) { this->_ChangeValueFunc = func; };
	virtual inline CvarType GetDataType() { return this->_Type; };
	virtual inline char* GetName() { return this->_Name; };
	virtual inline char* GetHint() { return this->_Hint; };
	//virtual void SetCurrentValue(std::variant<bool, int, float> value);
	~CVar();

private:

	char* _Name;
	CvarType _Type;
	//std::variant<bool, int, float>_DefaultValue, _CurrentValue;
	char* _Hint;

	//std::function<void(std::variant<bool, int, float>)> _ChangeValueFunc;
};

