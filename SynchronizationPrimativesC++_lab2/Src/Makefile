AC_SRC = $(wildcard atomic_counter_*.cpp)
AC_OBJ = $(addprefix obj/,$(AC_SRC:.cpp=.o))

UL_SRC = $(wildcard user_lock_*.cpp)
UL_OBJ = $(addprefix obj/,$(UL_SRC:.cpp=.o))

LDFLAGS = -pthread
CCFLAGS = -Wall -Wextra -Werror -O3
CXXFLAGS = $(CCFLAGS) -std=c++11

ifdef TSAN
LDFLAGS += -fsanitize=thread
CXXFLAGS += -fsanitize=thread
endif

default: test

.PHONY: all
all: test_atomic_counter test_user_lock

obj/atomic_counter_%.o: atomic_counter_%.cpp atomic_counters.hpp
	mkdir -p obj
	$(CXX) $(CXXFLAGS) -o $@ -c $<

test_atomic_counter: $(AC_OBJ) test_atomic_counter.cpp
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

obj/user_lock_%.o: user_lock_%.cpp user_locks.hpp
	mkdir -p obj
	$(CXX) $(CXXFLAGS) -o $@ -c $<

test_user_lock: $(UL_OBJ) test_user_lock.cpp
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

.PHONY: clean
clean:
	rm -r $(AC_OBJ) test_atomic_counter.exe $(UL_OBJ) test_user_lock.exe obj

.PHONY: test
test: test_atomic_counter test_user_lock
	@ echo "-------------------------------------------------------------------"
	./test_atomic_counter.exe lock
	@ echo "-------------------------------------------------------------------"
	./test_atomic_counter.exe atomic_incdec
	@ echo "-------------------------------------------------------------------"
	./test_atomic_counter.exe atomic_cas
	@ echo "-------------------------------------------------------------------"
	./test_user_lock.exe mutex
	@ echo "-------------------------------------------------------------------"
	./test_user_lock.exe dekker
	@ echo "-------------------------------------------------------------------"
	@ echo ""
	@ echo "CONGRATS, ALL TESTS FINISHED SUCCESSFULLY!!!"
	@ echo ""

bonus: test_user_lock
	./test_user_lock.exe clh
