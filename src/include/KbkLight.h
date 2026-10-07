#ifndef HH_KBKLIGHT
#define HH_KBKLIGHT

#include <glm/glm.hpp>
#include <glm/vec4.hpp>
#include <glm/vec3.hpp>

namespace Graphics
{
    class KMesh;

    enum class KLightType {

        Point,
        Directional,
        Spotlight
    };

    class KLight
    {
    public:
        KLight() = default;
        virtual ~KLight() = default;

        /**
         * @brief: set light color (specular will always be white)
         * @param[in] color: color to set (RGBA)
         * @return: void
         */
        void setColor(const glm::vec4& color);

        /**
         * @brief: get light color
         * @return: current color (RGBA)
         */
        const glm::vec4& getColor() const;

        /**
         * @brief: get light type
         * @return: type of the light
         */
        const KLightType& getType() const;

    protected:
        KLightType type_;
        glm::vec4 color_{1.0f};   // color of the light (specular will always be white)
    };

    class KPointLight : public KLight
    {
    public:
        KPointLight()
        {
            KLight::type_ = KLightType::Point; 
        }

        ~KPointLight() override = default;

        /** @brief: set light position */
        void setPosition(const glm::vec3& pos);

        /** @brief: get light position */
        const glm::vec3& getPosition() const;

        /** @brief: set the radius of the circular movement */
        void setMovementRadius(float movementRadius);

        /** @brief: get the radius of the circular movement */
        float getMovementRadius() const;

        /** @brief: set the current motion angle */
        void setMotionAngle(float motionAngle);

        /** @brief: get the current motion angle */
        float getMotionAngle() const;

        /** @brief: set the mesh used to represent the light (not owned) */
        void setSphere(KMesh* sphere);

        /** @brief: get the mesh used to represent the light */
        KMesh* getSphere() const;

        /** @brief: set the radius of the reach of the light */
        void setRadius(float radius);

        /** @brief: get the radius of the reach of the light */
        float getRadius() const;

    private:
        glm::vec3 position_{0.0f};   // position of the light
        float movementRadius_ = 0.0f;
        float motionAngle_    = 0.0f;
        KMesh* sphere_        = nullptr;
        float radius_         = 1.0f;   // radius of the reach of the light
    };
}

#endif // HH_KBKLIGHT