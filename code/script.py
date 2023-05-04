import subprocess


subprocess.run('cmake . -B build',shell=True)
subprocess.run('make -C build ', shell=True,
        stdout=subprocess.DEVNULL)

# usage: RandomGraphTest K <seed> <minimum-n> 
# <maximum-n> <max-m> <min-dist> <max-dist>

for i in range(1):
    inp = [2, 123, 1000, 1000, 10000, 10, 100]
    inp2 = [str(x) for x in inp]
    x = subprocess.run(['./build/tests/RandomGraphTest2', 
        inp2[0],inp2[1], inp2[2], inp2[3], inp2[4], inp2[5], inp2[6]],
        capture_output=True)

    print(x.stdout.decode('ascii'))
    with open('test.csv','a') as file:
        file.write(x.stdout.decode('ascii'))
