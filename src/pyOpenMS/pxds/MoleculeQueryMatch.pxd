from ScoredProcessingResult cimport *

cdef extern from "<OpenMS/METADATA/ID/MoleculeQueryMatch.h>" namespace "OpenMS::IdentificationDataInternal":
    cdef cppclass MoleculeQueryMatch(ScoredProcessingResult) :
     # wrap-inherits:
     #  ScoredProcessingResult
     MoleculeQueryMatch(MoleculeQueryMatch&) nogil except + #wrap-ignore