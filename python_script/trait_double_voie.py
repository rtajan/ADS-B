import sys
sys.path.insert(0, "../build/")
import ads_b
import numpy as np
import streampu as spu
import scipy.io

# Parameters
Fse= 4 #20

voie_denum_list = []
voie_num_list = []

init = np.zeros(240, dtype=np.float64)

mat_contents = scipy.io.loadmat('../double_data.mat')
print(mat_contents)
data = mat_contents['data']


# Module

#src=spu.source_user(960,"../Source_User.txt",auto_reset=False,dtype=spu.float64)

square_g    = ads_b.AbsolueCarre(960)
square_d    = ads_b.AbsolueCarre(480)

porte_F2_d  = ads_b.FIRFilter(960,[1.0,0.0,1.0,0.0],4)
porte_F2_g  = ads_b.FIRFilter(480,[1.0,1.0,],2)
porte_16ech_1= ads_b.FIRFilter(240,[1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0],16)
adapt_preamb1= ads_b.FIRFilter(480,[0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,1.0,0.0,0.0,0.0,1.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,1.0,0.0,0.0,0.0,1.0,0.0],32)

decim_g     = ads_b.DecimationPM(480,0)
decim_d     = ads_b.DecimationPM(960,1)

corr        = ads_b.Corr(240)
selectmax   = ads_b.SelectMax(240,init)
norme       = ads_b.Norme2(480)
extrait     = ads_b.Extract(240, 240, 2, 0.8)

decid       = ads_b.DecisionPM(224,0.5)

detect      = ads_b.DetectCRC(112)

redirig     = ads_b.Redirig(112)
decodnom    = ads_b.DecodNom(112)
decodcoord  = ads_b.DecodCoord(112)



# Process

#print("01")
#denum_step1 = square_g.process(src.generate.out_data)
#print("02")
#src.generate()
#print("02.1")
#print(src.generate.out_data)
#print("03")

list_data = data[0][0:960]
input = spu.array(list_data, dtype=spu.float64)

denum_step1 = square_g.process(input)
num_step1 = porte_F2_d.process(input)

denum_step2 = porte_F2_g.process(denum_step1)
num_voie = decim_d.process(num_step1)

denum_voie = decim_g.process(denum_step2)

seq1 = spu.Sequence(input.task)



for i in range(int(Fse/2)):
    temp1 = porte_16ech_1.process(denum_voie[i])
    voie_denum_list.append(temp1)

    temp2 = adapt_preamb1.process(num_voie[i])
    temp3 = square_d.process(temp2)
    voie_num_list.append(temp3)

seq_g = spu.Sequence(denum_voie[0].task)
seq_d = spu.Sequence(num_voie[0].task)



norme_sig = []
for k in range(int(Fse/2)):
    temp=norme.process(num_voie[k])
    norme_sig.append(temp)

seq_norme = spu.Sequence(num_voie[0].task) #problème, sequence déjà écrite avec ces données



#sig_max = [] # pour que sig_max soit accesible en dehors de la boucle
indice = -1

for j in range(int(Fse/2)):
    intercorr=corr.process(voie_num_list[j],voie_denum_list[j])
    j_spu = spu.array([j],dtype=spu.int32)
    sig_max,indice = selectmax.process(intercorr,j_spu)

seq_rec = spu.Sequence(voie_denum_list[0].task)



sigs = spu.array(np.vstack((np.array(norme_sig[0]),np.array(norme_sig[1]))),dtype=spu.float64)
#sigs = [norme_sig[0],norme_sig[1]]

print(f"sigs : {sigs}")

indice_spu = spu.array(indice,dtype=spu.int32)
tram = extrait.process(sig_max,indice_spu,sigs)

print(tram)

bits = decid.process(tram)

print(bits)

isClear = detect.process(bits)
if (isClear):
    adresse_sock,indic = redirig.process(bits)
    if (indic[0][0]==1):
        altitude,longitude,latitude=decodcoord.process(bits)
    elif (indic[0][0]==0):
        nom_sock = decodnom.process(bits)

seq_fin = spu.Sequence(sig_max.task)




# Affichage

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

prb_lon(longitude)
prb_lat(latitude)
prb_alt(altitude)
prb_addr(adresse_sock)
prb_nom(nom_sock)
ter = spu.Terminal_dump([repg, rep])


ter.legend()
seq_fin.exec(ter)







