#include "common/Scene/Geometry/Mesh/MeshObject.h"
#include "common/Acceleration/AccelerationCommon.h"
#include "common/Scene/Geometry/Primitives/PrimitiveBase.h"
#include "common/Scene/Geometry/Ray/Ray.h"
#include "common/Intersection/IntersectionState.h"

MeshObject::MeshObject() :
    storedMaterial(nullptr)
{
}

MeshObject::MeshObject(const Material* inputMaterial) :
    storedMaterial(inputMaterial)
{
}

MeshObject::~MeshObject()
{
}

void MeshObject::AddPrimitive(std::shared_ptr<PrimitiveBase> newPrimitive)
{
    elements.emplace_back(std::move(newPrimitive));
}

void MeshObject::Finalize()
{
    boundingBox.Reset();
    for (size_t i = 0; i < elements.size(); ++i) {
        elements[i]->Finalize();
        boundingBox.IncludeBox(elements[i]->GetBoundingBox());
    }
    assert(acceleration);
    acceleration->Initialize(elements);
}

void MeshObject::CreateAccelerationData(AccelerationTypes perObjectType)
{
    acceleration = AccelerationGenerator::CreateStructureFromType(perObjectType);
    assert(acceleration);
}

bool MeshObject::Trace(Ray* inputRay, IntersectionState* outputIntersection) const
{
    return acceleration->Trace(inputRay, outputIntersection);
}

const Material* MeshObject::GetMaterial() const
{
    return storedMaterial;
}

void MeshObject::SetMaterial(const class Material* inputMaterial)
{
    storedMaterial = inputMaterial;
}