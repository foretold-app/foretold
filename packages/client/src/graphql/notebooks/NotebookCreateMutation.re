[@bs.config {jsx: 3}];

module Query = [%graphql
  {|
    mutation notebookCreate($input: NotebookCreateInput!) {
        notebookCreate(input: $input) {
         id
        }
    }
  |}
];