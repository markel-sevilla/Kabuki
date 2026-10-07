/**
 * @file    KTexture.h
 * @brief   wrap up for the texture 
 *
 * @details  Has basic texture info
 *          
 *
 * @author  Markel Sevilla
 * @date    2026-07-25
 * @version 1.0
 */
#include "KbkTexture.h"
#include "KbkMesh.h"

namespace Graphics
{
   
    KMesh::KMesh()
    {
        vertices_.clear();
        indices_.clear();
        tex_ = nullptr;

        generateMesh();
    }

    KMesh::~KMesh()
    {
        vertices_.clear();
        indices_.clear();
    }

    void KMesh::generateMesh()

    {
        //vertex array bind
        glGenVertexArrays(1, &vao_);
        glBindVertexArray(vao_);

        //vertex buffer bind
        glGenBuffers(1, &vbo_);
        glBindBuffer(GL_ARRAY_BUFFER, vbo_);
        glBufferData(GL_ARRAY_BUFFER, sizeof(KVertex) * vertices_.size(), vertices_.data(), GL_STATIC_DRAW);

        ////element buffer objects
        glGenBuffers(1, &ebo_); 
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indices_.size(), indices_.data(), GL_STATIC_DRAW);

        glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(KVertex), (void*)0);
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(KVertex), reinterpret_cast<void*>(offsetof(KVertex, normal_)));
        glEnableVertexAttribArray(1);

        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(KVertex), reinterpret_cast<void*>(offsetof(KVertex, tangent_)));
        glEnableVertexAttribArray(2);

        glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(KVertex), reinterpret_cast<void*>(offsetof(KVertex, biTan_)));
        glEnableVertexAttribArray(3);

        glVertexAttribPointer(4, 2, GL_FLOAT, GL_FALSE, sizeof(KVertex), reinterpret_cast<void*>(offsetof(KVertex, uv_)));
        glEnableVertexAttribArray(4);

        glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(KVertex), reinterpret_cast<void*>(offsetof(KVertex, color_)));
        glEnableVertexAttribArray(5);

        glVertexAttribPointer(6, 3, GL_FLOAT, GL_FALSE, sizeof(KVertex), reinterpret_cast<void*>(offsetof(KVertex, avgNormal_)));
        glEnableVertexAttribArray(6);

        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

    }

    void KMesh::generateTexture(MatType type, unsigned char* data, unsigned width, unsigned height, unsigned channels, bool wrap)
    {
        // adds a default tex if we dont have one 
        if(!tex_.get())
            addEmptyTexture(); 

        tex_->generateTexture(type, data, width, height, channels, wrap);
    }

    void KMesh::setMaterial( const glm::vec4& color, bool metallic, float shininess, float specular, float roughness)
    {
        material_.setColor(color);
        material_.setMetallic(metallic);
        material_.setShininess(shininess);
        material_.setSpecular(specular);
        material_.setRoughness(roughness);
    }

    const std::vector<KVertex>& KMesh::getVertices() const
    {
        return vertices_;
    }
    const std::vector<unsigned int>& KMesh::getIndices() const
    {
        return indices_;
    }
    const std::vector<glm::mat4>& KMesh::getM2Ws() const
    {
        return m2ws_;
    }
    unsigned KMesh::getVAO() const
    {
        return vao_;
    }
    unsigned KMesh::getID() const
    {
        return id_;
    }
    const KMaterial& KMesh::getMaterial() const
    {
        return material_;
    }
    const KTexture* KMesh::getTexture() const
    {
        return tex_.get();
    }

    const std::string& KMesh::getName() const
    {
        return name_;
    }

    void KMesh::addM2W(const glm::mat4& mat) 
    {
        m2ws_.push_back(mat);
    }

    void KMesh::setName(const std::string& name) 
    {
        name_ = name;
    }

    void KMesh::addEmptyTexture()
    {
        tex_ = std::make_unique<KTexture>();
    }

    void KMesh::initVertices(size_t size)
    {
        vertices_.resize(size);
    }

    void KMesh::initIndices(size_t size)
    {
        indices_.resize(size);
    }

    void KMesh::addIndex(unsigned int idx)
    {
        indices_.push_back(idx);
    }

    void KMesh::addVertex(const Graphics::KVertex& vtx)
    {
        vertices_.push_back(vtx);
    }

    void KMesh::setVertexProperty(unsigned int idx, VertexProps property, const float* data)
    {
        switch(property)
        {
            case VertexProps::Pos:
                vertices_[idx].position_ = glm::vec4(data[3 * idx + 0], data[ 3 * idx + 1], data[ 3 * idx + 2], 1);
                break;
            case VertexProps::Normal:
                vertices_[idx].normal_ = glm::vec3(data[3 * idx + 0], data[ 3 * idx + 1], data[ 3 * idx + 2]);
                break;
            case VertexProps::Tangent:
                vertices_[idx].tangent_ = glm::vec3(data[3 * idx + 0], data[ 3 * idx + 1], data[ 3 * idx + 2]);
                break;
            case VertexProps::Bitan:
                vertices_[idx].biTan_ = glm::vec3(data[3 * idx + 0], data[ 3 * idx + 1], data[ 3 * idx + 2]);
                break;
            case VertexProps::UV:
                vertices_[idx].uv_ = glm::vec2(data[2 * idx + 0], data[2 * idx + 1]);
                break;
            case VertexProps::Color:
                vertices_[idx].color_ = glm::vec4(data[3 * idx + 0], data[3 * idx + 1], data[3 * idx + 2], 1);
                break;
            case VertexProps::AvgNormal:
                vertices_[idx].avgNormal_ = glm::vec3(data[0], data[1], data[2]);
                break;
        }
    }
}