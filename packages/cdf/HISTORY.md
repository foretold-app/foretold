# 1.0.14 (2019/11/9)

Fixes

- The Samples \_kde method filters out all values outside of the stated min and max, before processing with pdfast.
- Made `createUniform` a static method of the CDF class. Hopefully this doesn't break anything in other libraries.
