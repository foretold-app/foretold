type state = {selectedMeasurableId: option(string)};

type action =
  | SelectMeasurableId(string)
  | DeselectMeasurableId;

type notebookReducer = {
  state,
  dispatch: action => unit,
};