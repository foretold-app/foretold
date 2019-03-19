open Rationale.Function.Infix;
open Utils;

type user = {
  id: string,
  name: string,
};

module Bot = {
  type t = {
    competitorType: [ | `AGGREGATION | `COMPETITIVE | `OBJECTIVE],
    description: option(string),
    id: string,
    name: option(string),
  };
};

module Channel = {
  type t = string;
  let showLink = t => Urls.ChannelShow(t);
  let showUrl = showLink ||> Urls.mapLinkToUrl;
  let showPush = showLink ||> Urls.pushToLink;
  let present = (~hashClassName="", s) =>
    <span>
      <span className=hashClassName> {"#" |> ste} </span>
      <span> {s |> ste} </span>
    </span>;
};

module Agent = {
  type agentType =
    | Bot(Bot.t)
    | User(user);

  type t = {
    id: string,
    name: option(string),
    measurementCount: option(int),
    agentType: option(agentType),
  };

  let name = (a: t): option(string) =>
    switch (a.agentType) {
    | Some(Bot(b)) => b.name
    | Some(User(u)) => Some(u.name)
    | None => None
    };

  let make = (~id, ~name=None, ~measurementCount=None, ~agentType=None, ()) => {
    id,
    name,
    measurementCount,
    agentType,
  };
};

module Series = {
  type t = {
    id: string,
    description: option(string),
    name: option(string),
    creator: option(Agent.t),
  };

  let make = (~id, ~name=None, ~description=None, ~creator=None, ()) => {
    id,
    name,
    description,
    creator,
  };
};

type valueType = [ | `DATE | `FLOAT | `PERCENTAGE];

module MeasurableState = {
  type t = [ | `OPEN | `ARCHIVED | `JUDGED];

  let fromString = e =>
    switch (e) {
    | "OPEN" => `OPEN
    | "JUDGED" => `JUDGED
    | "ARCHIVED" => `ARCHIVED
    | _ => Js.Exn.raiseError("Invalid GraphQL State")
    };
};

module CompetitorType = {
  type t = [ | `AGGREGATION | `COMPETITIVE | `OBJECTIVE];
  let toString = e =>
    switch (e) {
    | `AGGREGATION => "Aggregation"
    | `COMPETITIVE => "Competitive"
    | `OBJECTIVE => "Objective"
    };
};

module Measurement = {
  type t = {
    id: string,
    description: option(string),
    value: Belt.Result.t(Value.t, string),
    competitorType: CompetitorType.t,
    taggedMeasurementId: option(string),
    createdAt: option(MomentRe.Moment.t),
    relevantAt: option(MomentRe.Moment.t),
    measurableId: option(string),
    agent: option(Agent.t),
  };

  let make =
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
};

module Measurable = {
  type t = {
    id: string,
    name: string,
    valueType,
    channel: option(string),
    description: option(string),
    resolutionEndpoint: option(string),
    resolutionEndpointResponse: option(float),
    measurementCount: option(int),
    measurerCount: option(int),
    state: option(MeasurableState.t),
    descriptionEntity: option(string),
    descriptionDate: option(MomentRe.Moment.t),
    descriptionProperty: option(string),
    createdAt: option(MomentRe.Moment.t),
    updatedAt: option(MomentRe.Moment.t),
    expectedResolutionDate: option(MomentRe.Moment.t),
    stateUpdatedAt: option(MomentRe.Moment.t),
    creator: option(Agent.t),
    measurements: option(list(Measurement.t)),
    series: option(Series.t),
  };

  let make =
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
};