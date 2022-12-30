import random

def random_point():
    return f"{random.uniform(0, 10.0)},{random.uniform(0, 10.0)},{random.uniform(0, 10.0)}\n"

f = open('data.csv', "a")
for i in range(1000):
    f.write(random_point())
f.close()
