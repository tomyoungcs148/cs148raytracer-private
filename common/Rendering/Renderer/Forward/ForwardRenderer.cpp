#include "common/Rendering/Renderer/Forward/ForwardRenderer.h"
#include "common/Scene/Scene.h"
#include "common/Sampling/Sampler.h"
#include "common/Scene/Lights/Light.h"
#include "common/Scene/Geometry/Primitives/Primitive.h"
#include "common/Scene/Geometry/Mesh/MeshObject.h"
#include "common/Rendering/Material/Material.h"
#include "common/Intersection/IntersectionState.h"

ForwardRenderer::ForwardRenderer(std::shared_ptr<Scene> scene, std::shared_ptr<Sampler> sampler):
    Renderer(scene, sampler)
{
}

void ForwardRenderer::InitializeRenderer()
{
}

glm::vec3 ForwardRenderer::ComputeSampleColor(const IntersectionState& intersection) const
{
    if (!intersection.hasIntersection) {
        return glm::vec3();
    }

    glm::vec3 intersectionPoint = intersection.intersectionRay.GetRayPosition(intersection.intersectionT);
    const MeshObject* parentObject = intersection.intersectedPrimitive->GetParentMeshObject();
    assert(parentObject);

    const Material* objectMaterial = parentObject->GetMaterial();
    assert(objectMaterial);

    // Compute the color at the intersection.
    glm::vec3 sampleColor;
    for (size_t i = 0; i < storedScene->GetTotalLights(); ++i) {
        const Light* light = storedScene->GetLightObject(i);
        assert(light);

        // Sample light using rays, Number of samples and where to sample is determined by the light.
        std::vector<Ray> sampleRays;
        light->ComputeSampleRays(sampleRays, intersectionPoint);

        for (size_t s = 0; s < sampleRays.size(); ++s) {
            // note that max T should be set to be right before the light.
            if (storedScene->Trace(&sampleRays[s], nullptr)) {
                continue;
            }
            const float lightAttenuation = light->ComputeLightAttenuation(intersectionPoint);

            // Note that the material should compute the parts of the lighting equation too.
            const glm::vec3 brdfResponse = objectMaterial->ComputeBRDF(intersectionPoint, *light, sampleRays[s], *intersection.intersectedPrimitive);
            sampleColor += brdfResponse * lightAttenuation;
        }
    }

    return sampleColor;
}