CXX      ?= g++
CXXFLAGS ?= -std=c++17 -Wall -Wextra -O2
LDFLAGS  ?=

PREFIX   ?= /usr
BINDIR   ?= $(PREFIX)/bin
DESTDIR  ?=

BUILD_DIR = build

# 排除 install.cc，避免和 make 的 install 目标冲突
SRCS = $(filter-out install.cc,$(wildcard *.cc))
BINS = $(SRCS:.cc=)
BUILD_BINS = $(addprefix $(BUILD_DIR)/,$(BINS))

.PHONY: all clean install uninstall install-bin

all: $(BUILD_BINS)

# 每个 .cc 编译到 build/ 下对应的可执行文件
$(BUILD_DIR)/%: %.cc | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -o $@ $< $(LDFLAGS)

# 确保 build 目录存在
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

install: all
	install -d $(DESTDIR)$(BINDIR)
	install -m 755 $(BUILD_BINS) $(DESTDIR)$(BINDIR)/

install-bin: install.cc | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -o $(BUILD_DIR)/tux-install $< $(LDFLAGS)

uninstall:
	@for bin in $(BINS); do \
		rm -f $(DESTDIR)$(BINDIR)/$$bin; \
	done

clean:
	rm -rf $(BUILD_DIR) tux-install $(wildcard *.o)
