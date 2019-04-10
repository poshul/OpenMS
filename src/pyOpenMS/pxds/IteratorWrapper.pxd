from Types cimport *
from libcpp cimport bool

cdef extern from "<OpenMS/METADATA/ID/MetaData.h>" namespace "OpenMS::IdentificationDataInternal":

    cdef cppclass IteratorWrapper[Iterator](Iterator):
        #
        # wrap-doc:
        #     ADD PYTHON DOCUMENTATION HERE
        #
        # wrap-inherits:
        #  Iterator
        IteratorWrapper() nogil except +
        IteratorWrapper(IteratorWrapper) nogil except + #wrap-ignore
        IteratorWrapper(Iterator & it) nogil except +
        bool operator<(IteratorWrapper & other) nogil except +
        operator uintptr_t() nogil except +

