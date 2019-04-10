from Types cimport *
from libcpp cimport bool
from libcpp.vector cimport vector as libcpp_vector
from IdentificationData cimport *
from FASTAFile cimport *
from MzTab cimport *
from PeptideIdentification cimport *
from ProteinIdentification cimport *

cdef extern from "<OpenMS/METADATA/ID/IdentificationDataConverter.h>" namespace "OpenMS":

    cdef cppclass IdentificationDataConverter "OpenMS::IdentificationDataConverter":
        #
        # wrap-doc:
        #     ADD PYTHON DOCUMENTATION HERE
        #
        IdentificationDataConverter() nogil except +
        IdentificationDataConverter(IdentificationDataConverter) nogil except + #wrap-ignore
        # TODO: static # void importIDs(IdentificationData & id_data, libcpp_vector[ ProteinIdentification ] & proteins, libcpp_vector[ PeptideIdentification ] & peptides) nogil except +
        # TODO: static # void exportIDs(IdentificationData & id_data, libcpp_vector[ ProteinIdentification ] & proteins, libcpp_vector[ PeptideIdentification ] & peptides, bool export_oligonucleotides) nogil except +
        # TODO: static # MzTab exportMzTab(IdentificationData & id_data) nogil except +
        # TODO: static # # NAMESPACE # void importSequences(IdentificationData & id_data, libcpp_vector[ FASTAFile::FASTAEntry ] & fasta, IdentificationData::MoleculeType type_, const String & decoy_pattern) nogil except +


# COMMENT: wrap static methods
cdef extern from "<OpenMS/METADATA/ID/IdentificationDataConverter.h>" namespace "OpenMS::IdentificationDataConverter":

        void importIDs(IdentificationData & id_data, libcpp_vector[ ProteinIdentification ] & proteins, libcpp_vector[ PeptideIdentification ] & peptides) nogil except + # wrap-attach:IdentificationDataConverter
        void exportIDs(IdentificationData & id_data, libcpp_vector[ ProteinIdentification ] & proteins, libcpp_vector[ PeptideIdentification ] & peptides, bool export_oligonucleotides) nogil except + # wrap-attach:IdentificationDataConverter
        MzTab exportMzTab(IdentificationData & id_data) nogil except + # wrap-attach:IdentificationDataConverter
        # NAMESPACE # void importSequences(IdentificationData & id_data, libcpp_vector[ FASTAFile::FASTAEntry ] & fasta, IdentificationData::MoleculeType type_, const String & decoy_pattern) nogil except + # wrap-attach:IdentificationDataConverter

