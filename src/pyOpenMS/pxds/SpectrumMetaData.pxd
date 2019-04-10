from Types cimport *
from SpectrumLookup cimport *
from FileHandler cimport *
from MSSpectrum cimport *
from ProteinIdentification cimport *
from MSExperiment cimport *

cdef extern from "<OpenMS/METADATA/SpectrumMetaDataLookup.h>" namespace "OpenMS::SpectrumMetaDataLookup":

    cdef cppclass SpectrumMetaData "OpenMS::SpectrumMetaDataLookup::SpectrumMetaData":
        #
        # wrap-doc:
        #     ADD PYTHON DOCUMENTATION HERE
        #
        SpectrumMetaData() nogil except +
        SpectrumMetaData(SpectrumMetaData) nogil except +
        double rt
        double precursor_rt
        double precursor_mz
        Int precursor_charge
        Size ms_level
        Int scan_number
        String native_id

