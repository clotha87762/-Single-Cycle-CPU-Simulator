#ifndef errorHandler_h
#define errorHandler_h


enum errorType{WritetoReg0,NumberOverflow,MemoryAddOverflow,DataMisaligned};


        void Deal_WritetoReg0();
        void Deal_NumberOverflow();
        void Deal_MemoryAddOverflow();
        void Deal_DataMisaligned();



#endif
