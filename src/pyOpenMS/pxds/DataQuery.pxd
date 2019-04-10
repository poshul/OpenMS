from Types cimport *
from libcpp cimport bool
from MetaData cimport *

cdef extern from "<OpenMS/METADATA/ID/DataQuery.h>" namespace "OpenMS::IdentificationDataInternal":

    cdef cppclass DataQuery(MetaInfoInterface) :
        #
        # wrap-doc:
        #     ADD PYTHON DOCUMENTATION HERE
        #
        # wrap-inherits:
        #  MetaInfoInterface
        DataQuery() nogil except +
        DataQuery(DataQuery) nogil except +
        String data_id
        # NAMESPACE # boost::optional[ InputFileRef ] input_file_opt
        double rt
        double mz
        # NAMESPACE #  DataQuery(const String & data_id, boost::optional[ InputFileRef ] input_file_opt, double rt, double mz) nogil except +
        bool operator<(DataQuery & other) nogil except +
        bool operator==(DataQuery & other) nogil except +

