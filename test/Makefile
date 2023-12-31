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
WORDFINDER_DIR=word_finder
CROSSWORD_DIR=crossword
CWCSP_DIR=cw_csp

# test dirs
CATCH_DIR=catch

# g++ settings
D = -D
DEFINES = $(D) CW_TOP
CPPFLAGS += -pipe
CXXFLAGS += -g -W -Werror -Wall -Wconversion -Wextra -std=c++20 $(DEFINES)
INC=-I$(SRC_DIR)

BIN = cw_test_driver

all : $(BIN)

clean :
	rm -f $(BIN) *.o

# path of where object files will be created
OFILES = \
		common_data_types.o \
		cw_utils.o \
		common_parent.o \
		\
		word_finder.o \
		word_finder_data_types.o \
		word_finder_test.o \
		word_finder_test_driver.o \
		\
		crossword.o \
		crossword_data_types.o \
		crossword_test.o \
		crossword_test_driver.o \
		\
		cw_csp.o \
		cw_csp_data_types.o \
		cw_csp_test.o \
		cw_csp_test_driver.o \
		\
		cw_test_driver.o \

# full path of all header files to build
HFILES = \
		$(SRC_DIR)/$(UTIL_DIR)/cw_utils.h \
		$(SRC_DIR)/$(COMMON_DIR)/common_data_types.h \
		$(SRC_DIR)/$(COMMON_DIR)/common_parent.h \
		\
		$(SRC_DIR)/$(WORDFINDER_DIR)/word_finder.h \
		$(SRC_DIR)/$(WORDFINDER_DIR)/word_finder_data_types.h \
		$(SRC_DIR)/$(WORDFINDER_DIR)/json.hpp \
		$(TEST_DIR)/$(WORDFINDER_DIR)/word_finder_test_driver.h \
		\
		$(SRC_DIR)/$(CROSSWORD_DIR)/crossword.h \
		$(SRC_DIR)/$(CROSSWORD_DIR)/crossword_data_types.h \
		$(TEST_DIR)/$(CROSSWORD_DIR)/crossword_test_driver.h \
		\
		$(SRC_DIR)/$(CWCSP_DIR)/cw_csp.h \
		$(SRC_DIR)/$(CWCSP_DIR)/cw_csp_data_types.h \
		$(TEST_DIR)/$(CWCSP_DIR)/cw_csp_test_driver.h \
		\
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

# rule to compile all cpp files
%.o : $(TEST_DIR)/*/%.cpp $(HFILES)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $(INC) -c $<