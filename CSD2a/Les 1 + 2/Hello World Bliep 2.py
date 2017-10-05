
import simpleaudio as sa
import aifc

x = int(input ("Hoevaak wilt u het geluid horen? "))

#sound = sa.WaveObject.from_wave_file("C:/Users/Harold/Desktop/Piano Jamp.wav")
sound = sa.WaveObject.from_wave_file("u4ea Kick.wav")
for n in range(0, x):
    play_obj = sound.play()
    play_obj.wait_done()

