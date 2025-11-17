#include "TestPlugin/PluginEditor.h"
#include "TestPlugin/PluginProcessor.h"

namespace audio_plugin {
AudioPluginAudioProcessorEditor::AudioPluginAudioProcessorEditor(
    AudioPluginAudioProcessor& p)
    : AudioProcessorEditor(&p), processorRef(p) {
  juce::ignoreUnused(processorRef);
  // Make sure that before the constructor has finished, you've set the
  // editor's size to whatever you need it to be.
  setSize(400, 600);
  addAndMakeVisible(square);
}

AudioPluginAudioProcessorEditor::~AudioPluginAudioProcessorEditor() {}

void AudioPluginAudioProcessorEditor::paint(juce::Graphics&) {

}

void AudioPluginAudioProcessorEditor::resized() {
  // This is generally where you'll want to lay out the positions of any
  // subcomponents in your editor..
  square.setBounds(100, 100, 200, 200);
}
}  // namespace audio_plugin
