type withInsides('a, 'b, 'c, 'd, 'e, 'f) = [
  | `Cdf('a)
  | `Float('b)
  | `Binary('c)
  | `Percentage('d)
  | `UnresolvableResolution('e)
  | `Comment('f)
];

type withoutInsides = [
  | `Cdf
  | `Float
  | `Binary
  | `Percentage
  | `UnresolvableResolution
  | `Comment
];

module T: {
  type t('a, 'b, 'c, 'd, 'e, 'f) = withInsides('a, 'b, 'c, 'd, 'e, 'f);
  let toTypeName: withInsides('a, 'b, 'c, 'd, 'e, 'f) => withoutInsides;
  let toCdf: t('a, 'b, 'c, 'd, 'e, 'f) => option('a);
  let toFloat: t('a, 'b, 'c, 'd, 'e, 'f) => option('b);
  let toBinary: t('a, 'b, 'c, 'd, 'e, 'f) => option('c);
  let toPercentage: t('a, 'b, 'c, 'd, 'e, 'f) => option('d);
  let toUnresolvable: t('a, 'b, 'c, 'd, 'e, 'f) => option('e);
  let toComment: t('a, 'b, 'c, 'd, 'e, 'f) => option('f);
};

module Name: {
  type t = withoutInsides;
  let fromString: string => Belt.Result.t(t, string);
  let toString: t => string;
  let toIsFn: (t, T.t('a, 'b, 'c, 'd, 'e, 'f)) => bool;
  let toWrapperFn: (t, 'a) => T.t('a, 'a, 'a, 'a, 'a, 'a);
};