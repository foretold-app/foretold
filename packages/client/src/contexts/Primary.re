open Rationale.Function.Infix;
open Utils;
open Types;

let toTimestamp = (json: Js.Json.t): option(MomentRe.Moment.t) =>
  json |> E.J.toMoment |> E.O.some;
let toCreatedAt = toTimestamp;
let toUpdatedAt = toTimestamp;

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

  let toString = (channelMembershipRole: t) =>
    switch (channelMembershipRole) {
    | `ADMIN => "Admin"
    | `VIEWER => "Member"
    };

  let isAdmin = channel => {
    let role = channel |> E.O.fmap(channel => channel.myRole);
    switch (role) {
    | Some(Some(`ADMIN)) => true
    | _ => false
    };
  };
};

module Cursor = {
  type t = Types.cursor;
  let make = (cursor): t => Js.Json.string(cursor);
  let toInt = (cursor): int => cursor |> E.J.toString |> int_of_string;
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
    endCursor: pageInfo##endCursor |> E.O.fmap(Cursor.make),
    startCursor: pageInfo##startCursor |> E.O.fmap(Cursor.make),
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

  type direction =
    | None
    | Before(Types.cursor)
    | After(Types.cursor);

  let hasNextPage = (connection: t('a)) => connection.pageInfo.hasNextPage;
  let hasPreviousPage = (connection: t('a)) =>
    connection.pageInfo.hasPreviousPage;

  let nextPageDirection = (connection: t('a)) =>
    switch (hasNextPage(connection), connection.pageInfo.endCursor) {
    | (true, Some(endCursor)) => Some(After(endCursor))
    | _ => None
    };

  let lastPageDirection = (connection: t('a)) =>
    switch (hasPreviousPage(connection), connection.pageInfo.startCursor) {
    | (true, Some(startCursor)) => Some(Before(startCursor))
    | _ => None
    };
};

module Permissions = {
  type t = Types.permissions;

  let make = (permissions: list(permission)): t => {allow: permissions};

  let can = (permission: permission, permissions: option(t)): bool =>
    switch (permissions) {
    | Some(permissions) =>
      permissions.allow |> E.L.exists(r => r == permission)
    | _ => false
    };
};

module ChannelMembership = {
  type t = Types.channelMembership;
  let make =
      (
        ~role,
        ~channel=None,
        ~agent=None,
        ~permissions=None,
        ~isVerified=None,
        (),
      )
      : t => {
    role,
    channel,
    agent,
    permissions,
    isVerified,
  };
};

module User = {
  type t = Types.user;

  let getAgent = (user: t, clbFn) =>
    switch (user.agent) {
    | Some(agent) =>
      clbFn(agent);
      ();
    | _ => ()
    };

  // @todo: Move to logical components.
  let showif = (user: t): bool => {
    user.agent
    |> E.O.bind(_, (r: Types.agent) => r.preference)
    |> E.O.fmap((r: Types.preference) => r.enableExperimentalFeatures)
    |> E.O.default(false);
  };

  // @todo: Move to logical components.
  let show = (user: t, component: ReasonReact.reactElement) => {
    showif(user) ? component : <Null />;
  };

  // @todo: Move to logical components.
  let show2 = (user: option(t), component: ReasonReact.reactElement) => {
    switch (user) {
    | Some(user) => show(user, component)
    | None => <Null />
    };
  };

  // @todo: Move to logical components.
  let authorized = (user: option(t), component: ReasonReact.reactElement) => {
    switch (user) {
    | Some(_) => component
    | _ => <Null />
    };
  };

  let getName = (user: t) =>
    user.agent
    |> E.O.fmap((agent: Types.agent) => agent.name |> E.O.default("User"))
    |> E.O.default("User");

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
        ~isEmailVerified=None,
        ~createdAt=None,
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
    isEmailVerified,
    createdAt,
  };
};

module Bot = {
  type t = Types.bot;

  module CompetitorType = {
    let toString = (competitorType: Types.competitorType) =>
      switch (competitorType) {
      | `AGGREGATION => "Aggregation"
      | `COMPETITIVE => "Prediction"
      | `OBJECTIVE => "Resolution"
      | _ => Js.Exn.raiseError("Invalid Competitor Type")
      };
  };

  let make =
      (
        ~id,
        ~name=None,
        ~description=None,
        ~picture=None,
        ~competitorType=`AGGREGATION,
        ~token=None,
        ~agent=None,
        ~permissions=None,
        ~owner=None,
        (),
      )
      : t => {
    id,
    name,
    description,
    picture,
    competitorType,
    token,
    agent,
    permissions,
    owner,
  };
};

module Agent = {
  type t = Types.agent;

  let toAgentId = (json: Js.Json.t): Types.agentId => json |> E.J.toString;

  module AgentId = {
    type t = Js.Json.t;
    let make = (agentId: string): t => Js.Json.string(agentId);
  };

  let name = (agent: t): option(string) =>
    switch (agent.agentType) {
    | Some(Bot(b)) => b.name
    | Some(User(u)) => Some(u.name)
    | None => None
    };

  let firstMembership = (agent: t) =>
    agent.channelMemberships |> E.A.O.defaultEmpty |> E.A.get(_, 0);

  let firstChannel = (agent: t) =>
    agent |> firstMembership |> E.O.bind(_, r => r.channel);

  let make =
      (
        ~id,
        ~name=None,
        ~measurementCount=None,
        ~agentType=None,
        ~channels=[||],
        ~channelMemberships=None,
        ~preference=None,
        ~isMe=false,
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
    isMe,
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

  let toChannelId = (json: Js.Json.t): Types.channelId =>
    json |> E.J.toString;

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
      style([
        fontSize(`em(0.8)),
        float(`left),
        marginRight(`px(0)),
        marginTop(`em(-0.1)),
      ]);
  };

  let present = (~className="", channel: t) =>
    <span>
      <span className>
        {channel.isPublic
           ? <span className=Styles.hash> {"#" |> ste} </span>
           : <span className=Styles.lock> <Icon icon="LOCK" /> </span>}
      </span>
      <span> {channel.name |> ste} </span>
    </span>;

  let presentGlobal = (~className="", ~symbolClassName=Styles.globeList, ()) =>
    <span>
      <span className>
        <span className=symbolClassName> <Icon icon="HOME" /> </span>
      </span>
      <span> {"Home" |> ste} </span>
    </span>;

  let presentCommunities =
      (~className="", ~symbolClassName=Styles.globeList, ()) =>
    <span>
      <span className>
        <span className=symbolClassName> <Icon icon="LIST" /> </span>
      </span>
      <span> {"All Communities" |> ste} </span>
    </span>;

  let toMyCommunitiesItem =
      (channel: t, bookmark, onBookmark)
      : ForetoldComponents.MyCommunities.item => {
    bookmark,
    onBookmark,
    name: channel.name,
    icon: channel.isPublic ? "PEOPLE" : "LOCK",
    href: LinkType.toString(Internal(showLink(channel))),
    onClick: LinkType.onClick(Internal(showLink(channel))),
  };

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
        ~notebooksCount=None,
        ~isCurated=false,
        ~permissions=None,
        ~knowledgeGraph=None,
        ~isBookmarked=None,
        ~bookmarksCount=None,
        ~requireVerification=false,
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
    notebooksCount,
    isCurated,
    permissions,
    knowledgeGraph,
    isBookmarked,
    bookmarksCount,
    requireVerification,
  };

  let toChannel = (channel: Js.t('a)) => {
    make(
      ~id=channel##id,
      ~name=channel##name,
      ~isArchived=channel##isArchived,
      ~isPublic=channel##isPublic,
      (),
    );
  };

  let getGlobalChannel = (): t =>
    make(
      ~id="home",
      ~name="Home",
      ~description=
        Some(
          "Home lists questions and activity from communities you have joined. "
          ++ "To see the list of all public communities, click "
          ++ "[here](/communities).",
        ),
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
        ~channelId,
        ~subjects=None,
        ~properties=None,
        ~dates=None,
        (),
      )
      : t => {
    id,
    name,
    description,
    creator,
    measurableCount,
    channel,
    channelId,
    subjects,
    properties,
    dates,
  };
};

module Measurement = {
  type t = Types.measurement;

  let isJudgement = (measurement: t) =>
    measurement.competitorType == `OBJECTIVE;

  let valueSuggestsJudgement = (measurement: t) =>
    switch (measurement.value) {
    | Ok(`FloatPoint(_)) => Some(true)
    | Ok(`UnresolvableResolution(_)) => Some(true)
    | Ok(`Binary(_)) => Some(true)
    | Ok(`Percentage(_)) => Some(false)
    | Ok(`Comment(_)) => Some(false)
    | Ok(`FloatCdf(_)) => Some(false)
    | Error(_) => None
    };

  let make =
      (
        ~id,
        ~value,
        ~description=None,
        ~competitorType=`COMPETITIVE,
        ~taggedMeasurementId=None,
        ~createdAt=None,
        ~updatedAt=None,
        ~cancelledAt=None,
        ~relevantAt=None,
        ~agent=None,
        ~measurableId=None,
        ~measurable=None,
        ~valueText=None,
        ~measurementScoreSet=None,
        ~totalVoteAmount=None,
        ~permissions=None,
        ~vote=None,
        (),
      )
      : t => {
    id,
    value,
    description,
    competitorType,
    taggedMeasurementId,
    createdAt,
    updatedAt,
    cancelledAt,
    relevantAt,
    agent,
    measurableId,
    measurable,
    valueText,
    measurementScoreSet,
    totalVoteAmount,
    permissions,
    vote,
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
    measurable.state |> E.O.toExn("Needs state from GraphQL");
  };

  let isEqual = (measurable1: t, measurable2: t) =>
    measurable1.id == measurable2.id;

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

  let stableSort = measurable => E.A.stableSortBy(measurable, compare);

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
    | _ => Js.Exn.raiseError("Invalid Value Type: " ++ valueType)
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

  let iCanEdit = (~measurable: Types.measurable) => {
    let isAdmin = ChannelMembershipRole.isAdmin(measurable.channel);
    let iAmOwner = measurable.iAmOwner |> E.Bool.O.toBool;
    isAdmin || iAmOwner;
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
        ~labelStartAtDate=None,
        ~labelEndAtDate=None,
        ~labelConditionals=None,
        ~series=None,
        ~isArchived=None,
        ~iAmOwner=None,
        ~min=None,
        ~max=None,
        ~permissions=None,
        ~outcome=None,
        ~previousAggregate=None,
        ~recentMeasurement=None,
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
    labelStartAtDate,
    labelEndAtDate,
    labelConditionals,
    series,
    isArchived,
    iAmOwner,
    min,
    max,
    permissions,
    outcome,
    previousAggregate,
    recentMeasurement,
  };

  let ofFeedItemMeasurable =
      (~feedItem: Types.feedItem, ~feedItemBody: FeedItemBody.measurableB) => {
    make(
      ~id=feedItemBody.measurableId,
      ~name=feedItemBody.item,
      ~channelId=feedItem.channelId,
      ~labelSubject=feedItemBody.labelSubject,
      ~labelProperty=feedItemBody.labelProperty,
      ~labelCustom=feedItemBody.labelCustom,
      ~labelStartAtDate=feedItemBody.labelStartAtDate,
      ~labelEndAtDate=feedItemBody.labelEndAtDate,
      ~labelConditionals=feedItemBody.labelConditionals,
      (),
    );
  };
};

module FeedItem = {
  type t = Types.feedItem;

  let make =
      (
        ~id,
        ~channelId,
        ~body,
        ~channel,
        ~createdAt=None,
        ~updatedAt=None,
        ~agent,
        (),
      )
      : t => {
    id,
    channelId,
    body,
    channel,
    createdAt,
    updatedAt,
    agent,
  };
};

module CompetitorType = {
  type t = competitorType;
  let toString = competitorType =>
    switch (competitorType) {
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

  let availableInputs = (~measurable: Types.measurable) => {
    switch (Measurable.iCanEdit(~measurable), measurable.state) {
    | (true, Some(`JUDGED)) => [|`COMMENT, `OBJECTIVE, `UNRESOLVED|]
    | (true, _) => [|`COMMENT, `COMPETITIVE, `OBJECTIVE, `UNRESOLVED|]
    | (false, Some(`JUDGED)) => [|`COMMENT|]
    | (false, _) => [|`COMMENT, `COMPETITIVE|]
    };
  };

  let competitorTypeInitValue = (~measurable: Types.measurable) => {
    switch (Measurable.iCanEdit(~measurable), measurable.state) {
    | (true, Some(`JUDGED)) => "OBJECTIVE"
    | (false, Some(`JUDGED)) => "COMMENT"
    | _ => "COMPETITIVE"
    };
  };

  let toSelection = (competitorType: t) =>
    switch (competitorType) {
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
    | `AGGREGATION => E.React2.null
    };

  let availableSelections = (~measurable) =>
    availableInputs(~measurable) |> E.A.fmap(toSelection);
};

module GlobalSetting = {
  type t = Types.globalSetting;
  let make = (~id: string, ~entityGraph: option(KenTools.g)=None, ()): t => {
    id,
    entityGraph,
  };
};

module TimeAverageScore = {
  type t = Types.timeAverageScore;
  let make =
      (
        ~score: float,
        ~timeActivityRatio: float,
        ~marketType: marketType,
        ~finalComparisonMeasurement: finalComparisonMeasurement,
      )
      : t => {
    score,
    timeActivityRatio,
    marketType,
    finalComparisonMeasurement,
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
        ~timeAverageScore=None,
        ~totalVotesReceived=None,
        (),
      )
      : t => {
    id,
    measurable,
    agent,
    primaryPointScore,
    timeAverageScore,
    predictionCountTotal,
    createdAt,
    competitiveMeasurement,
    aggregationMeasurement,
    objectiveMeasurement,
    totalVotesReceived,
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
        ~totalVotesReceived=None,
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
    totalVotesReceived,
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
        ~timeAverageScore=None,
        ~competitiveMeasurement=None,
        ~aggregationMeasurement=None,
        ~objectiveMeasurement=None,
        ~totalVotesReceived=None,
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
    timeAverageScore,
    numberOfQuestionsScored,
    competitiveMeasurement,
    aggregationMeasurement,
    objectiveMeasurement,
    totalVotesReceived,
  };

  let fromAgentMeasurable = (agentMeasurable: Types.agentMeasurable) =>
    make(
      ~id=agentMeasurable.id,
      ~agent=Some(agentMeasurable.agent),
      ~measurable=Some(agentMeasurable.measurable),
      ~timeAverageScore=agentMeasurable.timeAverageScore,
      ~pointScore=agentMeasurable.primaryPointScore,
      ~predictionCountTotal=Some(agentMeasurable.predictionCountTotal),
      ~createdAt=Some(agentMeasurable.createdAt),
      ~objectiveMeasurement=agentMeasurable.objectiveMeasurement,
      ~totalVotesReceived=agentMeasurable.totalVotesReceived,
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
      ~totalVotesReceived=agentChannel.totalVotesReceived,
      (),
    );
};

module AgentType = {
  type t = Types.agentType;

  let getOwner = bot =>
    switch (bot##user) {
    | Some(user) =>
      let agentType =
        Some(
          User(
            User.make(
              ~id=user##id,
              ~name=user##name,
              ~picture=user##picture,
              ~agentId=user##agentId,
              (),
            ),
          ),
        );

      let agent =
        Agent.make(
          ~id=user##agentId,
          ~agentType,
          ~name=Some(user##name),
          (),
        );

      Some(agent);
    | _ => None
    };

  let getEmptyOwner = _ => None;

  let getAgentType = (~agent, ~getOwner=getOwner, ()) =>
    switch (agent##bot, agent##user) {
    | (Some(bot), None) =>
      Some(
        Bot(
          Bot.make(
            ~id=bot##id,
            ~name=Some(bot##name),
            ~description=bot##description,
            ~competitorType=bot##competitorType,
            ~owner=getOwner(bot),
            ~picture=bot##picture,
            (),
          ),
        ),
      )
    | (None, Some(user)) =>
      Some(
        User(
          User.make(
            ~id=user##id,
            ~name=user##name,
            ~description=user##description,
            ~picture=user##picture,
            ~agentId=user##agentId,
            (),
          ),
        ),
      )
    | (_, _) => None
    };

  let toAgent = (agent: Js.t('a)) => {
    let agentType = getAgentType(~agent, ());

    Agent.make(~id=agent##id, ~agentType, ~name=agent##name, ());
  };
};

module Notebook = {
  type t = Types.notebook;

  let toNotebookId = (json: Js.Json.t): Types.notebookId =>
    json |> E.J.toString;

  let convertJs =
      (
        ~id: Js.Json.t,
        ~name: Js.Json.t,
        ~body: Js.Json.t,
        ~ownerId: Js.Json.t,
        ~channelId: Js.Json.t,
        ~createdAt: Js.Json.t,
        ~updatedAt: Js.Json.t,
        ~isBookmarked: option(bool),
        ~bookmarksCount: int,
        ~owner: Js.t('a),
        ~channel: Js.t('b),
        ~permissions: Js.t('c),
        (),
      )
      : t => {
    let permissions' =
      permissions##mutations##allow
      |> E.A.O.concatSome
      |> E.A.to_list
      |> Permissions.make;

    {
      id: toNotebookId(id),
      name: name |> E.J.toString,
      body: body |> E.J.toString,
      ownerId: Agent.toAgentId(ownerId),
      channelId: Channel.toChannelId(channelId),
      createdAt: toCreatedAt(createdAt),
      updatedAt: toUpdatedAt(updatedAt),
      owner: AgentType.toAgent(owner),
      channel: Channel.toChannel(channel),
      isBookmarked,
      bookmarksCount: Some(bookmarksCount),
      permissions: Some(permissions'),
    };
  };

  let convertJsObject = notebook => {
    convertJs(
      ~id=notebook##id,
      ~name=notebook##name,
      ~body=notebook##body,
      ~ownerId=notebook##ownerId,
      ~channelId=notebook##channelId,
      ~createdAt=notebook##createdAt,
      ~updatedAt=notebook##updatedAt,
      ~owner=notebook##owner,
      ~channel=notebook##channel,
      ~isBookmarked=notebook##isBookmarked,
      ~bookmarksCount=notebook##bookmarksCount,
      ~permissions=notebook##permissions,
      (),
    );
  };
};

module Vote = {
  type t = Types.vote;

  let toVoteId = (json: Js.Json.t): Types.voteId => json |> E.J.toString;

  let convertJs =
      (
        ~id: Js.Json.t,
        ~voteAmount: int,
        ~createdAt: Js.Json.t,
        ~updatedAt: Js.Json.t,
        ~agent: option(Js.t('a))=None,
        (),
      )
      : t => {
    id: id |> toVoteId,
    voteAmount,
    createdAt: toCreatedAt(createdAt),
    updatedAt: toUpdatedAt(updatedAt),
    agent: agent |> E.O.fmap(AgentType.toAgent),
  };

  let convertJsObject = vote => {
    convertJs(
      ~id=vote##id,
      ~voteAmount=vote##voteAmount,
      ~createdAt=vote##createdAt,
      ~updatedAt=vote##updatedAt,
      (),
    );
  };

  let convertJsObject2 = vote => {
    convertJs(
      ~id=vote##id,
      ~voteAmount=vote##voteAmount,
      ~createdAt=vote##createdAt,
      ~updatedAt=vote##updatedAt,
      ~agent=vote##agent,
      (),
    );
  };
};