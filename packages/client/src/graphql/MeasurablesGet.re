open Rationale.Function.Infix;

type series = {
  id: string,
  description: option(string),
  name: option(string),
};

let toSeries = (c: series): Types.series =>
  Primary.Series.make(~id=c.id, ~description=c.description, ~name=c.name, ());

type creator = {
  id: string,
  name: option(string),
};

type channel = {
  id: string,
  name: string,
  description: option(string),
  isPublic: bool,
  isArchived: bool,
};

let toAgent = (c: creator): Types.agent =>
  Primary.Agent.make(~id=c.id, ~name=c.name, ());

type node = {
  id: string,
  name: string,
  channelId: string,
  channel: option(channel),
  valueType: Types.valueType,
  labelCustom: option(string),
  resolutionEndpoint: option(string),
  measurementCount: option(int),
  measurerCount: option(int),
  labelSubject: option(string),
  iAmOwner: bool,
  createdAt: MomentRe.Moment.t,
  updatedAt: MomentRe.Moment.t,
  expectedResolutionDate: option(MomentRe.Moment.t),
  state: Types.measurableState,
  stateUpdatedAt: option(MomentRe.Moment.t),
  creator: option(creator),
  series: option(series),
  isArchived: bool,
  labelOnDate: option(MomentRe.Moment.t),
  labelProperty: option(string),
  min: option(float),
  max: option(float),
};

/* TODO: Fix channel */
let toMeasurable = (m: node): Types.measurable => {
  let channel =
    switch (m.channel) {
    | Some(channel) =>
      Some(
        Primary.Channel.make(
          ~id=channel.id,
          ~name=channel.name,
          ~isArchived=channel.isArchived,
          ~isPublic=channel.isPublic,
          (),
        ),
      )
    | _ => None
    };

  Primary.Measurable.make(
    ~id=m.id,
    ~name=m.name,
    ~channelId=m.channelId,
    ~channel,
    ~isArchived=Some(m.isArchived),
    ~valueType=m.valueType,
    ~labelCustom=m.labelCustom,
    ~resolutionEndpoint=m.resolutionEndpoint,
    ~measurementCount=m.measurementCount,
    ~measurerCount=m.measurerCount,
    ~labelSubject=m.labelSubject,
    ~labelOnDate=m.labelOnDate,
    ~labelProperty=m.labelProperty,
    ~createdAt=Some(m.createdAt),
    ~updatedAt=Some(m.updatedAt),
    ~expectedResolutionDate=m.expectedResolutionDate,
    ~state=Some(m.state),
    ~stateUpdatedAt=m.stateUpdatedAt,
    ~creator=E.O.fmap(toAgent, m.creator),
    ~series=E.O.fmap(toSeries, m.series),
    ~iAmOwner=Some(m.iAmOwner),
    ~min=m.min,
    ~max=m.max,
    (),
  );
};

module Query = [%graphql
  {|
    query getMeasurables (
        $states: [measurableState]!
        $channelId: String
        $seriesId: String
        $creatorId: String
        $first: Int
        $last: Int
        $after: String
        $before: String
    ) {
        measurables: measurables(
            states: $states
            channelId: $channelId
            seriesId: $seriesId
            creatorId: $creatorId
            first: $first
            last: $last
            after: $after
            before: $before
        ) {
          total
          pageInfo{
            hasPreviousPage
            hasNextPage
            startCursor
            endCursor
          }
          edges{
            node @bsRecord{
              id
              name
              channelId
              isArchived
              channel: Channel @bsRecord {
                id
                name
                description
                isPublic
                isArchived
              }
              labelCustom
              resolutionEndpoint
              valueType
              measurementCount
              measurerCount
              labelSubject
              labelProperty
              iAmOwner
              labelOnDate @bsDecoder(fn: "E.J.O.toMoment")
              state @bsDecoder(fn: "Primary.MeasurableState.fromEnum")
              stateUpdatedAt @bsDecoder(fn: "E.J.O.toMoment")
              expectedResolutionDate @bsDecoder(fn: "E.J.O.toMoment")
              createdAt @bsDecoder(fn: "E.J.toMoment")
              updatedAt @bsDecoder(fn: "E.J.toMoment")
              creator @bsRecord{
                id
                name
              }
              series @bsRecord{
                id
                name
                description
              }
              min
              max
            }
          }
        }
    }
  |}
];

module QueryComponent = ReasonApollo.CreateQuery(Query);

let unpackEdges = a =>
  a##measurables |> E.O.fmap(Primary.Connection.fromJson(toMeasurable));

type measurableStates = Types.measurableState;

type direction = Primary.Connection.direction;

let queryDirection =
    (
      ~states: Js.Array.t(option(measurableStates)),
      ~seriesId=?,
      ~channelId=?,
      ~creatorId=?,
      ~pageLimit,
      ~direction,
      (),
    ) => {
  let fn = Query.make(~seriesId?, ~channelId?, ~creatorId?, ~states);
  switch ((direction: direction)) {
  | None => fn(~first=pageLimit, ())
  | After(after) => fn(~first=pageLimit, ~after, ())
  | Before(before) => fn(~last=pageLimit, ~before, ())
  };
};

let componentMaker = (query, innerComponentFn) =>
  <QueryComponent variables=query##variables>
    ...{o =>
      o.result
      |> HttpResponse.fromApollo
      |> HttpResponse.fmap(unpackEdges)
      |> HttpResponse.optionalToMissing
      |> innerComponentFn
    }
  </QueryComponent>;

/* TODO: I'm sure there is a dryer way to do this but couldn't figure out quickly. */
let component2 =
    (
      ~channelId,
      ~pageLimit,
      ~states,
      ~direction: direction,
      ~innerComponentFn,
    ) => {
  let query = queryDirection(~channelId, ~pageLimit, ~direction, ~states, ());
  componentMaker(query, innerComponentFn);
};

let componentWithSeries =
    (~seriesId, ~pageLimit, ~direction: direction, ~innerComponentFn) => {
  let query =
    queryDirection(
      ~seriesId,
      ~pageLimit,
      ~direction,
      ~states=[|Some(`OPEN)|],
      (),
    );
  componentMaker(query, innerComponentFn);
};

let componentWithCreator =
    (~creatorId, ~pageLimit, ~direction: direction, ~innerComponentFn) => {
  let query =
    queryDirection(
      ~creatorId,
      ~pageLimit,
      ~direction,
      ~states=[|Some(`OPEN)|],
      (),
    );
  componentMaker(query, innerComponentFn);
};