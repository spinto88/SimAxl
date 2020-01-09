from axelrod_py import *

N = 100
F = 100
q = 10

rand.seed(123450)

fz = lambda f, q: (1.00 - 1.00/q)**f

for q in np.arange(250, 5050, 50):

    mysys = Axl_network(n = N, f = F, q = q, model = 'Similarity_stochastic', topology = 'lattice', degree = N)

    mysys.evol2convergence()

    print q, fz(F,q), np.max(mysys.fragments_size())

