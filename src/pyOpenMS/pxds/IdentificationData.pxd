from MetaInfoInterface cimport *

cdef extern from "<OpenMS/METADATA/ID/IdentificationData.h>" namespace "OpenMS":
    cdef cppclass IdentificationData(MetaInfoInterface) :
        # wrap-inherits:
        #  MetaInfoInterface
        IdentificationData(IdentificationData&) nogil except + #wrap-ignore
