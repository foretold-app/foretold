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

let toChannel = (m): Primary.Channel.t =>
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

let component = (~channelMemberId: option(string)=?, ~isArchived=?, fn) => {
  let query = Query.make(~channelMemberId?, ~isArchived?, ());

  QueryComponent.make(~variables=query##variables, ({result}) =>
    result
    |> ApolloUtils.apolloResponseToResult
    |> E.R.fmap(e => e##channels |> E.A.O.concatSomes |> E.A.fmap(toChannel))
    |> E.R.fmap(fn)
    |> E.R.id
  )
  |> E.React.el;
};