#pragma once
#include "IShader.h"
class GLSLShader :
    public IShader
{
	uint32_t createShader(uint32_t shaderType) override;
	void shaderSource(uint32_t shaderIndex, uint32_t count, const char** shaderContents, int32_t* length) override;
	void compileShader(uint32_t shaderIndex) override;
	void getShaderIntValue(uint32_t shaderIndex, uint32_t parameter, int32_t* result) override;
	void getShaderInfoLog(uint32_t shaderIndex, uint32_t maxLength, int32_t* length, char* result) override;

	uint32_t createProgram() override;
	void attachShader(uint32_t programIndex, uint32_t shaderIndex) override;
	void linkProgram(uint32_t programIndex) override;
	void getProgramiv(uint32_t programIndex, uint32_t parameter, int32_t* result) override;
	void detachShader(uint32_t programIndex, uint32_t shaderIndex) override;
	void deleteShader(uint32_t shaderType) override;
	void useProgram(uint32_t programIndex) override;

	uint32_t getUniformLocation(uint32_t programIndex, const char* uniformName) override;

	void uniformMatrix4fv(uint32_t uniformIndex, uint32_t count, bool transpose, float* value) override;
	void uniform1f(uint32_t uniformIndex, float value) override;
	void uniform3f(uint32_t uniformIndex, float v0, float v1, float v2) override;

	void uniform1i(uint32_t uniformIndex, int value) override;
};

