
# define VERBOSE to see executed commands

TARGET=gpio_control
MAIN=main.cpp
SRCS_DIR:=src
OBJS_DIR:=bin

CFLAGS:=-pthread -std=c++11 -O2 -Wall -Wno-unused-result
LFLAGS:=-lpthread -pthread

CC:=g++

# test if compiler is installed
ifeq ($(shell which $(CC)), )
$(error compiler $(CC) is not installed)
endif
LL:=$(CC)

include $(SRCS_DIR)/Source.files

SRCS := $(SRCS) $(MAIN)
OBJS := $(SRCS:.cpp=.o)

SRCS := $(patsubst %,$(SRCS_DIR)/%,$(SRCS))
OBJS := $(patsubst %,$(OBJS_DIR)/%,$(OBJS))

DEP_DIR := $(OBJS_DIR)
DEPFLAGS = -MT $@ -MMD -MP -MF $(DEP_DIR)/$*.d
INC_DIR := -I$(SRCS_DIR)

# run "make VERBOSE=1" to see all commands
ifndef VERBOSE
	Verb := @
endif
Echo := $(Verb)echo

default: $(TARGET)

$(TARGET): $(OBJS)
	$(Echo) "Linking executable $(MAIN) into $@"
	$(Verb) $(LL) $(LFLAGS) -o $@ $(OBJS)

$(OBJS): $(OBJS_DIR)/%.o: $(SRCS_DIR)/%.cpp
	$(Echo) "Compiling $<"
	$(Verb) mkdir -p $(dir $@)
	$(Verb) $(CC) $(DEPFLAGS) $(CFLAGS) -o $@ -c $(INC_DIR) $<

clean:
	$(Verb) rm -rf $(OBJS_DIR)/*
	$(Verb) rm -f $(TARGET)
	$(Verb) rm -f $(MAINS)

define echo_var
	@echo $(1) = $($1)
endef

info:
	$(call echo_var,CC)
	$(call echo_var,SRCS)
#	$(call echo_var,OBJS)
	$(call echo_var,CFLAGS)

-include $(OBJS:.o=.d)
