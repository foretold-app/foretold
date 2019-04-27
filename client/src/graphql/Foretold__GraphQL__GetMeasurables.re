open Rationale.Function.Infix;

type series = {
  id: string,
  description: option(string),
  name: option(string),
};

let toSeries = (c: series): Context.Primary.Series.t =>
  Context.Primary.Series.make(
    ~id=c.id,
    ~description=c.description,
    ~name=c.name,
    (),
  );

type creator = {
  id: string,
  name: option(string),
};

type channel = {
  id: string,
  name: string,
  description: option(string),
  isPublic: bool,
};

let toAgent = (c: creator): Context.Primary.Agent.t =>
  Context.Primary.Agent.make(~id=c.id, ~name=c.name, ());

type node = {
  id: string,
  name: string,
  channel: option(channel),
  valueType: Context.Primary.valueType,
  labelCustom: option(string),
  resolutionEndpoint: option(string),
  measurementCount: option(int),
  measurerCount: option(int),
  labelSubject: option(string),
  iAmOwner: bool,
  createdAt: MomentRe.Moment.t,
  updatedAt: MomentRe.Moment.t,
  expectedResolutionDate: option(MomentRe.Moment.t),
  state: Context.Primary.MeasurableState.t,
  stateUpdatedAt: option(MomentRe.Moment.t),
  creator: option(creator),
  series: option(series),
  isArchived: bool,
  labelOnDate: option(MomentRe.Moment.t),
  labelProperty: option(string),
};

/* TODO: Fix channel */
let toMeasurable = (m: node): Context.Primary.Measurable.t =>
  Context.Primary.Measurable.make(
    ~id=m.id,
    ~name=m.name,
    ~channel=None,
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
    (),
  );

module Query = [%graphql
  {|
    query getMeasurables ( $states: [measurableState]!, $channelId: String, $seriesId: String, $creatorId: String,$first: Int, $last: Int, $after: String, $before: String) {
        measurables(states: $states, channelId: $channelId, seriesId: $seriesId, creatorId: $creatorId, first: $first, last: $last, after: $after, before: $before) {
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
              isArchived
              channel: Channel @bsRecord {
                id
                name
                description
                isPublic
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
              state @bsDecoder(fn: "Context.Primary.MeasurableState.fromEnum")
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
            }
          }
        }
    }
  |}
];

module QueryComponent = ReasonApollo.CreateQuery(Query);

let unpackEdges = a =>
  a##measurables
  |> E.O.fmap(Context.Primary.Connection.fromJson(toMeasurable));

let queryToComponent = (query, innerComponentFn) =>
  QueryComponent.make(~variables=query##variables, o =>
    o.result
    |> ApolloUtils.apolloResponseToResult
    |> E.R.fmap(unpackEdges ||> innerComponentFn)
    |> E.R.id
  )
  |> E.React.el;

type measurableStates = Context.Primary.MeasurableState.t;

type inputType('a) =
  (
    ~states: Js.Array.t(option(measurableStates)),
    ~channelId: string=?,
    ~seriesId: string=?,
    ~creatorId: string=?,
    ~first: int=?,
    ~last: int=?,
    ~after: string=?,
    ~before: string=?,
    unit
  ) =>
  'a;

type direction = Context.Primary.Connection.direction;

let queryDirection =
    (
      ~states: Js.Array.t(option(measurableStates)),
      ~seriesId=?,
      ~channelId=?,
      ~creatorId=?,
      ~pageLimit,
      ~direction,
      ~fn: inputType('a),
      (),
    ) => {
  let fn = fn(~seriesId?, ~channelId?, ~creatorId?, ~states);
  switch ((direction: direction)) {
  | None => fn(~first=pageLimit, ())
  | After(after) => fn(~first=pageLimit, ~after, ())
  | Before(before) => fn(~last=pageLimit, ~before, ())
  };
};

let componentMaker = (query, innerComponentFn) =>
  QueryComponent.make(~variables=query##variables, o =>
    o.result
    |> E.HttpResponse.fromApollo
    |> E.HttpResponse.fmap(unpackEdges)
    |> E.HttpResponse.optionalToMissing
    |> innerComponentFn
  )
  |> E.React.el;

/* TODO: I'm sure there is a dryer way to do this but couldn't figure out quickly. */
let component2 =
    (
      ~channelId,
      ~pageLimit,
      ~states,
      ~direction: direction,
      ~innerComponentFn,
    ) => {
  let query =
    queryDirection(
      ~channelId,
      ~pageLimit,
      ~direction,
      ~states,
      ~fn=Query.make,
      (),
    );
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
      ~fn=Query.make,
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
      ~fn=Query.make,
      (),
    );
  componentMaker(query, innerComponentFn);
};