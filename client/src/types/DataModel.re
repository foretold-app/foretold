let stringOfcompetitorType = e =>
  switch (e) {
  | `AGGREGATION => "Aggregation"
  | `COMPETITIVE => "Competitive"
  | `OBJECTIVE => "Objective"
  };

type user = {
  id: string,
  name: string,
};

type bot = {
  competitorType: [ | `AGGREGATION | `COMPETITIVE | `OBJECTIVE],
  description: option(string),
  id: string,
  name: option(string),
};

type series = {
  id: string,
  description: option(string),
  name: option(string),
};

let toSeries = (~id, ~name=None, ~description=None, ()) => {
  id,
  name,
  description,
};

type agentType =
  | Bot(bot)
  | User(user);

type agent = {
  id: string,
  name: option(string),
  measurementCount: option(int),
  agentType: option(agentType),
};

let toAgent = (~id, ~name=None, ~measurementCount=None, ~agentType=None, ()) => {
  id,
  name,
  measurementCount,
  agentType,
};

let agentName = (a: agent): option(string) =>
  switch (a.agentType) {
  | Some(Bot(b)) => b.name
  | Some(User(u)) => Some(u.name)
  | None => None
  };

type agents = array(agent);
type valueType = [ | `DATE | `FLOAT | `PERCENTAGE];

type measurableState = [ | `OPEN | `ARCHIVED | `JUDGED];

let string_to_measurableState = e: measurableState =>
  switch (e) {
  | "OPEN" => `OPEN
  | "JUDGED" => `JUDGED
  | "ARCHIVED" => `ARCHIVED
  | _ => Js.Exn.raiseError("Invalid GraphQL State")
  };

type competitorType = [ | `AGGREGATION | `COMPETITIVE | `OBJECTIVE];

type measurement = {
  id: string,
  description: option(string),
  value: Belt.Result.t(Value.t, string),
  competitorType,
  taggedMeasurementId: option(string),
  createdAt: option(MomentRe.Moment.t),
  relevantAt: option(MomentRe.Moment.t),
  measurableId: option(string),
  agent: option(agent),
};

let toMeasurement =
    (
      ~id,
      ~value,
      ~description=None,
      ~competitorType=`COMPETITIVE,
      ~taggedMeasurementId=None,
      ~createdAt=None,
      ~relevantAt=None,
      ~agent=None,
      ~measurableId=None,
      (),
    ) => {
  id,
  value,
  description,
  competitorType,
  taggedMeasurementId,
  createdAt,
  relevantAt,
  agent,
  measurableId,
};

type measurable = {
  id: string,
  name: string,
  valueType,
  channel: option(string),
  description: option(string),
  resolutionEndpoint: option(string),
  resolutionEndpointResponse: option(float),
  measurementCount: option(int),
  measurerCount: option(int),
  state: option(measurableState),
  descriptionEntity: option(string),
  descriptionDate: option(MomentRe.Moment.t),
  descriptionProperty: option(string),
  createdAt: option(MomentRe.Moment.t),
  updatedAt: option(MomentRe.Moment.t),
  expectedResolutionDate: option(MomentRe.Moment.t),
  stateUpdatedAt: option(MomentRe.Moment.t),
  creator: option(agent),
  measurements: option(list(measurement)),
  series: option(series),
};

let toMeasurable =
    (
      ~id,
      ~name,
      ~valueType=`FLOAT,
      ~channel=None,
      ~description=None,
      ~resolutionEndpoint=None,
      ~resolutionEndpointResponse=None,
      ~measurementCount=None,
      ~measurerCount=None,
      ~state=None,
      ~createdAt=None,
      ~updatedAt=None,
      ~expectedResolutionDate=None,
      ~stateUpdatedAt=None,
      ~creator=None,
      ~measurements=None,
      ~descriptionEntity=None,
      ~descriptionDate=None,
      ~descriptionProperty=None,
      ~series=None,
      (),
    ) => {
  id,
  name,
  channel,
  valueType,
  description,
  resolutionEndpoint,
  resolutionEndpointResponse,
  measurementCount,
  measurerCount,
  state,
  createdAt,
  updatedAt,
  expectedResolutionDate,
  stateUpdatedAt,
  creator,
  measurements,
  descriptionEntity,
  descriptionDate,
  descriptionProperty,
  series,
};

type measurables = array(measurable);