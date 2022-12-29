#include "GLSLShader.h"
#include "glm.hpp"

uint32_t GLSLShader::createShader(uint32_t shaderType) {
	return glCreateShader(shaderType);
}
void GLSLShader::shaderSource(uint32_t shaderIndex, uint32_t count, const char** shaderContents, int32_t* length) {
	glShaderSource(shaderIndex, count ,shaderContents,length);
}
void GLSLShader::compileShader(uint32_t shaderIndex) {
	glCompileShader(shaderIndex);
}
void GLSLShader::getShaderIntValue(uint32_t shaderIndex, uint32_t parameter, int32_t* result) {
	glGetShaderiv(shaderIndex, parameter, result);
}
void GLSLShader::getShaderInfoLog(uint32_t shaderIndex, uint32_t maxLength, int32_t* length, char* result) {
	glGetShaderInfoLog(shaderIndex, maxLength, length, result);
}
uint32_t GLSLShader::createProgram() {
	return glCreateProgram();
}
void GLSLShader::attachShader(uint32_t programIndex, uint32_t shaderIndex) {
	glAttachShader(programIndex, shaderIndex);
}
void GLSLShader::linkProgram(uint32_t programIndex) {
	glLinkProgram(programIndex);
}
void GLSLShader::getProgramiv(uint32_t programIndex, uint32_t parameter, int32_t* result) {
	glGetProgramiv(programIndex, parameter, result);
}
void GLSLShader::detachShader(uint32_t programIndex, uint32_t shaderIndex) {
	glDetachShader(programIndex, shaderIndex);
}
void GLSLShader::deleteShader(uint32_t shaderType) {
	glDeleteShader(shaderType);
}
void GLSLShader::useProgram(uint32_t programIndex) {
	glUseProgram(programIndex);
}

uint32_t GLSLShader::getUniformLocation(uint32_t programIndex, const char* uniformName) {
	return glGetUniformLocation(programIndex, uniformName);
}

void GLSLShader::uniformMatrix4fv(uint32_t uniformIndex, uint32_t count, bool transpose, float* value) {
	glUniformMatrix4fv(uniformIndex, count, transpose, value);
}
void GLSLShader::uniform1f(uint32_t uniformIndex, float value) {
	glUniform1f(uniformIndex, value);
}
void GLSLShader::uniform3f(uint32_t uniformIndex, float v0, float v1, float v2) {
	glUniform3f(uniformIndex, v0, v1, v2);
}

void GLSLShader::uniform1i(uint32_t uniformIndex, int value) {
	glUniform1i(uniformIndex, value);
}