class BaseException(Exception):
    pass

class MemoryAllocationFailureError(BaseException):
    pass

class ReadingProcessMemoryFailureError(BaseException):
    pass
class WritingProcessMemoryFailureError(BaseException):
    pass