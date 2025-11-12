#!/usr/bin/env bash
set -euo pipefail

# ---- config ----
APP="./diffusion.x"
DATA_DIR="data"
PREFIX_2D="concentration"   # files like data/concentration_1.dat, ...
OUT_GIF="diffusion.gif"
FRAMES_DIR="frames_2d"
PNG_PREFIX="frame"
GNUPLOT_TERM="pngcairo"
SIZE="1200,900"
DELAY_CS=40                 # centiseconds per frame (fixed: was +=)

# ---- sanity checks (do GIF only if tools exist) ----
need() { command -v "$1" >/dev/null 2>&1 || { echo "Missing $1" >&2; exit 1; }; }
need gnuplot

# ---- build & run ----
if [[ ! -x "$APP" ]]; then
  echo "Executable $APP not found; building with make..."
  make -j
fi

echo "Running the simulation..."
# ensure data dir exists (your code should mkdir already, but belt & suspenders)
mkdir -p "$DATA_DIR"
"$APP"

# ---- frames via gnuplot ----
echo "Generating PNG frames with gnuplot..."
rm -rf "$FRAMES_DIR"
mkdir -p "$FRAMES_DIR"

# find all 2D .dat files in data/ and sort numerically by trailing index
mapfile -t DAT2D < <(
  ls "${DATA_DIR}/${PREFIX_2D}_"*.dat 2>/dev/null \
    | sed -E 's/.*_([0-9]+)\.dat/\1 &/' \
    | sort -n \
    | cut -d' ' -f2
)

if [[ ${#DAT2D[@]} -eq 0 ]]; then
  echo "No ${DATA_DIR}/${PREFIX_2D}_*.dat files found. Did the run produce 2D slices?"
  exit 1
fi

i=1
for f in "${DAT2D[@]}"; do
  out=$(printf "%s/%s_%04d.png" "$FRAMES_DIR" "$PNG_PREFIX" "$i")
  gnuplot <<-GP
    set terminal ${GNUPLOT_TERM} size ${SIZE}
    set output "${out}"
    unset key
    set view map
    set size ratio -1
    # plot "matrix" (rows of numbers) as an image
    plot "${f}" matrix with image
GP
  ((i++))
done

# ---- animated GIF (prefer magick; fallback convert; else skip politely) ----
if command -v magick >/dev/null 2>&1; then
  echo "Creating GIF ${OUT_GIF} with ImageMagick (magick)..."
  magick -delay "$DELAY_CS" -loop 0 "${FRAMES_DIR}/${PNG_PREFIX}_*.png" "$OUT_GIF"
elif command -v convert >/dev/null 2>&1; then
  echo "Creating GIF ${OUT_GIF} with ImageMagick (convert)..."
  convert -delay "$DELAY_CS" -loop 0 "${FRAMES_DIR}/${PNG_PREFIX}_*.png" "$OUT_GIF"
else
  echo "No ImageMagick found (magick/convert). Skipping GIF; PNG frames are in ${FRAMES_DIR}/"
fi

