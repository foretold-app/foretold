[@bs.config {jsx: 3}];

open Style.Grid;

module ReducerConfig = {
  type itemType = Types.notebook;
  type callFnParams = option(string);

  let getId = (e: itemType) => e.id;
  let callFn = (channelId: callFnParams) =>
    NotebooksGet.component(~channelId);

  let isEqual = (a: itemType, b: itemType) => {
    a.id == b.id;
  };
};

module Reducer = PaginationFunctor.Make(ReducerConfig);

let pagination = (reducerParams: Reducer.Types.reducerParams) =>
  <Div>
    <Div
      float=`right
      styles=[
        Css.style([
          FC.PageCard.HeaderRow.Styles.itemTopPadding,
          FC.PageCard.HeaderRow.Styles.itemBottomPadding,
        ]),
      ]>
      {Reducer.Components.paginationPage(reducerParams)}
    </Div>
  </Div>;

[@react.component]
let make = (~channelId=None) => {
  let subComponent = (reducerParams: Reducer.Types.reducerParams) => {
    let items =
      switch (reducerParams.response) {
      | Success(connection) => connection.edges
      | _ => [||]
      };

    let isFound = Array.length(items) > 0;

    let body =
      switch (reducerParams.response) {
      | Success(_) =>
        isFound
          ? <FC.PageCard.Body> <NotebooksTable items /> </FC.PageCard.Body>
          : <NothingToShow />
      | _ => <Spin />
      };

    let head = isFound ? pagination(reducerParams) : ReasonReact.null;

    <SLayout head isFluid=true> body </SLayout>;
  };

  <Reducer itemsPerPage=20 callFnParams=channelId subComponent />;
};
