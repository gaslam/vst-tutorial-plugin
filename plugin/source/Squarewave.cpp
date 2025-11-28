#include "TestPlugin/Squarewave.h"
#include <numbers>

void Squarewave::prepare(float sampleRate) {
  Wave::prepare(sampleRate);
  m_TimeIncrement = 1.f / sampleRate;
}

void Squarewave::process(float* output, const int numSamples) {
  constexpr float piTwo = 2.0f * std::numbers::pi_v<float>;

  auto& smoothedFreq { getSmoothFrequency()};
  auto amplitude {getSmoothAmp()};
  const float sampleRate { getSampleRate() };

  for (int sample{}; sample < numSamples; ++sample) {
    float frequency { smoothedFreq.getNextValue()};
    const float phaseIncrease{(piTwo * frequency) / sampleRate};
    const float sine{std::sinf(m_CurrentTime)};
    const float sign{sine > 0.0f ? 1.0f : -1.0f};

    output[sample] += sign * amplitude.getNextValue();
    m_CurrentTime += phaseIncrease;

    if (m_CurrentTime > piTwo) {
      m_CurrentTime -= piTwo;
    }
  }
}
