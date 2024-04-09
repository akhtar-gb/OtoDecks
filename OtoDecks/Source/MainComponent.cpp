#include "MainComponent.h"

//==============================================================================
MainComponent::MainComponent()
{
    // Make sure you set the size of the component after
    // you add any child components.
    setSize (800, 600);

    // Some platforms require permissions to open input channels so request that here
    if (juce::RuntimePermissions::isRequired (juce::RuntimePermissions::recordAudio)
        && ! juce::RuntimePermissions::isGranted (juce::RuntimePermissions::recordAudio))
    {
        juce::RuntimePermissions::request (juce::RuntimePermissions::recordAudio,
                                           [&] (bool granted) { setAudioChannels (granted ? 2 : 0, 2); });
    }
    else
    {
        // Specify the number of input and output channels that we want to open
        setAudioChannels (2, 2);
    }
    
    addAndMakeVisible(playButton);
    
    // Set the range of the slider to -16 to +16, with an initial value of 0
    tempoSlider.setRange(-16.0, 16.0, 0.1); // Assuming a step of 0.1 for finer control
    tempoSlider.setValue(0.0); // Default value
    addAndMakeVisible(tempoSlider);
    tempoSlider.setSliderStyle(juce::Slider::LinearVertical);
    tempoSlider.setTextBoxStyle(juce::Slider::TextBoxAbove, false, 100, 20);
    // Use a lambda function to convert the slider value to a percentage string for the text box
//    std::function<String (double)> Slider::textFromValueFunction
    tempoSlider.textFromValueFunction = [](double value) {
        // Create a string with the value formatted as a percentage
        // Note: std::to_string can result in a long decimal, so you might want to format it more nicely
        return juce::String(value, 1) + "%"; // Using 1 decimal place for the value
    };
    // Use a dummy function for valueFromTextFunction since we're not parsing the text back to a value here
    tempoSlider.valueFromTextFunction = [](const juce::String& text) {
        return text.getDoubleValue();
    };
    
    addAndMakeVisible(positionSlider);
    positionSlider.setSliderStyle(juce::Slider::Rotary);
    positionSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 100, 20);
    
    addAndMakeVisible(volumeSlider);
    volumeSlider.setSliderStyle(juce::Slider::LinearVertical);
    volumeSlider.setTextBoxStyle(juce::Slider::TextBoxAbove, false, 100, 20);
    
    addAndMakeVisible(highEQRotarySlider);
    highEQRotarySlider.setSliderStyle(juce::Slider::Rotary);
    highEQRotarySlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 100, 20);
    
    addAndMakeVisible(midEQRotarySlider);
    midEQRotarySlider.setSliderStyle(juce::Slider::Rotary);
    midEQRotarySlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 100, 20);
    
    addAndMakeVisible(lowhEQRotarySlider);
    lowhEQRotarySlider.setSliderStyle(juce::Slider::Rotary);
    lowhEQRotarySlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 100, 20);
}

MainComponent::~MainComponent()
{
    // This shuts down the audio device and clears the audio source.
    shutdownAudio();
}

//==============================================================================
void MainComponent::prepareToPlay (int samplesPerBlockExpected, double sampleRate)
{
    // This function will be called when the audio device is started, or when
    // its settings (i.e. sample rate, block size, etc) are changed.

    // You can use this function to initialise any resources you might need,
    // but be careful - it will be called on the audio thread, not the GUI thread.

    // For more details, see the help for AudioProcessor::prepareToPlay()
}

void MainComponent::getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill)
{
    // Your audio-processing code goes here!

    // For more details, see the help for AudioProcessor::getNextAudioBlock()

    // Right now we are not producing any data, in which case we need to clear the buffer
    // (to prevent the output of random noise)
    bufferToFill.clearActiveBufferRegion();
}

void MainComponent::releaseResources()
{
    // This will be called when the audio device stops, or when it is being
    // restarted due to a setting change.

    // For more details, see the help for AudioProcessor::releaseResources()
}

//==============================================================================
void MainComponent::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    // You can add your drawing code here!
//    g.fillAll (juce::Colour{255, 0, 0});
    
    auto bounds = positionSlider.getBounds();
    // Draw a filled rectangle behind the child component
    g.setColour(juce::Colours::lightgrey); // Background color
    g.fillRect(bounds);

    // Draw a border around the child component
    g.setColour(juce::Colours::red); // Border color
    g.drawRect(bounds, 1); // 1-pixel thick border
}

void MainComponent::resized()
{
    // This is called when the MainContentComponent is resized.
    // If you add any child components, this is where you should
    // update their positions.

    
    auto windowArea = getLocalBounds();
    
    auto playButtonHeight = getHeight()/5;
    playButton.setBounds (windowArea.removeFromTop (playButtonHeight));
//    playButton.setBounds(0, 0, getWidth(), getHeight()/5);
    
//    tempoSlider.setBounds(0, getHeight()/5, getWidth()/10, getHeight()/5);
    auto tempoSliderWidth = getWidth() * 0.12;
    auto tempoSliderHeight = getHeight()/5;
    tempoSlider.setBounds (windowArea.removeFromLeft(tempoSliderWidth).withHeight(tempoSliderHeight));
    
    positionSlider.setBounds(getWidth() * 0.12, getHeight()/5, getWidth()/5, getHeight()/3);
    
    highEQRotarySlider.setBounds(getWidth() * 14/40, getHeight()/5, getWidth()/25, getHeight()/10);
    midEQRotarySlider.setBounds(getWidth() * 14/40, getHeight() * 3/10, getWidth()/25, getHeight()/10);
    lowhEQRotarySlider.setBounds(getWidth() * 14/40, getHeight() * 4/10, getWidth()/25, getHeight()/10);
    
    volumeSlider.setBounds(getWidth() * 4/10, getHeight()/5, getWidth()/10, getHeight()/5);
}
