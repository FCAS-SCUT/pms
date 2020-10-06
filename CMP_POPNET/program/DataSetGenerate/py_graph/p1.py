#!python
import re
p = re.compile(r'^(task_cnt\s)\S*\s\S$', re.M)
p1 = re.compile(r'^(tg_cnt\s)\S$',re.M)
for i in range(0, 7):
        filename = 'graph' + str(200 + 300 * i) + '.tgffopt'
        
        fp_o = open(filename, 'r')
        all = fp_o.read()
        fp_o.close
        #print all
        fp_i = open(filename, 'w')
        match = p.search(all)
        #print match.group(1)
        fp_i.write(p.sub(r'\g<1>' + str(200 + 300 * i) +' 1', all))
        fp_i.close

        fp_o = open(filename, 'r')
        all = fp_o.read()
        fp_o.close
        print all
        fp_i = open(filename, 'w')
        match1 = p1.search(all)
        fp_i.write(p1.sub(r'\g<1>' + '1', all))
        fp_i.close
