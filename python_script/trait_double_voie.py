import sys
sys.path.insert(0, "../build/")
import ads_b
import numpy as np
import streampu as spu

# Parameters
Fse= 4 #20

# Module
src=spu.source_user(960,"../Source_User.txt",auto_reset=False,dtype=spu.float64)

square_g    = ads_b.AbsolueCarre(960)
#square_d    = ads_b.AbsolueCarre(480)
#
#porte_F2_d  = ads_b.FIRFilter(960,[1.0,0.0,1.0,0.0],4)
#porte_F2_g  = ads_b.FIRFilter(480,[1.0,0.0,1.0,0.0],4)
#porte_16ech_1= ads_b.FIRFilter(480,[1.0,0.0,1.0,0.0,1.0,0.0,1.0,0.0,1.0,0.0,1.0,0.0,1.0,0.0,1.0,0.0,1.0,0.0,1.0,0.0,1.0,0.0,1.0,0.0,1.0,0.0,1.0,0.0,1.0,0.0,1.0,0.0],32)
#adapt_preamb1= ads_b.FIRFilter(960,[0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,1.0,0.0,0.0,0.0,1.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,1.0,0.0,0.0,0.0,1.0,0.0],32)
#
#decim_g     = ads_b.DecimationPM(480,0)
#decim_d     = ads_b.DecimationPM(960,1)
#
#corr_select = ads_b.CorrSelect(240)
#extrait     = ads_b.Extract(240, 2, 240, 0.8)
#
#decid       = ads_b.DecisionPM(224,0.5)
#
#redirig     = ads_b.Redirig(112)
#decodnom    = ads_b.DecodNom(112)
#decodcoord  = ads_b.DecodCoord(112)



# Process

print("01")
denum_step1 = square_g.process(src.generate.out_data)
print("02")
src.generate()
print("02.1")
print(src.generate.out_data)
print("03")

#num_step1 = porte_F2_d.process(data)
#
#denum_step2 = porte_F2_g.process(denum_step1)
#num_voie1,num_voie2 = decim_d.process(num_step1)
#
#
#denum_voie1,denum_voie2 = decim_g.process(denum_step2)


#for i in range(Fse/2):

#seq = spu.Sequence(src.generate)
#seq.exec()







