import sys
sys.path.insert(0, "build/")
import ads_b
import numpy as np
import streampu as spu
import scipy.io

# Parameters
n_elmts = 112 #without preambule
Fe = 20e6
Ts = 1e-6

simple_preamb = np.array([1, 0, 1, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0], dtype=np.float64)
preambule = np.kron(simple_preamb, np.ones(10))

mat_contents = scipy.io.loadmat('adsb_msgs.mat')
data = mat_contents['adsb_msgs']

msg_nb_ligne, msg_nb_colonne = data.shape
print(msg_nb_ligne, msg_nb_colonne)

# variable

list = np.array(data[:,0], dtype=np.float64)
input = spu.array(list, dtype=spu.float64)
buff=np.array([],dtype=np.float64)

# Module
emit=ads_b.EmetteurPM(n_elmts,Fe,Ts)
canal=ads_b.Canal(2400,10,int(Fe*Ts))
sk=spu.sink_user_binary(64800,"Simple_Source_user",dtype=spu.float64)



# Process
for i in range(msg_nb_colonne):
    list = np.array(data[:,i], dtype=np.float64)

    entre = spu.array(input, dtype=spu.float64)
    yl = emit.process(entre)

    prep = spu.array(np.concatenate((preambule,np.array(yl)[0])),dtype=spu.float64)
    # adding the preambule by concatenated
    #print(len(prep))

    sig = canal.process(prep)
    #print(sig)
    print(len(sig))
    buff=np.concatenate((buff,np.array(sig)[0]))

sk["send::in_data"]=buff
sk["send"].exec()