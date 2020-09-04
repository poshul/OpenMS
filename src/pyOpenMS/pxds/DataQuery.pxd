from MetaInfoInterface cimport *

cdef extern from "<OpenMS/METADATA/ID/DataQuery.h>" namespace "OpenMS::IdentificationDataInternal":
    cdef cppclass DataQuery(MetaInfoInterface) :
      # wrap-inherits:
      #  MetaInfoInterface
      DataQuery(DataQuery&) nogil except +  #wrap-ignore