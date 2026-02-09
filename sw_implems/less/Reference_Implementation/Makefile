CC = gcc

# --- PARAMETERS ---
# Example: Category 1, Smallest Signature (Target 45)
CAT_DEF    = -DCATEGORY=252
TARGET_DEF = -DTARGET=45
# ------------------------------

CFLAGS = -g -O3 -march=native -I./include $(CAT_DEF) $(TARGET_DEF)

# Find all library sources and convert them to .o
LIB_SOURCES = $(wildcard lib/*.c)
LIB_OBJECTS = $(LIB_SOURCES:.c=.o)

TARGETS = profile_keygen profile_sign profile_verify

# RULES
all: $(TARGETS)

# Pattern rule: Links profile_X from profile_X.o and all LIB_OBJECTS
profile_%: profile_%.o $(LIB_OBJECTS)
	$(CC) $(CFLAGS) $^ -o $@

# Rule to compile any .c to .o
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f *.o lib/*.o $(TARGETS) *.out
