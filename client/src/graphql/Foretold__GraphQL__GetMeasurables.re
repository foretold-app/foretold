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

type measurable = {
  id: string,
  name: string,
  channel: option(channel),
  valueType: Context.Primary.valueType,
  labelCustom: option(string),
  resolutionEndpoint: option(string),
  measurementCount: option(int),
  measurerCount: option(int),
  labelSubject: option(string),
  createdAt: MomentRe.Moment.t,
  updatedAt: MomentRe.Moment.t,
  expectedResolutionDate: option(MomentRe.Moment.t),
  state: Context.Primary.MeasurableState.t,
  stateUpdatedAt: option(MomentRe.Moment.t),
  creator: option(creator),
  series: option(series),
  labelOnDate: option(MomentRe.Moment.t),
  labelProperty: option(string),
};

/* TODO: Fix channel */
let toMeasurable = (m: measurable): Context.Primary.Measurable.t =>
  Context.Primary.Measurable.make(
    ~id=m.id,
    ~name=m.name,
    ~channel=None,
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
    (),
  );

module Query = [%graphql
  {|
    query getMeasurables ($offset: Int, $limit: Int, $channelId: String, $seriesId: String, $creatorId: String) {
        measurables(offset: $offset, limit: $limit, channelId: $channelId, seriesId: $seriesId, creatorId: $creatorId) @bsRecord {
           id
           name
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
  |}
];

module QueryComponent = ReasonApollo.CreateQuery(Query);

let queryToComponent = (query, innerComponentFn) =>
  QueryComponent.make(~variables=query##variables, o =>
    o.result
    |> ApolloUtils.apolloResponseToResult
    |> E.R.fmap(
         (d => d##measurables)
         ||> E.A.O.concatSomes
         ||> E.A.fmap(toMeasurable)
         ||> innerComponentFn,
       )
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
  let query =
    Query.make(~offset=page * pageLimit, ~limit=pageLimit, ~channelId, ());
  queryToComponent(query, innerComponentFn);
};

let component2 = (~channelId, ~pageNumber, ~pageLimit, ~innerComponentFn) => {
  let query =
    Query.make(
      ~offset=pageNumber * pageLimit,
      ~limit=pageLimit,
      ~channelId,
      (),
    );
  QueryComponent.make(~variables=query##variables, o =>
    o.result
    |> E.HttpResponse.fromApollo
    |> E.HttpResponse.fmap(e =>
         e##measurables |> E.A.O.concatSomes |> E.A.fmap(toMeasurable)
       )
    |> innerComponentFn
  )
  |> E.React.el;
};

let component3 = (~seriesId, ~pageNumber, ~pageLimit, ~innerComponentFn) => {
  let query =
    Query.make(
      ~offset=pageNumber * pageLimit,
      ~limit=pageLimit,
      ~seriesId,
      (),
    );
  QueryComponent.make(~variables=query##variables, o =>
    o.result
    |> E.HttpResponse.fromApollo
    |> E.HttpResponse.fmap(e =>
         e##measurables |> E.A.O.concatSomes |> E.A.fmap(toMeasurable)
       )
    |> innerComponentFn
  )
  |> E.React.el;
};

let component4 = (~creatorId, ~pageNumber, ~pageLimit, ~innerComponentFn) => {
  let query =
    Query.make(
      ~offset=pageNumber * pageLimit,
      ~limit=pageLimit,
      ~creatorId,
      (),
    );
  QueryComponent.make(~variables=query##variables, o =>
    o.result
    |> E.HttpResponse.fromApollo
    |> E.HttpResponse.fmap(e =>
         e##measurables |> E.A.O.concatSomes |> E.A.fmap(toMeasurable)
       )
    |> innerComponentFn
  )
  |> E.React.el;
};

let componentWithSeries =
    (channelId, seriesId, innerComponentFn: 'a => ReasonReact.reactElement) => {
  let query = Query.make(~offset=0, ~limit=200, ~channelId, ~seriesId, ());
  queryToComponent(query, innerComponentFn);
};

let componentWithCreator =
    (creatorId, innerComponentFn: 'a => ReasonReact.reactElement) => {
  let query = Query.make(~offset=0, ~limit=200, ~creatorId, ());
  queryToComponent(query, innerComponentFn);
};