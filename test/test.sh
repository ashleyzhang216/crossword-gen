# ==================================================================
# Author: Ashley Zhang (ayz27@cornell.edu)
# Date:   8/13/2023
# Description: test script to build project w/ Makefile and test
# ==================================================================

BUILDDIR="build"
TESTFILE="cw_test_driver"

echo "Running crossword test"

# soft clean
rm -rf ${BUILDDIR}
mkdir ${BUILDDIR}

# buildc
echo "Building"
make

# clean up
echo "Moving build files"
mv *.o ${BUILDDIR}
mv ${TESTFILE} ${BUILDDIR}

# run
echo "Running"
${BUILDDIR}/${TESTFILE} $1
