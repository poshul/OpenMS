from ScoredProcessingResult cimport *


cdef extern from "<OpenMS/METADATA/ID/QueryMatchGroup.h>" namespace "OpenMS::IdentificationDataInternal":
    cdef cppclass QueryMatchGroup(ScoredProcessingResult) :
     # wrap-inherits:
     #  ScoredProcessingResult
     QueryMatchGroup(QueryMatchGroup&) nogil except + #wrap-ignore