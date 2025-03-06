import sys

sys.path.insert(0, "build/")

import ads_b
import numpy as np
import matplotlib.pyplot as plt

import streampu as spu

import scipy.io

mat_contents = scipy.io.loadmat('adsb_msgs.mat')
print(mat_contents.keys())

data = mat_contents['adsb_msgs']
print(len(data))


REF_LON = -0.606629  # Longitude de référence
REF_LAT = 44.806884  # Latitude de référence


msg_nb_ligne, msg_nb_colonne = data.shape
print(msg_nb_colonne, msg_nb_ligne)

## Initialisation des coordonnées et de la structure temporaire
coord = np.zeros((2, msg_nb_colonne))
#temp = {
#    'adresse': '',
#    'format': 0,
#    'type': 0,
#    'nom': '',
#    'altitude': 0,
#    'timeFlag': False,
#    'cprFlag': False,
#    'latitude': 0.0,
#    'longitude': 0.0
#}
#
nom = ''
connaisNom = False

convert = ads_b.Bit2Register(msg_nb_ligne)

for i in range(msg_nb_colonne):

    print(data[:,i])
    list = np.array(data[:,i], dtype=np.float64)
    print(list)
    input = spu.array(list, dtype=spu.float64)

    print("la")

    temp = convert.process(input)

    print("ici")

    if (9 <= temp['type'] <= 18) or (20 <= temp['type'] <= 22):
        coord[0, i] = temp['latitude']
        coord[1, i] = temp['longitude']

    print("ou la")

    if 1 <= temp['type'] <= 4:
        nom = temp['nom']
        connaisNom = True

    print("ou ici")

    if connaisNom:
        temp['nom'] = nom

    print(temp)

sanszerolong = coord[1, :] != 0
sanszerolat = coord[0, :] != 0

#plt.figure()
#plt.imshow(plt.imread('General/fond.png'), extent=[REF_LON-1, REF_LON+1, REF_LAT-1, REF_LAT+1])
#plt.plot(coord[1, sanszerolat], coord[0, sanszerolong], '--o', markerfacecolor='blue')
#plt.xlabel('Longitude')
#plt.ylabel('Latitude')
#plt.title('Carte des positions ADS-B')
#plt.show()
