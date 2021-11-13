import os

def sound_play(sound_number:int):
    dirname:str = "./sound"
    names = os.listdir(dirname)
    names.sort()
    aplay:str = 'aplay '
    echo:str = 'echo '
    if sound_number < len(names):
        os.system(echo + dirname + names[sound_number])
    else :
        print('there is no sound which match inputed number')

if __name__ == '__main__':
    while(True):
        inp = int(input())
        sound_play(inp)