CC := clang
CFLAGS := -Wall -O3
INCLUDE := include
INCLUDES := $(INCLUDE)/*.h
TEST_DIR := tests
TESTS_SRC = $(wildcard tests/*.c)
TESTS_BIN = $(TESTS_SRC:.c=)

tests: $(TESTS_BIN)
	@total=0;passed=0;for exec in $^; do \
		echo "running test $$exec"; \
		((total=total+1));\
		$$exec&&((passed=passed+1));\
		done;\
		printf "TOTAL: %d\nPASSED: %d\n\x1b[31mFAILED: %d\x1b[0m\n" $$total $$passed  $$((total-passed))

$(TESTS_BIN): %:%.c $(INCLUDES)
	$(CC) $(CFLAGS) -I $(INCLUDE) $< -o $@
