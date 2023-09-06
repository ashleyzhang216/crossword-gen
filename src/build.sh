# ==================================================================
# Author: Ashley Zhang (ayz27@cornell.edu)
# Date:   9/6/2023
# Description: build script to build tool w/ Makefile and run
# ==================================================================

BUILDDIR="build"
MAINFILE="cw_gen"
LOGFILE="buildlog"

# soft clean
rm -rf ${BUILDDIR}
mkdir ${BUILDDIR}

# build
make > ${BUILDDIR}/${LOGFILE}.txt

# clean up
mv *.o ${BUILDDIR}
mv ${MAINFILE} ${BUILDDIR}

# run
${BUILDDIR}/${MAINFILE} $1 $2 $3