#include "TestPlugin/Sinewave.h"
#include <numbers>
#include <cmath>


void Sinewave::prepare(double sampleRate) {
  m_CurrentSampleRate = static_cast<float>(sampleRate);
  m_SmoothedFreq.reset(sampleRate,.1f);
  m_SmoothedFreq.setCurrentAndTargetValue(220);
}

void Sinewave::process(float* output,const int numSamples) {
  constexpr float piTwo = 2.0f*std::numbers::pi_v<float>;
for (int sample{}; sample < numSamples; ++sample) {
  float frequency = m_SmoothedFreq.getNextValue();
  const float phaseIncrease { (piTwo * frequency)/ m_CurrentSampleRate};

  output[sample] = m_Amplitude * std::sinf(m_CurrentTime);
  m_CurrentTime += phaseIncrease;

  if (m_CurrentTime > piTwo) {
    m_CurrentTime -= piTwo;
  }
  }
}
