from axelrod_py import *

N = 1024
F = 10
q = 4

rand.seed(123452)

fz = lambda f, q: (1.00 - 1.00/q)**f
    
mysys = Axl_network(n = N, f = F, q = q, model = 'Axelrod', topology = 'lattice', degree = N)

for q in np.arange(5, 96, 10):

  for i in range(5):

    mysys.re_init_agents(F,q)

    mysys.evol2convergence()

    mysys.save_fragments_distribution("Prueba.dat")

    print q, fz(F,q), np.max(mysys.fragments_size())

