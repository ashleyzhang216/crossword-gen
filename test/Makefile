# ==================================================================
# Author: Ashley Zhang (ayz27@cornell.edu)
# Date:   7/4/2023
# Description: Makefile to build project for testing
# ==================================================================

# main directories
SRC_DIR=../src
TEST_DIR=./

# src dirs
UTIL_DIR=utils
COMMON_DIR=common

# test dirs
CATCH_DIR=catch

# g++ settings
INC=-I$(SRC_DIR)

all : $(BIN)

clean :
	rm -f $(BIN) *.o

# path of where object files will be created
OFILES = cw_utils.o \
				cw_test_driver.o \

# full path of all header files to build
HFILES = $(SRC_DIR)/$(UTIL_DIR)/cw_utils.h \
				$(SRC_DIR)/$(COMMON_DIR)/common_data_types.h \
				$(TEST_DIR)/$(CATCH_DIR)/catch.hpp \

$(BIN) : $(OFILES)
	$(CXX) $(INC) *.o -o $(BIN)

cw_test_driver.o : $(TEST_DIR)/cw_test_driver.cpp
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $(INC) -c $<

#################### SRC ####################

# rule to compile all cpp files
%.o : $(SRC_DIR)/*/%.cpp $(HFILES)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $(INC) -c $<

#################### TST ####################

# TODO: implement when more tests are added