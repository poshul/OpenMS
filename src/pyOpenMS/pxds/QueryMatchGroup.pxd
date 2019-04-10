from Types cimport *
from libcpp cimport bool
from MoleculeQueryMatch cimport *

cdef extern from "<OpenMS/METADATA/ID/QueryMatchGroup.h>" namespace "OpenMS::IdentificationDataInternal":

    cdef cppclass QueryMatchGroup(ScoredProcessingResult) :
        #
        # wrap-doc:
        #     ADD PYTHON DOCUMENTATION HERE
        #
        # wrap-inherits:
        #  ScoredProcessingResult
        QueryMatchGroup() nogil except +
        QueryMatchGroup(QueryMatchGroup) nogil except + #wrap-ignore
        libcpp_set[ QueryMatchRef ] query_match_refs
        bool allSameMolecule() nogil except +
        bool allSameQuery() nogil except +
        bool operator==(QueryMatchGroup rhs) nogil except +
        bool operator!=(QueryMatchGroup & rhs) nogil except +

