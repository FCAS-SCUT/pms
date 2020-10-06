# -*- coding: utf-8 -*-
import re,sys
fp_i = open(str(sys.argv[1]) + '.csv', "r")
fp_o = open(str(sys.argv[1]) + '_int.csv', "w")
pattern = re.compile(r'^(\S*),(\S*)\s', re.M)
pattern_start = re.compile(r'^(Source,Target)\s?$', re.M)
name_array = []
x_array = []
y_array = []

while 1:
        line = fp_i.readline()
        if not line:
                break
        match_start = pattern_start.match(line)
        if 0 == cmp(match_start.group(1),'Source,Target'):
                while 1:
                        line = fp_i.readline()
                        match = pattern.match(line)
                        if not line:
                                break
                        group1 = match.group(1)
                        group2 = match.group(2)
                        x_len = len(x_array)
                        for i in range(len(name_array)):
                                if group1 == name_array[i]:
                                        x_array.append(i)
                                        break
                        if len(x_array) == x_len:
                                name_array.append(match.group(1))
                                x_array.append(len(name_array)-1)
                        y_len = len(y_array)
                        for i in range(len(name_array)):
                                if group2 == name_array[i]:
                                        y_array.append(i)
                                        break
                        if len(y_array) == y_len:
                                name_array.append(match.group(2))
                                y_array.append(len(name_array)-1)
print 'x_array length: ' + str(len(x_array)) + 'y_array length: ' + str(len(y_array)) + '\n'                             
for i in range(len(x_array)):
        print str(x_array[i]) + ',' + str(y_array[i]) + '\n'
        fp_o.write(str(x_array[i]) + ',' + str(y_array[i]) + '\n')
                                       
fp_o.close
fp_i.close
