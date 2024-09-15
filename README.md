# PyWinMemScanner

**PyWinMemScanner** is a powerful Python library designed for scanning and manipulating the memory of external processes on Windows systems. It provides an intuitive interface for developers looking to interact with the memory space of other applications, enabling advanced memory search, modification, and monitoring capabilities.

With **PyWinMemScanner**, you can easily access and scan a target process’s memory to locate specific values or addresses, manipulate these values, or monitor changes in real-time. This is particularly useful for tasks like reverse engineering, debugging, and hacking tools. By leveraging Windows APIs through the use of a DLL and Python's `ctypes`, this library offers direct access to the internal workings of processes.

## Key Features

- **Process Memory Scanning**: Scan the memory of target processes to find specific values or data patterns.
- **Memory Value Manipulation**: Modify memory values of the target process directly, with built-in support for various data types like integers, floats, and more.
- **Memory Monitoring**: Continuously monitor memory regions of a process and react to changes.
- **Intuitive API**: Provides an easy-to-use Python API for interfacing with low-level system functions.
- **Cross-compatibility**: Designed to work seamlessly with Python, making it accessible for all Python developers interested in system-level programming.

## Installation

You can easily install **PyWinMemScanner** using pip with the following command:

```bash
pip install pywinmemscanner
```
```python
from pywinmemscanner import MemoryScanner

# Initialize the scanner with the target process
pid = MemoryScanner.get_process_id_by_name(input('Enter process name :')) # Returns int
ms = memscan.MemoryScanner(pid) # PID can be direcly passed in integer format

# Scan for a value in memory
mss = ms.new_scan(your_value)

# Printing the addresses found
print(mss) # Give's the list in string format
# To obtain the actual list
print(mss.address_list)
print(len(mss.address_list))

# If the len(mss.address_list) is too high, it can be reduced by changing value in real process and passing that value in mss.next_scan(updated_value)
mss.next_scan(updated_value)
print(mss) # prints the updated memory addresses list

#Once the len(mss.address_list) is low enough (e.g. under 10) write_address_list(your_target_data) to write into the memory of entire address_list at once.
mss.write_address_list(target_data)
mss.close() # Closing is important to free the dynamically allocated memory.
# This should update the value in the process. 

```
## Compatibility

This library is Windows-specific and requires access to the target process’s memory. Ensure that your Python environment has appropriate permissions and that you are running your script with sufficient privileges (e.g., Administrator).

## Why PyWinMemScanner?

- **Efficient Memory Scanning**: The library allows you to quickly locate values in large memory regions without needing to write complicated, low-level code.
- **Built-in Support for Various Data Types**: It includes handling for common data types like integers, floats and double making memory interaction seamless.
- **Real-time Memory Monitoring**: For applications like game mods or cheat engines, real-time memory monitoring can be essential, and this library handles it with ease.
- **Extendable and Open-Source**: The codebase is written in Python with an underlying C/C++ DLL, and users can easily extend its functionality for their specific needs.

**PyWinMemScanner** simplifies memory scanning tasks for developers, providing the tools to interact with a process's memory easily. This library is perfect for both beginners and experienced developers working on system-level applications or reverse engineering projects.

