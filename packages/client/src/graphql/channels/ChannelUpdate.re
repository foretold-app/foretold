[@bs.config {jsx: 3}];

module Query = [%graphql
  {|
            mutation channelUpdate($id: String!, $input: ChannelInput!) {
                channelUpdate(id: $id, input: $input) {
                 id
                }
            }
    |}
];