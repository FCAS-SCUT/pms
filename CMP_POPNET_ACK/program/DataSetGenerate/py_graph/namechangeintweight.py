import re,string,sys
fp_i = open('twittercrawlweight' + '.csv', "r")#file to open
fp_o = open('twittercrawlweight' + '_int.csv', "a")#file to save
pattern = re.compile(r'^(\S*),(\S*),(\S*)\s?$', re.M)
pattern_start = re.compile(r'^(Source,Target,Weight)\s?$', re.M)#match the first line "Source,Target"
name_array = []#save names, and the position in the array is the ID.
x_array = []#save left column in digital form
y_array = []#save right column in digital form
z_array = []#save weight
while 1:#read the textual file from the first line to next
        line = fp_i.readline()
        if not line:
                break
        match_start = pattern_start.match(line)
        if 0 == cmp(match_start.group(1),'Source,Target,Weight'):#if the current line is "Source,Target,Weight", then transform from the next line to the end. 
                while 1:
                        line = fp_i.readline()
                        match = pattern.match(line)
                        if not line:
                                break
                        group1 = match.group(1)
                        group2 = match.group(2)
                        group3 = match.group(3)
                        x_len = len(x_array)
                        for i in range(len(name_array)):
                                if group1 == name_array[i]:#if it is not a new name, save the ID to x_array
                                        x_array.append(i)
                                        break
                        if len(x_array) == x_len:#if not a new name, then do this
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
                        z_array.append(int(float(group3)))
print 'x_array length: ' + str(len(x_array)) + 'y_array length: ' + str(len(y_array)) + '\n'                             
for i in range(len(x_array)):#save them to files
        print str(x_array[i]) + ',' + str(y_array[i]) + ',' + str(z_array[i]) + '\n'
        fp_o.write(str(x_array[i]) + ',' + str(y_array[i]) + ',' + str(z_array[i]) + '\n')                                      
fp_o.close
fp_o.flush
fp_i.close
