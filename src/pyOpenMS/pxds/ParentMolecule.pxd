from ScoredProcessingResult cimport *

cdef extern from "<OpenMS/METADATA/ID/ParentMolecule.h>" namespace "OpenMS::IdentificationDataInternal":
    cdef cppclass ParentMolecule(ScoredProcessingResult) :
      # wrap-inherits:
      #  ScoredProcessingResult
      ParentMolecule(ParentMolecule&) nogil except +  #wrap-ignore