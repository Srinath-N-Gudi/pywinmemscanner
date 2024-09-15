import ctypes

memscan = ctypes.CDLL("pywinmemscanner/utils/backend/MemoryScanner.dll")

HANDLE = ctypes.c_void_p
UCP = ctypes.POINTER(ctypes.c_ubyte)
UCPP = ctypes.POINTER(ctypes.POINTER(ctypes.c_ubyte))
INTP = ctypes.POINTER(ctypes.c_int)
FLOATP = ctypes.POINTER(ctypes.c_float)
DOUBLEP = ctypes.POINTER(ctypes.c_double)


class UCharAddressVector(ctypes.Structure):
     _fields_ = [("array", UCPP),
                ("size", ctypes.c_int),
                ("capacity", ctypes.c_int)]





memscan.get_pid_by_name.argtypes = [ctypes.c_char_p]
memscan.get_pid_by_name.restype = ctypes.c_int

memscan.getHandleByPID.argtypes = [ctypes.c_int]
memscan.getHandleByPID.restype = HANDLE

memscan.closeHandle.argtypes = [HANDLE]
memscan.closeHandle.restype = ctypes.c_int

memscan.createAddressArray.argtypes = None
memscan.createAddressArray.restype = ctypes.POINTER(UCharAddressVector)
#RPM
memscan.RPM_INT.argtypes = [HANDLE, UCP, INTP]
memscan.RPM_INT.restype = ctypes.c_int

memscan.RPM_FLOAT.argtypes = [HANDLE, UCP, FLOATP]
memscan.RPM_FLOAT.restype = ctypes.c_int

memscan.RPM_DOUBLE.argtypes = [HANDLE, UCP, DOUBLEP]
memscan.RPM_DOUBLE.restype = ctypes.c_int
#WPM
memscan.WPM_INT.argtypes = [HANDLE, UCP, ctypes.c_int]
memscan.WPM_INT.restype = ctypes.c_int

memscan.WPM_FLOAT.argtypes = [HANDLE, UCP, ctypes.c_float]
memscan.WPM_FLOAT.restype = ctypes.c_int

memscan.WPM_DOUBLE.argtypes = [HANDLE, UCP, ctypes.c_double]
memscan.WPM_DOUBLE.restype = ctypes.c_int
# Scanners 
memscan.scanINT.argtypes = [HANDLE, ctypes.c_int, ctypes.POINTER(UCharAddressVector)]
memscan.scanINT.restype = ctypes.c_int

memscan.scanFLOAT.argtypes = [HANDLE, ctypes.c_float, ctypes.POINTER(UCharAddressVector)]
memscan.scanFLOAT.restype = ctypes.c_int

memscan.scanDOUBLE.argtypes = [HANDLE, ctypes.c_double, ctypes.POINTER(UCharAddressVector)]
memscan.scanDOUBLE.restype = ctypes.c_int
# Next Scanners 
memscan.scanNextINT.argtypes = [HANDLE, ctypes.POINTER(UCharAddressVector), ctypes.c_int]
memscan.scanNextINT.restype = ctypes.c_int

memscan.scanNextFLOAT.argtypes = [HANDLE, ctypes.POINTER(UCharAddressVector), ctypes.c_float]
memscan.scanNextFLOAT.restype = ctypes.c_int

memscan.scanNextDOUBLE.argtypes = [HANDLE, ctypes.POINTER(UCharAddressVector), ctypes.c_double]
memscan.scanNextDOUBLE.restype = ctypes.c_int

memscan.freeAddressArray.argtypes = [ctypes.POINTER(UCharAddressVector)]
memscan.freeAddressArray.restype = None

get_pid_by_name = memscan.get_pid_by_name
getHandleByPID = memscan.getHandleByPID
closeHandle = memscan.closeHandle
createAddressArray = memscan.createAddressArray
RPM_INT = memscan.RPM_INT
RPM_FLOAT = memscan.RPM_FLOAT
RPM_DOUBLE = memscan.RPM_DOUBLE
WPM_INT = memscan.WPM_INT
WPM_FLOAT = memscan.WPM_FLOAT
WPM_DOUBLE = memscan.WPM_DOUBLE
scanINT = memscan.scanINT
scanFLOAT = memscan.scanFLOAT
scanDOUBLE = memscan.scanDOUBLE
scanNextINT = memscan.scanNextINT
scanNextFLOAT = memscan.scanNextFLOAT
scanNextDOUBLE = memscan.scanNextDOUBLE
freeAddressArray = memscan.freeAddressArray
