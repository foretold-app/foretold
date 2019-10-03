open Rationale.Function.Infix;
open Utils;
open Types;

module MeasurableState = {
  type t = measurableState;

  let fromString = e =>
    switch (e) {
    | "OPEN" => `OPEN
    | "JUDGED" => `JUDGED
    | "JUDGEMENT_PENDING" => `JUDGEMENT_PENDING
    | "CLOSED_AS_UNRESOLVED" => `CLOSED_AS_UNRESOLVED
    | _ => Js.Exn.raiseError("Invalid GraphQL State: " ++ e)
    };

  let toString = (e): string =>
    switch (e) {
    | Some(`OPEN) => "OPEN"
    | Some(`JUDGED) => "JUDGED"
    | Some(`JUDGEMENT_PENDING) => "JUDGEMENT_PENDING"
    | Some(`CLOSED_AS_UNRESOLVED) => "CLOSED_AS_UNRESOLVED"
    | _ => Js.Exn.raiseError("Invalid GraphQL State")
    };

  let fromEnum = e =>
    switch (e) {
    | `OPEN => `OPEN
    | `JUDGEMENT_PENDING => `JUDGEMENT_PENDING
    | `JUDGED => `JUDGED
    | `CLOSED_AS_UNRESOLVED => `CLOSED_AS_UNRESOLVED
    | _ => Js.Exn.raiseError("Invalid GraphQL State ")
    };

  let toInt = (status: t) =>
    switch (status) {
    | `OPEN => 3
    | `JUDGEMENT_PENDING => 2
    | `JUDGED => 1
    | `CLOSED_AS_UNRESOLVED => 0
    };
};

module ChannelMembershipRole = {
  type t = channelMembershipRole;
  let toString = (t: t) =>
    switch (t) {
    | `ADMIN => "Admin"
    | `VIEWER => "Member"
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
    | (true, Some(endCursor)) => Some(After(endCursor))
    | _ => None
    };

  let lastPageDirection = (t: t('a)) =>
    switch (hasPreviousPage(t), t.pageInfo.startCursor) {
    | (true, Some(startCursor)) => Some(Before(startCursor))
    | _ => None
    };
};

module Permissions = {
  type t = Types.permissions;

  let make = (a: list(permission)): t => {allow: a};

  let can = (permission: permission, permissions: option(t)): bool =>
    switch (permissions) {
    | Some(permissions) =>
      permissions.allow |> E.L.exists(r => r == permission)
    | _ => false
    };
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

module User = {
  type t = Types.user;

  let getAgent = (user: t, clbFn) =>
    switch (user.agent) {
    | Some(agent) => clbFn(agent)
    };

  let showif = (user: t): bool => {
    user.agent
    |> E.O.bind(_, (r: Types.agent) => r.preference)
    |> E.O.fmap((r: Types.preference) => r.enableExperimentalFeatures)
    |> E.O.default(false);
  };

  let show = (user: t, component: ReasonReact.reactElement) => {
    showif(user) ? component : ReasonReact.null;
  };

  let make =
      (
        ~id,
        ~name="",
        ~agentId,
        ~email=None,
        ~picture=None,
        ~description=None,
        ~auth0Id=None,
        ~agent=None,
        ~bots=None,
        (),
      )
      : t => {
    id,
    name,
    agentId,
    email,
    picture,
    description,
    auth0Id,
    agent,
    bots,
  };
};

module Bot = {
  type t = Types.bot;

  module CompetitorType = {
    let toString = (c: Types.competitorType) =>
      switch (c) {
      | `AGGREGATION => "Aggregation"
      | `COMPETITIVE => "Prediction"
      | `OBJECTIVE => "Resolution"
      };
  };

  let make =
      (
        ~id,
        ~name=None,
        ~description=None,
        ~competitorType=`AGGREGATION,
        ~token=None,
        ~agent=None,
        ~permissions=None,
        ~user=None,
        (),
      )
      : t => {
    id,
    competitorType,
    description,
    name,
    token,
    agent,
    permissions,
    user,
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
        ~preference=None,
        (),
      )
      : t => {
    id,
    name,
    measurementCount,
    agentType,
    channels,
    channelMemberships,
    preference,
  };
};

module Preference = {
  type t = Types.preference;

  let make =
      (~id, ~stopAllEmails=None, ~enableExperimentalFeatures=false, ()): t => {
    id,
    stopAllEmails,
    enableExperimentalFeatures,
  };
};

module Channel = {
  type t = Types.channel;
  let showLink = (channel: t) => Routing.Url.ChannelShow(channel.id);
  let globalLink = () => Routing.Url.ChannelShow("home");
  let showUrl = showLink ||> Routing.Url.toString;
  let showPush = showLink ||> Routing.Url.push;

  module Styles = {
    open Css;
    let hash = style([marginRight(`px(4))]);
    let globeList =
      style([
        marginRight(`px(1)),
        marginLeft(`px(-4)),
        float(`left),
        marginTop(`px(-2)),
      ]);
    let globe =
      style([marginRight(`px(4)), float(`left), marginTop(`px(-2))]);
    let lock =
      style([fontSize(`em(0.8)), float(`left), marginRight(`px(0))]);
  };

  let present = (~className="", channel: t) =>
    <span>
      <span className>
        {channel.isPublic
           ? <span className=Styles.hash> {"#" |> ste} </span>
           : <span className=Styles.lock> <Icon.Icon icon="LOCK" /> </span>}
      </span>
      <span> {channel.name |> ste} </span>
    </span>;

  let presentGlobal = (~className="", ~symbolClassName=Styles.globeList, ()) =>
    <span>
      <span className>
        <span className=symbolClassName> <Icon.Icon icon="HOME" /> </span>
      </span>
      <span> {"Home" |> ste} </span>
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
        ~isCurated=false,
        ~permissions=None,
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
    isCurated,
    permissions,
  };

  let getGlobalChannel = (): t =>
    make(
      ~id="home",
      ~name="Home",
      ~description=Some("Global Community"),
      ~isArchived=false,
      ~isPublic=true,
      ~myRole=Some(`VIEWER),
      ~membershipCount=Some(0),
      (),
    );
};

module Series = {
  type t = Types.series;

  let make =
      (
        ~id,
        ~name=None,
        ~description=None,
        ~creator=None,
        ~measurableCount=None,
        ~channel=None,
        (),
      )
      : t => {
    id,
    name,
    description,
    creator,
    measurableCount,
    channel,
  };
};

module Measurement = {
  type t = Types.measurement;

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
        ~measurable=None,
        ~valueText=None,
        ~measurementScoreSet=None,
        (),
      )
      : t => {
    id,
    value,
    description,
    competitorType,
    taggedMeasurementId,
    createdAt,
    relevantAt,
    agent,
    measurableId,
    measurable,
    valueText,
    measurementScoreSet,
  };
};

module MeasurementScoreSet = {
  type t = Types.measurementScoreSet;

  let make = (~primaryPointScore=None, ~nonMarketLogScore=None, ()): t => {
    primaryPointScore,
    nonMarketLogScore,
  };
};

module Measurable = {
  type t = Types.measurable;

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

  let valueTypeToStr = (valueType: valueType): string =>
    switch (valueType) {
    | `FLOAT => "FLOAT"
    | `PERCENTAGE => "PERCENTAGE"
    | `DATE => "DATE"
    };

  let valueTypeToEnum = (valueType: string): valueType =>
    switch (valueType) {
    | "FLOAT" => `FLOAT
    | "PERCENTAGE" => `PERCENTAGE
    | "DATE" => `DATE
    };

  let toMinMaxDescription = (measurable: t) => {
    switch (
      measurable.min |> E.O.fmap(E.Float.with3DigitsPrecision),
      measurable.max |> E.O.fmap(E.Float.with3DigitsPrecision),
    ) {
    | (Some(min), Some(max)) =>
      Some(
        "This has a minimum of " ++ min ++ " and a maximum of " ++ max ++ ".",
      )
    | (Some(min), None) => Some("This has a minimum of " ++ min ++ ".")
    | (None, Some(max)) => Some("This has a maximum of " ++ max ++ ".")
    | (None, None) => None
    };
  };

  let make =
      (
        ~id,
        ~name,
        ~valueType=`FLOAT,
        ~channel=None,
        ~channelId,
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
        ~min=None,
        ~max=None,
        ~permissions=None,
        ~outcome=None,
        ~previousAggregate=None,
        (),
      )
      : t => {
    id,
    name,
    channel,
    channelId,
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
    min,
    max,
    permissions,
    outcome,
    previousAggregate,
  };
};

module FeedItem = {
  type t = Types.feedItem;

  let make =
      (~id, ~channelId, ~body, ~channel, ~createdAt=None, ~updatedAt=None, ())
      : t => {
    id,
    channelId,
    body,
    channel,
    createdAt,
    updatedAt,
  };
};

module CompetitorType = {
  type t = competitorType;
  let toString = e =>
    switch (e) {
    | `AGGREGATION => "Aggregation"
    | `COMPETITIVE => "Competitive"
    | `OBJECTIVE => "Objective"
    | `UNRESOLVED => "Unresolved"
    | `COMMENT => "Comment"
    };

  let fromString = str =>
    switch (str) {
    | "Aggregation" => `AGGREGATION
    | "Competitive" => `COMPETITIVE
    | "Objective" => `OBJECTIVE
    | "Unresolved" => `UNRESOLVED
    | "Comment" => `COMMENT
    | _ => Js.Exn.raiseError("Invalid Competitor Type: " ++ str)
    };

  let availableInputs =
      (~isOwner: bool, ~state: option(Types.measurableState)) => {
    switch (isOwner, state) {
    | (true, Some(`JUDGED)) => [|`COMMENT, `OBJECTIVE, `UNRESOLVED|]
    | (true, _) => [|`COMMENT, `COMPETITIVE, `OBJECTIVE, `UNRESOLVED|]
    | (false, Some(`JUDGED)) => [|`COMMENT|]
    | (false, _) => [|`COMMENT, `COMPETITIVE|]
    };
  };

  let toSelection = (t: t) =>
    switch (t) {
    | `COMPETITIVE =>
      <Antd.Select.Option value="COMPETITIVE">
        {"Predict" |> ste}
      </Antd.Select.Option>
    | `OBJECTIVE =>
      <Antd.Select.Option value="OBJECTIVE">
        {"Resolve" |> ste}
      </Antd.Select.Option>
    | `COMMENT =>
      <Antd.Select.Option value="COMMENT">
        {"Comment" |> ste}
      </Antd.Select.Option>
    | `UNRESOLVED =>
      <Antd.Select.Option value="UNRESOLVED">
        {"Close without Answer" |> ste}
      </Antd.Select.Option>
    | `AGGREGATION => E.React.null
    };

  let availableSelections =
      (~isOwner: bool, ~state: option(Types.measurableState)) =>
    availableInputs(~isOwner, ~state) |> E.A.fmap(toSelection);
};

module GlobalSetting = {
  type t = Types.globalSetting;
  let make = (~id: string, ~entityGraph: option(Js.Json.t)=None, ()): t => {
    id,
    entityGraph,
  };
};

module AgentMeasurable = {
  type t = Types.agentMeasurable;
  let make =
      (
        ~id: string,
        ~measurable,
        ~agent,
        ~primaryPointScore=None,
        ~predictionCountTotal,
        ~createdAt,
        ~competitiveMeasurement=None,
        ~aggregationMeasurement=None,
        ~objectiveMeasurement=None,
        (),
      )
      : t => {
    id,
    measurable,
    agent,
    primaryPointScore,
    predictionCountTotal,
    createdAt,
    competitiveMeasurement,
    aggregationMeasurement,
    objectiveMeasurement,
  };
};

module AgentChannel = {
  type t = Types.agentChannel;
  let make =
      (
        ~id: string,
        ~channel,
        ~agent,
        ~agentId,
        ~channelId,
        ~primaryPointScore,
        ~numberOfPredictions,
        ~numberOfQuestionsScored,
        ~createdAt,
        ~updatedAt,
        (),
      )
      : t => {
    id,
    channel,
    agent,
    agentId,
    channelId,
    primaryPointScore,
    numberOfPredictions,
    numberOfQuestionsScored,
    createdAt,
    updatedAt,
  };
};

module LeaderboardItem = {
  type t = Types.leaderboardItem;

  let make =
      (
        ~id: string,
        ~measurable=None,
        ~agent=None,
        ~channel=None,
        ~pointScore=None,
        ~createdAt=None,
        ~predictionCountTotal=None,
        ~numberOfQuestionsScored=None,
        ~competitiveMeasurement=None,
        ~aggregationMeasurement=None,
        ~objectiveMeasurement=None,
        (),
      )
      : t => {
    id,
    measurable,
    agent,
    channel,
    pointScore,
    createdAt,
    predictionCountTotal,
    numberOfQuestionsScored,
    competitiveMeasurement,
    aggregationMeasurement,
    objectiveMeasurement,
  };

  let fromAgentMeasurable = (agentMeasurable: Types.agentMeasurable) =>
    make(
      ~id=agentMeasurable.id,
      ~agent=Some(agentMeasurable.agent),
      ~measurable=Some(agentMeasurable.measurable),
      ~pointScore=agentMeasurable.primaryPointScore,
      ~predictionCountTotal=Some(agentMeasurable.predictionCountTotal),
      ~createdAt=Some(agentMeasurable.createdAt),
      ~objectiveMeasurement=agentMeasurable.objectiveMeasurement,
      (),
    );

  let fromAgentChannel = (agentChannel: Types.agentChannel) =>
    make(
      ~id=agentChannel.id,
      ~agent=Some(agentChannel.agent),
      ~channel=Some(agentChannel.channel),
      ~pointScore=agentChannel.primaryPointScore,
      ~predictionCountTotal=Some(agentChannel.numberOfPredictions),
      ~numberOfQuestionsScored=Some(agentChannel.numberOfQuestionsScored),
      (),
    );
};

module AgentType = {
  type t = Types.agentType;

  let getAgentType = agent =>
    switch (agent##bot, agent##user) {
    | (Some(bot), None) =>
      Some(
        Types.Bot(
          Bot.make(
            ~id=bot##id,
            ~name=Some(bot##name),
            ~competitorType=bot##competitorType,
            (),
          ),
        ),
      )
    | (None, Some(user)) =>
      Some(
        Types.User(
          User.make(
            ~id=user##id,
            ~name=user##name,
            ~agentId=user##agentId,
            (),
          ),
        ),
      )
    | (_, _) => None
    };
};
