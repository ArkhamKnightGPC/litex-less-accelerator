#!/bin/bash

# Ensure the executables are built
echo "--- Compiling Profiles ---"
make all

if [ $? -ne 0 ]; then
    echo "Error: Compilation failed."
    exit 1
fi

# List of tasks to profile
TASKS=("profile_keygen" "profile_sign" "profile_verify")

echo "--- Starting Valgrind Profiling ---"

for TASK in "${TASKS[@]}"; do
    if [ -f "./$TASK" ]; then
        echo "Profiling $TASK..."

        valgrind --tool=callgrind \
                 --dump-instr=yes \
                 --simulate-cache=yes \
                 --collect-jumps=yes \
                 --collect-atstart=no \
                 --instr-atstart=no \
                 --callgrind-out-file="${TASK}_callgrind.out" \
                 ./"$TASK"

        echo "Done: Results saved to ${TASK}_callgrind.out"
        echo "------------------------------------------"
    else
        echo "Warning: $TASK executable not found, skipping."
    fi
done

echo "Profiling complete for all tasks."
echo "You can view results with: kcachegrind profile_keygen_callgrind.out"