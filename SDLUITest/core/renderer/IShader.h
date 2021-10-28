#pragma once
#include <cstdint>

class IShader {
public:
	virtual uint32_t createShader(uint32_t shaderType) = 0;
	virtual void shaderSource(uint32_t shaderIndex,uint32_t count, const char** shaderContents, int32_t* length) = 0;
	virtual void compileShader(uint32_t shaderIndex) = 0;
	virtual void getShaderIntValue(uint32_t shaderIndex, uint32_t parameter, int32_t* result) = 0;
	virtual void getShaderInfoLog(uint32_t shaderIndex, uint32_t maxLength, int32_t* length, char* result) = 0;

	virtual uint32_t createProgram() = 0;
	virtual void attachShader(uint32_t programIndex, uint32_t shaderIndex) = 0;
	virtual void linkProgram(uint32_t programIndex) = 0;
	virtual void getProgramiv(uint32_t programIndex, uint32_t parameter, int32_t* result) = 0;
	virtual void detachShader(uint32_t programIndex, uint32_t shaderIndex) = 0;
	virtual void deleteShader(uint32_t shaderType) = 0;
	virtual void useProgram(uint32_t programIndex) = 0;

	virtual uint32_t getUniformLocation(uint32_t programIndex, const char* uniformName) = 0;
	
	virtual void uniformMatrix4fv(uint32_t uniformIndex, uint32_t count, bool transpose, float* value) = 0;
	virtual void uniform1f(uint32_t uniformIndex, float value) = 0;
	virtual void uniform3f(uint32_t uniformIndex, float v0, float v1, float v2) = 0;

	virtual void uniform1i(uint32_t uniformIndex, int value) = 0;
};