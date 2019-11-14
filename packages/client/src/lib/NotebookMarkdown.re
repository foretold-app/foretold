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

let foretoldJsRenderers = (~notebookReducer: notebookReducer) => {
  "code": (code: code) => {
    switch (
      Js.Nullable.toOption(code##language),
      Js.Nullable.toOption(code##value),
    ) {
    | (Some("foretoldJs"), Some(json)) =>
      switch (Json.parse(json)) {
      | Some(json) =>
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
            editor={reducerToEditor(notebookReducer)}
          />
        </div>
      | None => "Invalid Json. Check a formatting tool." |> Utils.ste
      }
    | (Some(language), Some(value)) =>
      <code className=language> {value |> Utils.ste} </code>
    | (None, Some(value)) => value |> Utils.ste
    | (_, None) => E.React2.null
    };
  },
};

[@react.component]
let make = (~source) => {
  let (state, dispatch) =
    React.useReducer(
      (state, action) =>
        switch (action) {
        | SelectMeasurableId(id) => {selectedMeasurableId: Some(id)}
        | DeselectMeasurableId => {selectedMeasurableId: None}
        },
      {selectedMeasurableId: None},
    );

  <Div flexDirection=`row>
    <Div flex={`num(5.)}>
      <div className=Markdown.Styles.all>
        <ReactMarkdown
          source
          renderers={foretoldJsRenderers(~notebookReducer={state, dispatch})}
        />
      </div>
    </Div>
    {state.selectedMeasurableId
     |> E.O.React.fmapOrNull(id =>
          <Div flex={`num(3.)}>
            {MeasurableGet.component(~id)
             |> E.F.apply((measurable: Types.measurable) => {
                  let defaultValueText =
                    measurable.recentMeasurement
                    |> E.O.bind(_, (r: Types.measurement) => r.valueText)
                    |> E.O.default("");
                  <div>
                    <MeasurementForm
                      measurable
                      measurableId={measurable.id}
                      isCreator=false
                      defaultValueText
                    />
                    <MeasurableBottomSection
                      measurableId={measurable.id}
                      channelId=None
                    />
                  </div>;
                })}
          </Div>
        )}
  </Div>;
};