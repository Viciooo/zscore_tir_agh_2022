import numpy as np
arr = np.random.normal(loc=0.0, scale=10, size=[1000,3])

with open('data.csv', "a") as f:
    for row in arr:
        f.write(f"{row[0]},{row[1]},{row[2]}\n")
