# Custom SoC for the LESS hardware accelerator

The design of this SoC follows the [standard LiteX design flow](https://github.com/enjoy-digital/litex/wiki/LiteX-for-Hardware-Engineers).

There are 3 keys scripts in the build procedure:

1. **customSoC.py** extends the *SoCCore* class using the migen toolbox and LiteX IP to describe the components and specs for our custom SoC.

2. **customPlatform.py** extends the *Platform* class to describe the build environment for our custom SoC.

3. the main function in **main.py** passes our custom Platform object to a Builder object to generate the **hw_customSoC/build** folder.
