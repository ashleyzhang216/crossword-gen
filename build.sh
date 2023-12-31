# ==================================================================
# Author: Ashley Zhang (ayz27@cornell.edu)
# Date:   9/6/2023
# Description: build script to build tool w/ Makefile
# ==================================================================

SRCDIR="src"
BUILDDIR="build"
MAINFILE="cw_gen"
LOGFILE="buildlog"

cd ${SRCDIR}

# soft clean
rm -rf ${BUILDDIR}
mkdir ${BUILDDIR}

# build
make > ${BUILDDIR}/${LOGFILE}.txt

# clean up
mv *.o ${BUILDDIR}
cd ..
mv ${SRCDIR}/${MAINFILE} ${MAINFILE}