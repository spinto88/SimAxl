import matplotlib.pyplot as plt
import pandas as pd
import numpy as np

def read_data(fName):

    fp = open(fName,'r').read().split('\n')
    fp = [f.split(',') for f in fp[:len(fp)-1]]

    q = [int(f[1]) for f in fp]
    fragments = [[int(g) for g in f[2:]] for f in fp]

    return q, fragments

def biggest_fragment(fName):

    import numpy as np
    p, fragments = read_data(fName)

    p_range = sorted(list(set(p)))
    bigfrag_p = []

    for p_iter in p_range:
        frag_p = []
        for d in range(len(fragments)):
            if p[d] == p_iter:
                frag_p += [np.max(fragments[d])]

        bigfrag_p.append([p_iter, frag_p])

    return bigfrag_p

def average_fragment(fName):

    import numpy as np

    p, fragments = read_data(fName)

    p_range = sorted(list(set(p)))

    def mean_frag_function(frags):

        if len(frags) >= 1:
          ns = np.bincount(frags)
          s = np.array(range(0,np.max(frags)+1), dtype = np.float)
          ms = ((ns * s)/ns.dot(s)).dot(s)
          return ms
        else:
            return 0.00

    bigfrag_p = []

    for p_iter in p_range:
        frag_p = []
        for d in range(len(fragments)):
            if p[d] == p_iter:
                aux = fragments[d]
                aux.remove(np.max(aux))
                frag_p += aux

        bigfrag_p.append([p_iter, mean_frag_function(frag_p)])

    return bigfrag_p

N = 1024
degree = N

plt.figure(1)
plt.axes([0.15, 0.15, 0.75, 0.75])

p = lambda f,q: 1.00 - (1.00 - 1.00/q)**f

for F in [10]:

  data = biggest_fragment('Prueba.dat'.format(degree, F))

  plt.plot([b[0] for b in data], [np.mean(b[1]) for b in data], '-', linewidth = 3, alpha = 0.75, label = '$F = {}$'.format(F))

  for b in data:
    for i in range(len(b[1])):
      plt.scatter(b[0], b[1][i], s = 20, alpha = 0.15, color = 'k')

plt.xlabel('Q', size = 15)
plt.ylabel('Biggest fragment', size = 15)
#plt.xscale('log')
plt.xticks(size = 12)
plt.yticks(size = 12)
plt.grid(True, alpha = 0.15)
plt.legend(loc = 'best', fontsize = 12)
#plt.savefig('Biggest_fragment.png', dpi = 300)

"""
plt.figure(2)
plt.axes([0.15, 0.15, 0.75, 0.75])

for F in [5, 10, 20, 50, 100]:

  data = average_fragment('DataComplete/Degree{}_F{}.dat'.format(degree, F))

  plt.plot([b[0] for b in data], [b[1] for b in data], '-', linewidth = 3, alpha = 0.75, label = '$F = {}$'.format(F))

plt.xlabel('Q', size = 15)
plt.xscale('log')
plt.ylabel('Average fragment', size = 15)
plt.xticks(size = 12)
plt.yticks(size = 12)
plt.grid(True, alpha = 0.15)
plt.legend(loc = 'best', fontsize = 12)
plt.savefig('Average_fragment.png', dpi = 300)
"""
plt.show()

