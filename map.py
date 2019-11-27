# -*- coding=big5-hkscs -*-
import sys

input_file=open(sys.argv[1],'r',encoding='big5-hkscs')
output_file=open(sys.argv[2],'w',encoding='big5-hkscs')

ZhuYin_dict={}
for line in input_file:
    line=line.replace('\n','')
    line=line.split(' ')
    char=line[0]
    ZhuYin=line[1]
    ZhuYin=ZhuYin.split('/')
    for z in ZhuYin:
        if z[0] not in ZhuYin_dict:
            ZhuYin_dict[z[0]]=[]
        ZhuYin_dict[z[0]].append(char)

for key in ZhuYin_dict.keys():
    line=''
    for z in ZhuYin_dict[key]:
        line=line+' '+z
    line='%s%s\n' %(key,line)
    output_file.write(line)

input_file.close()
output_file.close()