from Types cimport *
from libcpp cimport bool
from libcpp.pair cimport pair as libcpp_pair
from libcpp.vector cimport vector as libcpp_vector
from DataProcessingStep cimport *
from DataQuery cimport *
from DBSearchParam cimport *
from IdentifiedCompound cimport *
from IdentifiedSequence cimport *
from MetaData cimport *
from MoleculeParentMatch cimport *
from MoleculeQueryMatch cimport *
from ParentMolecule cimport *
from ParentMoleculeGroup cimport *
from QueryMatchGroup cimport *
from ScoreType cimport *
from MzTab cimport *

cdef extern from "<OpenMS/METADATA/ID/IdentificationData.h>" namespace "OpenMS":

    cdef cppclass IdentificationData(MetaInfoInterface) :
        #
        # wrap-doc:
        #     ADD PYTHON DOCUMENTATION HERE
        #
        # wrap-inherits:
        #  MetaInfoInterface
        IdentificationData() nogil except +
        IdentificationData(IdentificationData) nogil except +
        InputFileRef registerInputFile(const String & file_) nogil except +
        ProcessingSoftwareRef registerDataProcessingSoftware(DataProcessingSoftware & software) nogil except +
        SearchParamRef registerDBSearchParam(DBSearchParam & param) nogil except +
        ProcessingStepRef registerDataProcessingStep(DataProcessingStep & step) nogil except +
        ProcessingStepRef registerDataProcessingStep(DataProcessingStep & step, SearchParamRef search_ref) nogil except +
        ScoreTypeRef registerScoreType(ScoreType & score) nogil except +
        DataQueryRef registerDataQuery(DataQuery & query) nogil except +
        ParentMoleculeRef registerParentMolecule(ParentMolecule & parent) nogil except +
        void registerParentMoleculeGrouping(ParentMoleculeGrouping & grouping) nogil except +
        IdentifiedPeptideRef registerIdentifiedPeptide(IdentifiedPeptide & peptide) nogil except +
        IdentifiedCompoundRef registerIdentifiedCompound(IdentifiedCompound & compound) nogil except +
        IdentifiedOligoRef registerIdentifiedOligo(IdentifiedOligo & oligo) nogil except +
        QueryMatchRef registerMoleculeQueryMatch(MoleculeQueryMatch & match) nogil except +
        MatchGroupRef registerQueryMatchGroup(QueryMatchGroup & group) nogil except +
        InputFiles getInputFiles() nogil except +
        DataProcessingSoftwares getDataProcessingSoftwares() nogil except +
        DataProcessingSteps getDataProcessingSteps() nogil except +
        DBSearchParams getDBSearchParams() nogil except +
        DBSearchSteps getDBSearchSteps() nogil except +
        ScoreTypes getScoreTypes() nogil except +
        DataQueries getDataQueries() nogil except +
        ParentMolecules getParentMolecules() nogil except +
        ParentMoleculeGroupings getParentMoleculeGroupings() nogil except +
        IdentifiedPeptides getIdentifiedPeptides() nogil except +
        IdentifiedCompounds getIdentifiedCompounds() nogil except +
        IdentifiedOligos getIdentifiedOligos() nogil except +
        MoleculeQueryMatches getMoleculeQueryMatches() nogil except +
        QueryMatchGroups getQueryMatchGroups() nogil except +
        void addScore(QueryMatchRef match_ref, ScoreTypeRef score_ref, double value) nogil except +
        void setCurrentProcessingStep(ProcessingStepRef step_ref) nogil except +
        ProcessingStepRef getCurrentProcessingStep() nogil except +
        void clearCurrentProcessingStep() nogil except +
        libcpp_vector[ QueryMatchRef ] getBestMatchPerQuery(ScoreTypeRef score_ref) nogil except +
        libcpp_pair[ ScoreTypeRef, bool ] findScoreType(const String & score_name) nogil except +
        void calculateCoverages(bool check_molecule_length) nogil except +
        void cleanup(bool require_query_match, bool require_identified_sequence, bool require_parent_match, bool require_parent_group, bool require_match_group) nogil except +
        # TODO: static # bool isBetterScore(double first, double second, bool higher_better) nogil except +


# COMMENT: wrap static methods
cdef extern from "<OpenMS/METADATA/ID/IdentificationData.h>" namespace "OpenMS::IdentificationData":

        bool isBetterScore(double first, double second, bool higher_better) nogil except + # wrap-attach:IdentificationData

