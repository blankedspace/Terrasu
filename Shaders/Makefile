BASE_DIR := .

DIRS := $(shell find $(BASE_DIR)/* -type d)

all: $(DIRS)

$(DIRS):
	@$(MAKE) -C $@

.PHONY: all $(DIRS)