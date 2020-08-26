from Types cimport *
from libcpp cimport bool
from libcpp.pair cimport pair as libcpp_pair
from libcpp.vector cimport vector as libcpp_vector
from libcpp.set cimport set as libcpp_set
from MetaInfoInterface cimport *
from Software cimport *
from AASequence cimport AASequence as AAS
from NASequence cimport *

cdef extern from "<OpenMS/METADATA/ID/MetaData.h>" namespace "OpenMS::IdentificationDataInternal":
    ctypedef libcpp_set[String] InputFiles

cdef extern from "<OpenMS/METADATA/ID/DataProcessingSoftware.h>" namespace "OpenMS::IdentificationDataInternal":
    cdef cppclass DataProcessingSoftware(Software) :
      # wrap-inherits:
      #  Software
      DataProcessingSoftware() nogil except +

cdef extern from "<OpenMS/METADATA/ID/DBSearchParam.h>" namespace "OpenMS::IdentificationDataInternal":
    cdef cppclass DBSearchParam(MetaInfoInterface) :
      # wrap-inherits:
      #  MetaInfoInterface
      DBSearchParam() nogil except +

cdef extern from "<OpenMS/METADATA/ID/DataProcessingStep.h>" namespace "OpenMS::IdentificationDataInternal":
    cdef cppclass DataProcessingStep(MetaInfoInterface) :
      # wrap-inherits:
      #  MetaInfoInterface
      DataProcessingStep() nogil except +

cdef extern from "<OpenMS/METADATA/ID/ScoreType.h>" namespace "OpenMS::IdentificationDataInternal":
    cdef cppclass ScoreType(MetaInfoInterface) :
      # wrap-inherits:
      #  MetaInfoInterface
      ScoreType() nogil except +

cdef extern from "<OpenMS/METADATA/ID/DataQuery.h>" namespace "OpenMS::IdentificationDataInternal":
    cdef cppclass DataQuery(MetaInfoInterface) :
      # wrap-inherits:
      #  MetaInfoInterface
      DataQuery()

cdef extern from "<OpenMS/METADATA/ID/ScoredProcessingResult.h>" namespace "OpenMS::IdentificationDataInternal":
    cdef cppclass ScoredProcessingResult(MetaInfoInterface) :
      # wrap-inherits:
      #  MetaInfoInterface
      ScoredProcessingResult() nogil except + 

cdef extern from "<OpenMS/METADATA/ID/ParentMolecule.h>" namespace "OpenMS::IdentificationDataInternal":
    cdef cppclass ParentMolecule(ScoredProcessingResult) :
      # wrap-inherits:
      #  ScoredProcessingResult
      ParentMolecule() nogil except +

cdef extern from "<OpenMS/METADATA/ID/IdentifiedSequence.h>" namespace "OpenMS::IdentificationDataInternal":
    cdef cppclass IdentifiedSequence(ScoredProcessingResult) :
      # wrap-inherits:
      #  ScoredProcessingResult
      IdentifiedSequence() nogil except +

    # Cython lacks the ability to understand these?
    #ctypedef IdentifiedSequence[AAS] IdentifiedPeptide
    #ctypedef IdentifiedSequence[NASequence] IdentifiedOligo

cdef extern from "<OpenMS/METADATA/ID/IdentifiedCompound.h>" namespace "OpenMS::IdentificationDataInternal":
    cdef cppclass IdentifiedCompound(ScoredProcessingResult) :
      # wrap-inherits:
      #  ScoredProcessingResult
      IdentifiedCompound() nogil except +

cdef extern from "<OpenMS/METADATA/ID/MoleculeQueryMatch.h>" namespace "OpenMS::IdentificationDataInternal":
    cdef cppclass MoleculeQueryMatch(ScoredProcessingResult) :
     # wrap-inherits:
     #  ScoredProcessingResult
     MoleculeQueryMatch() nogil except +

cdef extern from "<OpenMS/METADATA/ID/QueryMatchGroup.h>" namespace "OpenMS::IdentificationDataInternal":
    cdef cppclass QueryMatchGroup(ScoredProcessingResult) :
     # wrap-inherits:
     #  ScoredProcessingResult
     QueryMatchGroup() nogil except +



cdef extern from "<OpenMS/METADATA/ID/IdentificationData.h>" namespace "OpenMS":
    #ctypedef IdentificationDataInternal.InputFiles InputFiles
    
    cdef cppclass IdentificationData(MetaInfoInterface) :
        # wrap-inherits:
        #  MetaInfoInterface

        IdentificationData() nogil except +
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
        #int pythonRegisterIdentifiedPeptide(IdentifiedPeptide& peptide) nogil except +
        int pythonRegisterIdentifiedCompound(IdentifiedCompound& compound) nogil except +
        #int pythonRegisterIdentifiedOligo(IdentifiedOligo& oligo) nogil except +
        int pythonRegisterMoleculeQueryMatch(MoleculeQueryMatch& match) nogil except +
        int pythonRegisterQueryMatchGroup(QueryMatchGroup& group) nogil except +
        void pythonAddScore(int match_ref, int score_ref, double value) nogil except +
        void pythonSetCurrentProcessingStep(int step_ref) nogil except +
        int pythonGetCurrentProcessingStep() nogil except +
        libcpp_vector[int] pythonGetBestMatchPerQuery(int score_ref) nogil except + #do I add const here?
        libcpp_pair[int,bool] pythonFindScoreType(String& score_name) nogil except + #const?