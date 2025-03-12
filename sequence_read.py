import sys
sys.path.insert(0, "build/")

import ads_b
import numpy as np

import streampu as spu

import scipy.io

def extract_adresse(_,t,__):
    adresse_sock=t.sock_in.numpy
    adresse_arr=np.array(adresse_sock)
    adresse=""
    for j in range(len(adresse_arr[0])):
        value=chr(adresse_arr[0][j])
        adresse+=value
    print("voici l'adresse ",adresse)
    return adresse

class ConvertAdresse(spu.Stateful):

    def __init__(self):
        super().__init__()
        self.create_task("conv")
        self.create_socket_in(self["conv"], "sock_in", 32, spu.int8)
        self.create_codelet(self["conv"], extract_adresse)

    def execut(self,adresse_sock):
        return self["conv"](adresse_sock)





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
initializer = spu.initializer(msg_nb_ligne, dtype=spu.float64)
convertAdresse = ConvertAdresse()

# Input
list = np.array(data[:,0], dtype=np.float64)
initializer.set_init_data=list  # ne fonctionne pas
input = initializer.initialize()

print(input)

# Process
isClear = detect.process(input)
adresse_sock,format_list,type_sock,nom_sock,altitude_sock,timeFlag_sock,cprFlag_sock,latitude_sock,longitude_sock = convert.process(input)

adresse = convertAdresse.execut(adresse_sock)
print(adresse)





#for i in range(1,msg_nb_colonne):
#