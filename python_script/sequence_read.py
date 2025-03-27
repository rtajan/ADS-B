import sys

sys.path.insert(0, "../build/")

import ads_b
import numpy as np

import streampu as spu

import scipy.io

spu.rang.enable_colors()


def transmit(_, t, __):
    element = t.sock_in.numpy
    # t.sock_out.numpy=element
    return element


mat_contents = scipy.io.loadmat("../adsb_msgs.mat")
data = mat_contents["adsb_msgs"]

REF_LON = -0.606629  # Longitude de référence
REF_LAT = 44.806884  # Latitude de référence

msg_nb_ligne, msg_nb_colonne = data.shape
#print(msg_nb_colonne, msg_nb_ligne)

# Module :
convert = ads_b.Bit2Register(msg_nb_ligne)
detect = ads_b.DetectCRC(msg_nb_ligne)

# Input :
list = np.array(data[:, 0], dtype=np.float64)
input = spu.array(list, dtype=spu.float64)
src = spu.source_user(112, "../trames.txt", auto_reset=False, dtype=spu.float64)
# spu.help(src)
# print(input)
# Process :
isClear = detect.process(src.generate.out_data)
print(f" isClear = {isClear}")
print(src.generate.out_data)
(
    adresse_sock,
    format_list,
    type_sock,
    nom_sock,
    altitude_sock,
    timeFlag_sock,
    cprFlag_sock,
    latitude_sock,
    longitude_sock,
) = convert.process(src.generate.out_data)

repg = spu.Reporter_probe("Aircraft Genral infos", "")
rep = spu.Reporter_probe("Aircraft position", "")

prb_lon = spu.probe_value(1, "longitude", None, dtype=spu.float64)
prb_lat = spu.probe_value(1, "latitude", None, dtype=spu.float64)
prb_alt = spu.probe_value(1, "altitude", None, dtype=spu.int32)
rep.register_probes([prb_lon, prb_lat, prb_alt])

prb_addr = spu.probe_value(6, "adress", None, dtype=spu.int8)
prb_nom = spu.probe_value(6, "nom", None, dtype=spu.int8)
prb_addr.str_display = True
prb_nom.str_display = True

repg.register_probes([prb_addr, prb_nom])

prb_lon(longitude_sock)
prb_lat(latitude_sock)
prb_alt(altitude_sock)
prb_addr(adresse_sock)
prb_nom(nom_sock)
ter = spu.Terminal_dump([repg, rep])


# Sequence :
seq = spu.Sequence(src.generate)
seq.export_dot("sequence_read.dot")


ter.legend()

seq.exec(ter)

"""
    # J'ai laissé ça là car j'y ai mis comment j'ai fait pour écrire le fichier trames.txt
    f.write(f"{data.shape[1]}\n\n{data.shape[0]}\n\n")
    for i in range(1, msg_nb_colonne):
    list[:] = data[:, i]
    # list = np.array(data[:,i], dtype=np.float64)
    # print(input)
    seq.exec_n_times(1)
    ter.temp_report()
    f.write(
        f"{list[:]}".replace("[", "")
        .replace(".", "")
        .replace("]", "")
        .replace("\n", "")
        + "\n"
    )
"""
