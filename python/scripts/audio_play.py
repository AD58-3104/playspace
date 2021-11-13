import os

# pyfiles/soundディレクトリに入っている音源を再生する.
# 音源のインデックスを引数に入力する.
# インデックスはsound内のファイルを辞書順に並べて0スタートで決められている.例インデックス0は001.wav等.
def sound_play(sound_number:int):
    dirname:str = "./sound"
    names = os.listdir(dirname)
    names.sort()
    aplay:str = 'aplay '
    echo:str = 'echo '
    if sound_number < len(names):
        os.system(aplay + dirname + "/" + names[sound_number])
    else :
        print('there is no sound which match inputed number')

if __name__ == '__main__':
    while(True):
        inp = int(input())
        sound_play(inp)