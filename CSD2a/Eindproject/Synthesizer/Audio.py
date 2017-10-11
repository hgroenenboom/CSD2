#Handles communication with the audio devices using PyAudio

#
# Function showDevices() lists available input- and output devices
class paAuWrapper():
    def __init__(self, outputDevice):
        self.outputDevice = outputDevice
        print("Een audio wrapper van PyAudio")

    def showDevices(self, p):
        """print alle beschikbare in en output devices"""
        info = p.get_host_api_info_by_index(0)
        numdevices = info.get('deviceCount')
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