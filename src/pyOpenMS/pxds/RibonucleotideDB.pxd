from Types cimport *
from libcpp.pair cimport pair as libcpp_pair
from libcpp.string cimport string as libcpp_string
from Ribonucleotide cimport *
from String cimport *

cdef extern from "<OpenMS/CHEMISTRY/RibonucleotideDB.h>" namespace "OpenMS":

    cdef cppclass RibonucleotideDB "OpenMS::RibonucleotideDB":
        #
        # wrap-doc:
        #     ADD PYTHON DOCUMENTATION HERE
        #
        RibonucleotideDB() nogil except +
        RibonucleotideDB(RibonucleotideDB) nogil except +
        # TODO: static # # POINTER # RibonucleotideDB * getInstance() nogil except +
        ConstIterator begin() nogil except +
        ConstIterator end() nogil except +
        ConstRibonucleotidePtr getRibonucleotide(libcpp_string & code) nogil except +
        ConstRibonucleotidePtr getRibonucleotidePrefix(libcpp_string & seq) nogil except +
        libcpp_pair[ ConstRibonucleotidePtr, ConstRibonucleotidePtr ] getRibonucleotideAlternatives(libcpp_string & code) nogil except +


# COMMENT: wrap static methods
cdef extern from "<OpenMS/CHEMISTRY/RibonucleotideDB.h>" namespace "OpenMS::RibonucleotideDB":

        # POINTER # RibonucleotideDB * getInstance() nogil except + # wrap-attach:RibonucleotideDB

