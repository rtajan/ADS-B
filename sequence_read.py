import sys
sys.path.insert(0, "build/")

import ads_b
import numpy as np

import streampu as spu

import scipy.io

def transmit(_,t,__):
    element=t.sock_in.numpy
    #t.sock_out.numpy=element
    return element

mat_contents = scipy.io.loadmat('adsb_msgs.mat')
data = mat_contents['adsb_msgs']

REF_LON = -0.606629  # Longitude de référence
REF_LAT = 44.806884  # Latitude de référence

msg_nb_ligne, msg_nb_colonne = data.shape
print(msg_nb_colonne, msg_nb_ligne)

#nom = ''
#connaisNom = False

# Module :
convert = ads_b.Bit2Register(msg_nb_ligne)
detect = ads_b.DetectCRC(msg_nb_ligne)

# Input :
list = np.array(data[:,0], dtype=np.float64)
input = spu.array(list, dtype=spu.float64)

#print(input)

# Process :
isClear = detect.process(input)
adresse_sock,format_list,type_sock,nom_sock,altitude_sock,timeFlag_sock,cprFlag_sock,latitude_sock,longitude_sock = convert.process(input)

adresse_arr=np.array(adresse_sock)
adresse=""
for j in range(len(adresse_arr[0])):
    value=chr(adresse_arr[0][j])
    adresse+=value
print("voici l'adresse ",adresse)

type_arr=np.array(type_sock)
latitude_arr=np.array(latitude_sock)
longitude_arr=np.array(longitude_sock)
#print(longitude_sock)

rep=spu.Reporter_probe("Aircraft Info","")
prb_lon=spu.probe_value(1,"longitude",rep,dtype=spu.float64)
#prb_lon.probe(longitude_arr[0][0])
#prb_lon.tasks[0].sockets[0]=longitude_sock

#prb_lon.create_task("transmit")
#print(len(prb_lon.tasks))
#print(prb_lon.tasks[1].name)

#prb_lon.create_socket_in(prb_lon["transmit"], "sock_in", 1, spu.float64)
#prb_lon.create_socket_out(prb_lon["transmit"], "sock_out", 1, spu.float64)
#prb_lon.create_codelet(prb_lon["transmit"], transmit)
#prb_lon["transmit::sock_in"]=longitude_sock  #convert["process::longitude"]
#print(prb_lon["probe::in"])
#prb_lon["transmit"].exec()

#prb_lon["probe::in"]=longitude_sock
prb_lon(longitude_sock)

ter = spu.Terminal_dump([rep])
ter.legend()

#print(dir(ter))

# Sequence :
seq = spu.Sequence(input.task)
seq.export_dot("sequence_read.dot")



for i in range(1, msg_nb_colonne):
    list[:] = data[:, i]
    # list = np.array(data[:,i], dtype=np.float64)
    # print(input)
    seq.exec_n_times(1)
    ter.legend()
    ter.temp_report()
