#pragma once
#include "ParticleSystemProperty.h"
#include "../../Model.h"

namespace System_Particle
{
    class ParticleRenderer : public ParticleSystemProperty
    {

    public:
        ParticleRenderer();

        float minParticleSize = 0.0f;
        float maxParticleSize = 1.0f;

        Model* mParticleModel = nullptr;

        // Inherited via ParticleSystemProperty
        void OnPropertyDraw() override;
        void OnSceneDraw() override;
    };
}

