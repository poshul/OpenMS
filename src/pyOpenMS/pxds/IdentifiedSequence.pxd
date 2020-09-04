from AASequence cimport *
from NASequence cimport *
from ScoredProcessingResult cimport *

cdef extern from "<OpenMS/METADATA/ID/IdentifiedSequence.h>" namespace "OpenMS::IdentificationDataInternal":
    cdef cppclass IdentifiedSequence[T](ScoredProcessingResult) :
      # wrap-inherits:
      #  ScoredProcessingResult
      # wrap-instances:
      #   IdentifiedPeptide := IdentifiedSequence[ AASequence ]
      #   IdentifiedOligo   := IdentifiedSequence[ NASequence ]
      IdentifiedSequence(IdentifiedSequence&) nogil except + #wrap-ignore