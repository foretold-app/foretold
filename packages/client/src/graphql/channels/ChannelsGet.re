module Query = [%graphql
  {|
    query channels (
        $channelMemberId: String
        $isArchived: [isArchived]
        $order: [OrderChannels]
    ){
      channels (
        channelMemberId: $channelMemberId
        isArchived: $isArchived
        order: $order
        limit: 500
      ){
        id
        name
        description
        isArchived
        membershipCount
        isPublic
        isCurated
        isBookmarked
        openedMeasurablesCount
        bookmarksCount
      }
    }
  |}
];

module QueryComponent = ReasonApollo.CreateQuery(Query);

let toChannel = m =>
  Primary.Channel.make(
    ~id=m##id,
    ~name=m##name |> E.J.toString,
    ~description=m##description |> E.J.O.toString,
    ~isArchived=m##isArchived,
    ~isPublic=m##isPublic,
    ~isCurated=m##isCurated,
    ~isBookmarked=m##isBookmarked,
    ~membershipCount=Some(m##membershipCount),
    ~openedMeasurablesCount=Some(m##openedMeasurablesCount),
    ~bookmarksCount=Some(m##bookmarksCount),
    (),
  );

type order =
  option(
    array(
      option({
        .
        "direction": Types.direction,
        "field": Types.fieldChannels,
      }),
    ),
  );

let orderCommunities: order =
  Some([|
    Some({"field": `isCurated, "direction": `DESC}),
    Some({"field": `membersCount, "direction": `DESC}),
  |]);

let orderSidebar: order =
  Some([|Some({"field": `name, "direction": `ASC})|]);

let component =
    (
      ~channelMemberId: option(string)=?,
      ~isArchived=[|Some(`FALSE)|],
      ~order=orderCommunities,
      fn,
    ) => {
  let query = Query.make(~channelMemberId?, ~isArchived, ~order?, ());

  <QueryComponent variables=query##variables>
    ...{({result}) =>
      result
      |> ApolloUtils.apolloResponseToResult
      |> E.R.fmap(e =>
           e##channels |> E.A.O.concatSomes |> E.A.fmap(toChannel)
         )
      |> E.R.fmap(fn)
      |> E.R.id
    }
  </QueryComponent>;
};