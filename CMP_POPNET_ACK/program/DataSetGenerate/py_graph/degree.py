#!python
# -*- coding: utf-8 -*-
import re,sys
fp_i = open(sys.argv[1] + '.csv', "r")
fp_o = open(sys.argv[1] + '.degree', "w")
pattern = re.compile(r'^(\S*),(\S*)\s?$', re.M)
pattern_start = re.compile(r'^(Source,Target)\s?$', re.M)
name_array = []
nodes_degree_amount = []
x_array = []
y_array = []
degree = []
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
			print group1 + ' ' + group2
                        for i in range(len(name_array)):
                                if group1 == name_array[i]:
                                        x_array.append(i)
                                        nodes_degree_amount[i]+=1
                                        break
                        else:
                                name_array.append(match.group(1))
                                nodes_degree_amount.append(1)
                                x_array.append(len(name_array)-1)
                        for i in range(len(name_array)):
                                if group2 == name_array[i]:
                                        y_array.append(i)
                                        nodes_degree_amount[i]+=1
                                        break
                        else:
                                name_array.append(match.group(2))
                                nodes_degree_amount.append(1)
                                y_array.append(len(name_array)-1)
for i in range(len(nodes_degree_amount)):#len(nodes_degree_amount) == len(name_array) should be true
        for j in range(len(degree)):
                if nodes_degree_amount[i] == degree[j][0]:
                        degree[j][1]+=1
                        break
        else:#new degree
                degree.append([])
                degree[len(degree)-1].append(nodes_degree_amount[i])
                degree[len(degree)-1].append(1)
print len(degree)
for i in range(len(degree)):
        print str(degree[i][0]) + ' ' + str(degree[i][1])
degree.sort()
for i in range(len(degree)):
        fp_o.write(str(degree[i][0]) + ',' + str(degree[i][1]))
        if i != len(degree) - 1:
                fp_o.write('\n')
fp_o.close
fp_i.close
