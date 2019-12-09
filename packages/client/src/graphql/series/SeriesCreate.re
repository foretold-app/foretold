[@bs.config {jsx: 3}];

module Query = [%graphql
  {|
            mutation seriesCreate($input:SeriesCreateInput!) {
                seriesCreate(input: $input) {
                  name
                }
            }
    |}
];