module Base = {
  include MeasurableTableBase;
};

let all = MeasurableTableItems.all;

module Queries = {
  let find = (id: string) =>
    MeasurableTableItems.all
    |> Array.to_list
    |> Rationale.RList.find((r: MeasurableTableBase.measurableType) =>
         r.id == id
       );
};