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

module T = {
  type t('a, 'b, 'c, 'd, 'e, 'f) = withInsides('a, 'b, 'c, 'd, 'e, 'f);

  let toTypeName =
    fun
    | `Cdf(_) => `Cdf
    | `Float(_) => `Float
    | `Binary(_) => `Binary
    | `Percentage(_) => `Percentage
    | `UnresolvableResolution(_) => `UnresolvableResolution
    | `Comment(_) => `Comment;

  let toCdf = (t: t('a, 'b, 'c, 'd, 'e, 'f)) =>
    switch (t) {
    | `Cdf(a) => Some(a)
    | _ => None
    };

  let toFloat = (t: t('a, 'b, 'c, 'd, 'e, 'f)) =>
    switch (t) {
    | `Float(a) => Some(a)
    | _ => None
    };
  let toBinary = (t: t('a, 'b, 'c, 'd, 'e, 'f)) =>
    switch (t) {
    | `Binary(a) => Some(a)
    | _ => None
    };
  let toPercentage = (t: t('a, 'b, 'c, 'd, 'e, 'f)) =>
    switch (t) {
    | `Percentage(a) => Some(a)
    | _ => None
    };
  let toUnresolvable = (t: t('a, 'b, 'c, 'd, 'e, 'f)) =>
    switch (t) {
    | `UnresolvableResolution(a) => Some(a)
    | _ => None
    };
  let toComment = (t: t('a, 'b, 'c, 'd, 'e, 'f)) =>
    switch (t) {
    | `Comment(a) => Some(a)
    | _ => None
    };

  let isX = (typeName, t: t('a, 'b, 'c, 'd, 'e, 'f)) =>
    toTypeName(t) === typeName;
  let isCdf = t => isX(`Cdf, t);
  let isFloat = t => isX(`Float, t);
  let isBinary = t => isX(`Binary, t);
  let isPercentage = t => isX(`Percentage, t);
  let isUnresolvable = t => isX(`UnresolvableResolution, t);
  let isComment = t => isX(`Comment, t);
};

module Name = {
  type t = withoutInsides;

  let toWrapperFn = (t: t) =>
    switch (t) {
    | `Cdf => (r => `Cdf(r))
    | `Float => (r => `Float(r))
    | `Binary => (r => `Binary(r))
    | `Percentage => (r => `Percentage(r))
    | `UnresolvableResolution => (r => `UnresolvableResolution(r))
    | `Comment => (r => `Comment(r))
    };

  let toIsFn = (t: t) =>
    T.(
      switch (t) {
      | `Cdf => isCdf
      | `Float => isFloat
      | `Binary => isBinary
      | `Percentage => isPercentage
      | `UnresolvableResolution => isUnresolvable
      | `Comment => isComment
      }
    );

  let toString =
    fun
    | `Cdf => "floatCdf"
    | `Float => "floatPoint"
    | `Binary => "binary"
    | `Percentage => "percentage"
    | `UnresolvableResolution => "unresolvableResolution"
    | `Comment => "comment";

  let fromString = str =>
    switch (str) {
    | "floatPoint" => Belt.Result.Ok(`Float)
    | "floatCdf" => Ok(`Cdf)
    | "percentage" => Ok(`Percentage)
    | "binary" => Ok(`Binary)
    | "unresolvableResolution" => Ok(`UnresolvableResolution)
    | "comment" => Ok(`Comment)
    | _ => Error("Measurement datatype value not valid: " ++ str)
    };
};