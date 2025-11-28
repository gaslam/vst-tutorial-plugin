#ifndef SQUAREWAVECHANNEL_H
#define SQUAREWAVECHANNEL_H
#include <juce_audio_basics/juce_audio_basics.h>

#include <cmath>

class Squarewave {
public:
  void prepare(double sampleRate);
  void process(float* output,const int numSamples);

  [[nodiscard]] float getAmplitude() const {return m_Amplitude;};
  [[nodiscard]] float getFrequency() const {return m_SmoothedFreq.getCurrentValue();};

  void setAmplitude(const float amplitude) {
    m_Amplitude = amplitude;
  };
  void setFrequency(const float frequency) {m_SmoothedFreq.setTargetValue( frequency);}

private:
  float m_Amplitude = 0.2f;
  juce::SmoothedValue<float,juce::ValueSmoothingTypes::Multiplicative> m_SmoothedFreq;
  float m_CurrentSampleRate = 0.0f;
  float m_TimeIncrement = 0.0f;
  float m_CurrentTime{};
};

#endif
