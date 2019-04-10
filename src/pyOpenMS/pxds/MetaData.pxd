from Types cimport *
from libcpp cimport bool

cdef extern from "<OpenMS/METADATA/ID/MetaData.h>" namespace "OpenMS::IdentificationDataInternal":
    cdef cppclass IteratorWrapper[Iterator]:
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
        #FIXME: Operator overloading not supported
        #bool operator uintptr_t() nogil except +

    enum MoleculeType:
         PROTEIN = 0,
         COMPOUND = 1,
         RNA = 2,
         SIZE_OF_MOLECULETYPE = 3

    enum MassType:
         MONOISOTOPIC = 0,
         AVERAGE = 1,
         SIZE_OF_MASSTYPE = 2