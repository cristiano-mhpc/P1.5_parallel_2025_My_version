### Profiling using perf with Flame Graphs <img src="https://raw.githubusercontent.com/PokeAPI/sprites/master/sprites/pokemon/78.png" width="50" style="display:inline; vertical-align:middle;"> 



- We use Flame graphs to visualize profiling data collected by a profiler such as `perf`. Flame graphs help identify performance bottlenecks in applications by displaying stack traces in a visually intuitive manner.

What we need:
1. A linux environment (Leonardo, local machine, etc.) 
2. perf
3. Flame Ggraph implementation from Brendan Gregg's [repo](https://github.com/brendangregg/FlameGraph). 
```bash 
git clone git@github.com:brendangregg/FlameGraph.git
```
4. Some codes to profile. 

##### Case 1: Profile the diffusion equation solver serial version. 
