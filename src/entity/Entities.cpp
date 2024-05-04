

#include "Entities.hpp"

//static GLuint count = 0;


unsigned int size(const GLuint *arr)
{
    //get the size of an pointer array eg. GLuint *indiciesData = new GLuint[78];
    unsigned int count = 0;
    while((arr+count)!=nullptr)
    {
        ++count;
    }
    return count;
}


//template<typename T>
//template<GLuint *buffer> struct Size{ enum{ count = size<buffer+1>::count }; };
//template<> struct Size<nullptr>{ enum{count = 0}; };

//constexpr totalV = size<verticiesArr>::count;
//constexpr totalI = size<indiciesArr>::count;



Entity::Entity(Vertex *verticies, GLuint totalVerticies,
               GLuint *indicies, GLuint totalIndicies,
               const std::string &vertexShaderSourcePath,
               const std::string &fragmentShaderSourcePath) noexcept
    :
     m_verticies(verticies),
     m_indicies(indicies),
     m_totalVerticies(totalVerticies),
     m_totalIndicies(totalIndicies),
     m_verticiesSizeBytes(sizeof(Vertex)*totalVerticies),
     m_indiciesSizeBytes(sizeof(GLuint)*totalIndicies),
     m_shader(vertexShaderSourcePath, fragmentShaderSourcePath),
     m_VO(m_verticiesSizeBytes, m_verticies, m_indiciesSizeBytes, m_indicies)
{
    //compile, link the vertex and fragment shader
    m_shader.setup();
    m_shader.link();
}



void Entity::setShader(const std::string &vertexShaderSourcePath,
                       const std::string &fragmentShaderSourcePath)
{
    m_shader = Shader(vertexShaderSourcePath, fragmentShaderSourcePath);
    m_shader.setup();
    m_shader.link();
}


void Entity::setShader()
{
    m_shader.setup();
    m_shader.link();
}

void Entity::setVO()
{
    m_VO = VertexObjects(m_verticiesSizeBytes, m_verticies, m_indiciesSizeBytes, m_indicies);
}


void Entity::printVerticiesData()
{
    std::cout << "\n\n\n\n\n\n\n\n\n\n\n";
    for (std::size_t i = 0; i < m_totalVerticies; ++i)
    {
        std::cout << m_verticies[i] << '\n';
    }
}


void Entity::printIndiciesData()
{
    std::cout << "\n\n\n\n\n\n\n\n\n\n\n";
    for (std::size_t i = 0; i < m_totalIndicies; ++i)
    {
        std::cout << m_indicies[i] << '\n';
    }
}


void Entity::render()
{
    //glDrawArrays(GL_TRIANGLES, 0, 36)
    glBindVertexArray(m_VO.getVAO());
    glDrawElements(GL_TRIANGLES, m_totalIndicies, GL_UNSIGNED_INT, 0);
}


void Entity::update()
{
    m_shader.useProgram();

//    m_transform.rotate(0.0f, glm::vec3(1.0f, 0.0f, 0.0f));
//    m_transform.translate(glm::vec3(0.0f, 0.0f, 0.0f));

    m_transform.modelLocation(m_shader.ProgramID());
}


Entity::~Entity()
{
    //in constructor initializer list m_verticies and m_indicies was assigned to verticiesArr and indiciesArr
    //so now both owns the verticies and indicies data
    //we will delete the verticies and indicies data from the Entity destructor
    //it will prevent us from doing manual deletion in InitializeEngine.cpp where the entities are deleted explicitly at the end of the program
    delete m_verticies;
    delete m_indicies;
}
