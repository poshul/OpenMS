from Types cimport *
from ParentMolecule cimport *

cdef extern from "<OpenMS/METADATA/ID/ParentMoleculeGroup.h>" namespace "OpenMS::IdentificationDataInternal":

    cdef cppclass ParentMoleculeGroup "OpenMS::IdentificationDataInternal::ParentMoleculeGroup":
        #
        # wrap-doc:
        #     ADD PYTHON DOCUMENTATION HERE
        #
        ParentMoleculeGroup() nogil except +
        ParentMoleculeGroup(ParentMoleculeGroup) nogil except + #wrap-ignore
        libcpp_map[ ScoreTypeRef, double ] scores
        libcpp_set[ ParentMoleculeRef ] parent_molecule_refs

