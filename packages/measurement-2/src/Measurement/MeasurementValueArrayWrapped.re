type time = float;

type t =
  MeasurementValueWrapper.T.t(
    array(Cdf.t),
    array(float),
    array(bool),
    array(Percentage.t),
    array(MeasurementValue.UnresolvableResolution.t),
    array(MeasurementValue.Comment.t),
  );

// type tFn('a, 'b, 'c, 'd, 'e, 'f, 'g, 'h, 'i, 'j, 'k, 'l) = {
//   cdf: 'a => 'g,
//   float: 'b => 'h,
//   binary: 'c => 'i,
//   percentage: 'd => 'j,
//   unresolvableResolution: 'e => 'k,
//   comment: 'f => 'l,
// };

let length = (~t: t) =>
  switch (t) {
  | `Cdf(a) => Array.length(a)
  | `Float(a) => Array.length(a)
  | `Binary(a) => Array.length(a)
  | `Percentage(a) => Array.length(a)
  | `UnresolvableResolution(a) => Array.length(a)
  | `Comment(a) => Array.length(a)
  } /* }*/;

// let first = (fn: 'a => 'b, t: t) =>
//   switch (t) {
//   | `Cdf(a) => `Cdf(fn(a))
//   | `Float(a) => `Float(fn(a))
//   | `Binary(a) => a |> Belt.Array.get(_, 0)
//   | `Percentage(a) => a |> Belt.Array.get(_, 0)
//   | `UnresolvableResolution(a) => a |> Belt.Array.get(_, 0)
//   | `Comment(a) => a |> Belt.Array.get(_, 0)

// type aOrB('a, 'b) =
//   | A('a)
//   | B('b);

// type three = aOrB(float, int);

// let biMap = (a: aOrB('a, 'b), fn1: 'a => 'c, fn2: 'b => 'd) =>
//   switch (a) {
//   | A(a) => A(fn1(a))
//   | B(a) => B(fn2(a))
//   };

// // let foo = A(3);
// let foo: aOrB(int, string) = A(3);
// let bar = biMap(foo, _ => 5, _ => "hi");

// let map10: (aOrB(array('a), array('b)), array('f) => 'c) => aOrB('c, 'c) =
//   (a, fn1) =>
//     switch (a) {
//     | A(a) => A(fn1(a))
//     | B(a) => A(fn1(a))
//     };

// let foo: aOrB(array(string), array(float)) = A([|"k"|]);
// let bar = map10(foo, _ => 3.0);

// let map2 = (a: aOrB('a, 'b), fn1: ('a => 'c, 'b => 'c)) => {
//   let (f1, f2) = fn1;
//   switch (a) {
//   | A(a) => A(f1(a))
//   | B(a) => A(f2(a))
//   };
// };

// type arFn('a, 'b) = array('a) => 'b;

// let map5 = (a: aOrB(array('a), array('b)), fn1: array('c) => 'd) => {
//   switch (a) {
//   | A(a) => A(fn1(a))
//   | B(a) => A(fn1(a))
//   };
// };

// let map3 = (fn1: array('c) => int, c: aOrB(array('a), array('d))) => {
//   switch (c) {
//   | A(a) => A(fn1(a))
//   | B(a) => B(fn1(a)) /* let foo = map3(Array.length)*/
//   };