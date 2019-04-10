from Types cimport *
from libcpp cimport bool
from MetaData cimport *
from MetaInfoInterface cimport *
from CVTerm cimport *

cdef extern from "<OpenMS/METADATA/ID/ScoreType.h>" namespace "OpenMS::IdentificationDataInternal":

    cdef cppclass ScoreType(MetaInfoInterface) :
        #
        # wrap-doc:
        #     ADD PYTHON DOCUMENTATION HERE
        #
        # wrap-inherits:
        #  MetaInfoInterface
        ScoreType() nogil except +
        ScoreType(ScoreType) nogil except +
        CVTerm cv_term
        String name
        bool higher_better
        ScoreType(CVTerm & cv_term, bool higher_better) nogil except +
        ScoreType(const String & name, bool higher_better) nogil except +
        bool operator<(ScoreType & other) nogil except +
        bool operator==(ScoreType & other) nogil except +

