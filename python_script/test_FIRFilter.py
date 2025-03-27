import sys
sys.path.insert(0, "../build/")
import ads_b
import numpy as np
import streampu as spu


def test_FIRFilter():
    h = np.array([1.0,0.0,1.0,0.0], dtype=np.float64)
    sig = np.array([1.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0], dtype=np.float64)
    print( f"h : {h}")
    print( f"sig : {sig}")

    porte = ads_b.FIRFilter(9,h,4)
    result = porte.process(sig)

    print( f"result : {result}")

if __name__ == "__main__":
    test_FIRFilter()