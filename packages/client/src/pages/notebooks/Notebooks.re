open Style.Grid;

module ReducerConfig = {
  type itemType = Types.notebook;
  type callFnParams = string;

  let getId = (e: itemType) => e.id;

  let onItemDeselected = _ => ();
  let onItemSelected = (_: option(itemType)) => ();

  let callFn = (channelId: callFnParams) =>
    NotebooksGet.component(~channelId);

  let isEqual = (a: itemType, b: itemType) => {
    a.id == b.id;
  };
};

module Reducer = PaginationFunctor.Make(ReducerConfig);

module Pagination = {
  [@react.component]
  let make = (~reducerParams: Reducer.state, ~channelId: string) => {
    let context = React.useContext(Providers.app);
    <Div>
      {<Div
         float=`left
         styles=[
           Css.style([
             ForetoldComponents.PageCard.HeaderRow.Styles.itemTopPadding,
             ForetoldComponents.PageCard.HeaderRow.Styles.itemBottomPadding,
           ]),
         ]>
         <ForetoldComponents.Base.Button
           variant=ForetoldComponents.Base.Button.Primary
           size=ForetoldComponents.Base.Button.Small
           onClick={e =>
             LinkType.onClick(Internal(ChannelAddNotebook(channelId)), e)
           }>
           {"New Notebook" |> Utils.ste}
         </ForetoldComponents.Base.Button>
       </Div>
       |> Primary.User.authorized(context.loggedUser)
       |> E.React2.showIf(channelId != "")}
      <Div
        float=`right
        styles=[
          Css.style([
            ForetoldComponents.PageCard.HeaderRow.Styles.itemTopPadding,
            ForetoldComponents.PageCard.HeaderRow.Styles.itemBottomPadding,
          ]),
        ]>
        <Reducer.Components.PaginationPage state=reducerParams />
      </Div>
    </Div>;
  };
};

[@react.component]
let make = (~channelId: string) => {
  let subComponent = (reducerParams: Reducer.state) => {
    let items =
      switch (reducerParams.response) {
      | Success(connection) => connection.edges
      | _ => [||]
      };

    let isFound = Array.length(items) > 0;

    let table =
      switch (channelId) {
      | "" => <NotebooksTable items />
      | _ => <NotebooksTable items columns=NotebooksTable.Columns.short />
      };

    let body =
      switch (reducerParams.response, isFound) {
      | (Success(_), true) =>
        <ForetoldComponents.PageCard.Body>
          table
        </ForetoldComponents.PageCard.Body>
      | (Success(_), false) => <NothingToShow />
      | _ => <Spin />
      };

    let head = <Pagination reducerParams channelId />;

    <SLayout head> body </SLayout>;
  };

  <Reducer itemsPerPage=20 callFnParams=channelId subComponent />;
};