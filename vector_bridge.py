import ctypes
import os
import sys

class VectorSkipBridge:
    def __init__(self):
        if not os.path.exists("./libskip.so") and not os.path.exists("./libskip.dll"):
            if sys.platform.startswith("win"):
                os.system("gcc -shared -o libskip.dll skip_vector.c")
                lib_path = "./libskip.dll"
            else:
                os.system("gcc -shared -fPIC -o libskip.so skip_vector.c")
                lib_path = "./libskip.so"
        else:
            lib_path = "./libskip.dll" if sys.platform.startswith("win") else "./libskip.so"

        self.lib = ctypes.CDLL(lib_path)
        self.lib.init_skip_index.restype = ctypes.c_void_p
        self.lib.insert_vector.argtypes = [ctypes.c_void_p, ctypes.c_int, ctypes.POINTER(ctypes.c_float)]
        self.lib.query_nearest_vector.argtypes = [ctypes.c_void_p, ctypes.POINTER(ctypes.c_float)]
        self.lib.query_nearest_vector.restype = ctypes.c_int
        self.lib.free_skip_list.argtypes = [ctypes.c_void_p]
        
        self.idx_ptr = self.lib.init_skip_index()

    def index_document(self, doc_id: int, embedding: list):
        c_array = (ctypes.c_float * len(embedding))(*embedding)
        self.lib.insert_vector(self.idx_ptr, doc_id, c_array)

    def search_nearest(self, query_embedding: list) -> int:
        c_array = (ctypes.c_float * len(query_embedding))(*query_embedding)
        return self.lib.query_nearest_vector(self.idx_ptr, c_array)

    def __del__(self):
        if hasattr(self, 'lib') and self.idx_ptr:
            self.lib.free_skip_list(self.idx_ptr)
