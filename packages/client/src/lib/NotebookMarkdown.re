[@bs.config {jsx: 3}];
open MarkdownEditorActions;
open Style.Grid;

type code = {
  .
  "language": Js.nullable(string),
  "value": Js.nullable(string),
};

type renderers = {. "code": code => ReasonReact.reactElement};

let reducerToEditor =
    (notebookReducer: notebookReducer): DashboardTableEditor.editor => {
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

module Styles = {
  open Css;
  let sidebarOutside =
    style([background(`hex("f0f2f5")), paddingLeft(`em(1.0))]);
  let sidebar = style([position(`sticky), top(`em(2.0)), float(`left)]);
};
[@react.component]
let make = (~blocks: blocks) => {
  let (state, dispatch) =
    React.useReducer(
      (state, action) =>
        switch (action) {
        | SelectMeasurableId(id) => {selectedMeasurableId: Some(id)}
        | DeselectMeasurableId => {selectedMeasurableId: None}
        },
      {selectedMeasurableId: None},
    );

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
                    editor={reducerToEditor({state, dispatch})}
                  />
                </div>
              }
            )
         |> ReasonReact.array}
      </div>
    </Div>
    {state.selectedMeasurableId
     |> E.O.React.fmapOrNull(id =>
          <Div flex={`num(3.)} styles=[Styles.sidebarOutside]>
            {MeasurableGet.component(~id)
             |> E.F.apply((measurable: Types.measurable) => {
                  let defaultValueText =
                    measurable.recentMeasurement
                    |> E.O.bind(_, (r: Types.measurement) => r.valueText)
                    |> E.O.default("");
                  <div className=Styles.sidebar>
                    <SLayout
                      head={head(
                        ~channelId=None,
                        ~paginationPage=E.React2.null,
                        (),
                      )}
                      isFluid=true>
                      <FC.PageCard.Body>
                        <MeasurementForm
                          measurable
                          measurableId={measurable.id}
                          isCreator=false
                          defaultValueText
                          key={measurable.id}
                        />
                      </FC.PageCard.Body>
                    </SLayout>
                    <MeasurableBottomSection
                      measurableId={measurable.id}
                      channelId=None
                      key={measurable.id}
                    />
                  </div>;
                })}
          </Div>
        )}
  </Div>;
};