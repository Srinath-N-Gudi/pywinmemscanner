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

You can easily install **PyWinMemScanner** using PyPI with the following command:

```bash
pip install pywinmemscanner
```
```python
from pywinmemscanner import MemoryScanner

# Initialize the scanner with the target process
pid = MemoryScanner.get_process_id_by_name(input('Enter process name :')) # Returns int
ms = MemoryScanner(pid) # PID can be direcly passed in integer format

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

# This should update the value in the process. 
```

### Example: Memory Manipulation Tool (Inspired by Tools like Cheat Engine)
```python

import argparse as ap
from pywinmemscanner import MemoryScanner # Importing the Library.
import sys
FILENAME = None
PID = None

NOFADD = 10 # min_addresses_to_exit_monitor parameter
INTERVAL = 10 # interval parameter

## Parsing for arugments passed
parser = ap.ArgumentParser(description="Quick Memory Manipulation Tool")

parser.add_argument("-fn", "--filename",action='store', type=str, help="Name of the file to process")
parser.add_argument("-p", "--pid", action='store', type=int, help="PID of the Process")
parser.add_argument("-m", "--monitor", action='store_true', help="Automatically monitors changes in memory")
parser.add_argument("-s", "--scan", action='store', help="A value to perform first memory scan")
parser.add_argument("-i", "--int", action='store_true', help="Set the value of scan datatype to be int")
parser.add_argument("-f", "--float", action='store_true', help="Set the value of scan datatype to be float")
parser.add_argument("-d", "--double", action='store_true', help="Set the value of scan datatype to be double")
parser.add_argument('-n', '--nofadd', action='store', type=int, help="Number of min address before exiting monitor mode")
parser.add_argument('-t', '--interval', action='store', type=int, help='Inteveral between each scan')

args = parser.parse_args()

if args.filename == None and args.pid == None:
    print("Please provide either a filename or a PID.")
    sys.exit(1)

if args.int == False and args.float == False and args.double == False :
    print("Please specify the data type to scan for.")
    sys.exit(1)

if (args.int and args.float) or (args.float and args.double) or (args.int and args.double):
    print("Please select only one data type to scan for.")
    sys.exit(1)

if args.scan == None:
    print("Please provide a value to scan for.")
    sys.exit(1)

if args.filename:
    FILENAME = args.filename
    PID = MemoryScanner.get_process_id_by_name(FILENAME)
    if PID == 0:
        print(f"Process '{FILENAME}' not found.")
        sys.exit(1)

elif args.pid:
    PID = args.pid


def on_change(res, counter): # Calls made by pywinmemscanner.MemoryNextScan class
    """
    Handle memory change detection and user interaction.

    When a change is detected during the memory scan, this function prompts 
    the user to confirm if the change was intentional (The memory can be changed by the software themselves to work properly | Which was not changd by the user). If so, it allows the user 
    to select which value was changed.

    Args:
    - res (list): The result is nothing but mss.address_list or the address_list stored.
    - counter (collections.Counter): A Counter object of all the values read from the memory (with frequency).

    Returns:
    tuple:
        - (bool): True if the user confirms the change was intentional, False otherwise. (If the len(mss.address_list) < min_addresses_to_exit_monitor => Exits Monitoring)
        - (any): The selected value that was changed by the user, or None if no change was confirmed.

    Example:
    If a change is detected, the user is asked if they caused it. If yes, the user 
    selects from a list of values to identify which one was changed.
    """

    yn = input("Change in memory detected!. Was it changed by you ? ")
    if yn == 'y':
        print("Which one of these was it changed to ? ")
        for value, key in enumerate(counter.keys()):
            print(f"{value+1}. {key}")
        try: 
            choice = int(input())
        except:
            print("Excepted a number shows in above table ? Exiting...")
            sys.exit(1)
        return True, list(counter.keys())[choice-1]

    else:
        return False, None

ms = MemoryScanner(PID)
mss = None
try: 
    if args.int: 
        mss = ms.new_scan(int(args.scan))
    if args.float: 
        mss = ms.new_scan(float(args.scan))
    if args.double: 
        mss = ms.new_scan(float(args.scan), isdouble=True)
except Exception as e:
    print(f"Error occured : {e}")
    mss.close() ### Free dynamically allocated memory by backend of pywinmemscanner.
    

if args.nofadd:
    NOFADD = args.nofadd
if args.interval:
    INTERVAL = args.interval
if args.monitor:
    print("Monitoring changes in memory...")
    try:
        mss.monitor_scan(on_change=on_change, min_addresses_to_exit_monitor=NOFADD, interval=INTERVAL)
    except Exception as e:
        print(f"Error occured : {e}")
        mss.close() ### Free dynamically allocated memory by backend of pywinmemscanner. 
        

        
if (len(mss.address_list) == 0):
    print("No such value found in memory!")
    mss.close()
    sys.exit(0)
while 1:
    value = input("Enter the value you want to write  : ")
    if value.lower() == 'exit' or value.lower() == 'q' or value.lower() == 'quit':
        break
    try: 
        if args.int: 
            mss.write_address_list(int(value))
        elif args.float: 
            mss.write_address_list(float(value))
        elif args.double: 
            mss.write_address_list(float(value))
    except Exception as e:
        print(f"Error occured : {e}")
        mss.close() ### Free dynamically allocated memory by backend of pywinmemscanner.



mss.close() #Important to free the memory


"""
mss.monitor_scan(on_change=on_change, min_addresses_to_exit_monitor=10, interval=10)

The above monitor_scan function can be used to monitor changes in memory, on_change function will be triggered once there were changes.
pywinmemscanner requires the on_change function to return a tuple of (BOOL, TARGET), BOOL: If the address list should be updated.(If False, you can return None for the second one)(If True, Target is required i.e. which value to scan_next(value) for it will automatically update the address_list if True is returned and the monitor_scan() function will stop when the len(mss.address_list) < min_address_to_exit_monitor) (Interval is the rest time between each scan).
"""

```



## Compatibility

This library is Windows-specific and requires access to the target process’s memory. Ensure that your Python environment has appropriate permissions and that you are running your script with sufficient privileges (e.g., Administrator).

## Why PyWinMemScanner?

- **Efficient Memory Scanning**: The library allows you to quickly locate values in large memory regions without needing to write complicated, low-level code.
- **Built-in Support for Various Data Types**: It includes handling for common data types like integers, floats and double making memory interaction seamless.
- **Real-time Memory Monitoring**: For applications like game mods or cheat engines, real-time memory monitoring can be essential, and this library handles it with ease.
- **Extendable and Open-Source**: The codebase is written in Python with an underlying C/C++ DLL, and users can easily extend its functionality for their specific needs.

**PyWinMemScanner** simplifies memory scanning tasks for developers, providing the tools to interact with a process's memory easily. This library is perfect for both beginners and experienced developers working on system-level applications or reverse engineering projects.

### ⚠️ Important Disclaimer

This tool is intended for **educational** and **ethical purposes only**. By using this software, you agree to abide by all relevant laws and regulations. The author is **not responsible** for any illegal activity, misuse, or violation of terms of service that may result from using this tool. Please ensure that you have proper authorization before manipulating any software or process.

Use responsibly and ethically.

