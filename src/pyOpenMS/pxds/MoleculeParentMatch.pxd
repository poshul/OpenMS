from Types cimport *
from libcpp cimport bool
from ParentMolecule cimport *

cdef extern from "<OpenMS/METADATA/ID/MoleculeParentMatch.h>" namespace "OpenMS::IdentificationDataInternal":

    cdef cppclass MoleculeParentMatch(MetaInfoInterface) :
        #
        # wrap-doc:
        #     ADD PYTHON DOCUMENTATION HERE
        #
        # wrap-inherits:
        #  MetaInfoInterface
        MoleculeParentMatch() nogil except +
        MoleculeParentMatch(MoleculeParentMatch) nogil except + #wrap-ignore
        Size start_pos
        Size end_pos
        String left_neighbor
        String right_neighbor
        # const # expr Size UNKNOWN_POSITION
        # const # expr char UNKNOWN_NEIGHBOR
        # const # expr char LEFT_TERMINUS
        # const # expr char RIGHT_TERMINUS
        MoleculeParentMatch(Size start_pos, Size end_pos, String left_neighbor, String right_neighbor) nogil except +
        bool operator<(MoleculeParentMatch & other) nogil except +
        bool operator==(MoleculeParentMatch & other) nogil except +
        bool hasValidPositions(Size molecule_length, Size parent_length) nogil except +

