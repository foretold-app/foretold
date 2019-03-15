open Rationale;
open QueriesHelper;
open Utils;

type series = {
  id: string,
  description: option(string),
  name: option(string),
};

let toSeries = (c: series): DataModel.series => {
  id: c.id,
  description: c.description,
  name: c.name,
};

type creator = {
  id: string,
  name: option(string),
};

let toAgent = (c: creator): DataModel.agent =>
  DataModel.toAgent(~id=c.id, ~name=c.name, ());

type measurable = {
  id: string,
  name: string,
  channel: option(string),
  valueType: DataModel.valueType,
  description: option(string),
  resolutionEndpoint: option(string),
  measurementCount: option(int),
  measurerCount: option(int),
  descriptionEntity: option(string),
  createdAt: MomentRe.Moment.t,
  updatedAt: MomentRe.Moment.t,
  expectedResolutionDate: option(MomentRe.Moment.t),
  state: DataModel.measurableState,
  stateUpdatedAt: option(MomentRe.Moment.t),
  creator: option(creator),
  series: option(series),
  descriptionDate: option(MomentRe.Moment.t),
  descriptionProperty: option(string),
};

let toMeasurable = (m: measurable): DataModel.measurable =>
  DataModel.toMeasurable(
    ~id=m.id,
    ~name=m.name,
    ~channel=m.channel,
    ~valueType=m.valueType,
    ~description=m.description,
    ~resolutionEndpoint=m.resolutionEndpoint,
    ~measurementCount=m.measurementCount,
    ~measurerCount=m.measurerCount,
    ~descriptionEntity=m.descriptionEntity,
    ~descriptionDate=m.descriptionDate,
    ~descriptionProperty=m.descriptionProperty,
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
    query getMeasurables ($offset: Int, $limit: Int, $channel: String, $seriesId: String, $creatorId: String) {
        measurables(offset: $offset, limit: $limit, channel: $channel, seriesId: $seriesId, creatorId: $creatorId) @bsRecord {
           id
           name
           channel
           description
           resolutionEndpoint
           valueType
           measurementCount
           measurerCount
           descriptionEntity
           descriptionProperty
           descriptionDate @bsDecoder(fn: "optionalMoment")
           state @bsDecoder(fn: "string_to_measurableState")
           stateUpdatedAt @bsDecoder(fn: "optionalMoment")
           expectedResolutionDate @bsDecoder(fn: "optionalMoment")
           createdAt @bsDecoder(fn: "toMoment")
           updatedAt @bsDecoder(fn: "toMoment")
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
  Result.Infix.(
    QueryComponent.make(~variables=query##variables, o =>
      o.result
      |> ApolloUtils.apolloResponseToResult
      <$> (d => d##measurables)
      <$> E.A.Optional.concatSomes
      <$> (d => d |> E.A.fmap(toMeasurable))
      <$> (e => innerComponentFn(e))
      |> Result.result(idd, idd)
    )
    |> ReasonReact.element
  );

let component =
    (
      channel,
      page,
      pageLimit,
      innerComponentFn: 'a => ReasonReact.reactElement,
    ) => {
  let query =
    Query.make(~offset=page * pageLimit, ~limit=pageLimit, ~channel, ());
  queryToComponent(query, innerComponentFn);
};

let componentWithSeries =
    (channel, seriesId, innerComponentFn: 'a => ReasonReact.reactElement) => {
  let query = Query.make(~offset=0, ~limit=200, ~channel, ~seriesId, ());
  queryToComponent(query, innerComponentFn);
};

let componentWithCreator =
    (creatorId, innerComponentFn: 'a => ReasonReact.reactElement) => {
  let query = Query.make(~offset=0, ~limit=200, ~creatorId, ());
  queryToComponent(query, innerComponentFn);
};