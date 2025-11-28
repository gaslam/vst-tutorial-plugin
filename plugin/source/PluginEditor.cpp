#include "TestPlugin/PluginEditor.h"
#include "TestPlugin/PluginProcessor.h"

namespace audio_plugin {
AudioPluginAudioProcessorEditor::AudioPluginAudioProcessorEditor(
    AudioPluginAudioProcessor& p)
    : AudioProcessorEditor(&p),
      processorRef(p),
      m_FreqSliderAttachment(processorRef.getState(), "freqHz", m_FreqSlider),
      m_ButtonAttachment(processorRef.getState(), "play", m_PlayButton),
      m_ComboBoxAttachment(processorRef.getState(), "waveSelect", m_ComboBox)
{
  juce::ignoreUnused(processorRef);
  // Make sure that before the constructor has finished, you've set the
  // editor's size to whatever you need it to be.
  setSize(400, 600);
  m_FreqSlider.setSliderStyle(juce::Slider::LinearVertical);
  m_FreqSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 100, 50);
  addAndMakeVisible(m_FreqSlider);

  const juce::String playingText{"Playing"};
  const juce::String pausedText{"Paused"};
  m_PlayButton.setButtonText(playingText);
  m_PlayButton.setToggleState(true,
                              juce::NotificationType::dontSendNotification);
  m_PlayButton.setClickingTogglesState(true);

  m_PlayButton.setColour(juce::TextButton::ColourIds::buttonOnColourId,
                         juce::Colours::green);
  m_PlayButton.setColour(juce::TextButton::ColourIds::buttonColourId,
                         juce::Colours::red);

  m_PlayButton.onClick = [this, playingText, pausedText]() {
    const bool isPlaying = m_PlayButton.getToggleState();
    m_PlayButton.setButtonText(isPlaying ? playingText : pausedText);
  };
  addAndMakeVisible(m_PlayButton);
  m_ComboBox.setJustificationType(juce::Justification::centred);
  m_ComboBox.addItemList({"Square", "Sine"}, 1);
  m_ComboBox.setSelectedItemIndex(1);
  addAndMakeVisible(m_ComboBox);
}

AudioPluginAudioProcessorEditor::~AudioPluginAudioProcessorEditor() {}

void AudioPluginAudioProcessorEditor::paint(juce::Graphics&) {}

void AudioPluginAudioProcessorEditor::resized() {
  // This is generally where you'll want to lay out the positions of any
  // subcomponents in your editor..
  m_FreqSlider.setBounds(getWidth() / 2 - 50, getWidth() / 2 - 100, 100, 200);
  m_ComboBox.setBounds(getWidth() / 2 - 75, getWidth() / 2 - 140, 150, 20);
  m_PlayButton.setBounds(getWidth() / 2 - 50, getWidth() / 2 + 120, 100, 20);
}

}  // namespace audio_plugin