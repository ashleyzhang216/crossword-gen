# ==================================================================
# Author: Ashley Zhang (ayz27@cornell.edu)
# Date:   7/4/2023
# Description: Makefile to build tool for running
# ==================================================================

# main directories
SRC_DIR=./

# src dirs
COMMON_DIR=common
CROSSWORD_DIR=crossword
CWCSP_DIR=cw_csp
CWTREE_DIR=cw_tree
CWTIMETRACKER_DIR=cw_timetracker
UTIL_DIR=utils
WORDDOMAIN_DIR=word_domain
CWGEN_DIR=cli
LIB_SRC_DIR=lib/src

# g++ settings
D = -D
DEFINES = $(D) CW_TOP
CPPFLAGS += -pipe
CXXFLAGS += -g -W -Werror -Wall -Wconversion -Wextra -O2 -std=c++20 $(DEFINES)
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
		crossword.o \
		crossword_data_types.o \
		\
		cw_csp.o \
		cw_csp_data_types.o \
		\
		cw_tree.o \
		cw_tree_data_types.o \
		\
		cw_timetracker.o \
		cw_timetracker_data_types.o \
		\
		word_domain.o \
		word_domain_data_types.o \
		\
		cw_gen.o \

# full path of all header files to build
HFILES = \
		$(SRC_DIR)/$(UTIL_DIR)/cw_utils.h \
		$(SRC_DIR)/$(COMMON_DIR)/common_data_types.h \
		$(SRC_DIR)/$(COMMON_DIR)/common_parent.h \
		\
		$(SRC_DIR)/$(CROSSWORD_DIR)/crossword.h \
		$(SRC_DIR)/$(CROSSWORD_DIR)/crossword_data_types.h \
		\
		$(SRC_DIR)/$(CWCSP_DIR)/cw_csp.h \
		$(SRC_DIR)/$(CWCSP_DIR)/cw_csp_data_types.h \
		\
		$(SRC_DIR)/$(CWTREE_DIR)/cw_tree.h \
		$(SRC_DIR)/$(CWTREE_DIR)/cw_tree_data_types.h \
		\
		$(SRC_DIR)/$(CWTIMETRACKER_DIR)/cw_timetracker.h \
		$(SRC_DIR)/$(CWTIMETRACKER_DIR)/cw_timetracker_data_types.h \
		\
		$(SRC_DIR)/$(WORDDOMAIN_DIR)/word_domain.h \
		$(SRC_DIR)/$(WORDDOMAIN_DIR)/word_domain_data_types.h \
		\
		$(SRC_DIR)/$(CWGEN_DIR)/cw_gen.h \
		$(SRC_DIR)/$(CWGEN_DIR)/cw_gen_data_types.h \
		\
		$(SRC_DIR)/$(LIB_SRC_DIR)/cxxopts.hpp \
		$(SRC_DIR)/$(LIB_SRC_DIR)/json.hpp \

$(BIN) : $(OFILES)
	$(CXX) $(INC) *.o -o $(BIN)

cw_gen.o : $(SRC_DIR)/$(CWGEN_DIR)/cw_gen.cpp
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $(INC) -c $<

#################### SRC ####################

# rule to compile all cpp files
%.o : $(SRC_DIR)/*/%.cpp $(HFILES)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $(INC) -c $<