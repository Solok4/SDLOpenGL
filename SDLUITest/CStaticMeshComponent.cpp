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
			if (step == RenderStep::RenderJustBuffers)
			{
				program = OpenGL->GetShadersClass().GetCurrentShaderProgram();
				glBindVertexArray(this->_Model->VAO);
				glBindVertexArray(this->_Model->VAO);
				glEnableVertexAttribArray(MODEL_MESHBUFFER);
				if (this->_Model->HasTexcords)
					glEnableVertexAttribArray(MODEL_TEXCORDBUFFER);
				if (this->_Model->HasNormals)
					glEnableVertexAttribArray(MODEL_NORMALBUFFER);
				glDrawArrays(GL_TRIANGLES, 0, this->_Model->IndicesCount);
				if (this->_Model->HasNormals)
					glDisableVertexAttribArray(MODEL_NORMALBUFFER);
				if (this->_Model->HasTexcords)
					glDisableVertexAttribArray(MODEL_TEXCORDBUFFER);
				glDisableVertexAttribArray(MODEL_MESHBUFFER);
				glBindVertexArray(0);
			}
			else if (step == RenderStep::RenderDeferred)
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
				auto normal = this->_Model->Mat->GetTextureByType(TextureTypes::NormalMap);
				if (normal != -1)
				{
					glActiveTexture(GL_TEXTURE1);
					glBindTexture(GL_TEXTURE_2D, normal);
					glUniform1i(glGetUniformLocation(program, "Normal"), 1);
				}
				auto specular = this->_Model->Mat->GetTextureByType(TextureTypes::SpecularMap);
				if (specular != -1)
				{
					glActiveTexture(GL_TEXTURE2);
					glBindTexture(GL_TEXTURE_2D, specular);
					glUniform1i(glGetUniformLocation(program, "Specular"), 2);
				}
				glDrawArrays(GL_TRIANGLES, 0, this->_Model->IndicesCount);
				if (this->_Model->HasNormals)
					glDisableVertexAttribArray(MODEL_NORMALBUFFER);
				if (this->_Model->HasTexcords)
					glDisableVertexAttribArray(MODEL_TEXCORDBUFFER);
				glDisableVertexAttribArray(MODEL_MESHBUFFER);

				glBindVertexArray(0);
			}
		}
	}
}


