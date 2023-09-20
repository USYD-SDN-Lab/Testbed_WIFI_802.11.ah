from ctypes import *

# the memory pool size is 1024 > 480 or 360
memory_pool_size = 1024;

# C/C++ & python shared settings
_SDN_LAB_NNDATA_MEMORY_ID       = 7418;     # the memory pool id accross python and C/C++
_SDN_LAB_NNDATA_LEN             = 20;       # the data length of all list data
_SDN_LAB_MCS_NUM                = 29;       # supported MCS number
_SDN_LAB_NNDATA_ILLEGAL_DATA    = 0;

# these three classes map to the structures in NNData.h (C/C++)
'''
store the data (following the order of time increase)
for a single STA
memory pool size taken at (8+8+4)*20 = 400 bytes
'''
class NNFeature(Structure):
    _pack_ = 1;
    _fields_ = [
        #('id', c_uint32*1),
        ('time', c_double*_SDN_LAB_NNDATA_LEN),
        ('rxPower', c_double*_SDN_LAB_NNDATA_LEN),
        ('bandwidth', c_uint32*_SDN_LAB_NNDATA_LEN),
        ('staid', c_uint32)
    ];
'''
store MCS and its activate time point (following the order of data rate increase)
for a single STA
memory pool size taken at (4+8)*20 = 240 bytes
'''
class NNPredicted(Structure):
    _pack_ = 1;
    _fields_ = [
        ('mcs', c_uint32*_SDN_LAB_MCS_NUM),
        ('mcsActivateTime', c_double*_SDN_LAB_MCS_NUM),
    ];
'''
not used
'''
class NNTarget(Structure):
    _pack_ = 1;
    _fields_ = [
        ('target', c_uint8)
    ];