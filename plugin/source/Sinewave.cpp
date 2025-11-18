#include "TestPlugin/Sinewave.h"
#include <numbers>
#include <cmath>

void Sinewave::prepare(double sampleRate) {
  m_CurrentSampleRate = static_cast<float>(sampleRate);
  m_TimeIncrement = 1.f / m_CurrentSampleRate;
}

void Sinewave::process(float* output,const int numChannels) {
  constexpr float piTwo = 2.0f*std::numbers::pi_v<float>;
for (int sample{}; sample < numChannels; ++sample) {
  output[sample] = m_Amplitude * std::sinf(piTwo*m_Frequency * m_CurrentTime);
  m_CurrentTime += m_TimeIncrement;
  }
}
