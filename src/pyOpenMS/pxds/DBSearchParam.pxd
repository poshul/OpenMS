from MetaInfoInterface cimport *
from libcpp.set cimport set as libcpp_set
from DigestionEnzyme cimport *
from EnzymaticDigestion cimport *
from String cimport *

cdef extern from "<OpenMS/METADATA/ID/DBSearchParam.h>" namespace "OpenMS::IdentificationDataInternal":
    cdef cppclass DBSearchParam(MetaInfoInterface) :
      # wrap-inherits:
      #  MetaInfoInterface
      DBSearchParam(DBSearchParam&) nogil except +
      DBSearchParam() nogil except +

      #MoleculeType molecule_type
      #MassType mass_type
      
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

      const DigestionEnzyme * digestion_enzyme # not sure how this handles the polymorphism TODO check
      Specificity enzyme_term_specificity
      Size missed_cleavages
      Size min_length
      Size max_length