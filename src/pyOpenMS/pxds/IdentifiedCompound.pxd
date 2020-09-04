from ScoredProcessingResult cimport *

cdef extern from "<OpenMS/METADATA/ID/IdentifiedCompound.h>" namespace "OpenMS::IdentificationDataInternal":
    cdef cppclass IdentifiedCompound(ScoredProcessingResult) :
      # wrap-inherits:
      #  ScoredProcessingResult
      IdentifiedCompound(IdentifiedCompound&) nogil except + #wrap-ignore
