.DEFAULT_GOAL := help

CMAKE ?= cmake
TARGET ?= partyboard

LINUX_PRESET ?= linux-default-relwithdebinfo
WINDOWS_PRESET ?= windows-msvc-relwithdebinfo
MACOS_PRESET ?= macos-default-relwithdebinfo

UNAME_S := $(shell uname -s 2>/dev/null)
ifeq ($(OS),Windows_NT)
HOST_PRESET := $(WINDOWS_PRESET)
else ifeq ($(UNAME_S),Darwin)
HOST_PRESET := $(MACOS_PRESET)
else ifeq ($(UNAME_S),Linux)
HOST_PRESET := $(LINUX_PRESET)
else
HOST_PRESET :=
endif

PRESET ?= $(HOST_PRESET)

define require_preset
$(if $(strip $(1)),,$(error Unsupported host platform. Set PRESET=<cmake-preset>.))
endef

.PHONY: help build app configure linux linux-app windows windows-app macos macos-app clean

help:
	@echo "Party Board build shortcuts"
	@echo ""
	@echo "Generic targets:"
	@echo "  make build        Configure and build for the current host"
	@echo "  make app          Alias for make build"
	@echo "  make configure    Configure the current host preset only"
	@echo "  make clean        Clean the current host preset build tree"
	@echo ""
	@echo "Platform targets:"
	@echo "  make linux        Build with $(LINUX_PRESET)"
	@echo "  make windows      Build with $(WINDOWS_PRESET)"
	@echo "  make macos        Build with $(MACOS_PRESET)"
	@echo ""
	@echo "Overrides:"
	@echo "  make app PRESET=linux-clang-debug"
	@echo "  make windows WINDOWS_PRESET=windows-clang-relwithdebinfo"
	@echo "  make app TARGET=install"

build app:
	$(call require_preset,$(PRESET))
	$(CMAKE) --preset $(PRESET)
	$(CMAKE) --build --preset $(PRESET) --target $(TARGET) --parallel

configure:
	$(call require_preset,$(PRESET))
	$(CMAKE) --preset $(PRESET)

linux linux-app:
	$(CMAKE) --preset $(LINUX_PRESET)
	$(CMAKE) --build --preset $(LINUX_PRESET) --target $(TARGET) --parallel

windows windows-app:
	$(CMAKE) --preset $(WINDOWS_PRESET)
	$(CMAKE) --build --preset $(WINDOWS_PRESET) --target $(TARGET) --parallel

macos macos-app:
	$(CMAKE) --preset $(MACOS_PRESET)
	$(CMAKE) --build --preset $(MACOS_PRESET) --target $(TARGET) --parallel

clean:
	$(call require_preset,$(PRESET))
	$(CMAKE) --build --preset $(PRESET) --target clean
