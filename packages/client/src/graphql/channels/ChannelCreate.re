[@bs.config {jsx: 3}];

module Query = [%graphql
  {|
            mutation channelCreate($input: ChannelInput!) {
                channelCreate(input: $input) {
                 id
                }
            }
    |}
];