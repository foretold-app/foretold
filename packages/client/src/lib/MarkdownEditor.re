open MarkdownEditorActions;

[@react.component]
let make = () => {
  let (state, dispatch) =
    React.useReducer(
      (state, action) =>
        switch (action) {
        | SelectMeasurableId(id) => {selectedMeasurableId: Some(id)}
        | DeselectMeasurableId => {selectedMeasurableId: None}
        },
      {selectedMeasurableId: None},
    );

  <div> {ReasonReact.string("sdf")} </div>;
};