from ctypes import *

# the memory pool size is 4096 > 480 or 360
memory_pool_size = 4096;
# the memory pool id accross python and C/C++
_SDN_LAB_NNDATA_MEMORY_ID = 7418;
# the data length of all list data
_SDN_LAB_NNDATA_LEN = 20;

# these three classes map to the structures in NNData.h (C/C++)
'''
store the data (following the order of time increase)
for a single STA
memory pool size taken at (8+8)*30 = 480 bytes
'''
class NNFeature(Structure):
    _pack_ = 1;
    _fields_ = [
        ('time', c_double*_SDN_LAB_NNDATA_LEN),
        ('rxPower', c_double*_SDN_LAB_NNDATA_LEN)
    ];
'''
store MCS and its activate time point (following the order of data rate increase)
for a single STA
memory pool size taken at (4+8)*30 = 360 bytes
'''
class NNPredicted(Structure):
    _pack_ = 1;
    _fields_ = [
        ('mcs', c_uint32*_SDN_LAB_NNDATA_LEN),
        ('mcsActivateTime', c_double*_SDN_LAB_NNDATA_LEN)
    ];
'''
not used
'''
class NNTarget(Structure):
    _pack_ = 1;
    _fields_ = [
        ('target', c_uint8)
    ];