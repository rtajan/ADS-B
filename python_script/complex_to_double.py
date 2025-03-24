import sys
sys.path.insert(0, "../build/")
import numpy as np
import scipy.io

mat_contents = scipy.io.loadmat('../buffers.mat')
print(mat_contents)
data = mat_contents['buffers']

def complex_to_double_list(complex_array):
    real_imag_list = []
    for number in np.ravel(complex_array):
        real_imag_list.append(number.real)
        real_imag_list.append(number.imag)
    return real_imag_list

double_list = complex_to_double_list(data)

print(double_list)

mat_data_to_save = {'data': double_list}

output_file_path = 'double_data.mat'
scipy.io.savemat(output_file_path, mat_data_to_save)

print(f"Données sauvegardées dans {output_file_path}")