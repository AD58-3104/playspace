import os

def sound_play(sound_number:int):
    sound_name_list:str = [
    "001.wav",
    "002.wav",
    "003.wav",
    "004.wav",
    "005.wav",
    "006.wav",
    "007.wav",
    "008.wav",
    "ball_lost.wav",
    "douzo_douzo.wav",
    "ikkuzoo.wav",
    "initialized.wav",
    "localization.wav",
    "lowvoltage.wav",
    "ready.wav",
    "settingfailed.wav",
    "shoot.wav"]
    aplay:str = 'aplay '
    echo:str = 'echo '
    if sound_number < len(sound_name_list):
        os.system(echo+sound_name_list[sound_number])
    else :
        print('there is no sound which match inputed number')

if __name__ == '__main__':
    while(True):
        inp = int(input())
        sound_play(inp)