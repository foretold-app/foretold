module GetMeasurables = [%graphql
  {|
    query getMeasurables {
        measurables @bsRecord {
           id
           name
           createdAt @bsDecoder(fn: "jsonToString")
           updatedAt @bsDecoder(fn: "jsonToString")
        }
    }
  |}
];