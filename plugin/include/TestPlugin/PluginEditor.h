#pragma once

#include "PluginProcessor.h"
#include "TestPlugin/Square.h"

namespace audio_plugin {

class AudioPluginAudioProcessorEditor : public juce::AudioProcessorEditor {
public:
  explicit AudioPluginAudioProcessorEditor(AudioPluginAudioProcessor&);
  ~AudioPluginAudioProcessorEditor() override;

  void paint(juce::Graphics&) override;
  void resized() override;

private:
  Square square;
  juce::Slider m_FreqSlider;
  juce::TextButton m_PlayButton;
  juce::ComboBox m_ComboBox{"FrequencyCombobox"};
  // This reference is provided as a quick way for your editor to
  // access the processor object that created it.
  AudioPluginAudioProcessor& processorRef;

  juce::AudioProcessorValueTreeState::SliderAttachment m_FreqSliderAttachment;
  juce::AudioProcessorValueTreeState::ButtonAttachment m_ButtonAttachment;
  juce::AudioProcessorValueTreeState::ComboBoxAttachment m_ComboBoxAttachment;
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AudioPluginAudioProcessorEditor)
};
}  // namespace audio_plugin
