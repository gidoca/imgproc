set -e

SCRIPTDIR=$(dirname "$0")
BASEDIR="${SCRIPTDIR}/.."
BUILDDIR="${BASEDIR}/build"
SRCDIR="${BASEDIR}/src"
BINDIR="${BASEDIR}/bin"
TESTDIR="${BASEDIR}/testdata"
CURRENTTESTDIR="${TESTDIR}/current"

cmake -B "${BUILDDIR}" .
cmake --build "${BUILDDIR}"
cmake --install "${BUILDDIR}" --prefix "${BASEDIR}"

${BINDIR}/write_bmp "${CURRENTTESTDIR}/test.bmp"
diff "${CURRENTTESTDIR}/test.bmp" "${TESTDIR}/test.bmp"

${BINDIR}/write_mandelbrot "${CURRENTTESTDIR}/mandelbrot.bmp"
diff "${CURRENTTESTDIR}/mandelbrot.bmp" "${TESTDIR}/mandelbrot.bmp"

${BINDIR}/image_test
${BINDIR}/image_iterator_test
${BINDIR}/ansi_color_test
${BINDIR}/read_bmp_test

