# specify all source files here
# SRCS = fs.c disk.c
SRCS = $(wildcard solution/*.c)


# specify target here (name of executable)
TARG = solution/test_goatfs

# specify compiler, compile flags, and needed libs
CC = gcc
OPTS = -Wall -Werror
LIBS = -lm

# this translates .c files in src list to .o’s
OBJS = $(SRCS:.c=.o)

# all is not really needed, but is used to generate the target
all: $(TARG)

# this generates the target executable
$(TARG): $(OBJS)
	$(CC) -o $(TARG) $(OBJS) $(LIBS)

# this is a generic rule for .o files
%.o: %.c
	$(CC) $(OPTS) -c $< -o $@


# run all the tests at once...
# if you need to run one test at a time, you will do it from the top-level directory:
# ./tests/test_debug.sh for example
test:
	@for test_script in tests/test_*.sh; do $${test_script}; done

# and finally, a clean line
clean:
	rm -f $(OBJS) $(TARG)