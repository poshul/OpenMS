from Software cimport *

cdef extern from "<OpenMS/METADATA/ID/DataProcessingSoftware.h>" namespace "OpenMS::IdentificationDataInternal":
    cdef cppclass DataProcessingSoftware(Software) :
      # wrap-inherits:
      #  Software
      DataProcessingSoftware() nogil except + #wrap-ignore
      DataProcessingSoftware(String&) nogil except +
      DataProcessingSoftware(DataProcessingSoftware&) nogil except + #wrap-ignore