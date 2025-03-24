import sys
sys.path.insert(0, "../build/")
import ads_b
import streampu as spu



src=spu.source_user_binary(18000000,"../Source_user",True,True,dtype=spu.float64)

data=src["generate::out_data"]
src["generate"].exec()

for i in range(18000000):
    print(data[0+i:960+i])