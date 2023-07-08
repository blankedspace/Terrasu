BASE_DIR := .

ifeq ($(OS),Windows_NT)
DIRS := $(patsubst $(shell CHDIR )\\%,%,$(shell DIR /A:D /B /S $(BASE_DIR)))
else
DIRS := $(shell find $(BASE_DIR) -type d)
endif

all: $(DIRS)

$(DIRS):
	@$(MAKE) -C $@

.PHONY: all $(DIRS)