# Profiling the LESS reference implementation

Scripts to profile the LESS reference implementation using Valgrind/Callgrind.

## Setup
```bash
sudo apt install valgrind kcachegrind make gcc
```

## Commands

Run the automation script to compile and profile our three key functions: **KeyGen**, **Sign**, and **Verify**. The output from this command gives an information instruction count for each function.

```bash
chmod +x profile_valgrind.sh
./profile_valgrind.sh
```

To view the top bottleneck functions for a specific phase, run:

```bash
callgrind_annotate --auto=yes profile_sign_callgrind.out
callgrind_annotate --auto=yes profile_verify_callgrind.out
```

Callgrind also allows opening the results in a GUI to explore the call tree:

```bash
kcachegrind profile_sign_callgrind.out
kcachegrind profile_verify_callgrind.out
```

## Configuration

To change security categories or performance targets, edit the variables at the top of the `Makefile`:

* **`CAT_DEF`**: Sets the security category based on the parameter $N$.
    * `-DCATEGORY=252` (Category 1)
    * `-DCATEGORY=400` (Category 3)
    * `-DCATEGORY=548` (Category 5)
* **`TARGET_DEF`**: Sets the optimization goal (High Speed vs. Small Signature).
    * Category 1: `-DTARGET=192` (Speed), `68` (Balanced), or `45` (Size)
    * Category 3: `-DTARGET=220` (Speed) or `102` (Size)
    * Category 5: `-DTARGET=345` (Speed) or `137` (Size)

---
### Parameter Selection Reference
| Goal | Category 1 | Category 3 | Category 5 |
| :--- | :--- | :--- | :--- |
| **High Speed** | CAT=252, TGT=192 | CAT=400, TGT=220 | CAT=548, TGT=345 |
| **Small Signature** | CAT=252, TGT=45 | CAT=400, TGT=102 | CAT=548, TGT=137 |

To remove all generated binaries, object files, and profile data:

```bash
make clean
```
