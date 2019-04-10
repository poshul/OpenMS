from Types cimport *
from libcpp cimport bool
from DigestionEnzyme cimport *
from MetaData cimport *

cdef extern from "<OpenMS/METADATA/ID/DBSearchParam.h>" namespace "OpenMS::IdentificationDataInternal":

    cdef cppclass DBSearchParam(MetaInfoInterface) :
        #
        # wrap-doc:
        #     ADD PYTHON DOCUMENTATION HERE
        #
        # wrap-inherits:
        #  MetaInfoInterface
        DBSearchParam() nogil except +
        DBSearchParam(DBSearchParam) nogil except +
        enum MoleculeType molecule_type
        enum MassType mass_type
        String database
        String database_version
        String taxonomy
        libcpp_set[ int ] charges
        libcpp_set[ String ] fixed_mods
        libcpp_set[ String ] variable_mods
        double precursor_mass_tolerance
        double fragment_mass_tolerance
        bool precursor_tolerance_ppm
        bool fragment_tolerance_ppm
        # const # # POINTER # DigestionEnzyme * digestion_enzyme
        Size missed_cleavages
        Size min_length
        Size max_length
        bool operator<(DBSearchParam & other) nogil except +
        bool operator==(DBSearchParam & other) nogil except +

