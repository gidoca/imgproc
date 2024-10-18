set -e

SCRIPTDIR=$(dirname "$0")
BASEDIR="${SCRIPTDIR}/.."
SRCDIR="${BASEDIR}/src"
BINDIR="${BASEDIR}/bin"
TESTDIR="${BASEDIR}/testdata"
CURRENTTESTDIR="${TESTDIR}/current"

clang++ -std=c++20 -stdlib=libc++ -Wall "${SRCDIR}/write_bmp.cpp" -o ${BINDIR}/write_bmp
${BINDIR}/write_bmp "${CURRENTTESTDIR}/test.bmp"
diff "${CURRENTTESTDIR}/test.bmp" "${TESTDIR}/test.bmp"

clang++ -std=c++20 -stdlib=libc++ -Wall "${SRCDIR}/mandelbrot.cpp" -o ${BINDIR}/write_mandelbrot
${BINDIR}/write_mandelbrot "${CURRENTTESTDIR}/mandelbrot.bmp"
diff "${CURRENTTESTDIR}/mandelbrot.bmp" "${TESTDIR}/mandelbrot.bmp"

clang++ -std=c++20 -stdlib=libc++ -Wall -O2 "${SRCDIR}/image_test.cpp" -o ${BINDIR}/image_test
${BINDIR}/image_test

