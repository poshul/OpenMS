from Types cimport *
from libcpp cimport bool
from libcpp.map cimport map as libcpp_map
from libcpp.pair cimport pair as libcpp_pair
from libcpp.vector cimport vector as libcpp_vector
from DataProcessingStep cimport *
from ScoreType cimport *

cdef extern from "<OpenMS/METADATA/ID/AppliedProcessingStep.h>" namespace "OpenMS::IdentificationDataInternal":

    cdef cppclass AppliedProcessingStep "OpenMS::IdentificationDataInternal::AppliedProcessingStep":
        #
        # wrap-doc:
        #     ADD PYTHON DOCUMENTATION HERE
        #
        AppliedProcessingStep() nogil except +
        AppliedProcessingStep(AppliedProcessingStep) nogil except + #wrap-ignore
        # NAMESPACE # boost::optional[ ProcessingStepRef ] processing_step_opt
        libcpp_map[ ScoreTypeRef, double ] scores
        # NAMESPACE #  AppliedProcessingStep(boost::optional[ ProcessingStepRef ] & processing_step_opt, libcpp_map[ ScoreTypeRef, double ] & scores) nogil except +
        bool operator==(AppliedProcessingStep & other) nogil except +
        libcpp_vector[ libcpp_pair[ ScoreTypeRef, double ] ] getScoresInOrder() nogil except +

