# coding=Big5

import collections
import sys

d = dict()
with open('Big5-ZhuYin.map', 'r', encoding='big5-HKSCS') as f:
    for line in f:
        after_split = line.split(' ', 1)
        chinese_char = after_split[0]
        zhu_yin_set = after_split[1].split('/')

        if (chinese_char not in d):
            d[chinese_char] = set()
            d[chinese_char].add(chinese_char)

        
        for zhu_yin in zhu_yin_set:
            if (zhu_yin[0] not in d):
                d[zhu_yin[0]] = set()
            if (chinese_char not in d[zhu_yin[0]]):
                d[zhu_yin[0]].add(chinese_char)

        

with open('ZhuYin-Big5.map', 'w', encoding='big5-HKSCS') as f:
    for k in d:
        f.write(k)
        for v in d[k]:
            f.write(' ' + v)
        f.write('\n')
