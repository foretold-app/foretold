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

let toChannel = (m): Types.channel =>
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

let sortAsc = (arr: array(Types.channel)) => {
  Array.sort(
    (a: Types.channel, b: Types.channel) => String.compare(a.name, b.name),
    arr,
  );
  arr;
};

let component =
    (
      ~channelMemberId: option(string)=?,
      ~isArchived=?,
      ~sortFn=sortDefault,
      fn,
    ) => {
  let query = Query.make(~channelMemberId?, ~isArchived?, ());

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