#!/bin/bash
# Get the absolute path of the project root
export PROJECT_ROOT=$(pwd)

# 1. Save the original PATH so we can restore it later
#    (Only if we haven't already saved it, to prevent nested messing up)
if [ -z "$_OLD_PATH" ]; then
    export _OLD_PATH="$PATH"
fi


# Add the local ARM GCC bin folder to the FRONT of the system PATH
export PATH="$PROJECT_ROOT/tools/gcc/bin:$PATH"

# Optional: Add OpenOCD to PATH too, for convenience
export PATH="$PROJECT_ROOT/tools/openocd/bin:$PATH"

echo "Environment activated! Using local tools."
arm-none-eabi-gcc --version

echo "Environment activated."
echo "Type 'deactivate' to restore your path."
# 4. Define the 'deactivate' function
deactivate() {
    if [ -n "$_OLD_PATH" ]; then
        export PATH="$_OLD_PATH"
        unset _OLD_PATH
        unset PROJECT_ROOT
        unset -f deactivate
        echo "Environment deactivated."
    fi
}
