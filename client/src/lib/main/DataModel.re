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

type agent = {
  id: string,
  measurementCount: option(int),
  bot: option(bot),
  user: option(user),
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

type creator = {
  id: string,
  name: option(string),
};

type measurable = {
  id: string,
  name: string,
  valueType,
  description: option(string),
  resolutionEndpoint: option(string),
  resolutionEndpointResponse: option(float),
  measurementCount: option(int),
  measurerCount: option(int),
  state: option(measurableState),
  createdAt: MomentRe.Moment.t,
  updatedAt: MomentRe.Moment.t,
  expectedResolutionDate: option(MomentRe.Moment.t),
  stateUpdatedAt: option(MomentRe.Moment.t),
  creator: option(creator),
};

let toMeasurable =
    (
      ~id,
      ~name,
      ~valueType=`FLOAT,
      ~description=None,
      ~resolutionEndpoint=None,
      ~resolutionEndpointResponse=None,
      ~measurementCount=None,
      ~measurerCount=None,
      ~state=None,
      ~createdAt,
      ~updatedAt,
      ~expectedResolutionDate=None,
      ~stateUpdatedAt=None,
      ~creator=None,
      (),
    ) => {
  id,
  name,
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
};

type measurables = array(measurable);