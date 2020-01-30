#include "CSpriteComponent.h"
#include "CSceneManager.h"



CSpriteComponent::CSpriteComponent()
{
	this->_SpritePlane = -1;
}


CSpriteComponent::~CSpriteComponent()
{
}

void CSpriteComponent::PreDraw()
{
	if (this->_SpBehaviour == SpriteBehaviour::KEEP_ROTATION)
	{
		return;
	}
	else if (this->_SpBehaviour == SpriteBehaviour::EIGHT_DIRECTIONS)
	{
		auto camera = SceneManager->GetCurrentScene()->GetCamera();

		float DotProduct = glm::dot(camera->GetRotation(), this->GetRotation());
	}
	else if (this->_SpBehaviour == SpriteBehaviour::ALWAYS_LOOK_AT_CAMERA)
	{
		auto camera = SceneManager->GetCurrentScene()->GetCamera();
		auto DesiredRotation = camera->GetRotation();
		DesiredRotation.y += 180.f;
		this->SetRotation(DesiredRotation);
	}
}

void CSpriteComponent::Draw(DrawType dt)
{
	if (this->_SpritePlane != -1)
	{
		int program = 0;
		if (dt == DrawType::FullDraw)
		{
			program = OpenGL->GetShadersClass().GetCurrentShaderProgram();
//			glBindVertexArray(this->_SpritePlane);
//#ifdef HD4850 
//			glEnableVertexAttribArray(1);
//			if (this->_Model->HasTexcords)
//				glEnableVertexAttribArray(2);
//			if (this->_Model->HasNormals)
//				glEnableVertexAttribArray(0);
//			glActiveTexture(GL_TEXTURE0);
//			//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->_Model->EBO);
//			glBindTexture(GL_TEXTURE_2D, this->_Model->Mat->Tex[this->_Model->Mat->BaseTexIndex]);
//			glUniform1i(glGetUniformLocation(program, "Base"), 0);
//			if (this->_Model->Mat->NormalMapIndex != 255)
//			{
//				glActiveTexture(GL_TEXTURE1);
//				glBindTexture(GL_TEXTURE_2D, this->_Model->Mat->Tex[this->_Model->Mat->NormalMapIndex]);
//				glUniform1i(glGetUniformLocation(program, "Normal"), 1);
//			}
//			if (this->_Model->Mat->SpecularMapIndex != 255)
//			{
//				glActiveTexture(GL_TEXTURE2);
//				glBindTexture(GL_TEXTURE_2D, this->_Model->Mat->Tex[this->_Model->Mat->SpecularMapIndex]);
//				glUniform1i(glGetUniformLocation(program, "Specular"), 2);
//			}
//			glDrawArrays(GL_TRIANGLES, 0, this->_Model->IndicesCount);
//			if (this->_Model->HasNormals)
//				glDisableVertexAttribArray(0);
//			if (this->_Model->HasTexcords)
//				glDisableVertexAttribArray(2);
//			glDisableVertexAttribArray(1);
//#else
//			glEnableVertexAttribArray(0);
//			if (this->_Model->HasTexcords)
//				glEnableVertexAttribArray(1);
//			if (this->_Model->HasNormals)
//				glEnableVertexAttribArray(2);
//			glActiveTexture(GL_TEXTURE0);
//			//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->_Model->EBO);
//			glBindTexture(GL_TEXTURE_2D, this->_Model->Mat->Tex[this->_Model->Mat->BaseTexIndex]);
//			glUniform1i(glGetUniformLocation(program, "Base"), 0);
//			if (this->_Model->Mat->NormalMapIndex != 255)
//			{
//				glActiveTexture(GL_TEXTURE1);
//				glBindTexture(GL_TEXTURE_2D, this->_Model->Mat->Tex[this->_Model->Mat->NormalMapIndex]);
//				glUniform1i(glGetUniformLocation(program, "Normal"), 1);
//			}
//			if (this->_Model->Mat->SpecularMapIndex != 255)
//			{
//				glActiveTexture(GL_TEXTURE2);
//				glBindTexture(GL_TEXTURE_2D, this->_Model->Mat->Tex[this->_Model->Mat->SpecularMapIndex]);
//				glUniform1i(glGetUniformLocation(program, "Specular"), 2);
//			}
//			//glDrawElements(GL_STATIC_DRAW, this->_Model->IndicesCount, GL_UNSIGNED_INT, 0);
//			glDrawArrays(GL_TRIANGLES, 0, this->_Model->IndicesCount);
//			if (this->_Model->HasNormals)
//				glDisableVertexAttribArray(2);
//			if (this->_Model->HasTexcords)
//				glDisableVertexAttribArray(1);
//			glDisableVertexAttribArray(0);
//#endif
//			glBindVertexArray(0);
//		}
//		else if (dt == DrawType::VerticesOnly)
//		{
//			program = OpenGL->GetShadersClass().GetCurrentShaderProgram();
//			glBindVertexArray(this->_Model->VAO);
//#ifdef HD4850
//			glEnableVertexAttribArray(1);
//			glDrawArrays(GL_TRIANGLES, 0, this->_Model->IndicesCount);
//			glDisableVertexAttribArray(1);
//#else
//			glEnableVertexAttribArray(0);
//			glDrawArrays(GL_TRIANGLES, 0, this->_Model->IndicesCount);
//			glDisableVertexAttribArray(0);
//#endif
//			glBindVertexArray(0);
		}
	}
}
