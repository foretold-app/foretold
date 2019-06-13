open Rationale.Function.Infix;
open Utils;

type channelMembershipRole = [ | `ADMIN | `VIEWER];
type myMembershipRole = [ | `ADMIN | `VIEWER | `NONE];
type permission = [
  | `BOT_CREATE
  | `BOT_UPDATE
  | `CHANNEL_CREATE
  | `CHANNEL_MEMBERSHIP_CREATE
  | `CHANNEL_MEMBERSHIP_DELETE
  | `CHANNEL_MEMBERSHIP_ROLE_UPDATE
  | `CHANNEL_UPDATE
  | `JOIN_CHANNEL
  | `LEAVE_CHANNEL
  | `MEASURABLE_ARCHIVE
  | `MEASURABLE_CREATE
  | `MEASURABLE_UNARCHIVE
  | `MEASURABLE_UPDATE
  | `MEASUREMENT_CREATE
  | `SERIES_CREATE
  | `USER_UPDATE
];

module ChannelMembershipRole = {
  type t = channelMembershipRole;
  let toString = (t: t) =>
    switch (t) {
    | `ADMIN => "Admin"
    | `VIEWER => "Viewer"
    };
};

type competitorType = [ | `AGGREGATION | `COMPETITIVE | `OBJECTIVE];
module Types = {
  type permissions = {allow: list(permission)};
  type user = {
    id: string,
    auth0Id: option(string),
    agent: option(agent),
    name: string,
  }
  and bot = {
    competitorType,
    description: option(string),
    id: string,
    name: option(string),
    token: option(string),
    agent: option(agent),
  }
  and agentType =
    | Bot(bot)
    | User(user)
  and agent = {
    id: string,
    name: option(string),
    measurementCount: option(int),
    agentType: option(agentType),
    channels: Js.Array.t(channel),
    channelMemberships: option(Js.Array.t(channelMembership)),
  }
  and channel = {
    id: string,
    name: string,
    description: option(string),
    isArchived: bool,
    isPublic: bool,
    membershipCount: option(int),
    creator: option(agent),
    myRole: option(myMembershipRole),
    channelMemberships: option(Js.Array.t(channelMembership)),
    openedMeasurablesCount: option(int),
  }
  and channelMembership = {
    channel: option(channel),
    role: channelMembershipRole,
    agent: option(agent),
    permissions: option(permissions),
  };

  type pageInfo = {
    hasNextPage: bool,
    hasPreviousPage: bool,
    endCursor: option(string),
    startCursor: option(string),
  };

  type connection('a) = {
    pageInfo,
    total: option(int),
    edges: array('a),
  };
};

module PageInfo = {
  type t = Types.pageInfo;
  let fromJson =
      (
        pageInfo: {
          .
          "endCursor": option(string),
          "hasNextPage": bool,
          "hasPreviousPage": bool,
          "startCursor": option(string),
        },
      )
      : t => {
    endCursor: pageInfo##endCursor,
    startCursor: pageInfo##startCursor,
    hasPreviousPage: pageInfo##hasPreviousPage,
    hasNextPage: pageInfo##hasNextPage,
  };
};

module Connection = {
  type t('a) = Types.connection('a);
  let make =
      (~pageInfo: Types.pageInfo, ~total=None, ~edges: array('a)): t('a) => {
    pageInfo,
    total,
    edges,
  };
  type edges('a) = option(Js.Array.t(option({. "node": option('a)})));

  let flattenEdges = (edges: edges('a)) =>
    edges
    |> E.A.O.defaultEmpty
    |> E.A.O.concatSome
    |> E.A.fmap(e => e##node)
    |> E.A.O.concatSome;

  let fromJson = (nodeTransformation, json) => {
    let pageInfo = json##pageInfo |> PageInfo.fromJson;
    let total = json##total;
    let edges = json##edges |> flattenEdges |> E.A.fmap(nodeTransformation);
    make(~pageInfo, ~total, ~edges);
  };

  type cursorId = string;
  type direction =
    | None
    | Before(cursorId)
    | After(cursorId);

  let hasNextPage = (t: t('a)) => t.pageInfo.hasNextPage;
  let hasPreviousPage = (t: t('a)) => t.pageInfo.hasPreviousPage;

  let nextPageDirection = (t: t('a)) =>
    switch (hasNextPage(t), t.pageInfo.endCursor) {
    | (true, Some(b)) => Some(After(b))
    | _ => None
    };

  let lastPageDirection = (t: t('a)) =>
    switch (hasPreviousPage(t), t.pageInfo.startCursor) {
    | (true, Some(b)) => Some(Before(b))
    | _ => None
    };
};

module Permissions = {
  type t = Types.permissions;
  let make = (a: list(permission)): t => {allow: a};
  let canX = (permission: permission, t: t): bool =>
    t.allow |> E.L.exists(r => r == permission);
};

module ChannelMembership = {
  type t = Types.channelMembership;
  let make = (~role, ~channel=None, ~agent=None, ~permissions=None, ()): t => {
    role,
    channel,
    agent,
    permissions,
  };
};

module AgentType = {
  type t = Types.agentType;
};

module User = {
  type t = Types.user;
  let make = (~id, ~name="", ~auth0Id=None, ~agent=None, ()): t => {
    id,
    name,
    auth0Id,
    agent,
  };
};

module Bot = {
  type t = Types.bot;
  module CompetitorType = {
    let toString = (c: competitorType) =>
      switch (c) {
      | `AGGREGATION => "Aggregation"
      | `COMPETITIVE => "Prediction"
      | `OBJECTIVE => "Judgement"
      };
  };
  let make =
      (
        ~id,
        ~name=None,
        ~description=None,
        ~competitorType,
        ~token=None,
        ~agent=None,
        (),
      )
      : t => {
    id,
    competitorType,
    description,
    name,
    token,
    agent,
  };
};

module Agent = {
  type t = Types.agent;

  let name = (a: t): option(string) =>
    switch (a.agentType) {
    | Some(Bot(b)) => b.name
    | Some(User(u)) => Some(u.name)
    | None => None
    };

  let firstMembership = (t: t) =>
    t.channelMemberships |> E.A.O.defaultEmpty |> E.A.get(_, 0);

  let firstChannel = (t: t) =>
    t |> firstMembership |> E.O.bind(_, r => r.channel);

  let make =
      (
        ~id,
        ~name=None,
        ~measurementCount=None,
        ~agentType=None,
        ~channels=[||],
        ~channelMemberships=None,
        (),
      )
      : t => {
    id,
    name,
    measurementCount,
    agentType,
    channels,
    channelMemberships,
  };
};

module Channel = {
  type t = Types.channel;
  let showLink = (t: t) => Context__Routing.Url.ChannelShow(t.id);
  let showUrl = showLink ||> Context__Routing.Url.toString;
  let showPush = showLink ||> Context__Routing.Url.push;
  module Styles = {
    open Css;
    let hash = style([marginRight(`px(4))]);
    let lock =
      style([fontSize(`em(0.8)), float(`left), marginRight(`px(0))]);
  };
  let present = (~hashClassName="", s: t) =>
    <span>
      <span className=hashClassName>
        {s.isPublic
           ? <span className=Styles.hash> {"#" |> ste} </span>
           : <span className=Styles.lock> <Icon.Icon icon="LOCK" /> </span>}
      </span>
      <span> {s.name |> ste} </span>
    </span>;

  let make =
      (
        ~id,
        ~name,
        ~description=None,
        ~isArchived,
        ~isPublic,
        ~creator=None,
        ~membershipCount=None,
        ~myRole=None,
        ~channelMemberships=None,
        ~openedMeasurablesCount=None,
        (),
      )
      : t => {
    id,
    name,
    description,
    isArchived,
    isPublic,
    membershipCount,
    creator,
    myRole,
    channelMemberships,
    openedMeasurablesCount,
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
  type t = [ | `OPEN | `JUDGEMENT_PENDING | `JUDGED];

  let fromString = e =>
    switch (e) {
    | "OPEN" => `OPEN
    | "JUDGED" => `JUDGED
    | "JUDGEMENT_PENDING" => `JUDGEMENT_PENDING
    | _ => Js.Exn.raiseError("Invalid GraphQL State: " ++ e)
    };

  let fromEnum = e =>
    switch (e) {
    | `OPEN => `OPEN
    | `JUDGEMENT_PENDING => `JUDGEMENT_PENDING
    | `JUDGED => `JUDGED
    | _ => Js.Exn.raiseError("Invalid GraphQL State ")
    };

  let toInt = (status: t) =>
    switch (status) {
    | `OPEN => 2
    | `JUDGEMENT_PENDING => 1
    | `JUDGED => 0
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
    value: Belt.Result.t(MeasurementValue.t, string),
    competitorType: CompetitorType.t,
    taggedMeasurementId: option(string),
    createdAt: option(MomentRe.Moment.t),
    relevantAt: option(MomentRe.Moment.t),
    measurableId: option(string),
    agent: option(Agent.t),
  };

  let isJudgement = (m: t) => m.competitorType == `OBJECTIVE;

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
    isArchived: option(bool),
    resolutionEndpoint: option(string),
    resolutionEndpointResponse: option(float),
    measurementCount: option(int),
    measurerCount: option(int),
    state: option(MeasurableState.t),
    labelSubject: option(string),
    labelProperty: option(string),
    labelOnDate: option(MomentRe.Moment.t),
    labelCustom: option(string),
    createdAt: option(MomentRe.Moment.t),
    updatedAt: option(MomentRe.Moment.t),
    expectedResolutionDate: option(MomentRe.Moment.t),
    stateUpdatedAt: option(MomentRe.Moment.t),
    creator: option(Agent.t),
    measurements: option(list(Measurement.t)),
    series: option(Series.t),
    iAmOwner: option(bool),
  };

  let toStatus = (measurable: t) => {
    let state = measurable.state |> E.O.toExn("Needs state from GraphQL");
    state;
  };

  let isEqual = (a: t, b: t) => a.id == b.id;

  let compare = (measurableA: t, measurableB: t) =>
    switch (
      toStatus(measurableA),
      toStatus(measurableB),
      measurableA.expectedResolutionDate,
      measurableB.expectedResolutionDate,
    ) {
    | (a, b, Some(aa), Some(bb)) when a == b =>
      MomentRe.Moment.isAfter(aa, bb) ? 1 : (-1)
    | (a, b, _, _) =>
      MeasurableState.toInt(a) > MeasurableState.toInt(b) ? (-1) : 1
    };

  let stableSort = m => E.A.stableSortBy(m, compare);

  let make =
      (
        ~id,
        ~name,
        ~valueType=`FLOAT,
        ~channel=None,
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
        ~labelSubject=None,
        ~labelOnDate=None,
        ~labelProperty=None,
        ~labelCustom=None,
        ~series=None,
        ~isArchived=None,
        ~iAmOwner=None,
        (),
      ) => {
    id,
    name,
    channel,
    valueType,
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
    labelSubject,
    labelOnDate,
    labelProperty,
    labelCustom,
    series,
    isArchived,
    iAmOwner,
  };
};