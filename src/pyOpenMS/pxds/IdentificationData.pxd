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

cdef extern from "<OpenMS/METADATA/ID/MetaData.h>" namespace "OpenMS::IdentificationDataInternal":
    ctypedef libcpp_set[String] InputFiles
    cdef enum MoleculeType "OpenMS::IdentificationDataInternal::MoleculeType":
        PROTEIN
        COMPOUND
        RNA
        SIZE_OF_MOLECULETYPE

cdef extern from "<OpenMS/METADATA/ID/DataProcessingSoftware.h>" namespace "OpenMS::IdentificationDataInternal":
    cdef cppclass DataProcessingSoftware(Software) :
      # wrap-inherits:
      #  Software
      DataProcessingSoftware(DataProcessingSoftware&) nogil except + #wrap-ignore

cdef extern from "<OpenMS/METADATA/ID/DBSearchParam.h>" namespace "OpenMS::IdentificationDataInternal":
    cdef cppclass DBSearchParam(MetaInfoInterface) :
      # wrap-inherits:
      #  MetaInfoInterface
      DBSearchParam(DBSearchParam&) nogil except + #wrap-ignore

cdef extern from "<OpenMS/METADATA/ID/DataProcessingStep.h>" namespace "OpenMS::IdentificationDataInternal":
    cdef cppclass DataProcessingStep(MetaInfoInterface) :
      # wrap-inherits:
      #  MetaInfoInterface
      #DataProcessingStep() nogil except + #wrap-pass_constructor
      DataProcessingStep(DataProcessingStep&) nogil except + #wrap-ignore

cdef extern from "<OpenMS/METADATA/ID/ScoreType.h>" namespace "OpenMS::IdentificationDataInternal":
    cdef cppclass ScoreType(MetaInfoInterface) :
      # wrap-inherits:
      #  MetaInfoInterface
      ScoreType(ScoreType&) nogil except + #wrap-ignore
      

cdef extern from "<OpenMS/METADATA/ID/DataQuery.h>" namespace "OpenMS::IdentificationDataInternal":
    cdef cppclass DataQuery(MetaInfoInterface) :
      # wrap-inherits:
      #  MetaInfoInterface
      DataQuery(DataQuery&) nogil except +  #wrap-ignore

cdef extern from "<OpenMS/METADATA/ID/ScoredProcessingResult.h>" namespace "OpenMS::IdentificationDataInternal":
    cdef cppclass ScoredProcessingResult(MetaInfoInterface) :
      # wrap-inherits:
      #  MetaInfoInterface
      ScoredProcessingResult(ScoredProcessingResult&) nogil except +  #wrap-ignore

cdef extern from "<OpenMS/METADATA/ID/ParentMolecule.h>" namespace "OpenMS::IdentificationDataInternal":
    cdef cppclass ParentMolecule(ScoredProcessingResult) :
      # wrap-inherits:
      #  ScoredProcessingResult
      ParentMolecule(ParentMolecule&) nogil except +  #wrap-ignore

cdef extern from "<OpenMS/METADATA/ID/IdentifiedSequence.h>" namespace "OpenMS::IdentificationDataInternal":
    cdef cppclass IdentifiedSequence[T](ScoredProcessingResult) :
      # wrap-inherits:
      #  ScoredProcessingResult
      # wrap-instances:
      #   IdentifiedPeptide := IdentifiedSequence[ AASequence ]
      #   IdentifiedOligo   := IdentifiedSequence[ NASequence ]
      IdentifiedSequence(IdentifiedSequence&) nogil except + #wrap-ignore

ctypedef IdentifiedSequence[ AASequence ] IdentifiedPeptide
ctypedef IdentifiedSequence[ NASequence ] IdentifiedOligo

cdef extern from "<OpenMS/METADATA/ID/IdentifiedCompound.h>" namespace "OpenMS::IdentificationDataInternal":
    cdef cppclass IdentifiedCompound(ScoredProcessingResult) :
      # wrap-inherits:
      #  ScoredProcessingResult
      IdentifiedCompound(IdentifiedCompound&) nogil except + #wrap-ignore

cdef extern from "<OpenMS/METADATA/ID/MoleculeQueryMatch.h>" namespace "OpenMS::IdentificationDataInternal":
    cdef cppclass MoleculeQueryMatch(ScoredProcessingResult) :
     # wrap-inherits:
     #  ScoredProcessingResult
     MoleculeQueryMatch(MoleculeQueryMatch&) nogil except + #wrap-ignore

cdef extern from "<OpenMS/METADATA/ID/QueryMatchGroup.h>" namespace "OpenMS::IdentificationDataInternal":
    cdef cppclass QueryMatchGroup(ScoredProcessingResult) :
     # wrap-inherits:
     #  ScoredProcessingResult
     QueryMatchGroup(QueryMatchGroup&) nogil except + #wrap-ignore


cdef extern from "<OpenMS/METADATA/ID/IdentificationData.h>" namespace "OpenMS":
    #ctypedef IdentificationDataInternal.InputFiles InputFiles
    
    cdef cppclass IdentificationData(MetaInfoInterface) :
        # wrap-inherits:
        #  MetaInfoInterface
        IdentificationData(IdentificationData&) nogil except + #wrap-ignore

cdef extern from "<OpenMS/METADATA/ID/IdentificationDataWrapper.h>" namespace "OpenMS":
    cdef cppclass IdentificationDataWrapper(IdentificationData) :
        # wrap-inherits:
        #  MetaInfoInterface

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