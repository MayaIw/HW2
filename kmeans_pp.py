import numpy as np
import pandas as pd
import math
import sys
import copy
import mykmeanssp as km

#p1, p2 are arrays with float values, representing d dimentional points
#the function calculates the euclidean distance between p1 and p2
def distance(p1, p2, d):
    sum_of_squares = 0.0
    for i in range(d):
        sum_of_squares += math.pow(float(p1[i])-float(p2[i]), 2)
    distance= math.sqrt(sum_of_squares)
    return distance

#x is a point in R^d, centroids is an array with k points in R^d
def weight(x, centroids, d):
   min_distance = distance(x, centroids[0], d)
   for i in range(1, len(centroids)):
       dist = distance(x, centroids[i], d)
       if dist <= min_distance:
           min_distance = dist
   return min_distance

def is_float(n):
    try:
        float(n)
        return True
    except:
        return False

def main():
    k=0
    iter=0
    eps = 0.0
    file_name_1=""
    file_name_2=""
    elements = []
    centroids = []
    num_of_elements=0
    d=0

    if len(sys.argv) == 5:
        if sys.argv[1].isdigit():
            k = int(sys.argv[1])
        else:
            print("Invalid number of clusters!")
            exit(0)
        iter = 200
        eps= float(sys.argv[2])
        file_name_1 = str(sys.argv[3])
        file_name_2 = str(sys.argv[4]) 
    elif len(sys.argv) == 6:
         if sys.argv[1].isdigit():
            k = int(sys.argv[1])
         else:
            print("Invalid number of clusters!")
            exit(0)
         if sys.argv[2].isdigit():
             iter = int(sys.argv[2])
         else:
            print("Invalid maximum iteration!")
            exit(0)
         eps= float(sys.argv[3])
         file_name_1 = str(sys.argv[4])
         file_name_2 = str(sys.argv[5])
         if iter<1 or iter>1000:
            print("Invalid maximum iteration!")
            exit(0)
    else:
        print("An Error Has Occurred")
        exit(0)

    file1 = pd.read_csv(file_name_1)
    file2 = pd.read_csv(file_name_2)

    file = pd.merge(file1, file2,on=0)
    file.sort_values(ascending=True)
    elements = file.to_numpy()

    for line in file:
        point = [float(n) for n in line.split(',') if is_float(n)]
        elements.append(point)

    num_of_elements=len(elements)
    d=len(elements[0])

    if k<1 or k>num_of_elements:
        print("Invalid number of clusters!")
        exit(0)

    np.random.seed(0)
    centroids = copy.deepcopy(np.random.choice(elements))

    for i in range(1,k):
        weights = [weight(x, centroids, d) for x in elements]
        centroids.append(copy.deepcopy(np.random.choice(elements, p=weights)))
    
    #print(km.fit(elements, centroids, k, iter, eps, num_of_elements, d))

if __name__ == "__main__":
    main()