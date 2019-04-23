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
    query getMeasurables ($channelId: String, $seriesId: String, $creatorId: String, $first: Int, $after: String) {
        measurables(channelId: $channelId, seriesId: $seriesId, creatorId: $creatorId, first: $first, after: $after) {
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

let component =
    (
      channelId,
      page,
      pageLimit,
      innerComponentFn: 'a => ReasonReact.reactElement,
    ) => {
  let query = Query.make(~channelId, ());
  queryToComponent(query, innerComponentFn);
};

let component2 =
    (~channelId, ~pageLimit, ~after: option(string), ~innerComponentFn) => {
  let query =
    Query.make(
      ~channelId,
      ~first=pageLimit,
      ~after=after |> E.O.default(""),
      (),
    );
  QueryComponent.make(~variables=query##variables, o =>
    o.result
    |> E.HttpResponse.fromApollo
    |> E.HttpResponse.fmap(unpackEdges)
    |> E.HttpResponse.optionalToMissing
    |> innerComponentFn
  )
  |> E.React.el;
};

let component3 =
    (~seriesId, ~pageLimit, ~after: option(string), ~innerComponentFn) => {
  let query = Query.make(~seriesId, ~first=pageLimit, ());
  QueryComponent.make(~variables=query##variables, o =>
    o.result
    |> E.HttpResponse.fromApollo
    |> E.HttpResponse.fmap(unpackEdges)
    |> E.HttpResponse.optionalToMissing
    |> innerComponentFn
  )
  |> E.React.el;
};

let component4 =
    (~creatorId, ~pageLimit, ~after: option(string), ~innerComponentFn) => {
  let query = Query.make(~creatorId, ~first=pageLimit, ());
  QueryComponent.make(~variables=query##variables, o =>
    o.result
    |> E.HttpResponse.fromApollo
    |> E.HttpResponse.fmap(unpackEdges)
    |> E.HttpResponse.optionalToMissing
    |> innerComponentFn
  )
  |> E.React.el;
};

let componentWithSeries =
    (channelId, seriesId, innerComponentFn: 'a => ReasonReact.reactElement) => {
  let query = Query.make(~channelId, ~seriesId, ());
  queryToComponent(query, innerComponentFn);
};

let componentWithCreator =
    (creatorId, innerComponentFn: 'a => ReasonReact.reactElement) => {
  let query = Query.make(~creatorId, ());
  queryToComponent(query, innerComponentFn);
};