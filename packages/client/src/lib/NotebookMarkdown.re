[@bs.config {jsx: 3}];
open Style.Grid;

type code = {
  .
  "language": Js.nullable(string),
  "value": Js.nullable(string),
};

type renderers = {. "code": code => ReasonReact.reactElement};

let reducerToEditor =
    (notebookReducer: NotebookRedux.t): DashboardTableEditor.editor => {
  onSelect: r => notebookReducer.dispatch(SelectMeasurableId(r)),
  selectedId: notebookReducer.state.selectedMeasurableId,
};

type block =
  | Markdown(string)
  | Json(Js.Json.t);
type blocks = array(block);

let markdownToBlocks = (str: string): blocks => {
  let splitUp = Js.String.splitByRe([%re "/```foretoldJs|```/"], str);
  splitUp
  |> E.A.fmap(e =>
       switch (e) {
       | Some(str) =>
         switch (Json.parse(str)) {
         | Some(json) => Some(Json(json))
         | None => Some(Markdown(str))
         }
       | _ => None
       }
     )
  |> E.A.O.concatSome;
};

[@react.component]
let make = (~blocks: blocks, ~notebookRedux: NotebookRedux.t) => {
  let head = (~channelId: option(string), ~paginationPage, ()) => ReasonReact.null;

  <Div flexDirection=`row>
    <Div flex={`num(5.)}>
      <div className=Markdown.Styles.all>
        {blocks
         |> E.A.fmap(e =>
              switch (e) {
              | Markdown(str) => <ReactMarkdown source=str />
              | Json(json) =>
                <div
                  className=Css.(
                    style([
                      marginTop(`em(1.0)),
                      marginBottom(`em(1.5)),
                      Css.float(`left),
                      width(`percent(100.)),
                    ])
                  )>
                  <DashboardTableC
                    tableJson=json
                    editor={reducerToEditor(notebookRedux)}
                  />
                </div>
              }
            )
         |> ReasonReact.array}
      </div>
    </Div>
  </Div>;
};