from MetaInfoInterface cimport *
from DataProcessingSoftware cimport *

cdef extern from "<OpenMS/METADATA/ID/DataProcessingStep.h>" namespace "OpenMS::IdentificationDataInternal":
    cdef cppclass DataProcessingStep(MetaInfoInterface) :
      # wrap-inherits:
      #  MetaInfoInterface
      DataProcessingStep() nogil except + #wrap-pass_constructor
      DataProcessingStep(DataProcessingSoftware) nogil except + #wrap-ignore
      DataProcessingStep(DataProcessingStep&) nogil except + #wrap-ignore
