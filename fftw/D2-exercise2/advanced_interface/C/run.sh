#!/bin/bash 

set -euo pipefail 

# ----configuration---- 
APP="./diffusion.x"
PREFIX_2D="concentration"
OUT_GIF="diffusion.gif"
FRAMES_DIR="frames_2d"
PNG_PREFIX="frame"
GNUPLOT_TERM="pngcairo"
SIZE="1200,900" 
DELAY_CS+=40   # ImageMacgick delay (centiseconds perframe) 


#-----cleanup from previous runs ----
# make clean 
# make flush 

# ---- 0) sanity checks ----
need() { command -v "$1" >/dev/null 2>&1 || { echo "Missing $1"; exit 1; }; }
need gnuplot
if ! command -v convert >/dev/null 2>&1 && ! command -v magick >/dev/null 2>&1; then
  echo "Missing ImageMagick (convert or magick)."; exit 1
fi


# # ---- 1) run the simulation(build first if needed) ----
# if [[ ! -x "$APP" ]]; then 
#     echo "Executable $APP not found; building with make..." 
#     make -j
# fi 
# echo "Running the simulation..."
# "$APP" 

# ---- 2) create png Frames with gnuplot ---- 
echo "Generating PNG frames with gnuplot..."
rm -rf "$FRAMES_DIR"
mkdir -p "$FRAMES_DIR"


# FIND ALL 2D .dat files and sort numerically by the trailing index
mapfile -t DAT2D < <(ls ${PREFIX_2D}_*.dat 2>/dev/null | sed -E 's/.*_([0-9]+)\.dat/\1 &/' | sort -n | cut -d' ' -f2)


if [[ ${#DAT2D[@]} -eq 0 ]]; then 
    echo "No ${PREFIX_2D}_*.dat files found. Did the run produce 2D slices?"
    exit 1
fi 

i=1
for f in "${DAT2D[@]}"; do
  out=$(printf "%s/%s_%04d.png" "$FRAMES_DIR" "$PNG_PREFIX" "$i")
  gnuplot <<-GP
    set terminal ${GNUPLOT_TERM} size ${SIZE}
    set output "${out}"
    set pm3d map
    unset colorbox
    # 'matrix' because plot_data_2d writes a rectangular grid (rows of numbers)
    splot "${f}" matrix with image
GP
  ((i++))
done


# ---- 3) create the animated GIF with ImageMagick --
echo "Creating GIF ${OUT_GIF}..."

convert -delay $DELAY_CS -loop 0 "${FRAMES_DIR}/${PNG_PREFIX}_*.png" "$OUT_GIF"


