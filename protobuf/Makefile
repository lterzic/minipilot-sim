PROTOC := ../build/lib/minipilot/lib/protobuf/protoc

SRC_DIR = ./src
CS_OUT_DIR = ./out/cs
PY_OUT_DIR = ./out/py

SRCS := $(shell find $(SRC_DIR) -name '*.proto')

.PHONY: all
all: cs py

.PHONY: cs
cs:
	mkdir -p $(CS_OUT_DIR)
	$(PROTOC) -I=$(SRC_DIR) --csharp_out=$(CS_OUT_DIR) $(SRCS)

.PHONY: py
py:
	mkdir -p $(PY_OUT_DIR)
	$(PROTOC) -I=$(SRC_DIR) --python_out=$(PY_OUT_DIR) $(SRCS)