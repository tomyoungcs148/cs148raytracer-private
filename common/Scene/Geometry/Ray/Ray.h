#pragma once

#include "common/Scene/SceneObject.h"

class Ray : public SceneObject
{
public:
    Ray(glm::vec3 inputPosition, glm::vec3 inputDirection, float inputMaxT = std::numeric_limits<float>::max());

    virtual glm::vec4 GetForwardDirection() const override;
    glm::vec3 GetRayDirection() const;
private:
    glm::vec3 rayDirection;
    float maxT;
};