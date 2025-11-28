#include "TestPlugin/Squarewave.h"
#include <numbers>

void Squarewave::prepare(double sampleRate) {
  const float sampleRateFl{static_cast<float>(sampleRate)};
  m_TimeIncrement = 1.f / sampleRateFl;
  m_CurrentSampleRate = sampleRateFl;
  m_SmoothedFreq.reset(sampleRate,.1f);
  m_SmoothedFreq.setCurrentAndTargetValue(220.f);
}

void Squarewave::process(float* output, const int numSamples) {
  constexpr float piTwo = 2.0f * std::numbers::pi_v<float>;
  for (int sample{}; sample < numSamples; ++sample) {
    float frequency { m_SmoothedFreq.getNextValue()};
    const float phaseIncrease{(piTwo * frequency) / m_CurrentSampleRate};
    const float sine{std::sinf(m_CurrentTime)};
    const float sign{sine > 0.0f ? 1.0f : -1.0f};

    output[sample] = sign * m_Amplitude;
    m_CurrentTime += phaseIncrease;

    if (m_CurrentTime > piTwo) {
      m_CurrentTime -= piTwo;
    }
  }
}
