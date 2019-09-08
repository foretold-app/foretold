module T = {
  type t('a, 'b, 'c, 'd, 'e, 'f) = [
    | `Cdf('a)
    | `Float('b)
    | `Binary('c)
    | `Percentage('d)
    | `UnresolvableResolution('e)
    | `Comment('f)
  ];

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
  let toUnresolvableResolution = (t: t('a, 'b, 'c, 'd, 'e, 'f)) =>
    switch (t) {
    | `UnresolvableResolution(a) => Some(a)
    | _ => None
    };
  let toComment = (t: t('a, 'b, 'c, 'd, 'e, 'f)) =>
    switch (t) {
    | `Comment(a) => Some(a)
    | _ => None
    };

  let isCdf = (t: t('a, 'b, 'c, 'd, 'e, 'f)): bool =>
    t |> toCdf |> Rationale.Option.isSome;

  let isFloat = (t: t('a, 'b, 'c, 'd, 'e, 'f)): bool =>
    t |> toFloat |> Rationale.Option.isSome;

  let isBinary = (t: t('a, 'b, 'c, 'd, 'e, 'f)): bool =>
    t |> toBinary |> Rationale.Option.isSome;

  let isPercentage = (t: t('a, 'b, 'c, 'd, 'e, 'f)): bool =>
    t |> toPercentage |> Rationale.Option.isSome;

  let isComment = (t: t('a, 'b, 'c, 'd, 'e, 'f)): bool =>
    t |> toComment |> Rationale.Option.isSome;
};

module TypeName = {
  type t = [
    | `Cdf
    | `Float
    | `Binary
    | `Percentage
    | `UnresolvableResolution
    | `Comment
  ];

  let toWrapperFn = (t: t) =>
    switch (t) {
    | `Cdf => (r => `Cdf(r))
    | `Float => (r => `Float(r))
    | `Binary => (r => `Binary(r))
    | `Percentage => (r => `Percentage(r))
    | `UnresolvableResolution => (r => `UnresolvableResolution(r))
    | `Comment => (r => `Comment(r))
    };

  let fromType = (t: T.t('a, 'b, 'c, 'd, 'e, 'f)): t =>
    switch (t) {
    | `Cdf(_) => `Cdf
    | `Float(_) => `Float
    | `Binary(_) => `Binary
    | `Percentage(_) => `Percentage
    | `UnresolvableResolution(_) => `UnresolvableResolution
    | `Comment(_) => `Comment
    };
};