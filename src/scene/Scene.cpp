#include "Scene.hpp"
#include "../debug/LOG.hpp"
#include "../window/Window.hpp"
#include "entitiesAttributesData.hpp"
#include "../multithreading/runParallel.hpp"

#include <GLFW/glfw3.h>

#include <vector>



#define __RUNTIME__MULTITHREADING__
//#define __SINGLETHREADING__


namespace Synapse
{


void Scene::Init()
{
    this->LoadRenderableObjectsStatically();
}


void Scene::LoadRenderableObjectsStatically()
{
    namespace data = entitiesData;

    m_renderableObjects.push_back(new RenderableObject("light source cube",
                                                        data::cubeData::cubeVerticiesData, data::cubeData::cubeTotalVerticies,
                                                        data::cubeData::cubeIndiciesData, data::cubeData::cubeTotalIndicies,
                                                        "../src/shader/GLSL/vertexShaderSource1.glslv",
                                                        "../src/shader/GLSL/fragmentShaderSource1.glslf"));

    m_renderableObjects.push_back(new RenderableObject("ground",
                                                        data::groundData::groundVerticiesData, data::groundData::groundTotalVerticies,
                                                        data::groundData::groundIndiciesData, data::groundData::groundTotalIndicies,
                                                        "../src/shader/GLSL/vertexShaderSource1.glslv",
                                                        "../src/shader/GLSL/fragmentShaderSource1.glslf"));

    m_renderableObjects.push_back(new RenderableObject("anotherCube",
                                                        data::anotherCubeData::anotherCubeVerticiesData, data::anotherCubeData::anotherCubeTotalVerticies,
                                                        data::anotherCubeData::anotherCubeIndiciesData, data::anotherCubeData::anotherCubeTotalIndicies,
                                                        "../src/shader/GLSL/vertexShaderSource1.glslv",
                                                        "../src/shader/GLSL/fragmentShaderSource1.glslf"));


    //make any modification to the entities or scene after running useProgram() and before rendering otherwise it would be TOO bad!

//        entities[0]->getTransformation().translate(glm::vec3(0.0f, -0.01f, 0.0f));
//        entities[0]->getTransformation().modelLocation(entities[0]->getShader().ProgramID());



//    unsigned int entityIndex = 0;
//    m_renderableObjects[entityIndex]->Update();
//    m_renderableObjects[entityIndex]->m_model = glm::translate(m_renderableObjects[entityIndex]->m_model, glm::vec3(0.0f, -1.0f, 0.0f));

    constexpr std::size_t nullEntities = 0;

    //for now this is because of the benchmarking
    //add null entities
    for(std::size_t i=0; i<nullEntities; ++i)
    {
        m_renderableObjects.push_back(new RenderableObject("", nullptr, 0, nullptr, 0, "", ""));
    }

    for(RenderableObject *renderableObject: m_renderableObjects)
    {
        renderableObject->LoadShader();
    }

    //std::cin.get();
}


//have to fix and update this dynamic scene Loader which apparently doesn't work
void Scene::LoadRenderableObjectsDynamically()
{
    namespace data = entitiesData;

    //adding a single scene(which is here a cube) causes adding of other entities.
    //will need to fix this
    m_renderableObjects.push_back(new RenderableObject("light source cube",
                                              data::cubeData::cubeVerticiesData, data::cubeData::cubeTotalVerticies,
                                              data::cubeData::cubeIndiciesData, data::cubeData::cubeTotalIndicies,
                                              "../src/shader/GLSL/vertexShaderSource1.glslv",
                                              "../src/shader/GLSL/fragmentShaderSource1.glslf"));


    std::size_t lastEntityIndex = m_renderableObjects.size()-1;
    m_renderableObjects[lastEntityIndex]->LoadShader();


//    m_camera->AddShaderProgramID(m_entities[lastEntityIndex]->GetShader().ProgramID());
//
//    m_renderer->InitVAO(m_entities[lastEntityIndex]->GetVertexObjects().GetVAO());
//    m_renderer->InitIndicies(m_entities[lastEntityIndex]->TotalIndicies());

    //m_renderableObjects[lastEntityIndex]->Translate(m_camera->GetTargetPos());

    m_renderableObjects[lastEntityIndex]->Translate(glm::vec3(0.0f, -10.0f, 0.0f));
}




void Scene::Update(GLFWwindow *window)
{
    if(m_renderableObjects.size() < 0)
    {
        return;
    }

    std::cout << "Total renderable objects: " << m_renderableObjects.size() << '\n';
    //apply different transformation to the renderable objects if needed
    //but the objects are being treated like they are in one buffer
    //will NEED to fix it.

    //unsigned int entityIndex = 0;
    //m_renderableObjects[entityIndex]->Update();
    //m_renderableObjects[entityIndex]->m_model = glm::translate(m_renderableObjects[entityIndex]->m_model, glm::vec3(0.0f, -0.01f, 0.0f));
    //m_renderableObjects[entityIndex]->m_model = glm::rotate(m_renderableObjects[entityIndex]->m_model, glm::radians(1.0f), glm::vec3(1.0f, 1.0f, 1.0f));

#ifdef __RUNTIME__MULTITHREADING__
    unsigned short threadsToUtilize = 0x4;
    Threading::S_pragma_omp_parallel_loop<void, std::size_t>(0, m_renderableObjects.size(), threadsToUtilize,
    [this](auto i) -> void
    {
        m_renderableObjects[i]->Update();
    });
#elif defined(__SINGLETHREADING__)
    for(RenderableObject *renderableObject : m_renderableObjects)
    {
        renderableObject->Update();
    }
#endif

    if(glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS &&
       glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS)
    {
        DEBUG::__LOG__MANAGER__::LOG("PRESSED Dynamic Entity Loader!");
        this->LoadRenderableObjectsDynamically();
    }

}




Scene::~Scene()
{
    for(std::size_t i=0; i<m_renderableObjects.size(); ++i)
    {
        m_renderableObjects[i]->GetShader().RemoveProgram();
    }
}




}   //namespace 'Synapse'








