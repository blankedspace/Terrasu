
# Makefile for compiling shaders using shaderc.exe

# Compiler and flags
SHADERC = ../shaderc.exe
FRAG_DX12_FLAGS = --type f --platform-windows -p s_5_0 -O 3
VERT_DX12_FLAGS = --type v --platform-windows -p s_5_0 -O 3

# Directories
ASSETS_DIR = ../../Assets/shaders/dx11
BIN_DIR = bin

# Source files
FRAG_SHADERS = $(wildcard fs_*.sc)
VERT_SHADERS = $(wildcard vs_*.sc)

# Object files
FRAG_DX12_OBJS = $(patsubst %.sc,$(ASSETS_DIR)/%.bin,$(FRAG_SHADERS))
VERT_DX12_OBJS = $(patsubst %.sc,$(ASSETS_DIR)/%.bin,$(VERT_SHADERS))

ASSETS_GLSL_DIR = ../../Assets/shaders/glsl

FRAG_GLSL_OBJS = $(patsubst %.sc,$(ASSETS_GLSL_DIR)/%.bin,$(FRAG_SHADERS))
VERT_GLSL_OBJS = $(patsubst %.sc,$(ASSETS_GLSL_DIR)/%.bin,$(VERT_SHADERS))

FRAG_GLSL_FLAGS = --type f --platform-ams.js -O 3
VERT_GLSL_FLAGS = --type v --platform-ams.js -O 3

all: $(FRAG_DX12_OBJS) $(VERT_DX12_OBJS) $(FRAG_GLSL_OBJS) $(VERT_GLSL_OBJS)

$(FRAG_DX12_OBJS): $(FRAG_SHADERS)
	@echo "Compiling fragment shader $<"
	$(SHADERC) $(FRAG_DX12_FLAGS) -f $< -o $@ 

$(VERT_DX12_OBJS):  $(VERT_SHADERS)
	@echo "Compiling vector shader $<"
	$(SHADERC) $(VERT_DX12_FLAGS) -f $< -o $@ 

$(FRAG_GLSL_OBJS): $(FRAG_SHADERS)
	@echo "Compiling fragment shader $<"
	$(SHADERC) $(FRAG_GLSL_FLAGS) -f $< -o $@ 
	
$(VERT_GLSL_OBJS):  $(VERT_SHADERS)
	@echo "Compiling vector shader $<"
	$(SHADERC) $(VERT_GLSL_FLAGS) -f $< -o $@ 
	

clean:
	@echo "Cleaning up"
	@rm -rf $(ASSETS_DIR) $(BIN_DIR)

.PHONY: all clean