/*
 I'm not a huge fan of this file. This uses a different strategy than I've used
 elsewhere in this codebase. It my be reasonable to refactor later.
 */

type distribution = {
  xs: array(float),
  ys: array(float),
};

type distributions = array(distribution);