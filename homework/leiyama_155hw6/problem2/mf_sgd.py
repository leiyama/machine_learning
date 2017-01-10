import numpy as np
from scipy.sparse import csr_matrix
from numpy import linalg as LA
import random

def compute_gradient(Y,U,V):
    u_gradient = np.full(U.shape,0.0)
    v_gradient = np.full(V.shape,0.0)
    
    for iminibatch in range(len(Y)):
        a = Y[iminibatch,0] - 1
        b = Y[iminibatch,1] - 1
        t = np.dot(U[a,:],V[b,:])
        u_gradient[a,:] -= (Y[iminibatch,2] - t)*V[b,:]
        v_gradient[b,:] -= (Y[iminibatch,2] - t)*U[a,:]
    
    return u_gradient,v_gradient


def train_uv(Y, k):
    
    lambd = 0.75
    lr = 0.1
    m = np.max(Y[:,0])
    n = np.max(Y[:,1])
    print(n)
    n = 1682
    U = np.random.rand(m,k)/k
    V = np.random.rand(n,k)/k
    N = len(Y)
    print(N)
    residue_old = 0.0
    N_batch = 10

    for j in range(1000000):
        u_gradient = np.full((m,k),0.0)
        v_gradient = np.full((n,k),0.0)

        for n_minibatch in range(N_batch):

            i = random.randint(0,N-1)
            a = Y[i,0] - 1
            b = Y[i,1] - 1
            t = np.dot(U[a,:],V[b,:])
            u_gradient[a,:] -= (Y[i,2] - t)*V[b,:]
            v_gradient[b,:] -= (Y[i,2] - t)*U[a,:]
        u_gradient *= 1.0/N_batch
        v_gradient *= 1.0/N_batch

        u_gradient += lambd * 2.0 * U
        v_gradient += lambd * 2.0 * V

        if(j%10000==0):
            residue_new = compute_norm(U,V,Y,lambd)
            print("norm",residue_new)
        U -=  u_gradient * lr
        V -=  v_gradient * lr
        if((np.abs(u_gradient).max < 0.1) & (np.abs(v_gradient).max < 0.1)):
            break

    return U,V

def compute_norm(U,V,Y,lambd):
    misfit = 0.0
    print(len(Y))
    for i in range(len(Y)):
        a = Y[i,0] - 1
        b = Y[i,1] - 1
        t = np.dot(U[a,:], V[b,:])
        misfit += (Y[i,2] - t)**2.0
    misfit +=  (LA.norm(U) + LA.norm(V)) * lambd
    return misfit


'''Data Aquicition and Output Saving'''
'''================================='''
data = np.genfromtxt('./movies.txt', dtype = None, delimiter = '\t')
print(data.shape)

train_data = np.loadtxt('./train.txt')
test_data = np.loadtxt('./test.txt')

row = train_data[:,0]
col = train_data[:,1]
datav = train_data[:,2]

Matrix_movie = csr_matrix((datav, (row - 1 , col - 1)))
Ein = []
Eout = []

U = np.loadtxt('./u_decom_100.txt')
V = np.loadtxt('./v_decom_100.txt')

print(compute_norm(U, V, train_data, 0.0))
print(compute_norm(U, V, test_data, 0.0))

for k in [100]:
    U,V=train_uv(train_data, k)
    Ein.append(compute_norm(U, V, train_data,0.0))
    Eout.append(compute_norm(U, V, test_data,0.0))
    np.savetxt('u_decom_{0}_reg3.txt'.format(k), U)
    np.savetxt('v_decom_{0}_reg3.txt'.format(k), V)
