#pragma once

#include "TestPlugin/Squarewave.h"

#include <juce_audio_processors/juce_audio_processors.h>
#include <map>

namespace audio_plugin {
class AudioPluginAudioProcessor : public juce::AudioProcessor, public juce::AudioProcessorValueTreeState::Listener {
public:
  AudioPluginAudioProcessor();
  ~AudioPluginAudioProcessor() override;

  void prepareToPlay(double sampleRate, int samplesPerBlock) override;
  void releaseResources() override;

  bool isBusesLayoutSupported(const BusesLayout& layouts) const override;

  void processBlock(juce::AudioBuffer<float>&, juce::MidiBuffer&) override;
  using AudioProcessor::processBlock;

  juce::AudioProcessorEditor* createEditor() override;
  bool hasEditor() const override;

  const juce::String getName() const override;

  bool acceptsMidi() const override;
  bool producesMidi() const override;
  bool isMidiEffect() const override;
  double getTailLengthSeconds() const override;

  int getNumPrograms() override;
  int getCurrentProgram() override;
  void setCurrentProgram(int index) override;
  const juce::String getProgramName(int index) override;
  void changeProgramName(int index, const juce::String& newName) override;

  void getStateInformation(juce::MemoryBlock& destData) override;
  void setStateInformation(const void* data, int sizeInBytes) override;

  void parameterChanged(const juce::String& id, float newValue) override;

  [[nodiscard]] juce::AudioProcessorValueTreeState& getState() {return m_State;}
  bool IsPlaying() const {return m_IsPlaying;}

private:
  juce::StringArray m_WaveTypes{"Square","Sine"};
  std::map<int,std::vector<std::unique_ptr<Wave>>> m_Waves{};
  int m_SelectedWave{-1};
  juce::AudioProcessorValueTreeState m_State;
  bool m_IsPlaying = true;

  juce::AudioProcessorValueTreeState::ParameterLayout createParameters();
  std::vector<std::unique_ptr<Wave>> initWavesByName(const juce::String& name,const int resizeNr, const float sampleRate) ;

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AudioPluginAudioProcessor)
};
}  // namespace audio_plugin
