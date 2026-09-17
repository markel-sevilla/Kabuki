#ifndef HH_KBKTEXT
#define HH_KBKTEXT

#include <memory>
#include <glad/glad.h>


/*
 * @file    KTexture.h
 * @brief   Texture and material wrapper 
 *
 * @details  encapsulates everything that a texture should have
 *          
 *
 * @author  Markel Sevilla
 * @date    2026-09-15
 * @version 1.0
 */

namespace Graphics
{
   
    class Mesh
	{
        public:
        
        Mesh();
        ~Mesh();

        void GenerateMesh();

        private:

        unsigned int mVAO = 0, mVBO = 0, mEBO = 0;
		std::vector<Vertex> mVertices;

        std::vector<unsigned int> mIndices;
        
        std::string mName;
        std::vector<glm::mat4> mM2Ws;
        Material mMaterial;
        Texture* mTex;
        float maxCurvature = 0;
	};

}

#endif