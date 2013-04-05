# 
# Makefile for hiredis client
#

MY_NAME := hrc
MY_SRCS := $(wildcard *.cpp)
MY_OBJS := ${MY_SRCS:.cpp=.o}

MY_INCLUDE_DIRS := /usr/local/include /usr/local/include/hiredis
MY_LIBRARY_DIRS := /usr/local/lib
MY_LIBRARIES := hiredis gtest gtest_main

CXXFLAGS += $(foreach includedir,$(MY_INCLUDE_DIRS),-I$(includedir))
CXXFLAGS += -Wall -g -std=c++11 -pthread
LDFLAGS += $(foreach librarydir,$(MY_LIBRARY_DIRS),-L$(librarydir))
LDLIBS += $(foreach library,$(MY_LIBRARIES),-l$(library))

.PHONY: all clean

all: $(MY_NAME)

$(MY_NAME): $(MY_OBJS)
	$(LINK.cc) -o $(MY_NAME) $(MY_OBJS) $(LDLIBS)

clean:
	@- rm -rf $(MY_OBJS) $(MY_NAME)

