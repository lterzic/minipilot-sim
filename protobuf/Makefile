PROTOC := ../build/lib/minipilot/lib/protobuf/protoc

SRC_DIR = ./src
CS_OUT_DIR = ./out/cs

SRCS := $(shell find $(SRC_DIR) -name '*.proto')

.PHONY: cs
cs:
	mkdir -p $(CS_OUT_DIR)
	$(PROTOC) -I=$(SRC_DIR) --csharp_out=$(CS_OUT_DIR) $(SRCS)