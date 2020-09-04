from MetaInfoInterface cimport *

cdef extern from "<OpenMS/METADATA/ID/ScoredProcessingResult.h>" namespace "OpenMS::IdentificationDataInternal":
    cdef cppclass ScoredProcessingResult(MetaInfoInterface) :
      # wrap-inherits:
      #  MetaInfoInterface
      ScoredProcessingResult(ScoredProcessingResult&) nogil except +  #wrap-ignore
