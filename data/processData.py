from unidecode import unidecode
import random
import collections

fi = open("data/data_raw.txt", "r")
fo = open("data/Stop_raw.txt", "w+")
fo1 = open("data/Edges.txt", "w")
fo2 = open('data/ListBusinStop.txt', "w")

busStop = dict()
edges = dict()
distance = dict()
busList = dict()

temp = ""
num = 0
count = 0
pre = -1
busID = ""

def processString(str):
    if str[-1] == '\n': str = str[0 : -1]
    while str[0] == ' ':
        str = str[1 : ]
    while str[-1] == ' ':
        str = str[0 : -1]
    return str

def denoteStop(str, n):
    if str in busStop.keys():
        return busStop[str]
    busStop.update({str : n})
    global num
    num = n + 1
    return n

def addEdge(u, v):
    if u not in edges.keys():
        edges.update({u : list()})
        distance.update({u : list()})
    if v not in edges.keys():
        edges.update({v : list()})
        distance.update({v : list()})
    if v not in edges[u]:
        edges[u].append(v)
        distance[u].append(500 + random.randint(0, 300))
    if u not in edges[v]:
        edges[v].append(u)
        distance[v].append(500 + random.randint(0, 300))

def addBustoStop(bus, stop):
    lst = list()
    if stop not in busList.keys():
        busList.update({stop : lst})
    if bus not in busList[stop]:
        busList[stop].append(bus)

while True:
    busID = fi.readline()
    busID = busID[0 : -1]
    if busID == '':
        break
    if busID[0].isalnum() == False:
        busID = busID[1:]
    contents = fi.readline()
    contents = unidecode(contents).split('-')
    pre = -1
    for item in contents:
        s = processString(item)
        cur = denoteStop(s, num)
        if pre != -1:
            addEdge(cur, pre)
        pre = cur
        addBustoStop(busID, cur)

fo.write(str(len(busStop.keys())) + "\n")
for key, value in busStop.items():
    fo.write(key)
    fo.write("\n")

edges = collections.OrderedDict(sorted(edges.items()))
distance = collections.OrderedDict(sorted(distance.items()))
for key, value in edges.items():
    fo1.write(str(len(edges[key])) + "\n")
    for v in value:
        fo1.write(str(v))
        fo1.write(' ')
    fo1.write('\n')
    for v in distance[key]:
        fo1.write(str(v) + " ")
    fo1.write('\n')

busList = collections.OrderedDict(sorted(busList.items()))
for key in busList.keys():
    fo2.write(str(len(busList[key])) + "\n")
    for bus in busList[key]:
        fo2.write(bus + ' ')
    fo2.write('\n')

fi.close()
fo.close()
fo1.close()
fo2.close()

fi = open("data/Stop_raw.txt", "r")
fo = open("data/denotedStop.txt", "w+")

while True:
    c = fi.read(1)
    if c == '': break
    if 'A' <= c <= 'Z':
        c = chr(ord(c) - ord('A') + ord('a'))
    fo.write(c)

fi.close()
fo.close()
