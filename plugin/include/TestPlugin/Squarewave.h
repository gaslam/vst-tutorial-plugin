#ifndef SQUAREWAVECHANNEL_H
#define SQUAREWAVECHANNEL_H
#include "Wave.h"

#include <juce_audio_basics/juce_audio_basics.h>

#include <cmath>

class Squarewave : public Wave{
public:
  void prepare(float sampleRate) override;
  void process(float* output,const int numSamples) override;

private:
  float m_CurrentTime{};
  float m_TimeIncrement{};
};

#endif
