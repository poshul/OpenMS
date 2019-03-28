from Types cimport *
from libcpp.map cimport map as libcpp_map
from libcpp.set cimport set as libcpp_set
from DefaultParamHandler cimport *
from MSSpectrum cimport *
from DataArrays cimport *
from NASequence cimport *

cdef extern from "<OpenMS/CHEMISTRY/NucleicAcidSpectrumGenerator.h>" namespace "OpenMS":

    cdef cppclass NucleicAcidSpectrumGenerator(DefaultParamHandler) :
        # wrap-inherits:
        #  DefaultParamHandler
        NucleicAcidSpectrumGenerator() nogil except +
        NucleicAcidSpectrumGenerator(NucleicAcidSpectrumGenerator) nogil except +
        void getSpectrum(MSSpectrum & spectrum, NASequence & oligo, Int min_charge, Int max_charge) nogil except +
        # void getMultipleSpectra(libcpp_map[ Int, MSSpectrum ] & spectra, NASequence & oligo, libcpp_set[ int ] & charges, Int base_charge) nogil except +
        void updateMembers_() nogil except +

