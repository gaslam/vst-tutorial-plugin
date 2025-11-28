#include "TestPlugin/PluginProcessor.h"
#include "TestPlugin/PluginEditor.h"
#include "TestPlugin/Sinewave.h"

#include <ranges>

namespace audio_plugin {
AudioPluginAudioProcessor::AudioPluginAudioProcessor()
    : AudioProcessor(
          BusesProperties()
#if !JucePlugin_IsMidiEffect
#if !JucePlugin_IsSynth
              .withInput("Input", juce::AudioChannelSet::stereo(), true)
#endif
              .withOutput("Output", juce::AudioChannelSet::stereo(), true)
#endif
              ),
      m_State(*this, nullptr, "parameters", createParameters()) {
  m_State.addParameterListener("play", this);
  m_State.addParameterListener("freqHz", this);
  m_State.addParameterListener("waveSelect", this);
}

AudioPluginAudioProcessor::~AudioPluginAudioProcessor() {
  m_State.removeParameterListener("freqHz", this);
  m_State.removeParameterListener("play", this);
  m_State.removeParameterListener("waveSelect", this);
}

const juce::String AudioPluginAudioProcessor::getName() const {
  return JucePlugin_Name;
}

bool AudioPluginAudioProcessor::acceptsMidi() const {
#if JucePlugin_WantsMidiInput
  return true;
#else
  return false;
#endif
}

bool AudioPluginAudioProcessor::producesMidi() const {
#if JucePlugin_ProducesMidiOutput
  return true;
#else
  return false;
#endif
}

bool AudioPluginAudioProcessor::isMidiEffect() const {
#if JucePlugin_IsMidiEffect
  return true;
#else
  return false;
#endif
}

double AudioPluginAudioProcessor::getTailLengthSeconds() const {
  return 0.0;
}

int AudioPluginAudioProcessor::getNumPrograms() {
  return 1;  // NB: some hosts don't cope very well if you tell them there are 0
             // programs, so this should be at least 1, even if you're not
             // really implementing programs.
}

int AudioPluginAudioProcessor::getCurrentProgram() {
  return 0;
}

void AudioPluginAudioProcessor::setCurrentProgram(int index) {
  juce::ignoreUnused(index);
}

const juce::String AudioPluginAudioProcessor::getProgramName(int index) {
  juce::ignoreUnused(index);
  return {};
}

void AudioPluginAudioProcessor::changeProgramName(int index,
                                                  const juce::String& newName) {
  juce::ignoreUnused(index, newName);
}

void AudioPluginAudioProcessor::prepareToPlay(double sampleRate,
                                              int samplesPerBlock) {
  // Use this method as the place to do any pre-playback
  // initialisation that you need..
  juce::ignoreUnused(sampleRate, samplesPerBlock);

  const int inputChannels{getTotalNumInputChannels()};
  const float sampleRateFl{static_cast<float>(sampleRate)};

  for (int i{}; i < m_WaveTypes.size(); ++i) {
    m_Waves.emplace(i,
                    initWavesByName(m_WaveTypes[i], inputChannels, sampleRateFl));
  }
  if (m_Waves.size() > 0) {
    m_SelectedWave = 0;
  }
}

std::vector<std::unique_ptr<Wave>> AudioPluginAudioProcessor::initWavesByName(
    const juce::String& name,
    const int resizeNr,
    const float sampleRate) {
  std::vector<std::unique_ptr<Wave>> waves{};
  waves.reserve(resizeNr);

  if (name == m_WaveTypes[0]) {
    for (int i{}; i < resizeNr; ++i) {
      auto wave{std::make_unique<Squarewave>()};
      wave->prepare(sampleRate);
      waves.emplace_back(std::move(wave));
    }
    return waves;
  }

  if (name == m_WaveTypes[1]) {
    for (int i{}; i < resizeNr; ++i) {
      auto wave{std::make_unique<Sinewave>()};
      wave->prepare(sampleRate);
      waves.emplace_back(std::move(wave));
    }
    return waves;
  }
  return waves;
}

void AudioPluginAudioProcessor::releaseResources() {
  // When playback stops, you can use this as an opportunity to free up any
  // spare memory, etc.
}

bool AudioPluginAudioProcessor::isBusesLayoutSupported(
    const BusesLayout& layouts) const {
#if JucePlugin_IsMidiEffect
  juce::ignoreUnused(layouts);
  return true;
#else
  // This is the place where you check if the layout is supported.
  // In this template code we only support mono or stereo.
  // Some plugin hosts, such as certain GarageBand versions, will only
  // load plugins that support stereo bus layouts.
  if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono() &&
      layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
    return false;

  // This checks if the input layout matches the output layout
#if !JucePlugin_IsSynth
  if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
    return false;
#endif

  return true;
#endif
}

void AudioPluginAudioProcessor::processBlock(juce::AudioBuffer<float>& buffer,
                                             juce::MidiBuffer& midiMessages) {
  juce::ignoreUnused(midiMessages);

  if (!m_IsPlaying || m_SelectedWave == -1) {
    return;
  }

  juce::ScopedNoDenormals noDenormals;
  auto totalNumInputChannels = getTotalNumInputChannels();
  auto totalNumOutputChannels = getTotalNumOutputChannels();
  const int bufferChannels = buffer.getNumChannels();
  const int bufferSamples = buffer.getNumSamples();

  // In case we have more outputs than inputs, this code clears any output
  // channels that didn't contain input data, (because these aren't
  // guaranteed to be empty - they may contain garbage).
  // This is here to avoid people getting screaming feedback
  // when they first compile a plugin, but obviously you don't need to keep
  // this code if your algorithm always overwrites all the output channels.
  for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
    buffer.clear(i, 0, buffer.getNumSamples());

  for (auto& waves : m_Waves | std::views::values) {
    for (int channel{}; channel < bufferChannels; ++channel) {
      float* output = buffer.getWritePointer(channel);
      waves[channel]->process(output, bufferSamples);
    }
  }

  // This is the place where you'd normally do the guts of your plugin's
  // audio processing...
  // Make sure to reset the state if your inner loop is processing
  // the samples and the outer loop is handling the channels.
  // Alternatively, you can process the samples with the channels
  // interleaved by keeping the same state.
  for (int channel = 0; channel < totalNumInputChannels; ++channel) {
    auto* channelData = buffer.getWritePointer(channel);
    juce::ignoreUnused(channelData);
    // ..do something to the data...
  }
}

bool AudioPluginAudioProcessor::hasEditor() const {
  return true;  // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* AudioPluginAudioProcessor::createEditor() {
  return new AudioPluginAudioProcessorEditor(*this);
}

void AudioPluginAudioProcessor::getStateInformation(
    juce::MemoryBlock& destData) {
  // You should use this method to store your parameters in the memory block.
  // You could do that either as raw data, or use the XML or ValueTree classes
  // as intermediaries to make it easy to save and load complex data.
  juce::ignoreUnused(destData);
}

void AudioPluginAudioProcessor::setStateInformation(const void* data,
                                                    int sizeInBytes) {
  // You should use this method to restore your parameters from this memory
  // block, whose contents will have been created by the getStateInformation()
  // call.
  juce::ignoreUnused(data, sizeInBytes);
}
juce::AudioProcessorValueTreeState::ParameterLayout
AudioPluginAudioProcessor::createParameters() {
  return {std::make_unique<juce::AudioParameterFloat>(
              juce::ParameterID{"freqHz"}, "Frequency", 20.f, 2000.f, 220.f),
          std::make_unique<juce::AudioParameterBool>(juce::ParameterID{"play"},
                                                     "Play", true),
          std::make_unique<juce::AudioParameterChoice>(
              juce::ParameterID{"waveSelect"}, "WaveSelect",
              m_WaveTypes, 0)};
}
}  // namespace audio_plugin

// This creates new instances of the plugin.
// This function definition must be in the global namespace.
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter() {
  return new audio_plugin::AudioPluginAudioProcessor();
}

void audio_plugin::AudioPluginAudioProcessor::parameterChanged(
    const juce::String& id,
    float newValue) {
  if (id == "play") {
    const float newFreq = 0.4f * newValue;

    for (auto& wave : m_Waves[m_SelectedWave]) {
      wave->setAmplitude(newFreq);
    }

    m_IsPlaying = !m_IsPlaying;
  }

  if (id == "freqHz") {
    for (auto& wave : m_Waves[m_SelectedWave]) {
      wave->setFrequency(newValue);
    }
  }

  if (id == "waveSelect") {
    const int idx{static_cast<int>(newValue)};
    const float amplitude{0.4f * m_State.getRawParameterValue("play")->load()};
    const float freq{m_State.getRawParameterValue("freqHz")->load()};
    constexpr float zeroVolume{};
    for (auto& wave : m_Waves[m_SelectedWave]) {
      wave->setAmplitude(zeroVolume);
    }

    m_SelectedWave = idx;
    for (auto& wave : m_Waves[m_SelectedWave]) {
      wave->setFrequency(freq);
      wave->setAmplitude(amplitude);
    }
  }
}
