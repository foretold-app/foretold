[@bs.config {jsx: 3}];

open Style.Grid;

module ReducerConfig = {
  type itemType = Types.notebook;
  type callFnParams = string;

  let getId = (e: itemType) => e.id;
  let callFn = (channelId: callFnParams) =>
    NotebooksGet.component(~channelId);

  let isEqual = (a: itemType, b: itemType) => {
    a.id == b.id;
  };
};

module Reducer = PaginationFunctor.Make(ReducerConfig);

let pagination =
    (reducerParams: Reducer.Types.reducerParams, channelId: string) =>
  <Div>
    <Providers.AppContext.Consumer>
      ...{({loggedUser}) =>
        Primary.User.authorized(
          loggedUser,
          <Div
            float=`left
            styles=[
              Css.style([
                FC.PageCard.HeaderRow.Styles.itemTopPadding,
                FC.PageCard.HeaderRow.Styles.itemBottomPadding,
              ]),
            ]>
            <FC.Base.Button
              variant=FC.Base.Button.Primary
              size=FC.Base.Button.Small
              onClick={e =>
                LinkType.onClick(Internal(ChannelAddNotebook(channelId)), e)
              }>
              {"New Notebook" |> Utils.ste}
            </FC.Base.Button>
          </Div>,
        )
      }
    </Providers.AppContext.Consumer>
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
let make = (~channelId: string) => {
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
          ? <FC.PageCard.Body>
              <NotebooksTable items channelId />
            </FC.PageCard.Body>
          : <NothingToShow />
      | _ => <Spin />
      };

    let head = pagination(reducerParams, channelId);

    <SLayout head> body </SLayout>;
  };

  <Reducer itemsPerPage=20 callFnParams=channelId subComponent />;
};