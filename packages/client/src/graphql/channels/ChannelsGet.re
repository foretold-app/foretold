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

let toChannel = channel =>
  Primary.Channel.make(
    ~id=channel##id,
    ~name=channel##name |> E.J.toString,
    ~description=channel##description |> E.J.O.toString,
    ~isArchived=channel##isArchived,
    ~isPublic=channel##isPublic,
    ~isCurated=channel##isCurated,
    ~isBookmarked=channel##isBookmarked,
    ~membershipCount=Some(channel##membershipCount),
    ~openedMeasurablesCount=Some(channel##openedMeasurablesCount),
    ~bookmarksCount=Some(channel##bookmarksCount),
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

let orderChannels: order =
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
      ~order=orderChannels,
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