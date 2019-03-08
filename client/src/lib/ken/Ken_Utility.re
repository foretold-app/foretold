open Rationale.Function.Infix;

let log2 = (v1, v2) => {
  Js.log2(v1, v2);
  v2;
};

let isEqual = (a, b) => a == b;

let isNotEqual = (a, b) => a != b;

let rec accumulator =
        (~history: list('b), ~accum: 'b, f: ('b, 'a) => 'b, l: list('a)) =>
  switch (l) {
  | [] => history
  | [head, ...tail] =>
    let newel = f(accum, head);
    accumulator(
      ~accum=newel,
      ~history=List.append(history, [newel]),
      f,
      tail,
    );
  };

let asList = fn => Array.to_list ||> fn ||> Array.of_list;
let asArray = fn => Array.of_list ||> fn ||> Array.to_list;