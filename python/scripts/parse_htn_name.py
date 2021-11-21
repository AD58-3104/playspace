import sys
import os 

def parseAndWriteDictionaryFile(readfile_fd,writefile_fd):
    lines = readfile_fd.readlines()
    for line in lines:
        if "class" in line:
            line = line.replace('    ','')
            line = line.replace('\t','')
            line = line.split(" ")[1]
            line = line.split("(")[0]
            writefile_fd.write(line)
            print(line)
    readfile.close()
    writefile.close()    

readfile = open('common_network.py', 'r')
writefile = open('behavior_name_dict.txt','w')
parseAndWriteDictionaryFile(readfile,writefile)

print('--------------------------------------------------')
readfile = open('root_network.py')
writefile = open('behavior_name_dict.txt','a')
parseAndWriteDictionaryFile(readfile,writefile)