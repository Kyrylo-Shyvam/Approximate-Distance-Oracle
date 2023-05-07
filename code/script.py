import subprocess


subprocess.run('cmake -DCMAKE_BUILD_TYPE=Release -S . -B build',shell=True)
subprocess.run('make -C build ', shell=True,
        stdout=subprocess.DEVNULL)

# usage: RandomGraphTest K <seed> <minimum-n> 
# <maximum-n> <max-m> <min-dist> <max-dist>


# output format-
'''vertex,edges,graph-construct,dijkstra,dijkstra-average,oracle-contruct,average-query,oracle-fib-construct,oracle-fib-query,,Floyd-Warshal'''

def execute(inp, location):
    inp2 = [str(x) for x in inp]
    x = subprocess.run(['./build/tests/RandomGraphTest3',
        inp2[0],inp2[1], inp2[2], inp2[3], inp2[4], inp2[5]],
        capture_output=True)

    print(x.stdout.decode('ascii'),flush=True)
    with open(location,'a') as file:
        file.write(x.stdout.decode('ascii'))


def edgesVsTime():
    for i in range(5000,20001,2000):
        for j in range(10):
            inp = [2, j, 200, i, 10, 100]
            execute(inp, 'test3-200-x.csv')

def verticesVsTime():
    for i in range(200,1001,50):
        for j in range(10,20):
            inp = [2, j, i, 2000, 10, 100]
            execute(inp, 'test3-x-2000.csv')
#
# def kVsTime():
#     for i in range(2,22):
#         for j in range(20,30):
#             inp = [2, j, i, i, 2000, 10, 100]
#             execute(inp,'test3-kVsTime.csv')

edgesVsTime()
verticesVsTime()
# kVsTime()

