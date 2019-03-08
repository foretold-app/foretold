type route =
  | Home
  | AgentIndex
  | Redirect
  | Profile(string)
  | ItemShow(string)
  | AgentShow(string)
  | AgentMeasurables(string)
  | Channel(string)
  | MeasurableShow(string, string)
  | MeasurableEdit(string)
  | MeasurableNew(string)
  | NotFound;