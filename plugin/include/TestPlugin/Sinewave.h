#ifndef SINEWAVE_H
#define SINEWAVE_H
#include <juce_audio_basics/juce_audio_basics.h>

class Sinewave {
  public:
  void prepare(double sampleRate);
  void process(float* output,const int numSamples) ;
  [[nodiscard]] float getAmplitude() const {return m_Amplitude;};
  [[nodiscard]] float getFrequency() const {return m_SmoothedFreq.getCurrentValue();};

  void setAmplitude(float amplitude) {m_Amplitude = amplitude;};
  void setFrequency(float frequency) {
    m_SmoothedFreq.setTargetValue(frequency);
  };
  private:
  float m_Amplitude = 0.2f;
  float m_CurrentSampleRate = 0.0f;
  float m_CurrentTime{};
  std::vector<float> m_Phases{};

  juce::SmoothedValue<float,juce::ValueSmoothingTypes::Multiplicative> m_SmoothedFreq;

};
#endif

