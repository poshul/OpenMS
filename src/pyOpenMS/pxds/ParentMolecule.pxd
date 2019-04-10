from Types cimport *
from libcpp cimport bool
from ScoredProcessingResult cimport *

cdef extern from "<OpenMS/METADATA/ID/ParentMolecule.h>" namespace "OpenMS::IdentificationDataInternal":

    cdef cppclass ParentMolecule(ScoredProcessingResult) :
        #
        # wrap-doc:
        #     ADD PYTHON DOCUMENTATION HERE
        #
        # wrap-inherits:
        #  ScoredProcessingResult
        ParentMolecule() nogil except +
        ParentMolecule(ParentMolecule) nogil except +
        String accession
        enum MoleculeType molecule_type
        String sequence
        String description
        double coverage
        bool is_decoy
        ParentMolecule(const String & accession, MoleculeType molecule_type, const String & sequence, const String & description, double coverage, bool is_decoy, AppliedProcessingSteps & steps_and_scores) nogil except +
        ParentMolecule operator+=(ParentMolecule & other) nogil except +

