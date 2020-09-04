from Types cimport *
from String cimport *
from libcpp.set cimport set as libcpp_set


cdef extern from "<OpenMS/METADATA/ID/MetaData.h>" namespace "OpenMS::IdentificationDataInternal":
    ctypedef libcpp_set[String] InputFiles
    cdef enum MoleculeType "OpenMS::IdentificationDataInternal::MoleculeType":
        PROTEIN
        COMPOUND
        RNA
        SIZE_OF_MOLECULETYPE