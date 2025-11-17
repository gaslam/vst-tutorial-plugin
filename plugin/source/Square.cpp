#include "TestPlugin/Square.h"

void Square::paint(juce::Graphics& pGraphics) {
  // (Our component is opaque, so we must completely fill the background with a
  // solid colour)
  pGraphics.fillAll(
      getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));

  pGraphics.setColour(juce::Colours::white);
  pGraphics.setFont(30.0f);
  pGraphics.drawFittedText("TestPlugin!", getLocalBounds(),
                   juce::Justification::centred, 1);
}

void Square::resized() {

}
