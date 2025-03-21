import sys
sys.path.insert(0, "../build/")

import numpy as np

import streampu as spu

import scipy.io

mat_contents = scipy.io.loadmat('../buffers.mat')
print(mat_contents)
data = np.array(mat_contents['buffers'],dtype=np.complex128)
sk=spu.sink_user_binary(36000000,"../Source_user",dtype=spu.float64)

real_parts = np.real(data)
imaginary_parts = np.imag(data)

combined_data = np.column_stack((real_parts, imaginary_parts))

print(data.shape)
print(combined_data.shape)
print(combined_data[0:13][:])

output_file = 'Source_User.txt'
with open(output_file, 'w') as file:
    for i in range(0, real_parts.shape[0], 960):
        line_real = real_parts[i:i+960].flatten()
        line_imag = imaginary_parts[i:i+960].flatten()
        line_combined = np.column_stack((line_real, line_imag)).flatten()
        line_str = ' '.join(f"{val:.2f}" for val in line_combined)
        file.write(line_str + '\n')

print(f"Les données ont été écrites dans {output_file}")

sk["send::in_data"]=combined_data

sk["send"].exec()
