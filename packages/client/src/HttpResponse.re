[@bs.config {jsx: 3}];

type t('a) =
  | Loading
  | Error(string)
  | Success('a);

let fromApollo = (b: ReasonApolloTypes.queryResponse('a)) =>
  switch (b) {
  | Loading => Loading
  | Error(e) => Error(e##message)
  | Data(c) => Success(c)
  };

let fmap = (fn: 'a => 'b, result: t('a)): t('b) =>
  switch (result) {
  | Success(response) => Success(fn(response))
  | Error(e) => Error(e)
  | Loading => Loading
  };

let optionalToMissing = (result: t(option('a))): t('b) =>
  switch (result) {
  | Success(Some(response)) => Success(response)
  | Success(None) => Error("Missing Needed Data")
  | Error(e) => Error(e)
  | Loading => Loading
  };

let flatten = (successFn, errorFn, loadingFn, result: t('a)) =>
  switch (result) {
  | Success(response) => successFn(response)
  | Error(e) => errorFn(e)
  | Loading => loadingFn()
  };

let flattenDefault = (default, successFn: 'a => 'b) =>
  flatten(successFn, _ => default, () => default);

let isSuccess = (result: t('a)) =>
  switch (result) {
  | Success(_) => true
  | _ => false
  };

/* Useful after applying fmap to produce reactElement*/
let withReactDefaults = (result: t(ReasonReact.reactElement)) =>
  switch (result) {
  | Success(response) => response
  | Error(e) => <div> {"Error: " ++ e |> ReasonReact.string} </div>
  | Loading => <div> {"Loading..." |> ReasonReact.string} </div>
  };

let merge2 = (a: t('a), b: t('b)) =>
  switch (a, b) {
  | (Error(a), _) => Error(a)
  | (_, Error(b)) => Error(b)
  | (Loading, _) => Loading
  | (_, Loading) => Loading
  | (Success(a), Success(b)) => Success((a, b))
  };

let merge3 = (a: t('a), b: t('b), c: t('c)) =>
  switch (merge2(a, b), c) {
  | (Success((a, b)), Success(c)) => Success((a, b, c))
  | (Error(a), _) => Error(a)
  | (_, Error(b)) => Error(b)
  | (Loading, _) => Loading
  | (_, Loading) => Loading
  };

let isEq = (a: t('a), b: t('a), isEqual) =>
  switch (a, b) {
  | (Success(a), Success(b)) => isEqual(a, b)
  | (Loading, Loading) => true
  | (Error(a), Error(b)) => a == b
  | (_, _) => false
  };
