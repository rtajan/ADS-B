import sys
sys.path.insert(0, "../build/")

import numpy as np

import streampu as spu

import scipy.io

mat_contents = scipy.io.loadmat('../buffers.mat')
print(mat_contents)
data = np.array(mat_contents['buffers'],dtype=np.float64) #on convertit les complexes

sk=spu.sink_user_binary(18000000,"../Source_user",dtype=spu.float64)

sk["send::in_data"]=data

sk["send"].exec()
