from ..utils.backend.dist.memory_extract import createAddressArray, get_pid_by_name,getHandleByPID, freeAddressArray
from ..utils.backend.dist.memory_extract import scanINT, scanFLOAT, scanDOUBLE, scanNextINT, scanNextDOUBLE, scanNextFLOAT, RPM_INT, RPM_DOUBLE, RPM_FLOAT, WPM_DOUBLE, WPM_FLOAT, WPM_INT
from ..errors.errors import MemoryAllocationFailureError, ReadingProcessMemoryFailureError, WritingProcessMemoryFailureError
import ctypes
import time
from collections import Counter
from typing import Union



class MemoryNextScan:
    def __init__(self, hProcess, address_array, result, dt):
        """
        Initialize a new instance of MemoryNextScan.

        Args:
        - hProcess (ctypes.wintypes.HANDLE): The handle to the process.
        - address_array (ctypes.POINTER)   : The pointer to the address array.
        - result (list)                    : The list of found addresses.
        - dt (str)                         : The data type of the scan.
        
        Returns:
        - MemoryNextScan() object
        """
        self.__hProcess = hProcess
        self.__address_array = address_array
        self.__result = result
        self.__datatype = dt
    def __enter__(self):
        """
        Enter the context manager.
        """
        pass

    def next_scan(self, data):
        """
        Reads the memory of the every found address again to check if they 
        match if the 'data' parameter. If a address does not match with 'data'
        parameter it will be removed from the array of found addresses.
        The new address which match the data can be accessed by this class by using
        MemoryNextScan().address_list property

        Args:
        - data (int or float): The new value to which the old value was changed from.

        Returns:
        - None, but MemoryNextScan() object with updated address_list

        Raises:
        - ValueError: If the type of 'data' is not which was used for new_scan.
        """
        if self.__datatype == "INTEGER":
            if isinstance(data, int):
                scanNextINT(self.__hProcess, self.__address_array, data)
            else:
                self.close()
                raise ValueError("Expected an integer!")
        elif self.__datatype == "FLOAT":
            if isinstance(data, float):
                scanNextFLOAT(self.__hProcess, self.__address_array, data)
            else:
                self.close()
                raise ValueError("Expected a float!")
        elif self.__datatype == "DOUBLE":
            if isinstance(data, (float)):
                scanNextDOUBLE(self.__hProcess, self.__address_array, data)
            else:
                self.close()
                raise ValueError("Expected a floating point value!")
        self.__update_result()

    def __update_result(self):
        """
        Updates the address_list by reading the memory of each found address.

        Args:
        - None

        Returns:
        - None

        This method clears the current list of found addresses and then iterates over each address
        in the address array. It reads the memory at each address, converts it to an integer,
        and appends the hexadecimal representation of the address to the result list.
        """
        _found_address = self.__address_array.contents
        self.__result.clear()
        for i in range(_found_address.size):
            char_ptr = _found_address.array[i]
            address_as_int = ctypes.cast(char_ptr, ctypes.c_void_p).value
            hex_address = hex(address_as_int) 
            self.__result.append(hex_address)


    def write_address_list(self, data):
        """
        Writes the given data to all addresses in address_list.

        Args:
        - data (int or float): The data to be written to the memory addresses.

        Returns:
        - None

        Raises:
        - WritingProcessMemoryFailureError: If the write operation fails for any address.

        This function iterates over all addresses in address_list. For each address,
        it writes the given data to the memory. If the write operation fails for any address,
        a WritingProcessMemoryFailureError is raised.
        """
        _found_address = self.__address_array.contents
        if self.__datatype == 'INTEGER':
            for i in range(_found_address.size):
                writing_address = _found_address.array[i]
                if not WPM_INT(self.__hProcess, writing_address, int(data)):
                    self.close()
                    raise WritingProcessMemoryFailureError('Failed to write memory to the process!')

        elif self.__datatype == "DOUBLE":
            for i in range(_found_address.size):
                writing_address = _found_address.array[i]
                if not WPM_DOUBLE(self.__hProcess, writing_address, ctypes.c_double(data)):
                    self.close()
                    raise WritingProcessMemoryFailureError('Failed to write memory to the process!')
        elif self.__datatype == "FLOAT":
            for i in range(_found_address.size):
                writing_address = _found_address.array[i]

                if not WPM_FLOAT(self.__hProcess, writing_address, float(data)):
                    self.close()
                    raise WritingProcessMemoryFailureError('Failed to write memory to the process!')

        
    
    def write_memory(self, address_index, data):
        """
        Writes the given data to the memory address at the specified index of address_list.

        Args:
        - address_index (int) : The index of the memory address in the address_list.
        - data (int, float)   : The data to be written to the memory address.

        Returns:
        - bool: True if the write operation is successful, False otherwise.

        Raises:
        - WritingProcessMemoryFailureError: If the write operation fails.

        This function writes the given data to the memory address at the specified index.
        It checks the data type of the given data and calls the appropriate write function
        (WPM_INT, WPM_FLOAT, or WPM_DOUBLE) based on the data type. If the write operation
        is successful, it returns True. Otherwise, it raises a WritingProcessMemoryFailureError.
        """
        _found_address = self.__address_array.contents
        writing_address = _found_address.array[address_index]
        if self.__datatype == "INTEGER":
            if WPM_INT(self.__hProcess, writing_address, data):
                return True
            else:
                self.close()
                raise WritingProcessMemoryFailureError('Failed to write memory to the process!')
        elif self.__datatype == "FLOAT":
            if WPM_FLOAT(self.__hProcess, writing_address, data):
                return True
            else:
                self.close()
                raise WritingProcessMemoryFailureError('Failed to write memory to the process!')
        elif self.__datatype == "DOUBLE":
            if WPM_DOUBLE(self.__hProcess, writing_address, ctypes.c_double(data)):
                return True
            else:
                self.close()
                raise WritingProcessMemoryFailureError('Failed to write memory to the process!')

    def read_memory(self, address_index: int) -> Union[int, float]:
        """
        Reads the memory at the specified index of the address_list and returns the value.

        Args:
        - address_index (int): The index of the memory address in the address_list.

        Returns:
        - Union[int, float]: The value read from the memory. If the data type is INTEGER, it returns an integer.
                             If the data type is FLOAT or DOUBLE, it returns a float.

        Raises:
        - ReadingProcessMemoryFailureError: If the read operation fails.

        This function reads the memory at the specified index of the address_list. It checks the data type of the addresses
        and calls the appropriate read function (RPM_INT, RPM_FLOAT, or RPM_DOUBLE) based on the data type. If the read operation
        is successful, it returns the value read from the memory. Otherwise, it raises a ReadingProcessMemoryFailureError.
        """
        _found_address = self.__address_array.contents
        reading_address = _found_address.array[address_index]

        if self.__datatype == "INTEGER":
            data = ctypes.c_int(0)
            if RPM_INT(self.__hProcess, reading_address, ctypes.byref(data)):
                return data.value
            else:
                self.close()
                raise ReadingProcessMemoryFailureError('Failed to read memory from the process!')
        elif self.__datatype == "FLOAT":
            data = ctypes.c_float(0)
            if RPM_FLOAT(self.__hProcess, reading_address, ctypes.byref(data)):
                return data.value
            else:
                self.close()
                raise ReadingProcessMemoryFailureError('Failed to read memory from the process!')
        elif self.__datatype == "DOUBLE":
            data = ctypes.c_double(0)
            if RPM_DOUBLE(self.__hProcess, reading_address, ctypes.byref(data)):
                return data.value
            else:
                self.close()
                raise ReadingProcessMemoryFailureError('Failed to read memory from the process!')

        
    @property
    def address_list(self):
        """
        Returns the list of memory addresses found during the last scan.

        Returns:
        - list: A list of hexadecimal strings representing the memory addresses.
        """
        return self.__result


    def monitor_scan(self, on_change ,min_addresses_to_exit_monitor=10,interval=10):
        """
        Monitors the memory addresses for changes and triggers a callback function.

        Args:
        - on_change (function)                         : A callback function that takes two parameters: a list of memory values i.e. address_list and a collections.Counter object.
                                                         This function should return a tuple (result, target), where result is a boolean indicating whether
                                                         to continue monitoring or stop, and target is the new value to scan for.
        - min_addresses_to_exit_monitor (int, optional): The minimum number of addresses required to exit the monitoring loop.
                                                          Default is 10.
        - interval (int, optional)                     : The time interval in seconds between each memory scan. Default is 10s.

        Returns:
        - None

        Raises:
        - ReadingProcessMemoryFailureError if the read operation fails

        This function continuously reads the memory values at the addresses found during the last scan,
        compares them with the previous values, and triggers the callback function 'on_change' when a change is detected.
        The monitoring loop continues until the callback function returns a result of True and until the number of addresses
        falls below 'min_addresses_to_exit_monitor'.
        """
        _found_address = self.__address_array.contents
        res = []
        prev = None
        while True:
            for i in range(_found_address.size):
                reading_address = _found_address.array[i]
                if self.__datatype == "INTEGER":
                    data = ctypes.c_int(0)
                    if RPM_INT(self.__hProcess, reading_address, ctypes.byref(data)):
                        res.append(data.value)
                    else:
                        self.close()
                        raise ReadingProcessMemoryFailureError('Failed to read memory from the process!')
                elif self.__datatype == "FLOAT":
                    data = ctypes.c_float(0)
                    if RPM_FLOAT(self.__hProcess, reading_address, ctypes.byref(data)):
                        res.append(data.value)
                    else:
                        self.close()
                        raise ReadingProcessMemoryFailureError('Failed to read memory from the process!')
                elif self.__datatype == "DOUBLE":
                    data = ctypes.c_double(0)
                    if RPM_DOUBLE(self.__hProcess, reading_address, ctypes.byref(data)):
                        res.append(data.value)
                    else:
                        self.close()
                        raise ReadingProcessMemoryFailureError('Failed to read memory from the process!')       

            ctr = Counter(res)
            if prev:
                if ctr != prev:
                    result, target = on_change(res, ctr)
                    if result:
                        self.next_scan(target)
                        if len(self.__result) < min_addresses_to_exit_monitor:
                            return
                    prev = ctr.copy()
            else:
                prev = ctr.copy()
            time.sleep(interval)
            res.clear()

    def __repr__(self) -> str:
        """
        Returns a string representation of the MemoryNextScan object.

        This method overrides the built-in __repr__ method in Python. It returns a string
        representation of the object, which is useful for debugging and logging purposes.

        Args:
        - None

        Returns:
        - str: A string representation of the MemoryNextScan object. The string contains
               the hexadecimal representation of the addresses found during the last scan.
        """
        return str((self.__result))
    
    def __str__(self) -> str:
        """
        Returns a string representation of the MemoryNextScan object.

        This method overrides the built-in __str__ method in Python. It returns a string
        representation of the object, which is useful for debugging and logging purposes.

        Args:
        - None

        Returns:
        - str: A string representation of the MemoryNextScan object. The string contains
               the hexadecimal representation of the addresses found during the last scan.
        """
        return str(self.__result)
    
    def close(self) -> str:
        """
        Closes the memory scanner and frees the allocated memory.

        This method is called when the MemoryNextScan object is no longer needed. It frees the memory
        allocated for the address array and deletes the object.

        Args:
        - None

        Returns:
        - None
        """
        freeAddressArray(self.__address_array)
        del self

    def __exit__(self):
        """
        This method is called when the context manager is exited. It closes the memory scanner
        and frees the allocated memory.

        Args:
        - None.

        Returns:
        - None
        """
        self.close()



class MemoryScanner(object):
    def __init__(self, processPID: int):
        """
        Initialize a new instance of MemoryScanner.

        This constructor opens a handle to the process with the given process ID,
        creates an address array, and initializes the necessary attributes.

        Parameters:
        - processPID (int): The process ID of the target process.

        Returns:
        - None

        Raises:
        - None

        """
        self.__pid = processPID
        self.__hProcess = getHandleByPID(self.__pid)
        self.__address_array = createAddressArray()

    def new_scan(self, data, isdouble=False) -> MemoryNextScan:
        """
        Initiates a new memory scan with the given data.

        Parameters:
        - data (int, float)        : The value to search for in the memory.
        - isdouble (bool, optional): A flag indicating whether the data is of type double. Default is False.

        Returns:
        - MemoryNextScan: An instance of MemoryNextScan containing the results of the scan.

        Raises:
        - MemoryAllocationFailureError    : If memory allocation for buffer data fails.
        - ReadingProcessMemoryFailureError: If reading memory from the process fails.
        """
        if isinstance(data, int):
            res = scanINT(self.__hProcess, data, self.__address_array)
            if res == -1:
                freeAddressArray(self.__address_array)
                raise MemoryAllocationFailureError('Failed to allocate memory for buffer data!')
            elif res == -2:
                freeAddressArray(self.__address_array)
                raise ReadingProcessMemoryFailureError('Failed to read memory from the process!')
            elif res == 1:
                _found_address = self.__address_array.contents
                result = []
                for i in range(_found_address.size):
                    char_ptr = _found_address.array[i]
                    address_as_int = ctypes.cast(char_ptr, ctypes.c_void_p).value
                    hex_address = hex(address_as_int) 
                    result.append(hex_address)
                return MemoryNextScan(self.__hProcess, self.__address_array, result, "INTEGER")
        elif isdouble:
            res = scanDOUBLE(self.__hProcess, data, self.__address_array)
            if res == -1:
                freeAddressArray(self.__address_array)
                raise MemoryAllocationFailureError('Failed to allocate memory for buffer data!')
            elif res == -2:
                freeAddressArray(self.__address_array)
                raise ReadingProcessMemoryFailureError('Failed to read memory from the process!')
            elif res == 1:
                _found_address = self.__address_array.contents
                result = []
                for i in range(_found_address.size):
                    char_ptr = _found_address.array[i]
                    address_as_int = ctypes.cast(char_ptr, ctypes.c_void_p).value
                    hex_address = hex(address_as_int) 
                    result.append(hex_address)
                return MemoryNextScan(self.__hProcess, self.__address_array, result, "DOUBLE")
        elif isinstance(data, float):
            res = scanFLOAT(self.__hProcess, data, self.__address_array)
            if res == -1:
                freeAddressArray(self.__address_array)
                raise MemoryAllocationFailureError('Failed to allocate memory for buffer data!')
            elif res == -2:
                freeAddressArray(self.__address_array)
                raise ReadingProcessMemoryFailureError('Failed to read memory from the process!')
            elif res == 1:
                _found_address = self.__address_array.contents
                result = []
                for i in range(_found_address.size):
                    char_ptr = _found_address.array[i]
                    address_as_int = ctypes.cast(char_ptr, ctypes.c_void_p).value
                    hex_address = hex(address_as_int) 
                    result.append(hex_address)
                return MemoryNextScan(self.__hProcess, self.__address_array, result, "FLOAT")


    
    @staticmethod
    def get_process_id_by_name(data: str) -> int:
        """
        Retrieves the process ID of a process by its name.

        Args:
        - data (str): The name of the process.

        Returns:
        - int: The process ID of the process. If the process is not found, returns 0.

        This function uses the `get_pid_by_name` function from the `pywinmemscanner.utils.backend.dist.memory_extract` module.
        It encodes the process name as UTF-8 and passes it to the `get_pid_by_name` function to retrieve the process ID.
        """
        return get_pid_by_name(data.encode('utf-8'))
