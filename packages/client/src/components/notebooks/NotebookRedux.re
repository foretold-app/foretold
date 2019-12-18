type state = {selectedMeasurableId: option(string)};

type action =
  | SelectMeasurableId(string)
  | DeselectMeasurableId;

type t = {
  state,
  dispatch: action => unit,
};

let reducer = () => {
  let (state, dispatch) =
    React.useReducer(
      (_, action) =>
        switch (action) {
        | SelectMeasurableId(measurableId) => {
            selectedMeasurableId: Some(measurableId),
          }
        | DeselectMeasurableId => {selectedMeasurableId: None}
        },
      {selectedMeasurableId: None},
    );
  {state, dispatch};
};