from Types cimport *
from libcpp cimport bool
from libcpp.pair cimport pair as libcpp_pair
from libcpp.vector cimport vector as libcpp_vector
from libcpp.set cimport set as libcpp_set
from MetaInfoInterface cimport *

cdef extern from "<OpenMS/METADATA/ID/MetaData.h>" namespace "OpenMS::IdentificationDataInternal":
    ctypedef libcpp_set[String] InputFiles

cdef extern from "<OpenMS/METADATA/ID/DataProcessingSoftware.h>" namespace "OpenMS::IdentificationDataInternal":
    cdef cppclass DataProcessingSoftware

cdef extern from "<OpenMS/METADATA/ID/IdentificationData.h>" namespace "OpenMS":
    #ctypedef IdentificationDataInternal.InputFiles InputFiles
    
    cdef cppclass IdentificationData(MetaInfoInterface) :
        # wrap-inherits:
        #  MetaInfoInterface

        IdentificationData() nogil except +
	
        int pythonRegisterInputFile(String & file) nogil except +
        InputFiles & getInputFiles()

        int pythonRegisterDataProcessingSoftware(DataProcessingSoftware & software) nogil except +
        DataProcessingSoftwares & getDataProcessingSoftwares()