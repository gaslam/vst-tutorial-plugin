#include "TestPlugin/Squarewave.h"
#include <numbers>
void Squarewave::prepare(double sampleRate) {
  m_TimeIncrement = 1.f / static_cast<float>(sampleRate);
}


void Squarewave::process(float* output,const int numChannels) {
  constexpr float piTwo = 2.0f*std::numbers::pi_v<float>;
  for (int sample{}; sample < numChannels; ++sample) {
    const float sine{std::sinf(piTwo*m_Frequency * m_CurrentTime)};
    const float sign { sine > 0 ? 1.0f : -1.0f };
    const float value = m_Amplitude * sign;
output[sample] = value;
    m_CurrentTime += m_TimeIncrement;
  }
}
