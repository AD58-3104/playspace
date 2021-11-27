import sys
import os 

def parseAndWriteDictionaryFile(readfile_fd,word_list):
    lines = readfile_fd.readlines()
    for line in lines:
        if "class" in line:
            line = line.replace('    ','')
            line = line.replace('\t','')
            line = line.split(" ")[1]
            line = line.split("(")[0]
            word_list.add(line)
    readfile_fd.close()
    return word_list

def main(path_to_kid_dir):
    behavior_name_list = set()
    readfiles = ['common_network.py','root_network.py']
    for readfile in readfiles:
        behavior_name_list = parseAndWriteDictionaryFile(open(path_to_kid_dir + readfile, 'r'),behavior_name_list)
    writefile = open('behavior_name_dict.dat','w')
    for word in behavior_name_list:
        writefile.write(word + '\n')

    commnad_name_list = set()
    readfiles = ['goal.py','role.py','tactics.py']
    for readfile in readfiles:
        commnad_name_list = parseAndWriteDictionaryFile(open(path_to_kid_dir + readfile,'r'),commnad_name_list)
    writefile = open('command_dict.dat','w')
    for word in commnad_name_list:
        writefile.write(word + '\n')


if __name__ == '__main__': 
    main(sys.argv[1])