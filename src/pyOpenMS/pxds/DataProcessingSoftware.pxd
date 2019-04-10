from Types cimport *
from libcpp.vector cimport vector as libcpp_vector
from Software cimport *
from ScoreType cimport *
from IdentificationData cimport *

cdef extern from "<OpenMS/METADATA/ID/DataProcessingSoftware.h>" namespace "OpenMS::IdentificationDataInternal":

    cdef cppclass DataProcessingSoftware(Software) :
        #
        # wrap-doc:
        #     ADD PYTHON DOCUMENTATION HERE
        #
        # wrap-inherits:
        #  Software
        DataProcessingSoftware() nogil except +
        DataProcessingSoftware(DataProcessingSoftware) nogil except + #wrap-ignore
        libcpp_vector[ ScoreTypeRef ] assigned_scores
        DataProcessingSoftware(const String & name, const String & version, libcpp_vector[ ScoreTypeRef ] assigned_scores) nogil except +

