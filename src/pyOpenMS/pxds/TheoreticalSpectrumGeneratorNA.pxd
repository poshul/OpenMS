from Types cimport *
from libcpp.map cimport map as libcpp_map
from libcpp.set cimport set as libcpp_set
from DefaultParamHandler cimport *
from MSSpectrum cimport *
from DataArrays cimport *
from NASequence cimport *

cdef extern from "<OpenMS/CHEMISTRY/TheoreticalSpectrumGeneratorNA.h>" namespace "OpenMS":

    cdef cppclass TheoreticalSpectrumGeneratorNA(DefaultParamHandler) :
        #
        # wrap-doc:
        #     ADD PYTHON DOCUMENTATION HERE
        #
        # wrap-inherits:
        #  DefaultParamHandler
        TheoreticalSpectrumGeneratorNA() nogil except +
        TheoreticalSpectrumGeneratorNA(TheoreticalSpectrumGeneratorNA) nogil except +
        void getSpectrum(MSSpectrum & spectrum, NASequence & oligo, Int min_charge, Int max_charge) nogil except +
        void getMultipleSpectra(libcpp_map[ Int, MSSpectrum ] & spectra, NASequence & oligo, libcpp_set[ int ] & charges, Int base_charge) nogil except +
        void updateMembers_() nogil except +

