module Query = [%graphql
  {|
    query getChannels (
        $channelMemberId: String
        $isArchived: [isArchived]
    ){
      channels (
        channelMemberId: $channelMemberId
        isArchived: $isArchived
      ){
        id
        name
        description
        isArchived
        membershipCount
        isPublic
        isCurated
        openedMeasurablesCount
      }
    }
  |}
];

module QueryComponent = ReasonApollo.CreateQuery(Query);

let toChannel = m =>
  Primary.Channel.make(
    ~id=m##id,
    ~name=m##name,
    ~description=m##description,
    ~isArchived=m##isArchived,
    ~isPublic=m##isPublic,
    ~isCurated=m##isCurated,
    ~membershipCount=Some(m##membershipCount),
    ~openedMeasurablesCount=Some(m##openedMeasurablesCount),
    (),
  );

let sortDefault = arr => arr;

let isCuratedCompare = (a: Types.channel, b: Types.channel) =>
  a.isCurated > b.isCurated ? (-1) : 1;

let nameCompare = (a: Types.channel, b: Types.channel) =>
  String.compare(a.name, b.name);

let sortAsc = (arr: array(Types.channel)) => {
  Array.sort(nameCompare, arr);
  arr;
};

let sortCurated = (arr: array(Types.channel)) => {
  Array.sort(nameCompare, arr);
  Array.sort(isCuratedCompare, arr);
  arr;
};

let component =
    (
      ~channelMemberId: option(string)=?,
      ~isArchived=[|Some(`FALSE)|],
      ~sortFn=sortDefault,
      fn,
    ) => {
  let query = Query.make(~channelMemberId?, ~isArchived, ());

  <QueryComponent variables=query##variables>
    ...{({result}) =>
      result
      |> ApolloUtils.apolloResponseToResult
      |> E.R.fmap(e =>
           e##channels |> E.A.O.concatSomes |> E.A.fmap(toChannel) |> sortFn
         )
      |> E.R.fmap(fn)
      |> E.R.id
    }
  </QueryComponent>;
};