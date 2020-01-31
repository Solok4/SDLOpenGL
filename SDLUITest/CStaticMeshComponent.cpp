#include "pch.h"
#include "CStaticMeshComponent.h"
#include "GL/glew.h"
#include <string>
#include "CLog.h"
#include "COpengl.h"
#include "CSceneManager.h"


CStaticMeshComponent::CStaticMeshComponent()
{
	this->_CastShadow = true;
}

CStaticMeshComponent::CStaticMeshComponent(const CStaticMeshComponent& mesh):CBaseComponent(mesh)
{
	this->_Model = mesh._Model;
}

CStaticMeshComponent::CStaticMeshComponent(const std::shared_ptr<CStaticMeshComponent>& mesh):CBaseComponent(mesh)
{
	this->_Model = mesh->_Model;
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

void CStaticMeshComponent::Draw(DrawType DType)
{
	if (this->_IsActive)
	{
		if (this->_Model != nullptr)
		{
			int program = 0;
			if (DType == DrawType::FullDraw)
			{
				program = OpenGL->GetShadersClass().GetCurrentShaderProgram();
				glBindVertexArray(this->_Model->VAO);
//#ifdef HD4850 
				glEnableVertexAttribArray(MODEL_MESHBUFFER);
				if (this->_Model->HasTexcords)
					glEnableVertexAttribArray(MODEL_TEXCORDBUFFER);
				if (this->_Model->HasNormals)
					glEnableVertexAttribArray(MODEL_NORMALBUFFER);
				glActiveTexture(GL_TEXTURE0);
				//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->_Model->EBO);
				glBindTexture(GL_TEXTURE_2D, this->_Model->Mat->Tex[this->_Model->Mat->BaseTexIndex]);
				glUniform1i(glGetUniformLocation(program, "Base"), 0);
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
				glDrawArrays(GL_TRIANGLES, 0, this->_Model->IndicesCount);
				if (this->_Model->HasNormals)
					glDisableVertexAttribArray(MODEL_NORMALBUFFER);
				if (this->_Model->HasTexcords)
					glDisableVertexAttribArray(MODEL_TEXCORDBUFFER);
				glDisableVertexAttribArray(MODEL_MESHBUFFER);
//#else
//				glEnableVertexAttribArray(0);
//				if (this->_Model->HasTexcords)
//					glEnableVertexAttribArray(1);
//				if (this->_Model->HasNormals)
//					glEnableVertexAttribArray(2);
//				glActiveTexture(GL_TEXTURE0);
//				//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->_Model->EBO);
//				glBindTexture(GL_TEXTURE_2D, this->_Model->Mat->Tex[this->_Model->Mat->BaseTexIndex]);
//				glUniform1i(glGetUniformLocation(program, "Base"), 0);
//				if (this->_Model->Mat->NormalMapIndex != 255)
//				{
//					glActiveTexture(GL_TEXTURE1);
//					glBindTexture(GL_TEXTURE_2D, this->_Model->Mat->Tex[this->_Model->Mat->NormalMapIndex]);
//					glUniform1i(glGetUniformLocation(program, "Normal"), 1);
//				}
//				if (this->_Model->Mat->SpecularMapIndex != 255)
//				{
//					glActiveTexture(GL_TEXTURE2);
//					glBindTexture(GL_TEXTURE_2D, this->_Model->Mat->Tex[this->_Model->Mat->SpecularMapIndex]);
//					glUniform1i(glGetUniformLocation(program, "Specular"), 2);
//				}
//				//glDrawElements(GL_STATIC_DRAW, this->_Model->IndicesCount, GL_UNSIGNED_INT, 0);
//				glDrawArrays(GL_TRIANGLES, 0, this->_Model->IndicesCount);
//				if (this->_Model->HasNormals)
//					glDisableVertexAttribArray(2);
//				if (this->_Model->HasTexcords)
//					glDisableVertexAttribArray(1);
//				glDisableVertexAttribArray(0);
//#endif
				glBindVertexArray(0);
			}
			else if (DType == DrawType::VerticesOnly)
			{
				program = OpenGL->GetShadersClass().GetCurrentShaderProgram();
				glBindVertexArray(this->_Model->VAO);
//#ifdef HD4850
				glEnableVertexAttribArray(MODEL_MESHBUFFER);
				glDrawArrays(GL_TRIANGLES, 0, this->_Model->IndicesCount);
				glDisableVertexAttribArray(MODEL_MESHBUFFER);
//#else
//				glEnableVertexAttribArray(0);
//				glDrawArrays(GL_TRIANGLES, 0, this->_Model->IndicesCount);
//				glDisableVertexAttribArray(0);
//#endif
				glBindVertexArray(0);
			}
		}
	}
}


