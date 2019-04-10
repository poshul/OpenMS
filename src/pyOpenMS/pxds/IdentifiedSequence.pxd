from Types cimport *
from libcpp cimport bool
from AASequence cimport *
from NASequence cimport *
from MoleculeParentMatch cimport *
from ScoredProcessingResult cimport *

cdef extern from "<OpenMS/METADATA/ID/IdentifiedSequence.h>" namespace "OpenMS::IdentificationDataInternal":

    cdef cppclass IdentifiedSequence[](ScoredProcessingResult):
        #
        # wrap-doc:
        #     ADD PYTHON DOCUMENTATION HERE
        #
        # wrap-inherits:
        #  ScoredProcessingResult
        IdentifiedSequence() nogil except +
        IdentifiedSequence(IdentifiedSequence) nogil except +
        SeqType sequence
        ParentMatches parent_matches
        IdentifiedSequence(SeqType & sequence, ParentMatches & parent_matches, AppliedProcessingSteps & steps_and_scores) nogil except +
        IdentifiedSequence operator+=(IdentifiedSequence & other) nogil except +
        bool allParentsAreDecoys() nogil except +

