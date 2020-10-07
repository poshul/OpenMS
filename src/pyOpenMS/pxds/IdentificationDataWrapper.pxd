from Types cimport *
from libcpp cimport bool
from libcpp.pair cimport pair as libcpp_pair
from libcpp.vector cimport vector as libcpp_vector
from libcpp.set cimport set as libcpp_set
from MetaInfoInterface cimport *
from Software cimport *
from AASequence cimport *
from NASequence cimport *
from DataProcessing cimport *
from DateTime cimport *
from IdentificationData cimport *
from QueryMatchGroup cimport *
from MoleculeQueryMatch cimport *
from IdentifiedCompound cimport *
from IdentifiedSequence cimport *
from ParentMolecule cimport *
from ScoredProcessingResult cimport *
from DataQuery cimport *
from ScoreType cimport *
from DataProcessingStep cimport *
from DBSearchParam cimport *
from DataProcessingSoftware cimport *
from MetaData cimport *

cdef extern from "<OpenMS/METADATA/ID/IdentificationDataWrapper.h>" namespace "OpenMS":
    cdef cppclass IdentificationDataWrapper(IdentificationData) :
        # wrap-inherits:
        #  MetaInfoInterface
        IdentificationDataWrapper() nogil except +
        int pythonRegisterInputFile(String & file) nogil except +
        int pythonRegisterDataProcessingSoftware(DataProcessingSoftware& software) nogil except +
        int pythonRegisterDBSearchParam(DBSearchParam& param) nogil except +
        int pythonRegisterDataProcessingStep(DataProcessingStep& step) nogil except +
        int pythonRegisterDataProcessingStep(DataProcessingStep& step, int search_ref) nogil except +
        int pythonRegisterScoreType(ScoreType& score) nogil except +
        int pythonRegisterDataQuery(DataQuery& query) nogil except +
        int pythonRegisterParentMolecule(ParentMolecule& parent) nogil except +
        int pythonRegisterIdentifiedCompound(IdentifiedCompound& compound) nogil except +
        #int pythonRegisterIdentifiedPeptide(IdentifiedPeptide& peptide) nogil except +
        #int pythonRegisterIdentifiedOligo(IdentifiedOligo& oligo) nogil except +
        int pythonRegisterMoleculeQueryMatch(MoleculeQueryMatch& match) nogil except +
        int pythonRegisterQueryMatchGroup(QueryMatchGroup& group) nogil except +
        void pythonAddScore(int match_ref, int score_ref, double value) nogil except +
        void pythonSetCurrentProcessingStep(int step_ref) nogil except +
        int pythonGetCurrentProcessingStep() nogil except +
        libcpp_vector[int] pythonGetBestMatchPerQuery(int score_ref) nogil except + #do I add const here?
        libcpp_pair[int,bool] pythonFindScoreType(String& score_name) nogil except + #const?
        DataProcessingSoftware pythonNewDataProcessingSoftware(String& name, String& version, libcpp_vector[int] assigned_scores) nogil except +
        DataProcessingStep pythonNewDataProcessingStep(int software_ref, libcpp_vector[int]& input_file_refs, libcpp_vector[String]& primary_files, DateTime& date_time, libcpp_set[ProcessingAction] actions) nogil except +
        DataQuery pythonNewDataQuery(String& data_id, int input_file_opt, double rt, double mz) nogil except +
        ParentMolecule pythonNewParentMolecule(String& accession, MoleculeType molecule_type, String& sequence, String& description, double coverage, bool is_decoy) nogil except +
        #IdentifiedPeptide pythonNewIdentifiedPeptide(AASequence seq) nogil except +
        #IdentifiedOligo pythonNewIdentifiedOligo(NASequence seq) nogil except +
        IdentifiedCompound pythonNewIdentifiedCompound(String& identifier, EmpiricalFormula& formula, String& name, String& smile, String& inchi) nogil except +
        MoleculeQueryMatch pythonNewMoleculeQueryMatch(int identified_molecule_ref, int data_query_ref, int m_charge) nogil except +
        QueryMatchGroup pythonNewQueryMatchGroup(libcpp_vector[int] qmr) nogil except +