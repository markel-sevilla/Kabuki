#ifndef HH_KBKMESH
#define HH_KBKMESH

#include <memory>
#include <glad/glad.h>
#include <vector>
#include <glm/mat4x4.hpp>
#include "KbkTexture.h"
#include "KbkVertex.h"
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
   
    class KMesh
	{
        public:
        
        KMesh();
        ~KMesh();

        /* 
        * @brief: it generates the mesh with the default gl parameters
        * @return: void  
        */
        void generateMesh();

        /* 
        * @brief: getters for the different vertex info
        * @return: std::vector
        */
        const std::vector<KVertex>& getVertices() const;
        const std::vector<unsigned>& getIndices() const;
        const std::vector<glm::mat4>& getM2Ws() const;

        /* 
        * @brief: getters for the mesh identifications
        * @return: unsigned  
        */
        unsigned getVAO() const;
        unsigned getID() const;

        /* 
        * @brief: getter for the material
        * @return: void  
        */
        const KMaterial& getMaterial() const;

        /* 
        * @brief: getter for the texture
        * @return: texture  
        */
        const KTexture* getTexture() const;

        private:

        unsigned int vao_ = 0, vbo_ = 0, ebo_ = 0;
		std::vector<KVertex> vertices_;

        std::vector<unsigned int> indices_;
        
        unsigned id_;
        //std::string name_;
        std::vector<glm::mat4> m2ws_;
        KMaterial material_;
        std::unique_ptr<KTexture> tex_;
	};

}

#endif