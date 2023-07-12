LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := main

SDL_PATH := ../SDL

LOCAL_C_INCLUDES := $(LOCAL_PATH)/$(SDL_PATH)/include \
 $(LOCAL_PATH)/3rdParty/bgfx/include \
 $(LOCAL_PATH)/3rdParty/bx/include \
 $(LOCAL_PATH)/3rdParty/bimg/include \
 $(LOCAL_PATH)/3rdParty/yaml-cpp/include \
 $(LOCAL_PATH)/3rdParty/glm \
 $(LOCAL_PATH)/3rdParty/entt/src \
 $(LOCAL_PATH)/3rdParty/imgui \
 $(LOCAL_PATH)/3rdParty/bgfx-imgui/bgfx-imgui \
 $(LOCAL_PATH)/src/include
# Add your application source files here...
LOCAL_SRC_FILES := $(wildcard $(LOCAL_PATH)/src/*.cpp) \
$(wildcard $(LOCAL_PATH)/src/Core/*.cpp) \
$(wildcard $(LOCAL_PATH)/src/ECS/*.cpp) \
$(wildcard $(LOCAL_PATH)/src/Scripts/*.cpp) \
$(wildcard $(LOCAL_PATH)/3rdParty/imgui/*.cpp) \
$(LOCAL_PATH)/3rdParty/imgui/backends/imgui_impl_sdl2.cpp \
$(wildcard $(LOCAL_PATH)/3rdParty/bgfx-imgui/bgfx-imgui/*.cpp) \
$(wildcard $(LOCAL_PATH)/3rdParty/yaml-cpp/src/*.h) \
$(wildcard $(LOCAL_PATH)/3rdParty/yaml-cpp/src/*.cpp) \
$(wildcard $(LOCAL_PATH)/3rdParty/yaml-cpp/include/*.h)

LOCAL_CPPFLAGS := -DBX_CONFIG_DEBUG=1 -DYAML_CPP_STATIC_DEFINE
LOCAL_SHARED_LIBRARIES := SDL2

LOCAL_LDLIBS := -LD:/GitUnity/Terassu/3rdParty/bgfx/.build/android-x86_64/bin \
-lEGL -lGLESv1_CM -lGLESv2 -lOpenSLES -llog -landroid \
-lbxDebug -lbgfxDebug -lbimg_decodeDebug -lbimgDebug


include $(BUILD_SHARED_LIBRARY)
