from Types cimport *
from ScoredProcessingResult cimport *

cdef extern from "<OpenMS/METADATA/ID/IdentifiedCompound.h>" namespace "OpenMS::IdentificationDataInternal":

    cdef cppclass IdentifiedCompound(ScoredProcessingResult) :
        #
        # wrap-doc:
        #     ADD PYTHON DOCUMENTATION HERE
        #
        # wrap-inherits:
        #  ScoredProcessingResult
        IdentifiedCompound() nogil except +
        IdentifiedCompound(IdentifiedCompound) nogil except +
        String identifier
        EmpiricalFormula formula
        String name
        String smile
        String inchi
        IdentifiedCompound(const String & identifier, EmpiricalFormula & formula, const String & name, const String & smile, const String & inchi, AppliedProcessingSteps & steps_and_scores) nogil except +

