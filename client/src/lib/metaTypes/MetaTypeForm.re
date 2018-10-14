module Id =
  Belt.Id.MakeComparable({
    type t = string;
    let cmp: (string, string) => int = Pervasives.compare;
  });

type t = Belt.Map.t(Id.t, string, Id.identity);

type action =
  | Update(string, string);
type state = {values: t};
let component = ReasonReact.reducerComponent("ReForm");

let make = (~fields, children) => {
  ...component,
  initialState: () => {
    let v =
      Belt.Map.fromArray(~id=(module Id), Array.map(e => (e, ""), fields));
    {values: v};
  },
  reducer: (action, state) =>
    switch (action) {
    | Update(key, value) =>
      let values: t = Belt.Map.set(state.values, key, value);
      ReasonReact.Update({...state, values});
    },
  render: self =>
    <div>
      (
        Array.map(
          r =>
            <Form.Item>
              <h3> (r |> Utils.ste) </h3>
              <Antd.Input
                value=(Belt.Map.getExn(self.state.values, r))
                onChange=(
                  ReForm.Helpers.handleDomFormChange(e =>
                    self.send(Update(r, e))
                  )
                )
              />
            </Form.Item>,
          fields,
        )
        |> ReasonReact.array
      )
    </div>,
};