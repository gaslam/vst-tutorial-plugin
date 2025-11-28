#ifndef SINEWAVE_H
#define SINEWAVE_H
#include "Wave.h"

class Sinewave : public Wave{
  public:
  void process(float* output,const int numSamples) override;

  private:
  float m_CurrentTime{};
};
#endif

