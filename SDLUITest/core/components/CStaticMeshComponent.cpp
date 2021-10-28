#include "CStaticMeshComponent.h"
#include "GL/glew.h"
#include <string>
#include "../../CLog.h"
#include "../renderer/COpengl.h"
#include "../managers/CSceneManager.h"
#include "CObject3D.h"

CStaticMeshComponent::CStaticMeshComponent(CObject3D* ref) : CBaseComponent(ref)
{
	this->_CastShadow = true;
	this->_Type = Object3DComponent::STATIC_MESH_COMPONENT;
}

CStaticMeshComponent::CStaticMeshComponent(const CStaticMeshComponent& mesh, CObject3D* ref) :CBaseComponent(mesh, ref)
{
	this->_CastShadow = mesh._CastShadow;
	this->_Model = mesh._Model;
	this->_Type = Object3DComponent::STATIC_MESH_COMPONENT;
	this->AttachParrentObject(this->PossesingObject->GetRootComponent());
}

CStaticMeshComponent::~CStaticMeshComponent()
{
	this->_Model = nullptr;
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
			auto oglShaderClass = OpenGL->GetShadersClass();

			if (step == RenderStep::RenderVerticesOnly)
			{
				program = OpenGL->GetShadersClass()->GetCurrentShaderProgram();
				glBindVertexArray(this->_Model->VAO);
				glEnableVertexAttribArray(MODEL_MESHBUFFER);
				glDrawArrays(GL_TRIANGLES, 0, this->_Model->IndicesCount);
				glDisableVertexAttribArray(MODEL_MESHBUFFER);
				glBindVertexArray(0);
			}
			if (step == RenderStep::RenderJustBuffers)
			{
				program = OpenGL->GetShadersClass()->GetCurrentShaderProgram();
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
				program = OpenGL->GetShadersClass()->GetCurrentShaderProgram();
				glBindVertexArray(this->_Model->VAO);
				glEnableVertexAttribArray(MODEL_MESHBUFFER);
				if (this->_Model->HasTexcords)
					glEnableVertexAttribArray(MODEL_TEXCORDBUFFER);
				if (this->_Model->HasNormals)
					glEnableVertexAttribArray(MODEL_NORMALBUFFER);
				for (auto chunk : this->_Model->RenderingChunks)
				{
					auto matData = chunk->material;
					oglShaderClass->Uniform3f(matData->AmbientColor, "Mat.Ambient");
					oglShaderClass->Uniform3f(matData->DiffuseColor, "Mat.Diffuse");
					oglShaderClass->Uniform3f(matData->SpecularColor, "Mat.Specular");
					oglShaderClass->Uniform1f(matData->Shininess, "Mat.Shininess");
					glActiveTexture(GL_TEXTURE0);
					auto base = matData->DiffuseTex;
					if (!base)
					{
						return;
					}
					glBindTexture(GL_TEXTURE_2D, base);
					glUniform1i(glGetUniformLocation(program, "Base"), 0);
					auto normal = matData->NormalTex;
					if (normal != -1)
					{
						glActiveTexture(GL_TEXTURE1);
						glBindTexture(GL_TEXTURE_2D, normal);
						glUniform1i(glGetUniformLocation(program, "Normal"), 1);
					}
					auto specular = matData->SpecularTex;
					if (specular != -1)
					{
						glActiveTexture(GL_TEXTURE2);
						glBindTexture(GL_TEXTURE_2D, specular);
						glUniform1i(glGetUniformLocation(program, "Specular"), 2);
					}
					glDrawArrays(GL_TRIANGLES, chunk->chunkOffset, chunk->chunkSize);
				}
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