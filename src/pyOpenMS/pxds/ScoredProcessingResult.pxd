from Types cimport *
from libcpp cimport bool
from libcpp.map cimport map as libcpp_map
from libcpp.pair cimport pair as libcpp_pair
from AppliedProcessingStep cimport *

cdef extern from "<OpenMS/METADATA/ID/ScoredProcessingResult.h>" namespace "OpenMS::IdentificationDataInternal":

    cdef cppclass ScoredProcessingResult(MetaInfoInterface) :
        #
        # wrap-doc:
        #     ADD PYTHON DOCUMENTATION HERE
        #
        # wrap-inherits:
        #  MetaInfoInterface
        ScoredProcessingResult() nogil except +
        ScoredProcessingResult(ScoredProcessingResult) nogil except + #wrap-ignore
        AppliedProcessingSteps steps_and_scores
        # NAMESPACE # AppliedProcessingSteps::nth_index[ 1 ]::type getStepsAndScoresByStep() nogil except +
        # NAMESPACE # AppliedProcessingSteps::nth_index[ 1 ]::type getStepsAndScoresByStep() nogil except +
        void addProcessingStep(AppliedProcessingStep & step) nogil except +
        void addProcessingStep(ProcessingStepRef step_ref, libcpp_map[ ScoreTypeRef, double ] & scores) nogil except +
        # NAMESPACE # void addScore(ScoreTypeRef score_type, double score, boost::optional[ ProcessingStepRef ] & processing_step_opt) nogil except +
        #ScoredProcessingResult operator+=(ScoredProcessingResult & other) nogil except +
        libcpp_pair[ double, bool ] getScore(ScoreTypeRef score_ref) nogil except +
        # NAMESPACE # libcpp_pair[ double, bool ] getScore(ScoreTypeRef score_ref, boost::optional[ ProcessingStepRef ] processing_step_opt) nogil except +
        # NAMESPACE # std::tuple[ double, boost::optional[ ProcessingStepRef ], bool ] getScoreAndStep(ScoreTypeRef score_ref) nogil except +