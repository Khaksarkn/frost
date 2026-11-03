# InterFrost TH2

Thawing of a frozen inclusion. The upper half of the domain is modelled by
symmetry. Multiply the `functor_matprop` column of the CSV output by 2 for
the total water volume.

| File | Role |
| --- | --- |
| `interfrost_TH2_noGradient.i` | Zero head gradient |
| `interfrost_TH2_gradient3pct.i` | 3 % head gradient, restarts from the file below |
| `interfrost_TH2_gradient3pct_init.e` | Initial state for the 3 % case |

Outputs: `*_csv1.csv` (minimum temperature and water content at every step)
and Exodus files with the temperature at t = 0 and t = 20 000 s.
