#ifndef SQUAREWAVECHANNEL_H
#define SQUAREWAVECHANNEL_H
#include <cmath>

class Squarewave {
public:
  void prepare(double sampleRate);
  void process(float* output,const int numChannels);

  [[nodiscard]] float getAmplitude() const {return m_Amplitude;};
  [[nodiscard]] float getFrequency() const {return m_Frequency;};

  void setAmplitude(const float amplitude) {
    m_Amplitude = amplitude;
    m_Frequency = m_Amplitude / 2.f;
  };
  void setFrequency(const float frequency) {m_Frequency = frequency;};

private:
  float m_Amplitude = 0.2f;
  float m_Frequency = 440.0f;
  float m_CurrentSampleRate = 0.0f;
  float m_TimeIncrement = 0.0f;
  float m_CurrentTime{};
};

#endif
