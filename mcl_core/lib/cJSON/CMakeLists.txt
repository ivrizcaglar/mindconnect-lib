#Set sources
ADD_LIBRARY(cJSON OBJECT cJSON.c)

#PIC option is required in order to build cJSON with shared library target
SET_TARGET_PROPERTIES(cJSON PROPERTIES POSITION_INDEPENDENT_CODE ON
										FOLDER ${MCL_COMPONENT})