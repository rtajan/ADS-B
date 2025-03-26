import sys
sys.path.insert(0, "../build/")
import ads_b
import numpy as np
import streampu as spu

# Parameters

Fse     = 4
seuil   = 0.8
v0      = 0.5

longitude   = 0.0
altitude    = 0.0
latitude    = 0.0

# Module

src=spu.source_user(960,"../Source_User.txt",auto_reset=False,dtype=spu.float64)

square_g    = ads_b.AbsolueCarre(960)
square_d    = ads_b.AbsolueCarre(960)

porte_F2    = ads_b.FIRFilter(960,[1.0,0.0,1.0,0.0],4)

array_32    = np.ones(32,dtype=np.float64)
porte_32    = ads_b.FIRFilter(480,array_32,32)

part1_0     = [1.0,0.0,0.0,0.0]
part0_0     = [0.0,0.0,0.0,0.0]
part0_1     = [0.0,0.0,1.0,0.0]
preamb_array_reel=  np.concatenate([part1_0,part1_0,part0_0,part0_1,part0_1,part0_0,part0_0,part0_0])
preamb_array = np.empty(len(preamb_array_reel) * 2)
preamb_array[::2] = preamb_array_reel
preamb_array[1::2] = 0.0 #this signal is complex [reel, imag ...]
porte_preamb= ads_b.FIRFilter(960,preamb_array,64)

norme       = ads_b.Norme2(960)

selector    = ads_b.Select(480)
extrait     = ads_b.Extract(480,Fse,seuil)

decid       = ads_b.DecisionPM(224,v0)
detect      = ads_b.DetectCRC(112)

redirig     = ads_b.Redirig(112)
decodnom    = ads_b.DecodNom(112)
decodcoord  = ads_b.DecodCoord(112)


# Process

sig_reel    = square_g.process(src.generate.out_data)

denum       = porte_32.process(sig_reel)

use_sig     = porte_F2.process(src.generate.out_data)
pre_num     = porte_preamb.process(use_sig)
num         = square_d.process(pre_num)

decal_max   = selector.process(num,denum)

sig_norme   = norme.process(use_sig)

tram        = extrait.process(decal_max,sig_norme)

bits = decid.process(tram)
isClear = detect.process(bits)

adresse_sock,indic = redirig.process(bits)
if (indic[0][0]==1):
    altitude,longitude,latitude=decodcoord.process(bits)
elif (indic[0][0]==0):
    nom_sock = decodnom.process(bits)

#print(f"altitude : {altitude}")
#print(f"nom : {str(nom_sock)}")

# Affichage

repg = spu.Reporter_probe("Aircraft Genral infos", "")
rep = spu.Reporter_probe("Aircraft position", "")

prb_lon = spu.probe_value(1, "longitude", None, dtype=spu.float64)
prb_lat = spu.probe_value(1, "latitude", None, dtype=spu.float64)
prb_alt = spu.probe_value(1, "altitude", None, dtype=spu.float64)
rep.register_probes([prb_lon, prb_lat, prb_alt])

prb_addr = spu.probe_value(6, "adress", None, dtype=spu.int8)
prb_nom = spu.probe_value(6, "nom", None, dtype=spu.int8)
prb_addr.str_display = True
prb_nom.str_display = True

repg.register_probes([prb_addr, prb_nom])

prb_lon(longitude)
prb_lat(latitude)
prb_alt(altitude)
prb_addr(adresse_sock)
prb_nom(nom_sock)
ter = spu.Terminal_dump([repg, rep])

seq = spu.Sequence(src.generate)
seq.export_dot("seq_double_voie.dot")


ter.legend()
seq.exec(ter)