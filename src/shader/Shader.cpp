

#include "Shader.hpp"
#include "../debug/LOG.hpp"

#include <fstream>
#include <sstream>
#include <stdlib.h>

#define __DEBUG__

namespace Synapse
{

void Shader::ReadSources()
{
    if(m_vertexShader.path == "" || m_fragmentShader.path == "")
    {
        return;
    }

    std::ifstream vertexShaderFile(m_vertexShader.path);
    std::ifstream fragmentShaderFile(m_fragmentShader.path);

#if 0
     SYSTEM_LOG("if test -e /shader/GLSL/vertesShaderSource.glslv; then\n"
                "    echo \"File or directory exists.\"\n"
                "else\n"
                "    echo \"File or directory does not exist.\"\n"
                "fi");
#endif

     if(vertexShaderFile.fail())
     {
         std::cout << "didn't find vertex shader file!" << '\n';
         std::cout << "Vertex Shader File name: " << m_vertexShader.path << '\n';
         std::cin.get();    //this is definately bad for doing
         return;
     }

     if(fragmentShaderFile.fail())
     {
         std::cout << "didn't find fragment shader file!" << '\n';
         std::cout << "Fragment Shader File name: " << m_fragmentShader.path << '\n';
         std::cin.get();    //this is definately bad for doing
         return;
     }

     std::cout << "Vertex Shader file: " << m_vertexShader.path << '\n';
     std::cout << "Fragment Shader file: " << m_fragmentShader.path << '\n';

     std::string vertexLine;
     while ( std::getline(vertexShaderFile, vertexLine) )
         m_vertexShader.source += vertexLine + '\n';

     std::string fragmentLine;
     while ( std::getline(fragmentShaderFile, fragmentLine) )
         m_fragmentShader.source += fragmentLine + '\n';

     if(m_vertexShader.source == "" || m_fragmentShader.source == "")
     {
         return;
     }

     vertexShaderFile.close();
     fragmentShaderFile.close();
}





template<typename T>
void Shader::SetupSuccessLog(T __STATUS__, GLuint shaderID)
{
    int success;
    char infoLog[512];
    //thinking of using Bit Manipulation and Flags here
    glGetShaderiv(shaderID, __STATUS__, &success);
    if(!success)
    {
        glGetShaderInfoLog(shaderID, 512, NULL, infoLog);
        std::cerr << "ERROR:SHADER:COMPILATION:FAILED" << '\n';
    }
}




const char *vertexSS = "#version 330 core\n"
                       "layout (location = 0) in vec3 aPosition;\n"
                       "layout (location = 1) in vec3 aColor;\n"
                       "\n"
                       "out vec3 vertexColor;\n"
                       "\n"
                       "\n"
                       "//we won't add any tranformations to the object now\n"
                       "//uniform mat4 transform;\n"
                       "\n"
                       "uniform mat4 model;\n"
                       "uniform mat4 view;\n"
                       "uniform mat4 perspective;\n"
                       "\n"
                       "void core()\n"
                       "{   \n"
                       "    //gl_Position = transform * perspective * view * model * vec4(aPosition, 1.0f);\n"
                       "\n"
                       "    gl_Position = perspective * view * model * vec4(aPosition, 1.0f);\n"
                       "\n"
                       "    //gl_Position = transform * vec4(aPosition, 1.0f);    \n"
                       "    //gl_Position = vec4(aPosition, 1.0f);\n"
                       "\n"
                       "\n"
                       "    vertexColor = aColor;\n"
                       "\n"
                       "}";



const char *fragmentSS = "#version 330 core\n"
                         "\n"
                         "out vec4 FragColor;\n"
                         "\n"
                         "in vec3 vertexColor;\n"
                         "\n"
                         "//uniform vec3 vertexColor;\n"
                         "\n"
                         "//uniform vec3 objectColor;\n"
                         "//uniform vec3 lightColor;\n"
                         "\n"
                         "\n"
                         "void core()\n"
                         "{\n"
                         "    FragColor = vec4(vertexColor, 1.0f);\n"
                         "\n"
                         "    //FragColor = vec4(objectColor * lightColor, 1.0f);\n"
                         "}";


void Shader::Setup()
{
    this->ReadSources();
    if(m_vertexShader.source == "" || m_fragmentShader.source == "")
    {
        return;
    }

    const char* vertexShaderSource = m_vertexShader.source.c_str();
    const char* fragmentShaderSource = m_fragmentShader.source.c_str();

//    const char* vertexShaderSource = vertexSS;
//    const char* fragmentShaderSource = fragmentSS;

#define __DEBUG__
#ifdef __DEBUG__
     DEBUG::__LOG__MANAGER__::LOG(vertexShaderSource);
     DEBUG::__LOG__MANAGER__::LOG(fragmentShaderSource);
#endif

     //compile the vertex shader source
     glShaderSource(m_vertexShaderID, 1, &vertexShaderSource, NULL);
     glCompileShader(m_vertexShaderID);

     this->SetupSuccessLog(GL_COMPILE_STATUS, m_vertexShaderID);

     //compile the fragment shader source
     glShaderSource(m_fragmentShaderID, 1, &fragmentShaderSource, NULL);
     glCompileShader(m_fragmentShaderID);

     this->SetupSuccessLog(GL_COMPILE_STATUS, m_fragmentShaderID);


     //this->SetupSuccessLog(GL_COMPILE_STATUS | GL_LINK_STATUS);
}



void Shader::AttachAndLink()
{
     //attach and link the vertex and fragment shader
     glAttachShader(m_shaderProgramID, m_vertexShaderID);
     glAttachShader(m_shaderProgramID, m_fragmentShaderID);
     glLinkProgram(m_shaderProgramID);

     this->SetupSuccessLog(GL_LINK_STATUS, m_shaderProgramID);
}



void Shader::RemoveShaders()
{
     //delete both the shaders ID
     glDeleteShader(m_vertexShaderID);
     glDeleteShader(m_fragmentShaderID);
}


void Shader::UseProgram()
{
     glUseProgram(m_shaderProgramID);
}


void Shader::RemoveProgram()
{
     glDeleteProgram(m_shaderProgramID);
}



}








////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/*

void Shader::readSource()
{
    std::ifstream shaderFile(m_shaderFile.path);
    std::string line;
    while ( std::getline(shaderFile, line) )
        m_shaderFile.source += line + '\n';

    shaderFile.close();
}


void Shader::setupSuccessLog()
{
    int success;
    char infoLog[512];
    glGetShaderiv(m_shaderID, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        glGetShaderInfoLog(m_shaderID, 512, NULL, infoLog);    
        std::cerr << "ERROR:SHADER:COMPILATION:FAILED" << '\n';    
    }
}


void Shader::setup()
{    
    readSource();
    //LOG(m_shaderFile.source);
    const char* str_shaderSource = (m_shaderFile.source).c_str();
    //LOG(str_shaderSource);
    
    glShaderSource(m_shaderID, 1, &str_shaderSource, NULL);
    glCompileShader(m_shaderID);
    setupSuccessLog();
}


void Shader::compilationSuccessLog(GLuint shaderProgramID)
{
    int  success;
    char infoLog[512];
    glGetProgramiv(shaderProgramID, GL_LINK_STATUS, &success);
    if(!success)
    {
        glGetProgramInfoLog(shaderProgramID, 512, NULL, infoLog);
        std::cerr << "ERROR:SHADER:PROGRAM:LINKING:FAILED!" << '\n';
    }
}


auto Shader::attachAndLink(GLuint vertexShaderID, GLuint fragmentShaderID)->GLuint
{
    GLuint shaderProgramID = glCreateProgram();
    glAttachShader(shaderProgramID, vertexShaderID);
    glAttachShader(shaderProgramID, fragmentShaderID);
    glLinkProgram(shaderProgramID);

    Shader::compilationSuccessLog(shaderProgramID);

    return shaderProgramID;
}

*/

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
