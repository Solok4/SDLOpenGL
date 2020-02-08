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

void CStaticMeshComponent::Draw(RenderStep step)
{
	if (this->_IsActive)
	{
		if (this->_Model != nullptr)
		{
			int program = 0;

			if (step == RenderStep::RenderVerticesOnly)
			{
				program = OpenGL->GetShadersClass().GetCurrentShaderProgram();
				glBindVertexArray(this->_Model->VAO);
				glEnableVertexAttribArray(MODEL_MESHBUFFER);
				glDrawArrays(GL_TRIANGLES, 0, this->_Model->IndicesCount);
				glDisableVertexAttribArray(MODEL_MESHBUFFER);
				glBindVertexArray(0);
			}
			else if (step == RenderStep::RenderDiffuse)
			{
				program = OpenGL->GetShadersClass().GetCurrentShaderProgram();
				glBindVertexArray(this->_Model->VAO);
				glEnableVertexAttribArray(MODEL_MESHBUFFER);
				if (this->_Model->HasTexcords)
					glEnableVertexAttribArray(MODEL_TEXCORDBUFFER);
				if (this->_Model->HasNormals)
					glEnableVertexAttribArray(MODEL_NORMALBUFFER);
				glActiveTexture(GL_TEXTURE0);
				auto base = this->_Model->Mat->GetTextureByType(TextureTypes::BaseTex);
				if (!base)
				{
					return;
				}
				glBindTexture(GL_TEXTURE_2D, base);
				glUniform1i(glGetUniformLocation(program, "Base"), 0);
				glDrawArrays(GL_TRIANGLES, 0, this->_Model->IndicesCount);
				if (this->_Model->HasNormals)
					glDisableVertexAttribArray(MODEL_NORMALBUFFER);
				if (this->_Model->HasTexcords)
					glDisableVertexAttribArray(MODEL_TEXCORDBUFFER);
				glDisableVertexAttribArray(MODEL_MESHBUFFER);

				glBindVertexArray(0);
			}
			else if (step == RenderStep::RenderNormal)
			{
				program = OpenGL->GetShadersClass().GetCurrentShaderProgram();
				glBindVertexArray(this->_Model->VAO);
				glEnableVertexAttribArray(MODEL_MESHBUFFER);
				if (this->_Model->HasTexcords)
					glEnableVertexAttribArray(MODEL_TEXCORDBUFFER);
				if (this->_Model->HasNormals)
					glEnableVertexAttribArray(MODEL_NORMALBUFFER);
				glActiveTexture(GL_TEXTURE0);
				auto Normal = this->_Model->Mat->GetTextureByType(TextureTypes::NormalMap);
				if (!Normal)
				{
					return;
				}
				glBindTexture(GL_TEXTURE_2D, Normal);
				glUniform1i(glGetUniformLocation(program, "Normal"), 0);
				glDrawArrays(GL_TRIANGLES, 0, this->_Model->IndicesCount);
				if (this->_Model->HasNormals)
					glDisableVertexAttribArray(MODEL_NORMALBUFFER);
				if (this->_Model->HasTexcords)
					glDisableVertexAttribArray(MODEL_TEXCORDBUFFER);
				glDisableVertexAttribArray(MODEL_MESHBUFFER);

				glBindVertexArray(0);
			}
			else if (step == RenderStep::RenderSpecular)
			{
				program = OpenGL->GetShadersClass().GetCurrentShaderProgram();
				glBindVertexArray(this->_Model->VAO);
				glEnableVertexAttribArray(MODEL_MESHBUFFER);
				if (this->_Model->HasTexcords)
					glEnableVertexAttribArray(MODEL_TEXCORDBUFFER);
				if (this->_Model->HasNormals)
					glEnableVertexAttribArray(MODEL_NORMALBUFFER);
				glActiveTexture(GL_TEXTURE0);
				auto Specular = this->_Model->Mat->GetTextureByType(TextureTypes::SpecularMap);
				if (!Specular)
				{
					return;
				}
				glBindTexture(GL_TEXTURE_2D, Specular);
				glUniform1i(glGetUniformLocation(program, "Specular"), 0);
				glDrawArrays(GL_TRIANGLES, 0, this->_Model->IndicesCount);
				if (this->_Model->HasNormals)
					glDisableVertexAttribArray(MODEL_NORMALBUFFER);
				if (this->_Model->HasTexcords)
					glDisableVertexAttribArray(MODEL_TEXCORDBUFFER);
				glDisableVertexAttribArray(MODEL_MESHBUFFER);

				glBindVertexArray(0);
			}
			else if (step == RenderStep::RenderVerticesOnly)
			{
				program = OpenGL->GetShadersClass().GetCurrentShaderProgram();
				glBindVertexArray(this->_Model->VAO);
				glEnableVertexAttribArray(MODEL_MESHBUFFER);
				glDrawArrays(GL_TRIANGLES, 0, this->_Model->IndicesCount);
				glDisableVertexAttribArray(MODEL_MESHBUFFER);
				glBindVertexArray(0);
			}
		}
	}
}


