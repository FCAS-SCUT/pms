#!/usr/bin/python
#argv[1] is a filename(do not contain the extension and the file extension must be ".tgff")
import re,sys,string,os
fp = open(sys.argv[1] + '.txt', "r")
all = fp.read()
fp.close
os.mkdir(str(sys.argv[1]))
#find the number of vertexes
pattern1 = re.compile(r'TASK t0' + '_(\S*)\s*')
for match1 in pattern1.finditer(all):
	ver_max = match1.group(1)
ver_max = int(ver_max) + 1
pattern2 = re.compile(r'FROM\s*t0' + '_(\S*)\s*TO\s*t0' + '_(\S*)')
fp_o = open(str(sys.argv[1]) + '.csv', "w")
fp_o.write('Source,Target\n')
for match2 in pattern2.finditer(all):
	fp_o.write(match2.group(1) + ',' + match2.group(2) + '\n')
fp_o.flush()
fp_o.close

fp_i = open(str(sys.argv[1]) + '.csv','r')
all1 = fp_i.read()
fp_i.close
x_array = []
y_array = []
pattern = re.compile(r'^(\d*),(\d*)$', re.M)
for match in pattern.finditer(all1):#distill data
        x_array.append(match.group(1))
        y_array.append(match.group(2))
        print match.group(1),match.group(2)

#partition, equal distribution
a = ver_max / int(sys.argv[2])
print 'a is ' + str(a) + '\n'
b = ver_max % int(sys.argv[2])
print 'b is ' + str(b) + '\n'

fp_o = open(str(sys.argv[1]) + '.out', 'w')
fp_o.write(str(ver_max) + '\n')#print to file how many vertexes
#equal distribution on each core
for i in range(a):
        for j in range(int(sys.argv[2])):
                fp_o.write(str(j) + '\n')
for i in range(b):
        fp_o.write(str(i) + '\n')
xy_array = [['0' for x in range(ver_max)] for y in range(ver_max)]#initialize the matrix
for i in range(len(x_array)):
    xy_array[int(x_array[i])][int(y_array[i])] = '1'
for i in range(ver_max):
    for j in range(ver_max):
        print xy_array[i][j],
        fp_o.write(xy_array[i][j]),
        if j != ver_max - 1:
                fp_o.write(' '),
    if i != ver_max - 1:
        print '\n'
        fp_o.write('\n')
fp_o.flush()
fp_o.close
