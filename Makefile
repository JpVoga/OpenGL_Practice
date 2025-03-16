# Variables:

COMPILER = g++
BASE_FLAGS = -std=c++23 -Wall -Wextra -Werror -Wpedantic -L. -lstdc++exp -lyaml-cpp -fconcepts-diagnostics-depth=2 -finput-charset=UTF-8 -fmax-errors=1
DEBUG = 1
CG_IMPL = o/window.o o/init.o o/error.o o/vertexBuffer.o o/vertex.o o/indexBuffer.o o/shader.o o/compiledShaderStage.o o/uniform.o o/hash.o o/texture.o \
o/glad.o o/stbImageImpl.o

ifneq ($(DEBUG), 0) # If debug is not false (it's true)
FLAGS = $(BASE_FLAGS) -O0 -D_DEBUG=1 -UNDEBUG -g
else
FLAGS = $(BASE_FLAGS) -O3 -D_DEBUG=0 -DNDEBUG
endif


# Main app:
app.exe: $(wildcard src/*) $(wildcard src/*/**) $(CG_IMPL) src/lib/pch.hpp.pch
	$(COMPILER) src/app.cpp $(CG_IMPL) -lglfw3 -l:a/libglfw3dll.a -lopengl32 -o app.exe $(FLAGS)


# Dependencies written by me:

o/window.o: src/cg/window.cpp src/cg/window.hpp
	$(COMPILER) -c src/cg/window.cpp -o o/window.o $(FLAGS)

o/init.o: src/cg/init.cpp src/cg/init.hpp
	$(COMPILER) -c src/cg/init.cpp -o o/init.o $(FLAGS)

o/error.o: src/cg/error.cpp src/cg/error.hpp
	$(COMPILER) -c src/cg/error.cpp -o o/error.o $(FLAGS)

o/vertexBuffer.o: src/cg/vertexBuffer.cpp src/cg/vertexBuffer.hpp
	$(COMPILER) -c src/cg/vertexBuffer.cpp -o o/vertexBuffer.o $(FLAGS)

o/vertex.o: src/cg/vertex.cpp src/cg/vertex.hpp
	$(COMPILER) -c src/cg/vertex.cpp -o o/vertex.o $(FLAGS)

o/indexBuffer.o: src/cg/indexBuffer.cpp src/cg/indexBuffer.hpp
	$(COMPILER) -c src/cg/indexBuffer.cpp -o o/indexBuffer.o $(FLAGS)

o/shader.o: src/cg/shader.cpp src/cg/shader.hpp
	$(COMPILER) -c src/cg/shader.cpp -o o/shader.o $(FLAGS)

o/compiledShaderStage.o: src/cg/compiledShaderStage.cpp src/cg/shader.hpp
	$(COMPILER) -c src/cg/compiledShaderStage.cpp -o o/compiledShaderStage.o $(FLAGS)

o/uniform.o: src/cg/uniform.cpp
	$(COMPILER) -c src/cg/uniform.cpp -o o/uniform.o $(FLAGS)

o/hash.o: src/cg/hash.cpp src/cg/hash.hpp
	$(COMPILER) -c src/cg/hash.cpp -o o/hash.o $(FLAGS)

o/texture.o: src/cg/texture.cpp src/cg/texture.hpp
	$(COMPILER) -c src/cg/texture.cpp -o o/texture.o $(FLAGS)


# Outside dependencies:

o/glad.o: src/lib/glad/glad.c src/lib/glad/glad.h
	$(COMPILER) -c src/lib/glad/glad.c -o o/glad.o

o/stbImageImpl.o: src/lib/stbImageImpl.cpp
	$(COMPILER) -c src/lib/stbImageImpl.cpp -o o/stbImageImpl.o

src/lib/pch.hpp.pch: src/lib/pch.hpp
	$(COMPILER) -x c++-header -c src/lib/pch.hpp -o src/lib/pch.hpp.pch $(FLAGS)

# Use flag -B to recompile it all. Ex: 'make -B'
# Use flag -o to avoid recompilation of certain file. Ex: 'make -B -o src/lib/pch.hpp.pch' (recompile everything but that file)


# OBS0: Object binding seems to be thread dependant (getting VBO binding from other thread returns zero)
# OBS1: My thread_local global texture slotting system only works because contexts all share resources, but that should be fine
# OBS2: My buffer objects DON'T own data! Perhaps, in future implementations, read from buffer instead of holding void ptr?
# OBS3: Had to install git large file storage, then 'git lfs install' and 'git lfs track' the large file. May have to delete the '.git' folder and initialize git again.