from MetaInfoInterface cimport *

cdef extern from "<OpenMS/METADATA/ID/DBSearchParam.h>" namespace "OpenMS::IdentificationDataInternal":
    cdef cppclass DBSearchParam(MetaInfoInterface) :
      # wrap-inherits:
      #  MetaInfoInterface
      DBSearchParam(DBSearchParam&) nogil except + #wrap-ignore