type t('a) = array('a);

let make: array('a) => option(array('a));
let concat: (t('a), t('a)) => t('a);
let min: t('a) => option('a);
let max: t('a) => option('a);
let filterOutLessThan: ('a, t('a)) => t('a);
let filterOutGreaterThan: ('a, t('a)) => t('a);