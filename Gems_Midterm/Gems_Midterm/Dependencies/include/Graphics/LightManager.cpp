#include "LightManager.h"

LightManager::LightManager()
{
}

LightManager::~LightManager()
{
}

LightManager& LightManager::GetInstance()
{
	static LightManager instance;
	return instance;
}

void LightManager::SetLightShader(Shader& lightShader)
{
	this->lightShader = &lightShader;
}

void LightManager::AddLight(Light* light)
{
	if (lights.size() >= NUMBER_OF_LIGHTS_IN_USE)
	{
		Debugger::Print("Number of lights exceeded");
		return;
	}

	switch (light->lightType)
	{
	case Point:
		light->name = "Point Light";
		break;

	case Directional:
		light->name = "Directional Light";
		break;

	case Spot :
		light->name = "Spot Light";
		break;
	}

	lights.push_back(light);
}

void LightManager::AddShader(Shader* shader)
{
	shaders.push_back(shader);
}

void LightManager::RemoveLight(Light* light)
{
	/*if (lights.size() != 0)
	{
		lights.erase(std::remove(lights.begin(), lights.end(), light),lights.end());
	}*/
}

void LightManager::AddLight(Light& light)
{
	if (lights.size() >= NUMBER_OF_LIGHTS_IN_USE)
	{
		Debugger::Print("Number of lights exceeded");
		return;
	}

	lights.push_back(&light);
}

void LightManager::AddShader(Shader& shader)
{
	shaders.push_back(&shader);
}

void LightManager::RemoveLight(Light& light)
{
	/*if (lights.size() != 0)
	{
		lights.erase(std::remove(lights.begin(), lights.end(), light), lights.end());
	}*/
}

std::vector<Light*> LightManager::GetListOfLights()
{
	return lights;
}

void LightManager::RenderLight()
{
	for (int i = 0; i < shaders.size(); i++)
	{
		shaders[i]->Bind();
		for (int j = 0; j < lights.size(); j++)
		{
			//Light model view
			/*lightShader->Bind();
			lightShader->SetUniformMat("model", lights[j]->lightModel->transform.GetTransformMatrix());*/

			/*lightShader->Bind();
			glm::vec3 value = lights[j]->GetLightColor();
			lightShader->SetUniform3f("lightColor", value.x, value.y, value.z);
			*/

			std::string str = std::to_string(j);

			//lightPos
			glm::vec3 value = lights[j]->transform.position;
			shaders[i]->SetUniform3f("lights[" + str + "]." + "position", value.x, value.y, value.z);

			//direction
			value = lights[j]->transform.GetForward();
			shaders[i]->SetUniform3f("lights[" + str + "]." + "direction", value.x, value.y, value.z);

			//baseColor
			glm::vec4 valueVec4 = lights[j]->GetLightColor();
			shaders[i]->SetUniform4f("lights[" + str + "]." + "baseColor", valueVec4.x, valueVec4.y, valueVec4.z, valueVec4.w);

			//ambientColor
			valueVec4 = lights[j]->GetAmbientColor();
			shaders[i]->SetUniform4f("lights[" + str + "]." + "ambientColor", valueVec4.x, valueVec4.y, valueVec4.z, valueVec4.w);

			//atten
			shaders[i]->SetUniform4f("lights[" + str + "]." + "atten",
									lights[j]->attenuation.x,	//Constant
									lights[j]->attenuation.y,	//Linear
									lights[j]->attenuation.z,	//Quadratic
									lights[j]->attenuation.w);	//CutOff

			//type_innerAngle_outerAngle_w
			shaders[i]->SetUniform4f("lights[" + str + "]." + "type_innerAngle_outerAngle_w",
									(int)lights[j]->lightType,	//lightType
									lights[j]->innerAngle,		//InnerAngle
									lights[j]->outerAngle,		//OuterAngle
									0);							//TBD

			//param2
			shaders[i]->SetUniform4f("lights[" + str + "]." + "param2",
									(int)lights[j]->isActive,	//LightActive
									0,							//TBD
									0,							//TBD
									0);							//TBD
		}
		shaders[i]->Unbind();

	}

}

void LightManager::Clear()
{
	while (lights.size() != 0)
	{
		delete lights[0];
		lights.erase(lights.begin());
	}

	lights.clear();
}
