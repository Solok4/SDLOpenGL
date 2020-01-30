#pragma once

#include <functional>
#include <string>
//#include <variant>

enum CvarType
{
	CVAR_VOID=0,
	CVAR_INTEGER,
	CVAR_DOUBLE,
	CVAR_STRING,
	CVAR_MAX,
};

union CVarValue
{
	CVarValue() { IntValue = 0; };
	int IntValue;
	double DoubleValue;
	char* StringValue;
	~CVarValue() {};
};

class CVar
{
public:
	CVar()=delete;
	CVar(const char* name, int defaultvalue, const char* hint) :_Name(name),  _Hint(hint)
	{
		this->_Type = CVAR_INTEGER;
		this->_DefaultValue = std::make_shared<CVarValue>();
		this->_CurrentValue = std::make_shared<CVarValue>();
		this->_DefaultValue->IntValue = defaultvalue;
		this->_CurrentValue->IntValue = defaultvalue;
	};

	CVar(const char* name, double defaultvalue, const char* hint) :_Name(name), _Hint(hint)
	{
		this->_Type = CVAR_DOUBLE;
		this->_DefaultValue = std::make_shared<CVarValue>();
		this->_CurrentValue = std::make_shared<CVarValue>();
		this->_DefaultValue->DoubleValue = defaultvalue;
		this->_CurrentValue->DoubleValue = defaultvalue;
	};

	CVar(const char* name, std::string defaultvalue, const char* hint) :_Name(name), _Hint(hint)
	{
		this->_Type = CVAR_STRING;
		this->_DefaultValue = std::make_shared<CVarValue>();
		this->_CurrentValue = std::make_shared<CVarValue>();
		this->_DefaultValue->StringValue = (char*)defaultvalue.c_str();
		this->_CurrentValue->StringValue = (char*)defaultvalue.c_str();
	};
	void AttachFunction(std::function<void(CVar)> func) { this->_ChangeValueFunc = func; };
	virtual inline CvarType GetDataType() { return this->_Type; };
	virtual inline const char* GetName() { return this->_Name; };
	virtual inline const char* GetHint() { return this->_Hint; };
	virtual void SetCurrentValue(int value);
	virtual void SetCurrentValue(double value);
	virtual void SetCurrentValue(std::string value);
	~CVar();

private:

	const char* _Name;
	CvarType _Type;
	std::shared_ptr<CVarValue> _DefaultValue, _CurrentValue;
	const char* _Hint;

	std::function<void(CVar)> _ChangeValueFunc;
};

