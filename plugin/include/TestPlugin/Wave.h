//
// Created by gaspa on 28/11/2025.
//

#ifndef WOLFSOUNDAUDIOPLUGINTEMPLATE_WAVE_H
#define WOLFSOUNDAUDIOPLUGINTEMPLATE_WAVE_H
#include <juce_audio_basics/juce_audio_basics.h>

class Wave {
  public:
  Wave() = default;
  virtual ~Wave() = default;

  Wave(const Wave&) = delete;
  Wave& operator=(const Wave&) = delete;
  Wave(Wave&&) = delete;
  Wave& operator=(Wave&&) = delete;

  [[nodiscard]] float getAmplitude() const {return m_SmoothedAmp.getCurrentValue();}
  [[nodiscard]] float getFrequency() const {return m_SmoothedFreq.getCurrentValue();}
  [[nodiscard]] float getSampleRate() const {return m_CurrentSampleRate;}

  void setAmplitude(const float amplitude) {
    m_SmoothedAmp.setTargetValue(amplitude);
  };
  void setFrequency(const float frequency) {m_SmoothedFreq.setTargetValue( frequency);}

  virtual void prepare(float sampleRate) {
    const double sampleRateDbl{static_cast<double>(sampleRate)};
    m_SmoothedFreq.reset(sampleRateDbl,0.1);
    m_SmoothedFreq.setCurrentAndTargetValue(220.f);
    m_SmoothedAmp.reset(sampleRateDbl,0.1);
    m_SmoothedFreq.setCurrentAndTargetValue(0.4f);
    m_CurrentSampleRate = sampleRate;
  };
  virtual void process(float* output,const int numSamples) = 0;

  protected:
  [[nodiscard]] juce::SmoothedValue<float,juce::ValueSmoothingTypes::Multiplicative>&  getSmoothFrequency() {return m_SmoothedFreq;}
  [[nodiscard]] juce::SmoothedValue<float,juce::ValueSmoothingTypes::Multiplicative>&  getSmoothAmp() {return m_SmoothedAmp;}

  private:
  float m_CurrentSampleRate {};

  juce::SmoothedValue<float,juce::ValueSmoothingTypes::Multiplicative> m_SmoothedAmp;
  juce::SmoothedValue<float,juce::ValueSmoothingTypes::Multiplicative> m_SmoothedFreq;
};

#endif  // WOLFSOUNDAUDIOPLUGINTEMPLATE_WAVE_H
