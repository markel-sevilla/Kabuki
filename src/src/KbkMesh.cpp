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

        // Delete the VAO
        glDeleteVertexArrays(1, &vao_);

        // Delete the VBO and EBO
        glDeleteBuffers(1, &vbo_);
        glDeleteBuffers(1, &ebo_);

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
    const std::vector<KVertex>& KMesh::getVertices() const
    {
        return vertices_;
    }
    const std::vector<unsigned>& KMesh::getIndices() const
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

}