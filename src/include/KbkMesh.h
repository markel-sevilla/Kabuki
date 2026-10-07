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

        /**  
        * @brief: it generates the mesh with the default gl parameters
        * @return: void  
        */
        void generateMesh();

        /**
        * @brief: it generates the texture with the defined gl parameters
        * @return: void  
        */
        void generateTexture(MatType type, unsigned char* data, unsigned width, unsigned height, unsigned channels, bool wrap);

        /** 
        * @brief: getters for the different vertex info
        * @return: std::vector
        */
        const std::vector<KVertex>& getVertices() const;
        const std::vector<unsigned int>& getIndices() const;
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

        /* 
        * @brief: getter for the texture
        * @return: string  
        */
        const std::string& getName() const;
        

        /* 
        * @brief: add a matrix to the mesh
        * @return: void
        */
        void addM2W(const glm::mat4& mat4);

         /* 
        * @brief: sets name 
        * @return: void
        */
        void setName(const std::string& name);

        /* 
        * @brief: creates a default texture for the mesh 
        * @return: void
        */
        void addEmptyTexture();

        /**  
        * @brief: sets the material of the mesh 
        * @return: void
        */
        void setMaterial( const glm::vec4& color, bool metallic = false, float shininess = 0, float specular = 0, float roughness = 0);
        

        /**  
        * @brief: set vertices vector 
        * @param[in] size: size to init the vertex vector
        * @return: void
        */
        void initVertices(size_t size);

        /**  
        * @brief: set indices vector 
        * @param[in] size: size to init the vertex vector
        * @return: void
        */
        void initIndices(size_t size);

        /**  
        * @brief: set indices vector 
        * @param[in] idx: index to add
        * @return: void
        */
        void addIndex(unsigned int idx);

        /**  
        * @brief: add vertex 
        * @param[in] vtx: vertex to push back
        * @return: void
        */
        void addVertex(const Graphics::KVertex& vtx);

        /**  
        * @brief: set vertex property 
        * @param[in] idx: index
        * @param[in] property: property type
        * @param[in] data: float ptr defining the values of the prop
        * @return: void
        */
        void setVertexProperty( unsigned int idx, VertexProps property, const float* data);

        private:

        unsigned int vao_ = 0, vbo_ = 0, ebo_ = 0;
		std::vector<KVertex> vertices_;

        std::vector<unsigned int> indices_;
        
        unsigned id_;
        std::string name_;
        std::vector<glm::mat4> m2ws_;
        KMaterial material_;
        std::unique_ptr<KTexture> tex_;
	};

}

#endif