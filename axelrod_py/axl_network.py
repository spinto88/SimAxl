import os
import ctypes as C
import networkx as nx
import random as rand
import numpy as np
from axl_agent import *

libc = C.CDLL(os.getcwd() + '/axelrod_py/libc.so')

class Axl_network(nx.Graph, C.Structure):

    """
    Axelrod network: it has nagents axelrod agents, and an amount of noise in the dynamics of the system. This class inherites from the networkx.Graph the way to be described.
    """
    _fields_ = [('nagents', C.c_int),
                ('agent', C.POINTER(Axl_agent)),
		('f', C.c_int),
		('a', C.POINTER(C.POINTER(C.c_int))),
		('corr', C.POINTER(C.POINTER(C.c_double))),
		('seed', C.c_int)]

    def __init__(self, n, f, q, model = 'Axelrod', topology = 'lattice', **kwargs):
        
	"""
        Constructor: initializes the network.Graph first, and set the topology and the agents' states. 
	"""
        # Init graph properties
        nx.Graph.__init__(self)
        nx.empty_graph(n, self)
        self.nagents = n

        # Model parameters
        self.f = f
        self.q = q

        # Init agents' states
        self.init_agents(f, q)
 
        # Init topology
        self.set_topology(topology, **kwargs)

	# Random seed 
	self.seed = rand.randint(0, 10**7)

	# Type of model 
	self.model = model

    def set_topology(self, topology, **kwargs):
        """
        Set the network's topology
        """
        import set_topology as setop

        self.id_topology = topology

        self.adjm = setop.set_topology(self, topology, **kwargs).toarray()

        self.a = (self.nagents * C.POINTER(C.c_int))()
        for i in range(self.nagents):
            self.a[i] = ((self.nagents) * C.c_int)(*self.adjm[i])

    def init_agents(self, f, q):
        """
        Iniatialize the agents' state.
        """
        self.agent = (Axl_agent * self.nagents)()
    
        for i in range(self.nagents):
            self.agent[i] = Axl_agent(f, q)

        corr_matrix = np.zeros([self.nagents, self.nagents], dtype = np.float)
        for i in range(self.nagents):
            for j in range(i+1, self.nagents):
                corr_matrix[i,j] = self.agent[i].homophily(self.agent[j])

        corr_matrix += corr_matrix.T

        self.corr = (self.nagents * C.POINTER(C.c_double))()
        for i in range(self.nagents):
            self.corr[i] = ((self.nagents) * C.c_double)(*corr_matrix[i])

    
    def re_init_agents(self, f, q):

        for i in range(self.nagents):
            self.agent[i].__init__(f,q)

        corr_matrix = np.zeros([self.nagents, self.nagents], dtype = np.float)
        for i in range(self.nagents):
            for j in range(i+1, self.nagents):
                corr_matrix[i,j] = self.agent[i].homophily(self.agent[j])

        corr_matrix += corr_matrix.T

        self.corr = (self.nagents * C.POINTER(C.c_double))()
        for i in range(self.nagents):
            self.corr[i] = ((self.nagents) * C.c_double)(*corr_matrix[i])

    def evolution(self, steps = 1):
        """
	Make n steps asynchronius evolutions of the system
        """
	if self.model == 'Axelrod':
            libc.evolution_axelrod.argtypes = [C.POINTER(Axl_network)]

            for step in range(steps):
                libc.evolution_axelrod(C.byref(self))

	elif self.model == 'Similarity_average':
            libc.evolution_similarity_average.argtypes = [C.POINTER(Axl_network)]

            for step in range(steps):
                libc.evolution_similarity_average(C.byref(self))

	elif self.model == 'Similarity_stochastic':
            libc.evolution_similarity_stochastic.argtypes = [C.POINTER(Axl_network)]

            for step in range(steps):
                libc.evolution_similarity_stochastic(C.byref(self))

	elif self.model == 'Triangle_percolation':
            libc.evolution_triangle_percolation.argtypes = [C.POINTER(Axl_network)]

            libc.evolution_triangle_percolation(C.byref(self))

    def get_corr_matrix(self):

        corr_matrix = np.zeros([self.nagents, self.nagents], dtype = np.float)
        for i in range(self.nagents):
            for j in range(i+1, self.nagents):

		if self.model != 'Axelrod':
                    corr_matrix[i][j] = self.corr[i][j]
		else:
                    corr_matrix[i][j] = self.agent[i].homophily(self.agent[j])

        corr_matrix += corr_matrix.T
        for i in range(self.nagents):
            corr_matrix[i][i] = 1.00

        return corr_matrix


    def fragments_size(self, fragment_tau = 0.00):

        corr_matrix = self.get_corr_matrix()
        final_ad_matrix = np.zeros(corr_matrix.shape, dtype = np.int)
        for i in range(self.nagents):
            for j in range(i+1, self.nagents):
                if corr_matrix[i,j] > fragment_tau and self.adjm[i,j] == 1:
                    final_ad_matrix[i,j] = 1

        final_ad_matrix += final_ad_matrix.T
        final_graph = nx.from_numpy_array(final_ad_matrix)
        fragments = [len(x) for x in list(nx.connected_components(final_graph))]

        return fragments

    def is_there_active_links(self):

	if self.model == "Axelrod":
		libc.is_there_active_links_a.argtypes = [C.POINTER(Axl_network)]
		libc.is_there_active_links_a.restype = C.c_int

		return libc.is_there_active_links_a(C.byref(self))

	elif self.model == "Similarity_average":
		libc.is_there_active_links_sa.argtypes = [C.POINTER(Axl_network)]
		libc.is_there_active_links_sa.restype = C.c_int

		return libc.is_there_active_links_sa(C.byref(self))

	elif self.model == "Similarity_stochastic":
		libc.is_there_active_links_ss.argtypes = [C.POINTER(Axl_network)]
		libc.is_there_active_links_ss.restype = C.c_int

		return libc.is_there_active_links_ss(C.byref(self))

    def evol2convergence(self):

	if self.model != 'Triangle_percolation':
		steps = 0
		while(self.is_there_active_links() != 0):
			self.evolution(100)
			steps += 100
		return steps
	else:
		self.evolution()

    def fraction_of_zeros(self):
        pass

    def save_fragments_distribution(self, fname):
	 
        fragment_sizes = [d[1] for d in self.fragments().items()]

        fp = open(fname, 'a')
        fp.write('{},{},'.format(self.f, self.q))
        fp.write(', '.join([str(s) for s in fragment_sizes]))
        fp.write('\n')
        fp.close()
        
