

#ifndef COORDINATETRANSFORMATION_HPP
#define COORDINATETRANSFORMATION_HPP


#include <glad/glad.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/ext.hpp>


struct CoordinateTransformation
{
    glm::mat4 m_model = glm::mat4(1.0f);
    glm::mat4 m_view = glm::mat4(1.0f);
    glm::mat4 m_perspective = glm::mat4(1.0);

    //get the location of model matrix uniform in vertex shader
    inline void modelLocation(GLuint shaderProgramID)
    {
        GLuint modelLocation = glGetUniformLocation(shaderProgramID, "model");
        glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(m_model));
    }

    //view and perspective matrix and location finder is going to be in the Camera class
    inline void viewLocation(GLuint shaderProgramID)
    {
        GLuint viewLocation = glGetUniformLocation(shaderProgramID, "view");
        glUniformMatrix4fv(viewLocation, 1, GL_FALSE, glm::value_ptr(m_view));
    }

    //get the location of perspective matrix uniform in vertex shader
    inline void perspectiveLocation(GLuint shaderProgramID)
    {
        GLuint perspectiveLocation = glGetUniformLocation(shaderProgramID, "perspective");
        glUniformMatrix4fv(perspectiveLocation, 1, GL_FALSE, glm::value_ptr(m_perspective));
    }

    //transform the entity
    inline void rotate(float angleToRotateDegrees, glm::vec3 rotationVec)
    {
        m_model = glm::rotate(m_model, glm::radians(angleToRotateDegrees), rotationVec);
    }

    inline void translate(glm::vec3 translationVec)
    {
        m_model = glm::translate(m_model, translationVec);
    }

    inline void scale()
    {
        //scale the model matrix by parameter amount
    }

};


typedef struct CoordinateTransformation CoordinateTransformation;


#endif //   COORDINATETRANSFORMATION_HPP