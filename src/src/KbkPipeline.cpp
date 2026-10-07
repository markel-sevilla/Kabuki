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
#define GLM_ENABLE_EXPERIMENTAL
#include "KbkPipeline.h"
#include "KbkPass.h"
#include "Log.h"
#include "KbkCamera.h"
#include <glad/glad.h>
#include <glfw/glfw3.h>
#include <iomanip>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include "KbkLight.h"

#define TINYGLTF_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION

#include "gltf/tiny_gltf.h"


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

		//TODO : change this to a vector of cameras or something similar as
		// the idea is cool, but does not have any sense right now
        if(renderer_->getCam())
        {
			renderer_->getCam()
            //LOG_TRACE("You are using the rendrer's camera");
            renderer_->render(renderer_->getCam()->getView(), renderer_->getCam()->getProj());
        }
        else
        {
            LOG_TRACE("You are using an external camera");
            renderer_->render( view, proj );
        }
        // here we render with the renderer
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

    std::unique_ptr<Graphics::KCamera> KPipeline::loadCamera(const nlohmann::json& file)
    {
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

			LOG_TRACE("Renderer's camera was succesfully created");
			
			//create my new camera
			return std::unique_ptr<KCamera>(new KCamera(glm::radians(fovy), size_.x, size_.y, n, f, translate, target));
		}

        return nullptr;
    }

    void KPipeline::loadDecals(const nlohmann::json& file)
    {
        // //if we have decals
		// if (file.find("decals") != file.end())
		// {
		// 	for (auto& decal : file["decals"])
		// 	{
		// 		Decal* decalObj = new Decal();

		// 		std::string textures[3] = { decal["diffuse"] , decal["normal"], decal["metallic"] };

		// 		int width, height, channels;

		// 		if (decal.find("diffuse") != decal.end())
		// 		{
		// 			//load diffuse texture
		// 			unsigned char* diffuseData = stbi_load(textures[0].c_str(), &width, &height, &channels, 0);
		// 			decalObj->mTex->GenerateTexture(decalObj->mTex->mBaseID, diffuseData, width, height, channels, true);
		// 		}
				

		// 		if (decal.find("normal") != decal.end())
		// 		{
		// 			//loat normal texture
		// 			unsigned char* normalData = stbi_load(textures[1].c_str(), &width, &height, &channels, 0);
		// 			decalObj->mTex->GenerateTexture(decalObj->mTex->mNormalID, normalData, width, height, channels, true);

		// 		}
				
		// 		if(decal.find("metallic")!= decal.end())
		// 		{
		// 			//load metallic texture
		// 			unsigned char* metallicData = stbi_load(textures[2].c_str(), &width, &height, &channels, 0);
		// 			decalObj->mTex->GenerateTexture(decalObj->mTex->mMetallicID, metallicData, width, height, channels, true);

		// 		}
				
		// 		//trs
		// 		glm::vec3 tr = glm::vec3(0), rot = glm::vec3(0), sc = glm::vec3(0);

		// 		if (decal.find("translate") != decal.end())
		// 		{
		// 			tr.x = decal["translate"]["x"];
		// 			tr.y = decal["translate"]["y"];
		// 			tr.z = decal["translate"]["z"];
		// 		}

		// 		if (decal.find("rotate") != decal.end())
		// 		{
		// 			rot.x = decal["rotate"]["x"];
		// 			rot.y = decal["rotate"]["y"];
		// 			rot.z = decal["rotate"]["z"];
		// 		}

		// 		if (decal.find("scale") != decal.end())
		// 		{
		// 			sc.x = decal["scale"]["x"];
		// 			sc.y = decal["scale"]["y"];
		// 			sc.z = decal["scale"]["z"];
		// 		}

		// 		//create the modle to world
		// 		decalObj->mM2W = glm::translate(glm::mat4(1), tr) 

		// 			           * glm::rotate(glm::mat4(1), glm::radians(rot.x), glm::vec3(1, 0, 0))
		// 			           * glm::rotate(glm::mat4(1), glm::radians(rot.y), glm::vec3(0, 1, 0))
		// 			           * glm::rotate(glm::mat4(1), glm::radians(rot.z), glm::vec3(0, 0, 1))
					
		// 			           * glm::scale(glm::mat4(1), sc);

		// 		decalObj->mCubeVolume = new Mesh();

		// 		loadGltf(glm::mat4(1), "data/gltf/Cube.gltf", decalObj->mCubeVolume);

		// 		//store the decal
		// 		mScene->mDecals.push_back(decalObj);
		// 	}
		// }
    }

    void KPipeline::loadMeshes(const nlohmann::json& file)
    {
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
					loadGltf(m2w, mesh.c_str());
				}

				LOG_TRACE("Meshes were succesfully created");

			}
		}
    }

    void KPipeline::loadLights(const nlohmann::json& file)
    {
        //find the lights
		if (file.find("lights") != file.end())
		{
			for (auto& light : file["lights"])
			{
				KPointLight* l = new KPointLight();

				if (light.find("position") != light.end())
					l->setPosition(glm::vec3(light["position"]["x"], light["position"]["y"], light["position"]["z"] ));
				

				if (light.find("color") != light.end())
					l->setColor(glm::vec4(light["color"]["x"], light["color"]["y"], light["color"]["z"], 1 ));

				if (light.find("radius") != light.end())
					l->setRadius(light["radius"]);

				l->setSphere(new KMesh());
				
				loadGltf(glm::mat4(1), "data/gltf/Sphere.gltf", l->getSphere());
				
				//upload my lights
				renderer_->addLight(l);
			}
		}

		//find the lights
		// if (file.find("directional_light") != file.end())
		// {
		// 	auto& dirlight = file["directional_light"];

		// 	mScene->mDirLight = new DirLight();

		// 	if (dirlight.find("direction") != dirlight.end())
		// 	{
		// 		mScene->mDirLight->direction.x = dirlight["direction"]["x"];
		// 		mScene->mDirLight->direction.y = dirlight["direction"]["y"];
		// 		mScene->mDirLight->direction.z = dirlight["direction"]["z"];
		// 	}
		// 	if (dirlight.find("color") != dirlight.end())
		// 	{
		// 		mScene->mDirLight->color.x = dirlight["color"]["x"];
		// 		mScene->mDirLight->color.y = dirlight["color"]["y"];
		// 		mScene->mDirLight->color.z = dirlight["color"]["z"];
		// 	}

		// }
    }
    void KPipeline::loadScene(const char* filename)
    {
        nlohmann::json file(filename);
		std::ifstream fp(filename);

		//Read file to json
		if (fp.is_open() && fp.good())
		{
			fp >> std::setw(4) >> file;
			LOG_TRACE("File succesfully opened");
			fp.close();
		}else
			return;

		LOG_TRACE("Load Scene");

		renderer_->setCam(loadCamera(file));

        loadMeshes(file);

        loadLights(file);

        loadDecals(file);
		
    }

    void KPipeline::loadGltf(const glm::mat4& m2w, const std::string& filename, KMesh* lightmesh) 
    {
		tinygltf::TinyGLTF loader;
		tinygltf::Model model;
		std::string err;
		std::string warn;

		// assume ascii glTF.
		bool ret = loader.LoadASCIIFromFile(&model, &err, &warn, filename);

		if (!err.empty()) {
			std::cerr << "glTF parse error: " << err << std::endl;
		}

		if (!ret) {
			std::cerr << "Failed to load glTF: " << filename << std::endl;
			return;
		}

		//if we are here it was loaded correctl

		int i = 0;
		for (auto& node : model.nodes)
		{
			std::string path = filename.substr(0, filename.find_last_of("/") + 1);

			//std::cout << "NODE: "<< i++ << std::endl;
			loadNodes(m2w, node, model, lightmesh, path);
			
			int j = 0;
			
			for (auto& child : node.children)
				loadNodes(m2w, model.nodes[child], model, lightmesh, path);
		}

		return;
    }

    int isMeshDuplicated(const std::string& name, const std::vector<std::unique_ptr<KMesh>>& meshes)
    {
        int i = 0;
        for(auto& mesh: meshes)
        {
            if(mesh->getName() == name)
                return i;
        
            i++;
        }

        return -1;
    }
    void KPipeline::loadNodes(const glm::mat4& m2w ,tinygltf::Node& node, tinygltf::Model& model, KMesh* lightmesh, const std::string & filepath)
    {
        glm::mat4 newM2W = glm::mat4(1),
			tr = glm::mat4(1),
			rot = glm::mat4(1),
			sc = glm::mat4(1);

		if (!node.translation.empty())
		{
			glm::vec3 translation = glm::vec3(node.translation[0], node.translation[1], node.translation[2]);
			tr = glm::translate(glm::mat4(1), translation);
		}
		if (!node.rotation.empty())
		{
			glm::quat rotation = glm::quat( static_cast<float>(node.rotation[3]),static_cast<float>(node.rotation[0]), static_cast<float>(node.rotation[1]), static_cast<float>(node.rotation[2]));
			rot = glm::mat4_cast(rotation);
		}
		if (!node.scale.empty())
		{
			glm::vec3 scale = glm::vec3(node.scale[0], node.scale[1], node.scale[2]);
			sc = glm::scale(glm::mat4(1), scale);
		}

		glm::mat4 m = tr * rot * sc;
		// create the new model 2 world
		newM2W = m2w * m;

		if (node.mesh != -1)
		{
			//take the mesh of the node
			tinygltf::Mesh mesh = model.meshes[node.mesh];
			
			int i = 0;
			// int triangleCount = mScene->maxTriangles;
			
			//iterate through the primitives
			for (auto& primitive : mesh.primitives)
			{
				std::string name = mesh.name.append(std::to_string(i));
				
				i++;
				
				//check if the mesh is duplicated
				int access = isMeshDuplicated(name, renderer_->getMeshes());
				
				if (access != -1)
				{
					renderer_->addMatrixToMesh(access, newM2W);
					continue;
				}
				
				KMesh* newMesh; // we will pass this raw ptr to an unique ptr in the renderer
							
				if (lightmesh == nullptr)
				{
					newMesh = new KMesh();
					newMesh->setName(name);
				}
				else
				{
					newMesh = lightmesh;
				}
				
				//read the material
				if (!model.materials.empty())
				{
					tinygltf::Material material = model.materials[primitive.material];

					newMesh->setMaterial( glm::vec4(material.pbrMetallicRoughness.baseColorFactor[0],
						material.pbrMetallicRoughness.baseColorFactor[1],
						material.pbrMetallicRoughness.baseColorFactor[2],
						material.pbrMetallicRoughness.baseColorFactor[3]));

					//newMesh->mMaterial.mMetallic = material.pbrMetallicRoughness.metallicFactor;
					//newMesh->mMaterial.mRoughness = material.pbrMetallicRoughness.roughnessFactor;

					if (!model.textures.empty())
					{
						int width, height, channels;

						//first account for normal texture
						if (material.normalTexture.index != -1)
						{
							const tinygltf::Texture& texture = model.textures[material.normalTexture.index];
							const tinygltf::Image& image = model.images[texture.source];

							std::string path = filepath + image.uri;

							unsigned char* normalData = stbi_load(path.c_str(), &width, &height, &channels, 0);

							newMesh->generateTexture(MatType::NORMAL, normalData, width, height, channels, false);
						}

						if (material.pbrMetallicRoughness.baseColorTexture.index != -1)
						{
							//then for regular coloured texture
							const tinygltf::Texture& texture = model.textures[material.pbrMetallicRoughness.baseColorTexture.index];
							const tinygltf::Image& image = model.images[texture.source];

							std::string path = filepath + image.uri;

							unsigned char* baseData = stbi_load(path.c_str(), &width, &height, &channels, 0);

							newMesh->generateTexture(MatType::BASE, baseData, width, height, channels, false);

						}

						if (material.pbrMetallicRoughness.metallicRoughnessTexture.index != -1)
						{
							//then for metallic texture
							const tinygltf::Texture& texture = model.textures[material.pbrMetallicRoughness.metallicRoughnessTexture.index];
							const tinygltf::Image& image = model.images[texture.source];

							std::string path = filepath + image.uri;

							unsigned char* metalData = stbi_load(path.c_str(), &width, &height, &channels, 0);

							newMesh->generateTexture(MatType::METALLIC, metalData, width, height, channels, false);
						}
					}

				}

				int vertexCount = static_cast<int>(model.accessors[primitive.attributes.at("POSITION")].count);
				newMesh->initVertices(vertexCount);
				//set the transformation
				newMesh->addM2W(newM2W);

				
				for (int idx = 0; idx < vertexCount; idx++)
				{
					//std::cout << "IDX : " << idx << std::endl;

					{
						// read the positions 
						const tinygltf::Accessor& currentAtt = model.accessors[primitive.attributes.at("POSITION")];
						const tinygltf::BufferView& bufferView = model.bufferViews[currentAtt.bufferView];
						const tinygltf::Buffer& buff = model.buffers[bufferView.buffer];

						//we combine the offsets defined by the bhffer view and the positions
						const float* posBytes = reinterpret_cast<const float*>(&buff.data[bufferView.byteOffset + currentAtt.byteOffset]);

						//set the positions
						newMesh->setVertexProperty(idx, VertexProps::Pos, posBytes) ;
					}

					{
						// read the positions 
						const tinygltf::Accessor& currentAtt = model.accessors[primitive.attributes.at("NORMAL")];
						const tinygltf::BufferView& bufferView = model.bufferViews[currentAtt.bufferView];
						const tinygltf::Buffer& buff = model.buffers[bufferView.buffer];

						//we combine the offsets defined by the bhffer view and the positions
						const float* normBytes = reinterpret_cast<const float*>(&buff.data[bufferView.byteOffset + currentAtt.byteOffset]);

						//set the normals
						newMesh->setVertexProperty(idx, VertexProps::Normal, normBytes);

					}

					{
						//// read the  tangents
						const tinygltf::Accessor& currentAtt = model.accessors[primitive.attributes.at("TANGENT")];
						const tinygltf::BufferView& bufferView = model.bufferViews[currentAtt.bufferView];
						const tinygltf::Buffer& buff = model.buffers[bufferView.buffer];

						//we combine the offsets defined by the bhffer view and the positions
						const float* tangentBytes = reinterpret_cast<const float*>(&buff.data[bufferView.byteOffset + currentAtt.byteOffset]);

						//set the tangents
						newMesh->setVertexProperty(idx, VertexProps::Tangent, tangentBytes);
					}


					// read the  uvs
					{
						// read the positions W
						const tinygltf::Accessor& currentAtt = model.accessors[primitive.attributes.at("TEXCOORD_0")];
						const tinygltf::BufferView& bufferView = model.bufferViews[currentAtt.bufferView];
						const tinygltf::Buffer& buff = model.buffers[bufferView.buffer];

						//we combine the offsets defined by the bhffer view and the positions
						const float* uvBytes = reinterpret_cast<const float*>(&buff.data[bufferView.byteOffset + currentAtt.byteOffset]);

						//set the normals
						newMesh->setVertexProperty(idx, VertexProps::UV, uvBytes);
					}
					//color to the vertex
					newMesh->setVertexProperty(idx, VertexProps::Color, &newMesh->getMaterial().getColor()[0]);

					//newMesh->mVertices[idx].triangleIndex = static_cast<float>(triangleCount);
					
					// if ((idx + 1) % 3 == 0)
					// {
					// 	triangleCount++;
					// }

				}

				

				//mScene->maxTriangles = triangleCount;

				{
					// read the  indices
					const tinygltf::Accessor& currentAtt = model.accessors[primitive.indices];
					const tinygltf::BufferView& bufferView = model.bufferViews[currentAtt.bufferView];
					const tinygltf::Buffer& buff = model.buffers[bufferView.buffer];

					if (currentAtt.componentType == 5123)
					{
						const unsigned short* indexBytes = reinterpret_cast<const unsigned short*>(&buff.data[bufferView.byteOffset + currentAtt.byteOffset]);

						for (int i = 0; i < currentAtt.count; i++)
						{
							//we combine the offsets defined by the bhffer view and the positions
							newMesh->addIndex(indexBytes[i]);
						}
					}
					else if (currentAtt.componentType == 5125)
					{
						const unsigned int* indexBytes = reinterpret_cast<const unsigned int*>(&buff.data[bufferView.byteOffset + currentAtt.byteOffset]);

						for (int i = 0; i < currentAtt.count; i++)
						{
							//we combine the offsets defined by the bhffer view and the positions
							newMesh->addIndex(indexBytes[i]);
						}
					}
				}

				//mScene->ClaculateCurvatures( newMesh);
				
				glm::vec3 avgNormal = glm::vec3(0);


				struct IVec3Hash {
    			size_t operator()(const glm::ivec3& v) const {
        			return std::hash<int>()(v.x) ^ 
            			  (std::hash<int>()(v.y) << 1) ^ 
              			  (std::hash<int>()(v.z) << 2);
   				 }
				};
				
				std::unordered_map<glm::ivec3, std::vector<int>, IVec3Hash> grid;

				for (int i = 0; i < newMesh->getVertices().size(); i++)
				{
				    glm::ivec3 key = glm::ivec3(
				        glm::round(newMesh->getVertices().at(i).position_.x / 0.01f),
				        glm::round(newMesh->getVertices().at(i).position_.y / 0.01f),
				        glm::round(newMesh->getVertices().at(i).position_.z / 0.01f)
				    );
				    grid[key].push_back(i);
				}

				// compare vertices in the same grid
				for (auto& [key, indices] : grid)
				{
				    glm::vec3 avg = glm::vec3(0);
				    for (int i : indices)
				        avg += glm::vec3(newMesh->getVertices().at(i).normal_);
				    avg = glm::normalize(avg);

				    for (int i : indices)
				        newMesh->setVertexProperty(i, VertexProps::AvgNormal, &avg.x);
				}

				// // Normalize 
				// for (auto& vertex : newMesh->mVertices) 
				// {
				// 	vertex.mAvgNormal = glm::normalize(vertex.mAvgNormal);
				// }

				// we generate the mesh
				newMesh->generateMesh();

				if (lightmesh != nullptr)
				{
					//now we generate the mesh
					lightmesh = newMesh;
				}
				else
				{
					renderer_->addMesh(newMesh);
				}
			}

		}
    }



}