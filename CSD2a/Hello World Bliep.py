
import simpleaudio as sa

sound = sa.WaveObject.from_wave_file("u4ea Kick.wav")
play_obj = sound.play()
play_obj.wait_done()

