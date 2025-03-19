import sys
sys.path.insert(0, "../build/")
import ads_b
import numpy as np
import streampu as spu

# Parameters
Fse= 20 #4

h=np.array([1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0],dtype=np.float64)
print(h)

# Module
src=spu.source_user_binary(64800,"Simple_Source_user",True,True,dtype=spu.float64)
square_20   = ads_b.AbsolueCarre(2400)
square_2    = ads_b.AbsolueCarre(256)

porte_F2    = ads_b.Filter(2400,h,10)
porte_16ech_1= ads_b.Filter(241,[1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0],16)
#porte_16ech_0= ads_b.Filter(240,[1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,],16)
#adapt_preamb0= ads_b.Filter(240,[0.0,0.0,0.0,0.0,0.0,0.0,1.0,0.0,1.0,0.0,0.0,0.0,0.0,1.0,0.0,1.0],16)
adapt_preamb1= ads_b.Filter(241,[0.0,0.0,0.0,0.0,0.0,0.0,1.0,0.0,1.0,0.0,0.0,0.0,0.0,1.0,0.0,1.0],16)


decim       = ads_b.DecimationPM(2409)

select      = ads_b.Selector(10)


# Process
data=src["generate::out_data"]
src["generate"].exec()

list=data[0:120]
input = spu.array(list,dtype=spu.float64)

denum_p_square = square_20.process(input)
num_p_f2 = porte_F2.process(input)

denum_p_f2 = porte_F2.process(denum_p_square)
num_p_decim = decim.process(num_p_f2)




