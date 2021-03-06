#Handles communication with the audio devices using PyAudio
import pyaudio

#
# Function showDevices() lists available input- and output devices
class paAuWrapper():
    #create a handle to the pyAudio object.
    handle = pyaudio.PyAudio()


    def __init__(self, outputDevice, channels=2, audiorate=44100, framesPerBuffer = 256, width = 2):
        #initialize audio settings.
        self.channels = channels
        self.audioRate = audiorate
        self.framesPerBuffer = framesPerBuffer
        self.width = width
        self.outputDevice = outputDevice
        print("Een audio wrapper van PyAudio")

    def showDevices(self, p):
        """print all available in and output devices"""
        info = p.get_host_api_info_by_index(0)
        numdevices = info.get('deviceCount')
        #scan through available devices and print devive if the device is an audio device.
        for i in range (0,numdevices):
            if p.get_device_info_by_host_api_device_index(0,i).get('maxInputChannels')>0:
                print("Input Device id ", i, " - ", p.get_device_info_by_host_api_device_index(0,i).get('name'))
            if p.get_device_info_by_host_api_device_index(0,i).get('maxOutputChannels')>0:
                print("Output Device id ", i, " - ", p.get_device_info_by_host_api_device_index(0,i).get('name'))

    def setOutputDevice(self, p):
        info = p.get_host_api_info_by_index(0)
        numdevices = info.get('deviceCount')
        for i in range (0,numdevices):
            #print("maxOutputChannels at index ", i , ": ", p.get_device_info_by_host_api_device_index(0,i).get('maxOutputChannels'))
            if p.get_device_info_by_host_api_device_index(0,i).get('maxOutputChannels')>0:
                print(p.get_device_info_by_host_api_device_index(0,i).get('name').find("Built-in"))
            if p.get_device_info_by_host_api_device_index(0,i).get('name').find("Built-in") >= 0:
                outputDevice=i
            #print("Selected device number: ", str(self.outputDevice))

    #UNUSED: The pyAudio stream. The stream wont receive the callback created in Main.py as a variable.
    # def stream(self, callback, inp=False, out=True):
    #     # Start PyAudio stream with some given properties. TODO SHOULD - move de stream naar file 'Audio.py' - werkt niet.
    #     self.stream = self.handle.open(format=self.get_format_from_width(self.width),
    #                                    channels=self.channels,
    #                                    rate=self.audioRate,
    #                                    frames_per_buffer=self.framesPerBuffer,
    #                                    input=inp,  # no input
    #                            output=out,  # only output
    #                            output_device_index=self.outputDevice,  # choose output device
    #                            stream_callback=callback)