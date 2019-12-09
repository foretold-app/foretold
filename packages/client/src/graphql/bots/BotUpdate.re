[@bs.config {jsx: 3}];

module Query = [%graphql
  {|
        mutation botUpdate($id: String!, $input: BotInput!) {
            botUpdate(id: $id, input: $input) {
             id
            }
        }
    |}
];