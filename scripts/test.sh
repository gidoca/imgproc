set -e

SCRIPTDIR=$(dirname "$0")
BASEDIR="${SCRIPTDIR}/.."
BUILDDIR="${BASEDIR}/build"
SRCDIR="${BASEDIR}/src"
BINDIR="${BUILDDIR}/src"
TESTDIR="${BASEDIR}/testdata"
CURRENTTESTDIR="${TESTDIR}/current"

cmake -B "${BUILDDIR}" .
cmake --build "${BUILDDIR}"

${BINDIR}/write_bmp "${CURRENTTESTDIR}/test.bmp"
diff "${CURRENTTESTDIR}/test.bmp" "${TESTDIR}/test.bmp"

${BINDIR}/write_mandelbrot "${CURRENTTESTDIR}/mandelbrot.bmp"
diff "${CURRENTTESTDIR}/mandelbrot.bmp" "${TESTDIR}/mandelbrot.bmp"

${BINDIR}/image_test

