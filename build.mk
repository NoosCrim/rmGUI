LOG_DIR := log
OUT_DIR := out/

COMPILER_FLAGS := -fconcepts-diagnostics-depth=2 -Wextra -Wall -Werror -std=c++23 -g -D MGLU_ENABLE_LOGGING=1 -D MGLU_LOG_LEVEL=2 -D NO_TEST_LOG_BUFFERING=1
LINKER_FLAGS := 
SHADER_DIR := shaders/
COMPLETE_SHADERS_DIR := $(OUT_DIR)/shaders/

# glsl_preprocess
$(eval $(call reset_target))

TARGET_NAME := glsl_preprocess

TARGET_SRC_DIR := glsl_preprocess
$(eval $(call compile_target))

TARGET_TYPE := EXEC
$(eval $(call link_target))

$(eval $(call clean_target))

# preprocess_shaders
SHADERS := \
	guiElem.vert.glsl \
	guiElem.frag.glsl

preprocess_shaders: $(SHADER_DIR) $(COMPLETE_SHADERS_DIR) $(glsl_preprocess)
	for shader in $(SHADERS); do \
		$(glsl_preprocess_OUT_FILE) "$(SHADER_DIR)/$$shader" "$(COMPLETE_SHADERS_DIR)/$$shader"; \
	done

# rmGUI lib
$(eval $(call reset_target))

TARGET_NAME := rmGUI

TARGET_SRC_DIR := src/
TARGET_HEADER_DIRS := headers/

$(eval $(call compile_target))

TARGET_TYPE := STATIC
TARGET_LINK_DEPS := preprocess_shaders
$(eval $(call link_target))

$(eval $(call clean_target))

# rmGUI example
$(eval $(call reset_target))

TARGET_NAME := rmGUI_example

TARGET_SRC_DIR := example/
TARGET_HEADER_DIRS := example/ headers/

$(eval $(call compile_target))

TARGET_LINKER_FLAGS = $(LINKER_FLAGS) -lGL -L./out/rmGUI/ -lrmGUI -lglfw -lGLEW
TARGET_TYPE := EXEC
TARGET_LINK_DEPS := rmGUI
TARGET_LINK_FILES := $(mGLu_OUT_FILE)
$(eval $(call link_target))
$(eval $(call clean_target))