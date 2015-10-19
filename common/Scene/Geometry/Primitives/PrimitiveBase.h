#pragma once

#include "common/common.h"
#include "common/Acceleration/AccelerationNode.h"

class PrimitiveBase: public AccelerationNode
{
public:
    virtual const class MeshObject* GetParentMeshObject() const = 0;
    virtual void SetVertexPosition(int index, glm::vec3 position) = 0;
    virtual void SetVertexNormal(int index, glm::vec3 normal) = 0;
    virtual void SetVertexUV(int index, glm::vec2 uv) = 0;
    virtual int GetTotalVertices() const = 0;
    virtual void Finalize() = 0;
};