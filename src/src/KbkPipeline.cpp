/**
 * @file    Pipeline.h
 * @brief   Simple OpenGL pipeline wrap up class 
 *
 * @details  Pipeline class that implements the abstraction for OpenGL to later be used in the stager
 *          
 *
 * @author  Markel Sevilla
 * @date    2026-07-25
 * @version 1.0
 */
#include "KbkPipeline.h"
#include "KbkPass.h"
#include "Log.h"
#include "Camera.h"
#include <glad/glad.h>
#include <glfw/glfw3.h>

namespace Graphics
{

    //Helper function defined to debug shaders
    void APIENTRY GLDebugMessageCallback(GLenum source, GLenum type, GLuint /*id*/, GLenum severity, GLsizei /*length*/, const GLchar* message, const void* /*userParam*/)
	{

		std::stringstream stream;
		stream << message << '\n';

		//take the source and check where the error is coming from
		switch (source)
		{
		case  GL_DEBUG_SOURCE_API:
			stream << "Source: API";
			break;

		case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
			stream << "Source: Window Manager";
			break;

		case GL_DEBUG_SOURCE_SHADER_COMPILER:
			stream << "Source: Shader Compiler";
			break;

		case GL_DEBUG_SOURCE_THIRD_PARTY:
			stream << "Source: Third Party";
			break;

		case GL_DEBUG_SOURCE_APPLICATION:
			stream << "Source: Application";
			break;

		case GL_DEBUG_SOURCE_OTHER:
			stream << "Source: Other";
			break;

		default:break;
		}

		stream << '\n';

		// checkj the severity 
		switch (severity)
		{
		case GL_DEBUG_SEVERITY_HIGH:
			stream << "Severity: HIGH [ERROR]"; // this is an error
			break;

		case GL_DEBUG_SEVERITY_MEDIUM:
			stream << "Severity: MEDIUM [WARNING]"; // warning
			break;

		case GL_DEBUG_SEVERITY_LOW:
			stream << "Severity: LOW [WARNING]"; // warning
			break;

		case GL_DEBUG_SEVERITY_NOTIFICATION:
			stream << "Severity: NOTIFICATION [INFO]"; // just information
			return;

		default:break;
		}

		std::cerr << "OpenGL :" << type << std::endl << stream.str() << std::endl;
	}

    std::unique_ptr<KPipeline> KPipeline::kCreatePipelineGL(int width, int height, KRenderType type, bool resizeable , bool hintVisible )
    {
        return std::unique_ptr<KPipeline>(new KPipeline(width, height, KAPI::GL, type, resizeable, hintVisible));
    }
    
    std::unique_ptr<KPipeline> KPipeline::kCreatePipelineVK(int width, int height, KRenderType type, bool resizeable, bool hintVisible)
    {
        return std::unique_ptr<KPipeline>(new KPipeline(width, height, KAPI::VK, type, resizeable, hintVisible));
    }

    KPipeline::KPipeline(int width, int height, KAPI api, KRenderType type, bool resizeable, bool hintVisible) : 
                         api_(api),
                         type_(type),                     
                         size_ {width, height},
                         resizeable_(resizeable),
                         hintVisible_(hintVisible),
                         clearColor_({0,0,0,1}) 
    {
        initGLFWPipeline(width, height, resizeable, hintVisible);
        
        // Enable debug output
		glEnable(GL_DEBUG_OUTPUT);
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS); // Optional, forces synchronous callback

		//// Set the debug message callback
		glDebugMessageCallback(GLDebugMessageCallback, NULL);

        LOG_TRACE("GLFW Initialized");

        switch( type_ )
        {
            // we create the forward pipeline
            case KRenderType::FWD:
                createForwardGL();
                break;
            // we create a instanced pipeline
            case KRenderType::INS:
                createInstancedGL();
                break;

            // we create a deferred pipeline
            case KRenderType::DEF:
                createDeferredGL();
                break;

            // create the raytracing pipeline
            case KRenderType::RTC:
                createRayTraceGL();
                break;

            default : break;
        }
    }

    KPipeline::~KPipeline()
    {
        LOG_TRACE("Destroy K-Pipeline");
        glfwTerminate();
    }

    void KPipeline::setClearColor(const glm::vec4& rgba)
    {
        clearColor_ = rgba;
    }
    void KPipeline::render(const glm::mat4& view, const glm::mat4& proj)
    {
        glClearColor(clearColor_.x, clearColor_.y, clearColor_.z, clearColor_.w); // keep it black so it doesn't leak into g-buffer
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // here we render with the renderer
        renderer_->render( view, proj );
    }
    void framebuffer_size_callback(GLFWwindow* window, int width, int height)
    {
        glViewport(0, 0, width, height);
    }

    int KPipeline::initGLFWPipeline(int width, int height, bool resizeWindow, bool hintVisible)
    {
        //srand(static_cast<unsigned int>(std::time(0)));

        //initialize the glfw context
        glfwInit();

       //window hints
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
        glfwWindowHint(GLFW_RESIZABLE, resizeWindow);
        glfwWindowHint(GLFW_VISIBLE, static_cast<int>(hintVisible));

        //Create the window
        window_ = glfwCreateWindow(width, height, "K-Lib", NULL, NULL);

        if (window_ == NULL)
        {
            //In case of error
            LOG_TRACE("Failed to create GLFW window");
            glfwTerminate();
            return -1;
        }

        //create the context for the window
        glfwMakeContextCurrent(window_);
        glfwSetFramebufferSizeCallback(window_, framebuffer_size_callback);

        //check that we have the correct glaad initialized
        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        {
            LOG_TRACE("Failed to initialize GLAD");
            return -1;
        }

        //set the viewport and the size callback
        glViewport(0, 0, width, height);
        LOG_TRACE("Reloaded the Viewport");

        glfwSwapInterval(0);

        return 1;
    }

    void KPipeline::createForwardGL()
    {

    }
    void KPipeline::createInstancedGL()
    {

    }
    void KPipeline::createDeferredGL()
    {
        LOG_TRACE("Initialize DEFERRED OpenGL renderer");
        renderer_ = std::make_unique<KDeferredRenderer>(size_.x, size_.y);
    }
    void KPipeline::createRayTraceGL()
    {

    }

    void KPipeline::addMesh( KMesh* mesh)
    {
        renderer_->addMesh(mesh);
    }

    void KPipeline::addMeshes(const std::vector<KMesh*>& meshes)
    {
        for(auto& mesh: meshes)
            renderer_->addMesh(mesh);
    }

    void KPipeline::loadScene(const char* filename)
    {
        nlohmann::json file(filename);
		std::ifstream fp(filename);

		//Read file to json
		if (fp.is_open() && fp.good())
		{
			fp >> std::setw(4) >> file;
			fp.close();
		}

		//we find a camaera
		if (file.find("camera") != file.end())
		{
			glm::vec3 translate, rotate;

			//read the traanslate rotate OR the whole matrx
			if (file["camera"].find("translate") != file["camera"].end())
			{
				translate.x = file["camera"]["translate"]["x"];
				translate.y = file["camera"]["translate"]["y"];
				translate.z = file["camera"]["translate"]["z"];
			}

			if (file["camera"].find("rotate") != file["camera"].end())
			{
				rotate.x = file["camera"]["rotate"]["x"];
				rotate.y = file["camera"]["rotate"]["y"];
				rotate.z = file["camera"]["rotate"]["z"];
			}

			//specifics of the camera 
			float n = file["camera"]["near"];
			float f = file["camera"]["far"];
			float fovy = file["camera"]["FOVy"];

			glm::vec3 dir = glm::rotate(glm::mat4(1), glm::radians(rotate.x), glm::vec3(1, 0, 0)) *
				glm::rotate(glm::mat4(1), glm::radians(rotate.y), glm::vec3(0, 1, 0)) *
				glm::rotate(glm::mat4(1), glm::radians(rotate.z), glm::vec3(0, 0, 1)) * glm::vec4(0, 0, 1, 0);

			glm::vec3 up = glm::rotate(glm::mat4(1), glm::radians(rotate.x), glm::vec3(1, 0, 0)) *
				glm::rotate(glm::mat4(1), glm::radians(rotate.y), glm::vec3(0, 1, 0)) *
				glm::rotate(glm::mat4(1), glm::radians(rotate.z), glm::vec3(0, 0, 1)) * glm::vec4(0, 1, 0, 0);

			glm::vec3 target = translate + dir;

			//create my new camera
			mCam = new Camera(glm::radians(fovy), width, height, n, f, translate, target, up);
		}

		//find the objects
		if (file.find("objects") != file.end())
		{
			glm::vec3 translate, rotate, scale;

			// we have a mesh
			for (auto& obj : file["objects"])
			{
				//read the traanslate rotate OR the whole matrx
				if (obj.find("translate") != obj.end())
				{
					translate.x = obj["translate"]["x"];
					translate.y = obj["translate"]["y"];
					translate.z = obj["translate"]["z"];
				}

				if (obj.find("scale") != obj.end())
				{
					scale.x = obj["scale"]["x"];
					scale.y = obj["scale"]["y"];
					scale.z = obj["scale"]["z"];
				}

				if (obj.find("rotate") != obj.end())
				{
					rotate.x = obj["rotate"]["x"];
					rotate.y = obj["rotate"]["y"];
					rotate.z = obj["rotate"]["z"];
				}

				glm::mat4 m2w = glm::mat4(1) * glm::translate(glm::mat4(1), translate) *
					glm::rotate(glm::mat4(1), glm::radians(rotate.x), glm::vec3(1, 0, 0)) *
					glm::rotate(glm::mat4(1), glm::radians(rotate.y), glm::vec3(0, 1, 0)) *
					glm::rotate(glm::mat4(1), glm::radians(rotate.z), glm::vec3(0, 0, 1)) *
					glm::scale(glm::mat4(1), scale);

				//we want to find the mesh
				if (obj.find("mesh") != obj.end())
				{
					std::string mesh = obj["mesh"].dump();
					mesh = &mesh[1];
					mesh = mesh.substr(0, mesh.length() - 1);
					//here we load the gltf
					std::cout << "Loading objects" << std::endl;
					LoadGLTF(m2w, mesh.c_str());
				}
			}
		}

		//find the lights
		if (file.find("lights") != file.end())
		{
			for (auto& light : file["lights"])
			{
				Light* l = new Light();

				if (light.find("position") != light.end())
				{
					l->position.x = light["position"]["x"];
					l->position.y = light["position"]["y"];
					l->position.z = light["position"]["z"];
				}

				if (light.find("color") != light.end())
				{
					l->color.x = light["color"]["x"];
					l->color.y = light["color"]["y"];
					l->color.z = light["color"]["z"];
				}

				if (light.find("radius") != light.end())
					l->radius = light["radius"];

				l->sphere = new Mesh();
				
				LoadGLTF(glm::mat4(1), "data/gltf/Sphere.gltf", l->sphere);
				//upload my lights
				mScene->mLights.push_back(l);
			}
		}

		//find the lights
		if (file.find("directional_light") != file.end())
		{
			auto& dirlight = file["directional_light"];

			mScene->mDirLight = new DirLight();

			if (dirlight.find("direction") != dirlight.end())
			{
				mScene->mDirLight->direction.x = dirlight["direction"]["x"];
				mScene->mDirLight->direction.y = dirlight["direction"]["y"];
				mScene->mDirLight->direction.z = dirlight["direction"]["z"];
			}
			if (dirlight.find("color") != dirlight.end())
			{
				mScene->mDirLight->color.x = dirlight["color"]["x"];
				mScene->mDirLight->color.y = dirlight["color"]["y"];
				mScene->mDirLight->color.z = dirlight["color"]["z"];
			}

		}

		//if we have decals
		if (file.find("decals") != file.end())
		{
			for (auto& decal : file["decals"])
			{
				Decal* decalObj = new Decal();

				std::string textures[3] = { decal["diffuse"] , decal["normal"], decal["metallic"] };

				int width, height, channels;

				if (decal.find("diffuse") != decal.end())
				{
					//load diffuse texture
					unsigned char* diffuseData = stbi_load(textures[0].c_str(), &width, &height, &channels, 0);
					decalObj->mTex->GenerateTexture(decalObj->mTex->mBaseID, diffuseData, width, height, channels, true);
				}
				

				if (decal.find("normal") != decal.end())
				{
					//loat normal texture
					unsigned char* normalData = stbi_load(textures[1].c_str(), &width, &height, &channels, 0);
					decalObj->mTex->GenerateTexture(decalObj->mTex->mNormalID, normalData, width, height, channels, true);

				}
				
				if(decal.find("metallic")!= decal.end())
				{
					//load metallic texture
					unsigned char* metallicData = stbi_load(textures[2].c_str(), &width, &height, &channels, 0);
					decalObj->mTex->GenerateTexture(decalObj->mTex->mMetallicID, metallicData, width, height, channels, true);

				}
				
				//trs
				glm::vec3 tr = glm::vec3(0), rot = glm::vec3(0), sc = glm::vec3(0);

				if (decal.find("translate") != decal.end())
				{
					tr.x = decal["translate"]["x"];
					tr.y = decal["translate"]["y"];
					tr.z = decal["translate"]["z"];
				}

				if (decal.find("rotate") != decal.end())
				{
					rot.x = decal["rotate"]["x"];
					rot.y = decal["rotate"]["y"];
					rot.z = decal["rotate"]["z"];
				}

				if (decal.find("scale") != decal.end())
				{
					sc.x = decal["scale"]["x"];
					sc.y = decal["scale"]["y"];
					sc.z = decal["scale"]["z"];
				}

				//create the modle to world
				decalObj->mM2W = glm::translate(glm::mat4(1), tr) 

					           * glm::rotate(glm::mat4(1), glm::radians(rot.x), glm::vec3(1, 0, 0))
					           * glm::rotate(glm::mat4(1), glm::radians(rot.y), glm::vec3(0, 1, 0))
					           * glm::rotate(glm::mat4(1), glm::radians(rot.z), glm::vec3(0, 0, 1))
					
					           * glm::scale(glm::mat4(1), sc);

				decalObj->mCubeVolume = new Mesh();

				LoadGLTF(glm::mat4(1), "data/gltf/Cube.gltf", decalObj->mCubeVolume);

				//store the decal
				mScene->mDecals.push_back(decalObj);
			}
		}
    }

    void KPipeline::loadGltf(const glm::mat4& m2w, const std::string& filename) 
    {

    }

    void KPipeline::loadNodes(const glm::mat4& m2w ,tinygltf::Node& node, tinygltf::Model& model, const std::string & filepath)
    {

    }



}