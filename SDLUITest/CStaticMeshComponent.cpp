#include "pch.h"
#include "CStaticMeshComponent.h"
#include "GL/glew.h"
#include <string>


CStaticMeshComponent::CStaticMeshComponent()
{
}


CStaticMeshComponent::~CStaticMeshComponent()
{
}

void CStaticMeshComponent::BindModel(std::shared_ptr<Model> model)
{
	this->_Model = model;
}

std::shared_ptr<Model> CStaticMeshComponent::GetModel()
{
	return this->_Model;
}

void CStaticMeshComponent::Draw(int program)
{
	if (this->_Model != nullptr)
	{
		glBindVertexArray(this->_Model->VAO);
		glEnableVertexAttribArray(0);
		if(this->_Model->HasTexcords)
			glEnableVertexAttribArray(1);
		if (this->_Model->HasNormals)
			glEnableVertexAttribArray(2);
		glActiveTexture(GL_TEXTURE0);
		//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->_Model->EBO);
		glBindTexture(GL_TEXTURE_2D, this->_Model->Mat->Tex[this->_Model->Mat->BaseTexIndex]);
		glUniform1i(glGetUniformLocation(program, "Base"),0);
		if (this->_Model->Mat->NormalMapIndex != 255)
		{
			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, this->_Model->Mat->Tex[this->_Model->Mat->NormalMapIndex]);
			glUniform1i(glGetUniformLocation(program, "Normal"), 1);						
		}
		if (this->_Model->Mat->SpecularMapIndex != 255)
		{
			glActiveTexture(GL_TEXTURE2);
			glBindTexture(GL_TEXTURE_2D, this->_Model->Mat->Tex[this->_Model->Mat->SpecularMapIndex]);
			glUniform1i(glGetUniformLocation(program, "Specular"), 2);
		}
		//glDrawElements(GL_STATIC_DRAW, this->_Model->IndicesCount, GL_UNSIGNED_INT, 0);
		glDrawArrays(GL_TRIANGLES, 0, this->_Model->IndicesCount);
		if (this->_Model->HasNormals)
			glDisableVertexAttribArray(2);
		if (this->_Model->HasTexcords)
			glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(0);
		glBindVertexArray(0);
	}
}


