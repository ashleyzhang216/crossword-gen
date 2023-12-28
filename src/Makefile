# ==================================================================
# Author: Ashley Zhang (ayz27@cornell.edu)
# Date:   7/4/2023
# Description: Makefile to build tool for running
# ==================================================================

# main directories
SRC_DIR=./

# src dirs
UTIL_DIR=utils
COMMON_DIR=common
WORDFINDER_DIR=word_finder
CROSSWORD_DIR=crossword
CWCSP_DIR=cw_csp
CWTRIE_DIR=cw_trie
CWGEN_DIR=cli

# g++ settings
D = -D
DEFINES = $(D) CW_TOP
CPPFLAGS += -pipe
CXXFLAGS += -g -W -Werror -Wall -Wconversion -Wextra -std=c++20 $(DEFINES)
INC=-I$(SRC_DIR)

BIN = cw_gen

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
		\
		crossword.o \
		crossword_data_types.o \
		\
		cw_csp.o \
		cw_csp_data_types.o \
		\
		cw_trie.o \
		cw_trie_data_types.o \
		\
		cw_gen.o \

# full path of all header files to build
HFILES = \
		$(SRC_DIR)/$(UTIL_DIR)/cw_utils.h \
		$(SRC_DIR)/$(COMMON_DIR)/common_data_types.h \
		$(SRC_DIR)/$(COMMON_DIR)/common_parent.h \
		\
		$(SRC_DIR)/$(WORDFINDER_DIR)/word_finder.h \
		$(SRC_DIR)/$(WORDFINDER_DIR)/word_finder_data_types.h \
		$(SRC_DIR)/$(WORDFINDER_DIR)/json.hpp \
		\
		$(SRC_DIR)/$(CROSSWORD_DIR)/crossword.h \
		$(SRC_DIR)/$(CROSSWORD_DIR)/crossword_data_types.h \
		\
		$(SRC_DIR)/$(CWCSP_DIR)/cw_csp.h \
		$(SRC_DIR)/$(CWCSP_DIR)/cw_csp_data_types.h \
		\
		$(SRC_DIR)/$(CWTRIE_DIR)/cw_trie.h \
		$(SRC_DIR)/$(CWTRIE_DIR)/cw_trie_data_types.h \
		\
		$(SRC_DIR)/$(CWGEN_DIR)/cw_gen.h \
		$(SRC_DIR)/$(CWGEN_DIR)/cw_gen_data_types.h \
		$(SRC_DIR)/$(CWGEN_DIR)/cxxopts.hpp \

$(BIN) : $(OFILES)
	$(CXX) $(INC) *.o -o $(BIN)

cw_gen.o : $(SRC_DIR)/$(CWGEN_DIR)/cw_gen.cpp
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $(INC) -c $<

#################### SRC ####################

# rule to compile all cpp files
%.o : $(SRC_DIR)/*/%.cpp $(HFILES)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $(INC) -c $<