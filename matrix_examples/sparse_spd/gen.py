import sys
import numpy as np
import scipy.stats as stats
import scipy.sparse as sparse
import matplotlib.pyplot as plt
import sklearn.datasets



filename = sys.argv[1]
sz = int(filename)
matrix = sklearn.datasets.make_sparse_spd_matrix(n_dim=sz, alpha=(sz - min(10, sz - 1)) / sz, sparse_format='csr').sorted_indices() * 10

f = open(filename, 'w')

print(sz, sz, file=f)
print(len(matrix.data), file=f)
for elem in matrix.data:
    print(elem, end=' ', file=f)
print(file=f)

for elem in matrix.indices:
    print(elem, end=' ', file=f)
print(file=f)

for elem in matrix.indptr:
    print(elem, end=' ', file=f)
print(file=f)

f.close()
