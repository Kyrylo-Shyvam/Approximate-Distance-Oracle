import subprocess
import os
# Run ~15 oracles on 120 graphs

ValueOfK = [2,3,4,5,6,7,8,9,10,20,50,100]
base = ['/home/resources/Graphs/connected_graphs/as-caida_G_','_connected.mtx']
#
def execute(inp, location,k):
    inp2 = [str(x) for x in inp]
    x = subprocess.run(['./build/tests/GraphTest2',
        inp2[0],location],capture_output=True)

    loc2 = location.rsplit('/',1)[-1]
    print(x.stdout.decode('ascii'),flush=True)
    with open(f'./output/sparse/d/{loc2}-{k}','a') as file:
        file.write(x.stdout.decode('ascii'))

def findFile():
    for i in range(1,123):
        string = str(i).zfill(3)
        print([0],base[0]+string+base[1], 0)

findFile()

