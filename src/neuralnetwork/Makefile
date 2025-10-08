# ---------- toolchain --------------------------------------------------------
CXX       := /opt/homebrew/opt/llvm/bin/clang++   # brew llvm
SDK_PATH  := $(shell xcrun --show-sdk-path)

# ---------- third-party libs -------------------------------------------------
OPENBLAS := $(shell /opt/homebrew/bin/brew --prefix openblas)

# ---------- compile / link flags --------------------------------------------
CXXFLAGS  := -std=c++20 -g \
             -fsanitize=address,leak \
             -isysroot $(SDK_PATH) -stdlib=libc++ \
             -I$(OPENBLAS)/include            #  cblas.h

LDFLAGS   := -fsanitize=address,leak \
             -L$(OPENBLAS)/lib -lopenblas     #  libopenblas.dylib

# ---------- sources / targets ------------------------------------------------
SRC       := testactivation.cpp        # add more .cpp files here
OBJ       := $(SRC:.cpp=.o)
TARGET    := testcode                # final executable name

# ---------- build rules ------------------------------------------------------
$(TARGET): $(OBJ)
	$(CXX) $^ $(LDFLAGS) -o $@

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

.PHONY: clean
clean:
	rm -f $(OBJ) $(TARGET)
