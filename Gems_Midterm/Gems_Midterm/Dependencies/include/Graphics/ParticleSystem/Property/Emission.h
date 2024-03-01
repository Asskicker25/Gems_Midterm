#pragma once
#include "ParticleSystemProperty.h"

namespace System_Particle
{
	class Emission : public ParticleSystemProperty
	{

	public:

		struct Burst
		{

			Burst(float time, float count, float cycles, float interval, float probability)
			{
				this->time = time;
				this->count = count;
				this->cycles = cycles;
				this->interval = interval;
				this->probability = probability;
			}

			float time = 0;					// Burst start time
			int count = 10;					// Number of particle
			int cycles = 1;					// Number of bursts
			float interval = 1;				// Interval between bursts
			float probability = 1;			// 0 - 1

			int currentCycle = 0;
		};

		//Num Of particles per second
		float rateOverTime = 10;

		void AddBurst(Burst burstInfo);
		void RemoveBurst(int index);

		// Inherited via ParticleSystemProperty
		void OnPropertyDraw() override;
		void OnSceneDraw() override;
		void DrawBurst(Burst& burstInfo, int index);

		std::vector<Burst> mListOfBursts;

	};

}

