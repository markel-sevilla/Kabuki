#include "KbkLight.h"

namespace Graphics
{
    // ---------------------------------------------------------------
    // KLight
    // ---------------------------------------------------------------

    void KLight::setColor(const glm::vec4& color)
    {
        color_ = color;
    }

    const glm::vec4& KLight::getColor() const
    {
        return color_;
    }

    const KLightType& KLight::getType() const
    {
        return type_;
    }

    // ---------------------------------------------------------------
    // KPointLight
    // ---------------------------------------------------------------

    void KPointLight::setPosition(const glm::vec3& pos)
    {
        position_ = pos;
    }

    const glm::vec3& KPointLight::getPosition() const
    {
        return position_;
    }

    void KPointLight::setMovementRadius(float movementRadius)
    {
        movementRadius_ = movementRadius;
    }

    float KPointLight::getMovementRadius() const
    {
        return movementRadius_;
    }

    void KPointLight::setMotionAngle(float motionAngle)
    {
        motionAngle_ = motionAngle;
    }

    float KPointLight::getMotionAngle() const
    {
        return motionAngle_;
    }

    void KPointLight::setSphere(KMesh* sphere)
    {
        sphere_ = sphere;
    }

    KMesh* KPointLight::getSphere() const
    {
        return sphere_;
    }

    void KPointLight::setRadius(float radius)
    {
        radius_ = radius;
    }

    float KPointLight::getRadius() const
    {
        return radius_;
    }
}