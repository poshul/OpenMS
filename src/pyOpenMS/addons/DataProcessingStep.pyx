from DataProcessingSoftware cimport DataProcessingSoftware as _DataProcessingSoftware
from String cimport *
# two empty lines are important !


    def _init_0(self):
        self.inst = shared_ptr[_DataProcessingStep](new _DataProcessingStep(_DataProcessingSoftware()))
    
    def _init_1(self, DataProcessingStep in_0 ):
        assert isinstance(in_0, DataProcessingStep), 'arg in_0 wrong type'
    
        self.inst = shared_ptr[_DataProcessingStep](new _DataProcessingStep((deref(in_0.inst.get()))))
	
    def __init__(self, *args , **kwargs):

        if not args:
             self._init_0(*args)
        elif (len(args)==1) and (isinstance(args[0], DataProcessingStep)):
             self._init_1(*args)
        else:
               raise Exception('can not handle type of %s' % (args,))