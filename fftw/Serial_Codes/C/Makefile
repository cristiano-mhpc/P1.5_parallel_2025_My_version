CC ?= gcc
CFLAGS ?= -O3 -g -Wall -I$(FFTW_INC)
LIBS ?= -L$(FFTW_LIB) -lfftw3 -lm

EXE  = diffusion.x
SRC  = diffusion.c plot_data.c fft_wrapper.c derivative.c
HEAD = utilities.h
OBJ  = $(SRC:.c=.o)

.PHONY: all clean flush distclean run

all: $(EXE)

$(EXE): $(OBJ)
	$(CC) $(OBJ) $(LIBS) -o $@

%.o: %.c $(HEAD)
	$(CC) $(CFLAGS) -c $< -o $@

run: $(EXE)
	./$(EXE)

# Clean generated data/ and frames/, plus common artifacts
flush:
	@rm -f data/concentration_*.dat data/1d_conc_*.dat data/diffusivity_*.dat || true
	@rm -rf frames_2d || true
	@rm -f flame*.svg out.folded perf.data* *.gif *.png || true

clean:
	@rm -f *.o *.x *~ core core.* vgcore.*

distclean: clean flush

