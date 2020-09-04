from MetaInfoInterface cimport *

cdef extern from "<OpenMS/METADATA/ID/ScoreType.h>" namespace "OpenMS::IdentificationDataInternal":
    cdef cppclass ScoreType(MetaInfoInterface) :
      # wrap-inherits:
      #  MetaInfoInterface
      ScoreType(ScoreType&) nogil except + #wrap-ignore