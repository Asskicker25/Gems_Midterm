#include "Debugger.h"
#include <sstream>


void ClearErrors()
{
	while (glGetError() != GL_NO_ERROR)
	{
	}
}

bool GetError(const char* function, const char* file, int line)
{
	while (GLenum error = glGetError())
	{
		std::cout << "OPENGL ERROR [" << error << "] : " << function << " " << file << " : " << line << std::endl;
		return true;
	}
	return false;
}

bool Debugger::print = false;

void Debugger::Print(const char* message)
{
	if (!print) return;
	std::cout << message << std::endl;
}

void Debugger::Print(const std::string& message)
{
	if (!print) return;
	std::cout << message << std::endl;
}

void Debugger::Print(const int& message)
{
	if (!print) return;
	std::cout << message << std::endl;
}


void Debugger::Print(const unsigned int& message)
{
	if (!print) return;
	std::cout << message << std::endl;
}

void Debugger::Print(const glm::vec3& message)
{
	if (!print) return;
	std::cout << message.x << ",";
	std::cout << message.y << ",";
	std::cout << message.z << std::endl;
}

void Debugger::Print(const std::string& message, const std::string& message2)
{
	if (!print) return;
	std::cout << message << message2 << std::endl;
}


void Debugger::Print(const std::string& message, const int& message2)
{
	if (!print) return;
	std::cout << message << message2 << std::endl;
}

void Debugger::Print(const std::string& message, const float& message2)
{
	if (!print) return;
	std::cout << message << message2 << std::endl;
}

void Debugger::Print(const std::string& message, const unsigned int& message2)
{
	if (!print) return;
	std::cout << message << message2  <<std::endl;
}

void Debugger::Print(const std::string& message, const glm::vec3& messag2)
{
	if (!print) return;
	std::cout << message;
	std::cout << messag2.x << ",";
	std::cout << messag2.y << ",";
	std::cout << messag2.z << std::endl;
}


