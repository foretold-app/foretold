[@genType]
type withInsides('a, 'b, 'c, 'd, 'e, 'f) = [
  | `Cdf('a)
  | `Float('b)
  | `Binary('c)
  | `Percentage('d)
  | `UnresolvableResolution('e)
  | `Comment('f)
];

[@genType]
type withoutInsides = [
  | `Cdf
  | `Float
  | `Binary
  | `Percentage
  | `UnresolvableResolution
  | `Comment
];

module T: {
  [@genType]
  type t('a, 'b, 'c, 'd, 'e, 'f) = withInsides('a, 'b, 'c, 'd, 'e, 'f);
  [@genType]
  let toTypeName: withInsides('a, 'b, 'c, 'd, 'e, 'f) => withoutInsides;
  [@genType]
  let toCdf: t('a, 'b, 'c, 'd, 'e, 'f) => option('a);
  [@genType]
  let toFloat: t('a, 'b, 'c, 'd, 'e, 'f) => option('b);
  [@genType]
  let toBinary: t('a, 'b, 'c, 'd, 'e, 'f) => option('c);
  [@genType]
  let toPercentage: t('a, 'b, 'c, 'd, 'e, 'f) => option('d);
  [@genType]
  let toUnresolvable: t('a, 'b, 'c, 'd, 'e, 'f) => option('e);
  [@genType]
  let toComment: t('a, 'b, 'c, 'd, 'e, 'f) => option('f);
};

module Name: {
  [@genType]
  type t = withoutInsides;
  [@genType]
  let fromString: string => Belt.Result.t(t, string);
  [@genType]
  let toString: t => string;
  [@genType]
  let toIsFn: (t, T.t('a, 'b, 'c, 'd, 'e, 'f)) => bool;
  [@genType]
  let toWrapperFn: (t, 'a) => T.t('a, 'a, 'a, 'a, 'a, 'a);
};