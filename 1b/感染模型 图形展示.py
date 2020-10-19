import random
import numpy as np 
from matplotlib import pyplot as plt 

MAX_REN = 2000     #人数
MAX_DAY = 100     #最长观测天数
PERCENT = 1      #初始感染百分比

jizhan = [[200,200],[200,400],[400,200],[400,400]]  #基站位置初始化

map = [0]*600           #地图点，map[600][600],每一个都是一个集合，准备存储这一点上的人的编号
for i in range(600):
    map[i] = [0]*600
    for j in range(600):
        map[i][j] = set()

bingren = set()         #bingren集合，存放病人编号

ren = [0] * MAX_REN     #ren[MAX_REN][MAX_DAY],每一个都是一个字典，存储此人在这一天的xy和bingle、kejian
for i in range(MAX_REN):
    ren[i] = [0] * MAX_DAY
    if random.randint(0,100) < PERCENT:
        ren[i][0] = {'x':random.randint(0,599), 'y':random.randint(0,599), 'bingle':True, 'kejian':False}
        bingren.add(i)
    else:
        ren[i][0] = {'x':random.randint(0,599), 'y':random.randint(0,599), 'bingle':False, 'kejian':False}
    map[ren[i][0]['x']][ren[i][0]['y']].add(i)
    for j in range(1,MAX_DAY):
        ren[i][j] = {}


#Move + Infect
tempbingren = set()
tempmap = set()

for day in range(1,MAX_DAY):
    for i in range(MAX_REN):
        map[ren[i][day-1]['x']][ren[i][day-1]['y']].remove(i)
        if random.randint(0,100) < 10:
            ren[i][day]['x'] = ren[i][day-1]['x'] + 2
        elif random.randint(0,100) < 20:
            ren[i][day]['x'] = ren[i][day-1]['x'] - 2
        elif random.randint(0,100) < 30:
            ren[i][day]['x'] = ren[i][day-1]['x'] + 3
        elif random.randint(0,100) < 40:
            ren[i][day]['x'] = ren[i][day-1]['x'] - 3
        elif random.randint(0,100) < 50:
            ren[i][day]['x'] = ren[i][day-1]['x'] + 4
        elif random.randint(0,100) < 60:
            ren[i][day]['x'] = ren[i][day-1]['x'] - 4
        elif random.randint(0,100) < 80:
            ren[i][day]['x'] = ren[i][day-1]['x'] + 5
        else:
            ren[i][day]['x'] = ren[i][day-1]['x'] - 5


        if random.randint(0,100) < 10:
            ren[i][day]['y'] = ren[i][day-1]['y'] + 2
        elif random.randint(0,100) < 20:
            ren[i][day]['y'] = ren[i][day-1]['y'] - 2
        elif random.randint(0,100) < 30:
            ren[i][day]['y'] = ren[i][day-1]['y'] + 3
        elif random.randint(0,100) < 40:
            ren[i][day]['y'] = ren[i][day-1]['y'] - 3
        elif random.randint(0,100) < 50:
            ren[i][day]['y'] = ren[i][day-1]['y'] + 4
        elif random.randint(0,100) < 60:
            ren[i][day]['y'] = ren[i][day-1]['y'] - 4
        elif random.randint(0,100) < 80:
            ren[i][day]['y'] = ren[i][day-1]['y'] + 5
        else:
            ren[i][day]['y'] = ren[i][day-1]['y'] - 5    
        
        if ren[i][day]['x']<0 or ren[i][day]['x']>599:
            ren[i][day]['x']%=600
        if ren[i][day]['y']<0 or ren[i][day]['y']>599:
            ren[i][day]['y']%=600
        
        ren[i][day]['bingle'] = ren[i][day-1]['bingle']
        map[ren[i][day]['x']][ren[i][day]['y']].add(i)

    tempbingren.clear()
    tempbingren = bingren.copy()
    while len(tempbingren) > 0:
        k = tempbingren.pop()
        for s in range(-4,4):
            for t in range(-4,4):
                tempmap.clear()
                if ren[k][day]['x']+s>=0 and ren[k][day]['x']+s<600 and ren[k][day]['y']+t>=0 and ren[k][day]['y']+t<600:
                    tempmap = map[ren[k][day]['x']+s][ren[k][day]['y']+t].copy()
                    while len(tempmap) > 0:
                        num = tempmap.pop()
                        ren[num][day]['bingle'] = True
                        bingren.add(num)


'''
plt.title("Matplotlib demo") 
plt.xlabel("x axis caption") 
plt.ylabel("y axis caption") 
'''

fig,ax = plt.subplots()

for j in range(MAX_DAY):
    x = []
    y = []
    bingx = []
    bingy = []
    for i in range(MAX_REN):
        if ren[i][j]['bingle']:
            bingx += [ren[i][j]['x']]
            bingy += [ren[i][j]['y']]
        else:
            x += [ren[i][j]['x']]
            y += [ren[i][j]['y']]
    ax.cla()
    ax.scatter(x,y,color ='green',alpha = 0.6) 
    ax.scatter(bingx,bingy,color ='red',alpha = 0.6)
    ax.plot([200,200,400,400],[200,400,200,400],'^b')
    ax.add_patch(plt.Rectangle(xy=(-5,-5),width=610,height=610,edgecolor='black',fill=False, linewidth=5))
    ax.add_patch(plt.Circle(xy=(200,200),radius=40,color ='blue', alpha = 0.3))
    ax.add_patch(plt.Circle(xy=(200,400),radius=40,color ='blue', alpha = 0.3))
    ax.add_patch(plt.Circle(xy=(400,200),radius=40,color ='blue', alpha = 0.3))
    ax.add_patch(plt.Circle(xy=(400,400),radius=40,color ='blue', alpha = 0.3))
    plt.axis('scaled')
    plt.axis('equal')

    plt.pause(0.1)
    
plt.show()