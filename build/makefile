COMPILER := g++-4.9
THOOL_ROOT := ../

vpath %.cpp $(THOOL_ROOT)

# Library constants

THOOL_SRC_DIRS := thool
THOOL_OBJ_DIRS := $(addprefix obj/, $(THOOL_SRC_DIRS))

THOOL_SRCS := \
	thool/task_queue.cpp \
	thool/thread_pool.cpp \
	thool/thread.cpp

THOOL_OBJS := $(patsubst %.cpp, obj/%.o, $(THOOL_SRCS))

# Library targets

thool: thool_dirs $(THOOL_OBJS)
	ar rcs libthool.a $(THOOL_OBJS)

thool_dirs:
	mkdir -p $(THOOL_OBJ_DIRS)
	
# Test constants

THOOL_LIB_PATH := $(THOOL_ROOT)build
THOOL_TEST_LIB := -lthool -lpthread

THOOL_TEST_SRC_DIRS := test
THOOL_TEST_OBJ_DIRS := $(addprefix obj/, $(THOOL_TEST_SRC_DIRS))

THOOL_TEST_SRCS := \
	test/test.cpp

THOOL_TEST_OBJS := $(patsubst %.cpp, obj/%.o, $(THOOL_TEST_SRCS))

# Test targets

test: thool test_dirs $(THOOL_TEST_OBJS)
	$(COMPILER) $(THOOL_TEST_OBJS) -L $(THOOL_LIB_PATH) $(THOOL_TEST_LIB) -o thool-test
	
test_dirs:
	mkdir -p $(THOOL_TEST_OBJ_DIRS)

clean:
	rm -rf obj/ > /dev/null
	rm -f libthool.a > /dev/null
	rm -f thool-test > /dev/null
	
obj/%.o : %.cpp
	$(COMPILER) -std=c++11 -I $(THOOL_ROOT) -Wall -c -o $@ $<
