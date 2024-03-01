#include "Shader.h"
#include "../LightManager.h"
#include "ShaderSystem.h"

ShaderSource ParseShader(const std::string& path)
{
	std::fstream file;
	std::string line;
	std::stringstream sstream[2];
	Shader::ShaderType currentType = Shader::ShaderType::NONE;
	file.open(path);

	if (file.is_open())
	{
		//Debugger::Print("File is Open!");
		while (std::getline(file, line))
		{
			if (line.empty())
				continue;

			if (line.find("#vertex") != std::string::npos)
			{
				currentType = Shader::ShaderType::VERTEX_SHADER;
			}
			else if (line.find("#fragment") != std::string::npos)
			{
				currentType = Shader::ShaderType::FRAGMENT_SHADER;
			}
			else
			{
				sstream[(int)currentType] << line << std::endl;
			}
		}
	}
	else
	{
		Debugger::Print("File not Open!");
	}

	return ShaderSource{ sstream[0].str(), sstream[1].str() };
}

unsigned int CompileShader(unsigned int shaderType, const std::string& shaderSrc)
{
	unsigned int shader = glCreateShader(shaderType);

	const char* src = shaderSrc.c_str();

	GLCALL(glShaderSource(shader, 1, &src, nullptr));

	GLCALL(glCompileShader(shader));

	int compileStatus;
	GLCALL(glGetShaderiv(shader, GL_COMPILE_STATUS, &compileStatus));

	if (compileStatus == GL_FALSE)
	{
		int length;
		GLCALL(glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length));

		char* messaage = (char*)alloca(sizeof(char) * length);

		GLCALL(glGetShaderInfoLog(shader, length, &length, messaage));

		Debugger::Print("Shader Compilation Failed");
		Debugger::Print(messaage);

		GLCALL(glDeleteShader(shader));
		return 0;
	}

	return shader;

}

unsigned int CompileProgram(const std::string& vertexSrc, const std::string& fragmentSrc)
{
	unsigned int program = glCreateProgram();

	unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexSrc);
	unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentSrc);


	GLCALL(glAttachShader(program, vs));
	GLCALL(glAttachShader(program, fs));

	GLCALL(glLinkProgram(program));
	GLCALL(glValidateProgram(program));

	GLCALL(glDeleteShader(vs));
	GLCALL(glDeleteShader(fs));

	return program;
}

Shader::Shader()
{
	ShaderSystem::GetInstance().AddShader(this);
}

Shader::Shader(const std::string& path, bool addToSystem, BlendMode transparentMode, bool useLightCalculation)
{
	this->blendMode = transparentMode;

	ShaderSource shaderSrc = ParseShader(path);

	shaderId = CompileProgram(shaderSrc.vertexSrc, shaderSrc.fragmentSrc);
	
	if (useLightCalculation)
	{
		LightManager::GetInstance().AddShader(this);
	}

	if (addToSystem)
	{
		ShaderSystem::GetInstance().AddShader(this);
	}
}

Shader::~Shader()
{
	GLCALL(glDeleteProgram(shaderId));
}

void Shader::LoadShader(const std::string& path, BlendMode transparentMode, bool useLightCalculation)
{
	this->blendMode = transparentMode;

	ShaderSource shaderSrc = ParseShader(path);

	shaderId = CompileProgram(shaderSrc.vertexSrc, shaderSrc.fragmentSrc);

	if (useLightCalculation)
	{
		LightManager::GetInstance().AddShader(this);
	}

}

void Shader::Bind()
{
	GLCALL(glUseProgram(shaderId));
}

void Shader::Unbind()
{
	GLCALL(glUseProgram(0));
}

unsigned int Shader::GetShaderId()
{
	return shaderId;
}

void Shader::SetUniform1f(const std::string& property, float x)
{
	GLCALL(glUniform1f(GetLocation(property), x));
}

void Shader::SetUniform3f(const std::string& property, float x, float y, float z)
{
	GLCALL(glUniform3f(GetLocation(property), x, y, z));
	//GLCALL(glUniform3f(GetLocation(property), 0.1f, 0.2f, 0.3f));
}

void Shader::SetUniform4f(const std::string& property, float x, float y, float z, float w)
{
	GLCALL(glUniform4f(GetLocation(property), x, y, z, w));
}

void Shader::SetUniform1i(const std::string& property, int slot)
{
	GLCALL(glUniform1i(GetLocation(property), slot));
}

void Shader::SetUniformMat(const std::string& property, glm::mat4 value)
{
	GLCALL(glUniformMatrix4fv(GetLocation(property), 1, GL_FALSE, glm::value_ptr(value)));
}


int Shader::GetLocation(const std::string& property)
{
	auto value = cachedLocations.find(property);

	if (value == cachedLocations.end())
	{

		int location;
		GLCALL(location = glGetUniformLocation(shaderId, property.c_str()));

		if (location == -1)
		{
			std::cout <<"Shader :" << shaderId << " Uniform Location not found ! : " << property << std::endl;
			return -1;
		}
		//std::cout << "Uniform " << property << " Location :" << location << std::endl;

		cachedLocations[property] = location;
		return location;
	}
	else
	{
		return value->second;
	}


}

void Shader::OnPropertyDraw()
{
}

void Shader::OnSceneDraw()
{
}

void Shader::SetProjection(glm::mat4 matrix)
{
	SetUniformMat("projection", matrix);
}

void Shader::SetView(glm::mat4 matrix)
{
	SetUniformMat("view", matrix);
}

void Shader::SetViewPosition(glm::vec3 pos)
{
	SetUniform3f("viewPos", pos.x, pos.y, pos.z);
}
