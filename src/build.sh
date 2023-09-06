# ==================================================================
# Author: Ashley Zhang (ayz27@cornell.edu)
# Date:   9/6/2023
# Description: build script to build tool w/ Makefile and run
# ==================================================================

BUILDDIR="build"
MAINFILE="cw_gen"

# soft clean
rm -rf ${BUILDDIR}
mkdir ${BUILDDIR}

# build
make

# clean up
mv *.o ${BUILDDIR}
mv ${MAINFILE} ${BUILDDIR}

# run
echo "Running"
${BUILDDIR}/${MAINFILE} $1 $2 $3