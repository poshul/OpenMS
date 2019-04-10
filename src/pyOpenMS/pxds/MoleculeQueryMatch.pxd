from Types cimport *
from MetaData cimport *
from IdentifiedCompound cimport *
from IdentifiedSequence cimport *
from PeptideHit cimport *

cdef extern from "<OpenMS/METADATA/ID/MoleculeQueryMatch.h>" namespace "OpenMS::IdentificationDataInternal":

    cdef cppclass MoleculeQueryMatch(ScoredProcessingResult) :
        #
        # wrap-doc:
        #     ADD PYTHON DOCUMENTATION HERE
        #
        # wrap-inherits:
        #  ScoredProcessingResult
        MoleculeQueryMatch() nogil except +
        MoleculeQueryMatch(MoleculeQueryMatch) nogil except +
        IdentifiedMoleculeRef identified_molecule_ref
        DataQueryRef data_query_ref
        Int charge
        PeakAnnotationSteps peak_annotations
        MoleculeQueryMatch(IdentifiedMoleculeRef identified_molecule_ref, DataQueryRef data_query_ref, Int charge, AppliedProcessingSteps & steps_and_scores, PeakAnnotationSteps & peak_annotations) nogil except +
        MoleculeType getMoleculeType() nogil except +
        IdentifiedPeptideRef getIdentifiedPeptideRef() nogil except +
        IdentifiedCompoundRef getIdentifiedCompoundRef() nogil except +
        IdentifiedOligoRef getIdentifiedOligoRef() nogil except +
        MoleculeQueryMatch operator+=(MoleculeQueryMatch & other) nogil except +

