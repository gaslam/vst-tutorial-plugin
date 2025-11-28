#include "TestPlugin/Sinewave.h"
#include <numbers>

void Sinewave::process(float* output, const int numSamples) {
  constexpr float piTwo = 2.0f * std::numbers::pi_v<float>;
  auto& smoothFreq{getSmoothFrequency()};
  const float sampleRate{getSampleRate()};
  auto amplitude{getSmoothAmp()};

  for (int sample{}; sample < numSamples; ++sample) {
    float frequency = smoothFreq.getNextValue();
    const float phaseIncrease{(piTwo * frequency) / sampleRate};

    output[sample] += amplitude.getNextValue() * std::sinf(m_CurrentTime);
    m_CurrentTime += phaseIncrease;

    if (m_CurrentTime > piTwo) {
      m_CurrentTime -= piTwo;
    }
  }
}
