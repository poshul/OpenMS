from Types cimport *
from ParentMolecule cimport *

cdef extern from "<OpenMS/METADATA/ID/ParentMoleculeGroup.h>" namespace "OpenMS::IdentificationDataInternal":

    cdef cppclass ParentMoleculeGrouping(ScoredProcessingResult) :
        #
        # wrap-doc:
        #     ADD PYTHON DOCUMENTATION HERE
        #
        # wrap-inherits:
        #  ScoredProcessingResult
        ParentMoleculeGrouping() nogil except +
        ParentMoleculeGrouping(ParentMoleculeGrouping) nogil except + #wrap-ignore
        String label
        ParentMoleculeGroups groups

